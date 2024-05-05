// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionInstance.h"
#include "ScenarioDecisionNode.h"
#include "ScenarioDecisionAnalyzer.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SCENARIODECISIONSYSTEM_API UScenarioDecisionAnalyzer : public UScenarioDecisionNode
{
	GENERATED_BODY()

public:
	void WrappedAnalyzeItems(UScenarioDecisionInstance& DecisionInstance);

protected:
	virtual void AnalyzeItems(UScenarioDecisionInstance& DecisionInstance) const { checkNoEntry(); }
	
};

