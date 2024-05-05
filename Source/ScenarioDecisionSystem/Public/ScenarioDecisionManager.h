// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionInstance.h"
#include "ScenarioDecisionManager.generated.h"

/**
 * 
 */
UCLASS()
class SCENARIODECISIONSYSTEM_API UScenarioDecisionManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return !IsTemplate(); }
	virtual TStatId GetStatId() const override{ RETURN_QUICK_DECLARE_CYCLE_STAT(UScenarioDecisionManager, STATGROUP_Tickables); }
	
	int32 RunDecision(UObject* Initiator, const FScenarioDecisionRequest& Request, const FOnScenarioDecisionFinished& FinishDelegate);
	void AborDecision(int32 DecisionID);

private:
	int32 RunDecision(TObjectPtr<UScenarioDecisionInstance> Instance, const FOnScenarioDecisionFinished& FinishDelegate);
	TObjectPtr<UScenarioDecisionInstance> CreateScenarioDecisionInstance(UObject* Initiator, const FScenarioDecisionRequest& Request);
	void DestroyScenarioDecisionInstance(TObjectPtr<UScenarioDecisionInstance> Instance);

private:
	TArray<TObjectPtr<UScenarioDecisionInstance>> PendingInstances;
	TArray<TObjectPtr<UScenarioDecisionInstance>> RunningInstances;
};
