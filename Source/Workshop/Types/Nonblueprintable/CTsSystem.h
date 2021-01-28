// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include "Containers/Array.h"
#include "Containers/Map.h"

//++++ more tests to verify correctness
//++++ a way to clear vectors' memory after a lot of objects has been removed
//++++ add function to add array of Objects
//++++ change TArray return type to TQueue<Object*, EQueueMode::Spsc>
//???? may be RemoveObject doesn't work in the most effective way

/** Node struct used in CTsSearch. */
template<class Object>
struct Node
{
  TArray<std::weak_ptr<Node<Object>>> AdjasentNodes;
  TArray<size_t> IndexesInAdjasentNodesArrays;

  /**
   * Counter is used during a CT search and counts the number of met CTs.
   * If the counter is -1 this node is associated with a CT and not an Object, so it should stay unchanged.
   */
  int32 Counter = 0;

  /**
   * Object associated with that node.
   */
  Object* Origin = nullptr;

  Node(bool bIsCTNode = false)
  {
    if (bIsCTNode)
    {
      Counter = -1;
    }
  };

  ~Node()
  {
    if (Counter == -1)
    {
      return;
    }

    for (size_t EdgeIndex = 0; EdgeIndex < AdjasentNodes.Num(); EdgeIndex++)
    {
      std::weak_ptr<Node<Object>> CTNode = AdjasentNodes[EdgeIndex];

      if (CTNode.expired())
      {
        continue;
      }

      size_t IndexInList = IndexesInAdjasentNodesArrays[EdgeIndex];
      std::shared_ptr<Node<Object>> CTNodeSharedPointer = CTNode.lock();

      // If objectNode isn't in the end of the list it can't be easily popped
      if (IndexInList < CTNode.lock()->AdjasentNodes.Num() - 1)
      {
        // Find node to swap with in list of adjasent nodes
        std::weak_ptr<Node<Object>> NodeForSwap = CTNodeSharedPointer->AdjasentNodes[
          CTNodeSharedPointer->AdjasentNodes.Num() - 1
        ];

        std::shared_ptr<Node<Object>> NodeForSwapSharedPointer = NodeForSwap.lock();

        // Set new index in CTNode's list of adjacent nodes for this node
        for (size_t SearchIndex = 0; SearchIndex < NodeForSwapSharedPointer->AdjasentNodes.Num(); SearchIndex++)
        {
          if (NodeForSwapSharedPointer->AdjasentNodes[SearchIndex].lock().get() == CTNodeSharedPointer.get())
          {
            NodeForSwapSharedPointer->IndexesInAdjasentNodesArrays[SearchIndex] = IndexInList;
            break;
          }
        }

        CTNodeSharedPointer->AdjasentNodes.Swap(IndexInList, CTNodeSharedPointer->AdjasentNodes.Num() - 1);
      }

      CTNodeSharedPointer->AdjasentNodes.Pop();
      CTNodeSharedPointer->IndexesInAdjasentNodesArrays.Pop();
    }
  }
};

/**
 * Provides system to search objects by needed CTs.
 *
 * @warning It should be guaranteed that Object has functions std::shared_ptr<NodeType>& GetNodeForCT(void) and TArray<CT>* GetCTs(void)
 */
template<class CT, class Object>
class CTsSearch
{
private:
  using NodeType = Node<Object>;
  using iterator = typename std::initializer_list<CT>::iterator;

  TMap<CT, std::shared_ptr<NodeType>> CTToNodeMap;
  TArray<std::weak_ptr<NodeType>> ObjectNodes;

public:
  /**
   * Include CT to the search system.
   * 
   * @note The objects which are already added won't be updated
   */
  void AddCT(CT CTToAdd)
  {
    CTToNodeMap.Add(CTToAdd, std::shared_ptr<NodeType>(new NodeType(true)));
  }

  /**
   * Remove all CTs and include several CTs to the search system.
   */
  void InitialiseCTs(std::initializer_list<CT> AllAvailableCTs)
  {
    CTToNodeMap.Empty();

    for (iterator CTIterator = AllAvailableCTs.begin(); CTIterator != AllAvailableCTs.end(); ++CTIterator)
    {
      AddCT(*CTIterator);
    }
  }

  CTsSearch()
  {

  }

  CTsSearch(std::initializer_list<CT> AllAvailableCTs)
  {
    InitialiseCTs(AllAvailableCTs);
  }
 
  /**
   * Connects the object to the search system.
   */
  void AddObject(Object* ObjectToAdd)
  {
    std::shared_ptr<NodeType>& ObjectNode = ObjectToAdd->GetNodeForCT();
    ObjectNode = std::make_shared<NodeType>();
    ObjectNode->Origin = ObjectToAdd;

    ObjectNodes.Add(ObjectNode);

    for (CT CTOfObject : *(ObjectToAdd->GetCTs()))
    {
      if (!CTToNodeMap.Find(CTOfObject))
      {
        UE_LOG(LogTemp, Error, TEXT("CT of object isn't included in CT system!"));
        continue;
      }

      CTToNodeMap[CTOfObject]->IndexesInAdjasentNodesArrays.Add(ObjectNode->AdjasentNodes.Num());
      ObjectNode->IndexesInAdjasentNodesArrays.Add(CTToNodeMap[CTOfObject]->AdjasentNodes.Num());

      CTToNodeMap[CTOfObject]->AdjasentNodes.Add(ObjectNode);
      ObjectNode->AdjasentNodes.Add(CTToNodeMap[CTOfObject]);
    }
  }

  /**
   * Removes the object from the search system.
   */
  void RemoveObject(Object* ObjectToRemove)
  {
    ObjectToRemove->GetNodeForCT().reset();
  }

  /**
   * Finds objects which have enough number of CTs from CTsArray.
   */
  TArray<Object*> FindByCTs(const TArray<int32> CTsArray, int EnoughNumberOfCTs) const
  {
    if (EnoughNumberOfCTs < 0)
    {
      throw "Number of CTs can't be negative";
    }

    TArray<std::weak_ptr<NodeType>> ExaminedNodes;

    for (CT ExaminedCT : CTsArray)
    {
      for (std::weak_ptr<NodeType> ExaminedNode : CTToNodeMap[ExaminedCT]->AdjasentNodes)
      {
        ExaminedNode.lock()->Counter++;
        if (ExaminedNode.lock()->Counter == 1)
        {
          ExaminedNodes.Add(ExaminedNode);
        }
      }
    }

    TArray<Object*> FoundNodes;

    for (std::weak_ptr<NodeType> ExaminedNode : ExaminedNodes)
    {
      if (ExaminedNode.lock()->Counter >= EnoughNumberOfCTs)
      {
        FoundNodes.Add(ExaminedNode.lock()->Origin);
      }

      ExaminedNode.lock()->Counter = 0;
    }

    return FoundNodes;
  }

  /**
   * Get all objects connected to the search system.
   */
  TArray<Object*> GetAllObjects()
  {
    TArray<Object*> AllObjects;
    
    size_t Index = 0;
    while (Index < ObjectNodes.Num())
    {
      if (ObjectNodes[Index].expired())
      {
        ObjectNodes.RemoveAtSwap(Index);
      }
      else
      {
        AllObjects.Add(ObjectNodes[Index].lock()->Origin);
        Index++;
      }
    }

    return AllObjects;
  }
};
