// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioDecisionGenerator.h"

#include "NavigationSystem.h"
#include "Algo/RemoveIf.h"
#include "Items/ScenarioDecisionItem_Point.h"

void UScenarioDecisionGenerator::WrappedGenerateItems(UScenarioDecisionInstance& DecisionInstance)
{
	GenerateItems(DecisionInstance);
}

void UScenarioDecisionGenerator::ProjectAndFilterNavPoints(TArray<FNavLocation>& Points,
	UScenarioDecisionInstance& DecisionInstance) const
{
	const ANavigationData* NavData = FindNavigationDataForQuery(DecisionInstance);
	
	const UObject* Querier = DecisionInstance.Owner.Get();
	if (NavData && Querier)
	{
		RunNavProjection(*NavData, *Querier, Points);
	}
	
}

void UScenarioDecisionGenerator::StoreNavPoints(const TArray<FNavLocation>& Points,
	UScenarioDecisionInstance& DecisionInstance) const
{
	DecisionInstance.ReserveItemData(Points.Num());
	for (int32 Idx = 0; Idx < Points.Num(); Idx++)
	{
		// store using default function to handle creating new data entry
		DecisionInstance.AddItemData<UScenarioDecisionItem_Point>(Points[Idx]);
	}
}

const ANavigationData* UScenarioDecisionGenerator::FindNavigationDataForQuery(
	UScenarioDecisionInstance& DecisionInstance) const
{
	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(DecisionInstance.World);
	if (NavSys == nullptr)
	{
		return nullptr;
	}

	// try to match navigation agent for querier
	if (const INavAgentInterface* NavAgent = DecisionInstance.Owner.IsValid() ? Cast<INavAgentInterface>(DecisionInstance.Owner.Get()) : nullptr)
	{
		const FNavAgentProperties& NavAgentProps = NavAgent->GetNavAgentPropertiesRef();
		if (NavAgentProps.IsValid() || NavAgentProps.PreferredNavData.IsValid())
		{
			return NavSys->GetNavDataForProps(NavAgentProps, NavAgent->GetNavAgentLocation());
		}
	}

	return NavSys->GetDefaultNavDataInstance();
}

void UScenarioDecisionGenerator::RunNavProjection(const ANavigationData& NavData, const UObject& Querier, TArray<FNavLocation>& Points) const
{
	FSharedConstNavQueryFilter NavQueryFilter = UNavigationQueryFilter::GetQueryFilter(NavData, &Querier, NavigationFilter);
	TArray<FNavigationProjectionWork> Workload;
	Workload.Reserve(Points.Num());

	if (ProjectDown == ProjectUp)
	{
		for (const auto& Point : Points)
		{
			Workload.Add(FNavigationProjectionWork(Point.Location));
		}
	}
	else
	{
		const FVector VerticalOffset = FVector(0, 0, (ProjectUp - ProjectDown) / 2);
		for (const auto& Point : Points)
		{
			Workload.Add(FNavigationProjectionWork(Point.Location + VerticalOffset));
		}
	}

	const FVector ProjectionExtent(ExtentX, ExtentX, (ProjectDown + ProjectUp) / 2);
	NavData.BatchProjectPoints(Workload, ProjectionExtent, NavQueryFilter);

	for (int32 Idx = Workload.Num() - 1; Idx >= 0; Idx--)
	{
		if (Workload[Idx].bResult)
		{
			Points[Idx] = Workload[Idx].OutLocation;
			Points[Idx].Location.Z += PostProjectionVerticalOffset;
		}
	}
	
	const FNavLocation* PointsBegin = Points.GetData();
	int32 NewNum = Algo::StableRemoveIf(Points, [&Workload, PointsBegin](FNavLocation& Point)
	{
		return !Workload[IntCastChecked<int32>(&Point - PointsBegin)].bResult;
	});
	const bool bAllowShrinking = false;
	Points.SetNum(NewNum, bAllowShrinking);
}
