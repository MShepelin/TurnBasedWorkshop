// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 1. Interactive objects
// 1.1 Default values
#define DEFAULT_DELAY 0.5f
   // Used for slots in AbilitiesWidget
#define DEFAULT_SLOTS_COUNT 4

// 1.2 Y-order of components
#define SPRITE_Y_ORDER 0
#define WIDGET_Y_ORDER 1

// 1.3 Animation
#define IDLE_ANIMATION_ID 1
#define EXHAUST_ANIMATION_ID 2

// 2. Debug
#define DEBUG_COLOR FColor(255, 0, 0)
#define DEBUG_TIME 5

// 3. UI
#define LINE_THICKNESS 10.0f
#define FONT_SMALL_SIZE 64
#define SMALL_PADDING 90
#define FONT_MIDDLE_SIZE 80
#define BAR_MAX_VALUE 100 

// -1. Others
#define PHASE_ENUM_NAME_LENGTH 12
#define INTERACTIVE_TYPE_SEPARATOR_MASK 7
#define COLLISION_BOX_WIDTH 4
#define MAX_OBJECT_SIZE 1000
#define RAY_CAST_LENGTH 1000
#define AVERAGE_MANAGED_OBJECTS 8
#define COLLISION_INTERACTIVE ECollisionChannel::ECC_GameTraceChannel1
#define SLEEP_MULTIPLIER 0.9f

//++++ This comment means that some code must be added
//???? This comment means that some code may be useful to add
