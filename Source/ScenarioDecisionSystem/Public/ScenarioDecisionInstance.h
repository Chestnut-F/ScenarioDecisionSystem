// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionDelegates.h"
#include "ScenarioDecisionItem.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "ScenarioDecisionInstance.generated.h"

UCLASS()
class UScenarioDecisionInstance : public UObject
{
	GENERATED_BODY()
	
public:
	void Advance();

	bool IsCompleted() const { return bIsCompleted; }

	void InitializeItemData(const TSubclassOf<UScenarioDecisionItem>& InItemType);
	
	void ReserveItemData(int32 NumItems);

	template<typename Item>
	void AddItemData(const typename Item::FValueType& ItemValue);
	
	TSharedPtr<FScenarioDecisionResult> GetResult() { return ScenarioDecisionResult; }

public:
	TWeakObjectPtr<UObject> Owner;

	TObjectPtr<UWorld> World;

	UPROPERTY()
	TObjectPtr<UScenarioDecisionItem> ItemType;
	
	TArray<FScenarioDecisionItemHandle> ItemHandles;
	TSharedPtr<FScenarioDecisionItems> ScenarioDecisionItems;
	
	int32 ScenarioDecisionID;
	
	FOnScenarioDecisionFinished FinishDelegate;

	bool bIsCompleted;

private:
	friend class UScenarioDecisionManager;

	UPROPERTY()
	TObjectPtr<UScenarioDecisionGenerator> Generator;

	UPROPERTY()
	TArray<TObjectPtr<UScenarioDecisionAnalyzer>> Analyzers;
	
	TSharedPtr<FScenarioDecisionResult> ScenarioDecisionResult;
};


template <typename Item>
void UScenarioDecisionInstance::AddItemData(const typename Item::FValueType& ItemValue)
{
	check(ItemType->GetValueSize() == sizeof(Item::FValueType));
	FScenarioDecisionItemHandle NewHandle;
	NewHandle.ScenarioDecisionItems = ScenarioDecisionItems;
	ScenarioDecisionItems->AddZeroed(ItemType->GetValueSize(), NewHandle);
	NewHandle.SetItemValue(ItemValue);
	ItemHandles.Emplace(MoveTemp(NewHandle));
}
