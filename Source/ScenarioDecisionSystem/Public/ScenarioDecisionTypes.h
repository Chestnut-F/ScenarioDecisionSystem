// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecision.h"
#include "ScenarioDecisionTypes.generated.h"

USTRUCT(BlueprintType)
struct FScenarioDecisionRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<const UScenarioDecision> DecisionTemplate;

	bool IsValid()
	{
		return true;
	}
};

USTRUCT()
struct FScenarioDecisionResult
{
	GENERATED_BODY()

	int32 ScenarioDecisionID;

	TWeakObjectPtr<UObject> Owner;

	TArray<FVector> Items;
};

