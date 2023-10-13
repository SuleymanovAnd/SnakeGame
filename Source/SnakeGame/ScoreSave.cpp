// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSave.h"

UScoreSave::UScoreSave()
{
	for (int32 i = 1; i <= 10; i++)
	{
		ScoreArr.Add(FScoreStruct(i, TEXT("Plr"), 100 / i)); 
	}
	
}

TArray <FScoreStruct> UScoreSave:: GetScoreArr()
{
	return ScoreArr;
}
