// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_RunScenarioDecision.h"

#include "ScenarioDecisionManager.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"




UBTTask_RunScenarioDecision::UBTTask_RunScenarioDecision(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	QueryFinishedDelegate = FOnScenarioDecisionFinished::CreateUObject(this, &UBTTask_RunScenarioDecision::OnDecisionFinished);
}

EBTNodeResult::Type UBTTask_RunScenarioDecision::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* QueryOwner = OwnerComp.GetOwner();
	AController* ControllerOwner = Cast<AController>(QueryOwner);
	if (ControllerOwner)
	{
		QueryOwner = ControllerOwner->GetPawn();
	}

	if (QueryOwner && ScenarioDecisionRequest.IsValid())
	{
		FBTcenarioDecisionTaskMemory* MyMemory = (FBTcenarioDecisionTaskMemory*)NodeMemory;

		if (UScenarioDecisionManager* ScenarioDecisionManager = Cast<UScenarioDecisionManager>(USubsystemBlueprintLibrary::GetGameInstanceSubsystem(&OwnerComp, UScenarioDecisionManager::StaticClass())))
		{
			MyMemory->ScenarioDecisionID = ScenarioDecisionManager->RunDecision(QueryOwner, ScenarioDecisionRequest, QueryFinishedDelegate);
		}
		
		const bool bValid = (MyMemory->ScenarioDecisionID >= 0);
		if (bValid)
		{
			// WaitForMessage(OwnerComp, AIMessage_DecisionFinished, MyMemory->ScenarioDecisionID);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_RunScenarioDecision::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UScenarioDecisionManager* ScenarioDecisionManager = Cast<UScenarioDecisionManager>(USubsystemBlueprintLibrary::GetGameInstanceSubsystem(&OwnerComp, UScenarioDecisionManager::StaticClass())))
	{
		FBTcenarioDecisionTaskMemory* MyMemory = (FBTcenarioDecisionTaskMemory*)NodeMemory;
		ScenarioDecisionManager->AborDecision(MyMemory->ScenarioDecisionID);
	}

	return EBTNodeResult::Aborted;
}

uint16 UBTTask_RunScenarioDecision::GetInstanceMemorySize() const
{
	return sizeof(FBTcenarioDecisionTaskMemory);
}

void UBTTask_RunScenarioDecision::OnDecisionFinished(TSharedPtr<FScenarioDecisionResult> Result)
{
	AActor* MyOwner = Cast<AActor>(Result->Owner.Get());
	if (APawn* PawnOwner = Cast<APawn>(MyOwner))
	{
		MyOwner = PawnOwner->GetController();
	}

	UBehaviorTreeComponent* MyComp = MyOwner ? MyOwner->FindComponentByClass<UBehaviorTreeComponent>() : NULL;
	if (!MyComp)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to find behavior tree to notify about finished decision from %s!"), *GetNameSafe(MyOwner));
		return;
	}

	UE_LOG(LogBehaviorTree, Display, TEXT("BestLocation: [%f, %f, %f]"), Result->Items[0].X, Result->Items[0].Y, Result->Items[0].Z);
	// FAIMessage::Send(MyComp, FAIMessage(UBrainComponent::AIMessage_QueryFinished, this, Result->ScenarioDecisionID, true));
	FinishLatentTask(*MyComp, EBTNodeResult::Succeeded);
}
