// Fill out your copyright notice in the Description page of Project Settings.


#include "ScenarioDecision.h"

#include "Analyzers/ScenarioDecisionAnalyzer_Distance.h"
#include "Generators/ScenarioDecisionGenerator_Donut.h"
#include "Items/ScenarioDecisionItem_Point.h"

UScenarioDecision::UScenarioDecision(const FObjectInitializer& ObjectInitializer)
{
	ItemType = UScenarioDecisionItem_Point::StaticClass();
	Generator = CreateDefaultSubobject<UScenarioDecisionGenerator_Donut>(TEXT("Generator"));
	Analyzers.Emplace(NewObject<UScenarioDecisionAnalyzer_Distance>());
}
