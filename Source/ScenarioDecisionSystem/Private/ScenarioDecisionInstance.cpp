// Fill out your copyright notice in the Description page of Project Settings.


#include "ScenarioDecisionInstance.h"
#include "ScenarioDecisionAnalyzer.h"
#include "ScenarioDecisionGenerator.h"
#include "Algo/MaxElement.h"
#include "Algo/MinElement.h"

void UScenarioDecisionInstance::Advance()
{
	Generator->WrappedGenerateItems(*this);

	for (auto& Analyzer: Analyzers)
	{
		Analyzer->WrappedAnalyzeItems(*this);
	}

	ScenarioDecisionResult = MakeShareable(new FScenarioDecisionResult());
	ScenarioDecisionResult->ScenarioDecisionID = ScenarioDecisionID;
	ScenarioDecisionResult->Owner = Owner;
	FScenarioDecisionItemHandle* MaxItem = Algo::MinElement(ItemHandles, [](const FScenarioDecisionItemHandle& A, const FScenarioDecisionItemHandle& B)
		{
			return A.Score() < B.Score();
		});
	ScenarioDecisionResult->Items.Add(MaxItem->ItemValue<FNavLocation>());
	FinishDelegate.ExecuteIfBound(ScenarioDecisionResult);
	bIsCompleted = true;
}

void UScenarioDecisionInstance::InitializeItemData(const TSubclassOf<UScenarioDecisionItem>& InItemType)
{
	ItemType = InItemType.GetDefaultObject();
	ScenarioDecisionItems = MakeShareable<FScenarioDecisionItems>(new FScenarioDecisionItems());
}

void UScenarioDecisionInstance::ReserveItemData(int32 NumItems)
{
	ScenarioDecisionItems->Reserve(NumItems, ItemType->GetValueSize());
	ItemHandles.Reserve(NumItems);
}


