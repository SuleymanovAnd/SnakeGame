// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerPawnBase.h"
#include "ScoreSave.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API UScoreSave : public USaveGame
{
	GENERATED_BODY()
public:
	UScoreSave();

	UPROPERTY(EditAnywhere)
		TArray <FScoreStruct> ScoreArr;
};
