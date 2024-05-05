// Fill out your copyright notice in the Description page of Project Settings.


#include "Analyzers/ScenarioDecisionAnalyzer_Distance.h"

namespace
{
	FORCEINLINE float CalcDistance3D(const FVector& PosA, const FVector& PosB)
	{
		return static_cast<float>(FVector::Distance(PosA, PosB));
	}

	FORCEINLINE float CalcDistance3DSIMD(const FVector& PosA, const FVector& PosB)
	{
		const VectorRegister4Float U = MakeVectorRegisterFloat(PosA.X, PosA.Y, PosA.Z, 0.f);
		const VectorRegister4Float V = MakeVectorRegisterFloat(PosB.X, PosB.Y, PosB.Z, 0.f);
		const VectorRegister4Float UV = VectorSubtract(V, U);
		const VectorRegister4Float UVSquared = VectorMultiply(UV, UV);
		FVector DestVector;
		VectorStoreFloat3(UVSquared, &DestVector);
		return static_cast<float>(FMath::Sqrt(DestVector.X + DestVector.Y + DestVector.Z));
	}
}

UScenarioDecisionAnalyzer_Distance::UScenarioDecisionAnalyzer_Distance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UScenarioDecisionAnalyzer_Distance::AnalyzeItems(UScenarioDecisionInstance& DecisionInstance) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_ScenarioDecisionAnalyzer_Distance_AnalyzeItems);
	AActor* Initiator = Cast<AActor>(DecisionInstance.Owner);
	FVector DistanceTo = Initiator->GetActorLocation();
	FConstScenarioDecisionItemsView ItemsView(DecisionInstance.ItemHandles);
	
	ItemsView.ParallelFor([DistanceTo](FScenarioDecisionItemHandle& ItemHandle, int32 Index)
	{
		const FVector ItemLocation = ItemHandle.ItemValue<FNavLocation>();
		const float Distance = CalcDistance3D(ItemLocation, DistanceTo);
		ItemHandle.SetScore(Distance);
	});
}
