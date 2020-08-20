// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include "Containers/Array.h"
#include "Containers/Map.h"


//++++ more tests to verify correctness
//++++ a way to clear vectors' memory after a lot of objects has been removed
//++++ add function to add list of Objects
//???? may be RemoveObject doesn't work in the most effective way

/**
 * Node struct used in TagsGraph.
 */
template<class Object>
struct Node
{
  TArray<std::weak_ptr<Node<Object>>> AdjasentNodes;
  TArray<size_t> IndexesInAdjasentNodesArrays;

  // If counter is -1 this node is associated with a Tag and
  // not an Object (mark that its' counter can't be changed)
  // Otherwise it is used for search
  int32 Counter = 0;
  Object* Origin = nullptr;

  Node(bool bIsTagNode = false)
  {
    if (bIsTagNode)
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
      std::weak_ptr<Node<Object>> TagNode = AdjasentNodes[EdgeIndex];

      if (TagNode.expired())
      {
        continue;
      }

      size_t IndexInList = IndexesInAdjasentNodesArrays[EdgeIndex];
      std::shared_ptr<Node<Object>> TagNodeSharedPointer = TagNode.lock();

      // If objectNode isn't in the end of the list it can't be easily popped
      if (IndexInList < TagNode.lock()->AdjasentNodes.Num() - 1)
      {
        // Find node to swap with in list of adjasent nodes
        std::weak_ptr<Node<Object>> NodeForSwap = TagNodeSharedPointer->AdjasentNodes[
          TagNodeSharedPointer->AdjasentNodes.Num() - 1
        ];

        std::shared_ptr<Node<Object>> NodeForSwapSharedPointer = NodeForSwap.lock();

        // Set new index in tagNode's list of adjacent nodes for this node
        for (size_t SearchIndex = 0; SearchIndex < NodeForSwapSharedPointer->AdjasentNodes.Num(); SearchIndex++)
        {
          if (NodeForSwapSharedPointer->AdjasentNodes[SearchIndex].lock().get() == TagNodeSharedPointer.get())
          {
            NodeForSwapSharedPointer->IndexesInAdjasentNodesArrays[SearchIndex] = IndexInList;
            break;
          }
        }

        TagNodeSharedPointer->AdjasentNodes.Swap(IndexInList, TagNodeSharedPointer->AdjasentNodes.Num() - 1);
      }

      TagNodeSharedPointer->AdjasentNodes.Pop();
      TagNodeSharedPointer->IndexesInAdjasentNodesArrays.Pop();
    }
  }
};


/**
 * Provides system to search objects by needed tags.
 *
 * @warning It should be guaranteed that Object has functions std::shared_ptr<NodeType>& GetNode(void) and TArray<Tag>& GetTags(void)
 */
template<class Tag, class Object>
class TagsGraph
{
private:
  using NodeType = Node<Object>;
  using iterator = typename std::initializer_list<Tag>::iterator;

  TMap<Tag, std::shared_ptr<NodeType>> TagToNodeMap;

public:
  void AddTag(Tag TagToAdd)
  {
    TagToNodeMap.Add(TagToAdd, std::shared_ptr<NodeType>(new NodeType(true)));
  }

  /**
   * Initialise list of tags which can be connected to.
   */
  void InitialiseTags(std::initializer_list<Tag> AllAvailableTags)
  {
    TagToNodeMap.Empty();

    for (iterator TagIterator = AllAvailableTags.begin(); TagIterator != AllAvailableTags.end(); ++TagIterator)
    {
      AddTag(*TagIterator);
    }
  }

  TagsGraph()
  {

  }

  TagsGraph(std::initializer_list<Tag> AllAvailableTags)
  {
    InitialiseTags(AllAvailableTags);
  }

  /** 
   * Connects object to system of tags
   */
  void AddObject(Object* ObjectToAdd)
  {
    std::shared_ptr<NodeType>& ObjectNode = ObjectToAdd->GetNode();
    ObjectNode = std::make_shared<NodeType>();
    ObjectNode->Origin = ObjectToAdd;

    for (Tag TagOfObject : ObjectToAdd->GetTags())
    {
      TagToNodeMap[TagOfObject]->IndexesInAdjasentNodesArrays.Add(ObjectNode->AdjasentNodes.Num());
      ObjectNode->IndexesInAdjasentNodesArrays.Add(TagToNodeMap[TagOfObject]->AdjasentNodes.Num());

      TagToNodeMap[TagOfObject]->AdjasentNodes.Add(ObjectNode);
      ObjectNode->AdjasentNodes.Add(TagToNodeMap[TagOfObject]);
    }
  }

  /**
   * Removes object connection to tags.
   */
  void RemoveObject(Object* ObjectToRemove)
  {
    ObjectToRemove->GetNode().reset();
  }

  /**
   * Finds objects which have enough number of tags from tagsList
   */
  TArray<Object*> FindByTags(const TArray<int32> TagsArray, int EnoughNumberOfTags) const
  {
    // Sanity check
    if (EnoughNumberOfTags < 0)
    {
      throw "Number of tags can't be negative";
    }

    TArray<std::weak_ptr<NodeType>> ExaminedNodes;

    for (Tag ExaminedTag : TagsArray)
    {
      for (std::weak_ptr<NodeType> ExaminedNode : TagToNodeMap[ExaminedTag]->AdjasentNodes)
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
      if (ExaminedNode.lock()->Counter >= EnoughNumberOfTags)
      {
        FoundNodes.Add(ExaminedNode.lock()->Origin);
      }

      ExaminedNode.lock()->Counter = 0;
    }

    return FoundNodes;
  }
};