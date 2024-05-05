// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionDelegates.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RunScenarioDecision.generated.h"

struct FBTcenarioDecisionTaskMemory
{
	/** Scenario Decision ID */
	int32 ScenarioDecisionID;
};

/**
 * 
 */
UCLASS()
class SCENARIODECISIONSYSTEM_API UBTTask_RunScenarioDecision : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RunScenarioDecision(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override;

	void OnDecisionFinished(TSharedPtr<FScenarioDecisionResult> Result);

protected:
	UPROPERTY(EditAnywhere)
	FScenarioDecisionRequest ScenarioDecisionRequest;

private:
	FOnScenarioDecisionFinished QueryFinishedDelegate;

	const FName AIMessage_DecisionFinished = TEXT("DecisionFinished");
};
