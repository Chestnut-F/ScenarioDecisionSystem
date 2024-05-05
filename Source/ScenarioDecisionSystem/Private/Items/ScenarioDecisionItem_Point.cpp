// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ScenarioDecisionItem_Point.h"

UScenarioDecisionItem_Point::UScenarioDecisionItem_Point(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValueSize = sizeof(FNavLocation);
}
