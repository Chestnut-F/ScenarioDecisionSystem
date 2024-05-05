// Fill out your copyright notice in the Description page of Project Settings.


#include "ScenarioDecisionManager.h"

#include "ScenarioDecisionAnalyzer.h"
#include "ScenarioDecisionGenerator.h"


void UScenarioDecisionManager::Tick(float DeltaTime)
{
	TArray<TObjectPtr<UScenarioDecisionInstance>> RunningTemp;
	RunningTemp.Reserve(RunningInstances.Num());
	for (TObjectPtr<UScenarioDecisionInstance> RunningInstance : RunningInstances)
	{
		if (!RunningInstance)
		{
			// TODO
			continue;
		}
	
		if (RunningInstance->IsCompleted())
		{
			DestroyScenarioDecisionInstance(RunningInstance);
		}
		else
		{
			RunningTemp.Emplace(RunningInstance);
		}
	}
	RunningInstances = MoveTemp(RunningTemp);
	
	while (PendingInstances.Num() > 0)
	{
		TObjectPtr<UScenarioDecisionInstance> Instance = PendingInstances.Pop();
		Instance->Advance();
		// RunningInstances.Push(Instance);
	}
}

int32 UScenarioDecisionManager::RunDecision(UObject* Initiator, const FScenarioDecisionRequest& Request,
                                              const FOnScenarioDecisionFinished& FinishDelegate)
{
	TObjectPtr<UScenarioDecisionInstance> Instance = CreateScenarioDecisionInstance(Initiator, Request);
	return RunDecision(Instance, FinishDelegate);
}

int32 UScenarioDecisionManager::RunDecision(TObjectPtr<UScenarioDecisionInstance> Instance,
	const FOnScenarioDecisionFinished& FinishDelegate)
{
	Instance->FinishDelegate = FinishDelegate;
	PendingInstances.Emplace(Instance);
	return Instance->ScenarioDecisionID;
}

void UScenarioDecisionManager::AborDecision(int32 DecisionID)
{
}

TObjectPtr<UScenarioDecisionInstance> UScenarioDecisionManager::CreateScenarioDecisionInstance(
	UObject* Initiator, const FScenarioDecisionRequest& Request)
{
	TObjectPtr<UScenarioDecisionInstance> NewInstance(NewObject<UScenarioDecisionInstance>());
	NewInstance->Owner = Initiator;
	NewInstance->World = Initiator->GetWorld();
	NewInstance->InitializeItemData(Request.DecisionTemplate->ItemType);
	NewInstance->Generator = Cast<UScenarioDecisionGenerator>(StaticDuplicateObject(Request.DecisionTemplate->Generator, NewInstance.Get()));
	for (UScenarioDecisionAnalyzer* Analyzer : Request.DecisionTemplate->Analyzers)
	{
		TObjectPtr<UScenarioDecisionAnalyzer> NewAnalyzer = Cast<UScenarioDecisionAnalyzer>(StaticDuplicateObject(Analyzer, NewInstance.Get()));
		NewInstance->Analyzers.Emplace(NewAnalyzer);
	}
	return NewInstance;
}

void UScenarioDecisionManager::DestroyScenarioDecisionInstance(
	TObjectPtr<UScenarioDecisionInstance> Instance)
{
	
}
