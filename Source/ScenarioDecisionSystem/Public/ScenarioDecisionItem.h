// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionItem.generated.h"

UCLASS(Abstract, MinimalAPI)
class UScenarioDecisionItem : public UObject
{
	GENERATED_BODY()

public:
	typedef void FValueType;

	/** get ValueSize */
	FORCEINLINE uint16 GetValueSize() const { return ValueSize; }

protected:
	/** size of value for this type */
    uint16 ValueSize;

};

struct FScenarioDecisionItemHandle;

struct FScenarioDecisionItems
{
	FScenarioDecisionItems()
		: Size (0)
	{
		Scores.Reset();
		IsDiscarded.Reset();
		RawData.Reset();
	}
	
	void Reserve(int32 ItemsNum, int32 ValueSize)
	{
		Scores.Reserve(ItemsNum);
		IsDiscarded.Reserve(ItemsNum);
		RawData.Reserve(ItemsNum * ValueSize);
	}
	
	void AddZeroed(int32 ValueSize, FScenarioDecisionItemHandle& OutHandle);
	
	TArray<float> Scores;
	TArray<uint8> IsDiscarded;
	TArray<uint8> RawData;
	int32 Size;
};


struct FScenarioDecisionItemHandle
{
public:
	FScenarioDecisionItemHandle() : ScenarioDecisionItems(nullptr), ItemIndex(-1), DataOffset(-1) {}
	FScenarioDecisionItemHandle(FScenarioDecisionItems* InItems, int32 InItemIndex, int32 InDataOffset)
		: ScenarioDecisionItems(InItems)
		, ItemIndex(InItemIndex)
		, DataOffset(InDataOffset) {}
	
	void SetScore(float Score) { ScenarioDecisionItems->Scores[ItemIndex] = Score; }

	float Score() const { return ScenarioDecisionItems->Scores[ItemIndex]; } 
	
	template<typename Type>
	void SetItemValue(Type Item) { FScenarioDecisionItemHandle::SetValueInMemory(ScenarioDecisionItems->RawData.GetData() + DataOffset, Item); }
	
	template<typename Type>
	Type ItemValue() const { return FScenarioDecisionItemHandle::GetValueFromMemory<Type>(ScenarioDecisionItems->RawData.GetData() + DataOffset); } 
	
	bool IsValid() const { return !ScenarioDecisionItems->IsDiscarded[ItemIndex]; }

private:
	friend struct FScenarioDecisionItems;
	friend class UScenarioDecisionInstance;
	
	TSharedPtr<FScenarioDecisionItems> ScenarioDecisionItems;
	
	int32 ItemIndex;
	int32 DataOffset;


	/** helper function for writing typed data to memory block */
	template<typename T>
	static void SetValueInMemory(uint8* MemoryBlock, const T& Value)
	{
		*((T*)MemoryBlock) = Value;
	}

	/** helper function for reading typed data from memory block */
	template<typename T>
	static const T& GetValueFromMemory(const uint8* MemoryBlock)
	{
		return *((T*)MemoryBlock);
	}
};

struct FConstScenarioDecisionItemsView
{
	FConstScenarioDecisionItemsView(TArray<FScenarioDecisionItemHandle>& InItemsHandle)
		: ItemHandles(InItemsHandle) {}
	
	TArray<FScenarioDecisionItemHandle>& ItemHandles;
	
	void ParallelFor(TFunctionRef<void(FScenarioDecisionItemHandle&, int32)> InCallable)
	{
		FConstScenarioDecisionItemsView::ParallelForImp(ItemHandles, InCallable);
	}

private:
	static void ParallelForImp(TArray<FScenarioDecisionItemHandle>& InItemHandles, TFunctionRef<void(FScenarioDecisionItemHandle&, int32)> InCallable)
	{
		const int32 InNum = InItemHandles.Num();
		auto PassThrough = [InCallable, &InItemHandles](int32 Idx)
		{
			InCallable(InItemHandles[Idx], Idx);
		};

		::ParallelFor(InNum, PassThrough, false);
	}
};

struct FScenarioDecisionItemsView : public FConstScenarioDecisionItemsView
{
	using Base = FConstScenarioDecisionItemsView;
	
	FScenarioDecisionItemsView(TArray<FScenarioDecisionItemHandle>& InItemsHandle)
		: Base(InItemsHandle) {}
};
