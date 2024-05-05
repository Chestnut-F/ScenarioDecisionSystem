// Fill out your copyright notice in the Description page of Project Settings.


#include "ScenarioDecisionItem.h"


void FScenarioDecisionItems::AddZeroed(int32 ValueSize, FScenarioDecisionItemHandle& OutHandle)
{
	Scores.AddZeroed();
	IsDiscarded.AddZeroed();
	const int32 DataOffset = RawData.AddZeroed(ValueSize);
	
	OutHandle.ItemIndex = Size++;
	OutHandle.DataOffset = DataOffset;
}
