// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 1. Interactive objects
// 1.1 Character

const int32    DefaultPointsValue = 10;
const FName    DefaultStringValue = "Dude";

const int32    ObjectNameStatID = 1;
const int32    HealthStatID = 2;

// 2. Debug
// Used for drawing lines, spheres and other shapes.

const FColor DebugColor(255, 0, 0);

const int32 DebugTime = 5;

const FName DebugString("Hello, world!");

// Some used difinitions:
// Interactive with capital I is a name of class and not this word itself
// CT - custom tag (CTs - custom tags), always int32
// //++++ comment means that some code must be added
// //???? comment means that some code may be useful to add