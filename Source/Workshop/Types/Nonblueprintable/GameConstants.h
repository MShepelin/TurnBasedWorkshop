// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 1. Interactive objects
// 1.1 Default values
#define DefaultPointsValue 10
#define DefaultStringValue "Dude"
#define DEFAULT_DELAY 2.f

// 1.2 Y-order of components
#define MainSpriteYOrder 0
#define WidgetComponentYOrder 1

// 1.3 Animation
#define IdleAnimation 1

// 1.4 CharacterStats
#define CharacterIntegerStats 8

// 2. Debug
// Used for drawing lines, spheres and other shapes.
#define DEBUG_COLOR FColor(255, 0, 0)
#define DebugTime 5
#define DebugString FName("Hello, world!")

// 3. Code specifics
#define LengthOfInteractiveTypeName 18
#define LengthOfPhaseTypeName 12
#define LengthOfAdvantageTypeName 16
#define InteractiveTypeSeparator 7
#define CollisionBoxWidth 4
#define DefaultNumberOfSlots 4
#define MaxActorSize 1000
#define RayCastLength 1000
#define AVERAGE_MANAGED_OBJECTS 12
#define COLLISION_INTERACTIVE ECollisionChannel::ECC_GameTraceChannel1

// 4. UI
#define MaxIconSize 512.0
#define MinIconSize 256.0

// Some used difinitions:
// Interactive with capital I is a name of class and not this word itself
// CT - custom tag (CTs - custom tags), always int32
// //++++ comment means that some code must be added
// //???? comment means that some code may be useful to add
// Nonpositive values of CTs and stats are reserved and should not be used
