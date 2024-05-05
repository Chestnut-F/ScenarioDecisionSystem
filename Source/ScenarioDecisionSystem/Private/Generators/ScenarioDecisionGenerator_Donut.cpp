// Fill out your copyright notice in the Description page of Project Settings.


#include "Generators/ScenarioDecisionGenerator_Donut.h"

UScenarioDecisionGenerator_Donut::UScenarioDecisionGenerator_Donut(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InnerRadius = 300.0f;
	OuterRadius = 3000.0f;
	NumberOfRings = 10;
	PointsPerRing = 16;
}

void UScenarioDecisionGenerator_Donut::GenerateItems(UScenarioDecisionInstance& DecisionInstance) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_UScenarioDecisionGenerator_Donut_GenerateItems);
	AActor* Initiator = Cast<AActor>(DecisionInstance.Owner);
	FVector Center = Initiator->GetActorLocation();

	if ((InnerRadius < 0.) || (OuterRadius <= 0.) ||
	(InnerRadius > OuterRadius) ||
	(NumberOfRings < 1) || (PointsPerRing < 1))
	{
		return;
	}

	const FVector::FReal RadiusDelta = (OuterRadius - InnerRadius) / (NumberOfRings - 1);
	const FVector::FReal AngleDelta = 2. * UE_DOUBLE_PI / PointsPerRing;
	FVector::FReal SectionAngle = 0;

	TArray<FNavLocation> Points;
	Points.Reserve(NumberOfRings * PointsPerRing);

	for (int32 SectionIdx = 0; SectionIdx < PointsPerRing; SectionIdx++, SectionAngle += AngleDelta)
	{
		const FVector::FReal SinValue = FMath::Sin(SectionAngle);
		const FVector::FReal CosValue = FMath::Cos(SectionAngle);

		FVector::FReal RingRadius = InnerRadius;
		for (int32 RingIdx = 0; RingIdx < NumberOfRings; RingIdx++, RingRadius += RadiusDelta)
		{
			const FVector RingPos(RingRadius * CosValue, RingRadius * SinValue, 0.);
			const FNavLocation PointPos = FNavLocation(Center + RingPos);
			Points.Add(PointPos);
		}
	}

	ProjectAndFilterNavPoints(Points, DecisionInstance);
	StoreNavPoints(Points, DecisionInstance);
}
