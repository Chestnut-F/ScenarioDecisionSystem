// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionItem.h"
#include "UObject/Object.h"
#include "ScenarioDecision.generated.h"

class UScenarioDecisionAnalyzer;
class UScenarioDecisionGenerator;

/**
 * 
 */
UCLASS(BlueprintType)
class UScenarioDecision : public UDataAsset
{
	GENERATED_BODY()

public:
	UScenarioDecision(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	friend class UScenarioDecisionManager;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UScenarioDecisionGenerator> Generator;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UScenarioDecisionAnalyzer>> Analyzers;

	TSubclassOf<UScenarioDecisionItem> ItemType;
	
};
