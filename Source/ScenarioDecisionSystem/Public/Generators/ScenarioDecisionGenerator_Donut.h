// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionGenerator.h"
#include "ScenarioDecisionGenerator_Donut.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SCENARIODECISIONSYSTEM_API UScenarioDecisionGenerator_Donut : public UScenarioDecisionGenerator
{
	GENERATED_BODY()
public:
	UScenarioDecisionGenerator_Donut(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void GenerateItems(UScenarioDecisionInstance& DecisionInstance) const override;

protected:
	/** min distance between point and context */
	UPROPERTY(EditDefaultsOnly, Category = Generator)
	float InnerRadius;

	/** max distance between point and context */
	UPROPERTY(EditDefaultsOnly, Category = Generator)
	float OuterRadius;
	
	/** number of rings to generate */
	UPROPERTY(EditDefaultsOnly, Category = Generator)
	int32 NumberOfRings;
	
	/** number of items to generate for each ring */
	UPROPERTY(EditDefaultsOnly, Category = Generator)
	int32 PointsPerRing;

	/** anchor */
	// UPROPERTY(EditAnywhere, Category = Generator)
	// FVector Center;
};
