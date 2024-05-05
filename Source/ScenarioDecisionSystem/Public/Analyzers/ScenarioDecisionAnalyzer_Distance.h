// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionAnalyzer.h"
#include "ScenarioDecisionAnalyzer_Distance.generated.h"

/**
 * 
 */
UCLASS()
class SCENARIODECISIONSYSTEM_API UScenarioDecisionAnalyzer_Distance : public UScenarioDecisionAnalyzer
{
	GENERATED_BODY()

public:
	UScenarioDecisionAnalyzer_Distance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void AnalyzeItems(UScenarioDecisionInstance& DecisionInstance) const;

protected:
	/** ancher */
	// UPROPERTY(EditDefaultsOnly, Category=Distance)
	// FVector DistanceTo;
};
