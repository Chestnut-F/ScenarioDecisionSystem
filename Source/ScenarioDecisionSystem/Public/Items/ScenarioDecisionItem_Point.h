// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionItem.h"
#include "ScenarioDecisionItem_Point.generated.h"


/**
 * 
 */
UCLASS()
class SCENARIODECISIONSYSTEM_API UScenarioDecisionItem_Point : public UScenarioDecisionItem
{
	GENERATED_BODY()
public:
	typedef const FNavLocation FValueType;

	UScenarioDecisionItem_Point(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
