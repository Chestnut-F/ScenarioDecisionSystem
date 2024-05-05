// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioDecisionInstance.h"
#include "ScenarioDecisionNode.h"
#include "ScenarioDecisionGenerator.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SCENARIODECISIONSYSTEM_API UScenarioDecisionGenerator : public UScenarioDecisionNode
{
	GENERATED_BODY()

public:
	void WrappedGenerateItems(UScenarioDecisionInstance& DecisionInstance);

protected:
	virtual void GenerateItems(UScenarioDecisionInstance& DecisionInstance) const { checkNoEntry(); }

	/** project all points in array and remove those outside navmesh */
	void ProjectAndFilterNavPoints(TArray<FNavLocation>& Points, UScenarioDecisionInstance& DecisionInstance) const;

	/** store points as generator's result */
	void StoreNavPoints(const TArray<FNavLocation>& Points, UScenarioDecisionInstance& DecisionInstance) const;

private:
	const ANavigationData* FindNavigationDataForQuery(UScenarioDecisionInstance& DecisionInstance) const;

	void RunNavProjection(const ANavigationData& NavData, const UObject& Querier, TArray<FNavLocation>& Points) const;

protected:
	/** navigation filter for tracing */
	UPROPERTY(EditDefaultsOnly, Category=Trace)
	TSubclassOf<UNavigationQueryFilter> NavigationFilter;

	/** search height: below point */
	UPROPERTY(EditDefaultsOnly, Category=Trace, meta=(UIMin=0, ClampMin=0))
	float ProjectDown = 1024.f;

	/** search height: above point */
	UPROPERTY(EditDefaultsOnly, Category=Trace, meta=(UIMin=0, ClampMin=0))
	float ProjectUp = 1024.f;

	/** shape parameter for trace */
	UPROPERTY(EditDefaultsOnly, Category=Trace, meta=(UIMin=0, ClampMin=0))
	float ExtentX;

	/** shape parameter for trace */
	UPROPERTY(EditDefaultsOnly, Category=Trace, meta=(UIMin=0, ClampMin=0))
	float ExtentY;

	/** shape parameter for trace */
	UPROPERTY(EditDefaultsOnly, Category=Trace, meta=(UIMin=0, ClampMin=0))
	float ExtentZ;

	/** this value will be added to resulting location's Z axis. Can be useful when 
	 *	projecting points to navigation since navmesh is just an approximation of level 
	 *	geometry and items may end up being under collide-able geometry which would 
	 *	for example falsify visibility tests.*/
	UPROPERTY(EditDefaultsOnly, Category=Trace)
	float PostProjectionVerticalOffset;
};


