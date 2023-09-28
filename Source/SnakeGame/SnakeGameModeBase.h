// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameModeBase.generated.h"
class APlayerPawnBase;

enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS()
class SNAKEGAME_API ASnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASnakeGameModeBase();

	APlayerPawnBase* BasePawn;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	EGamePlayState GetCurrentState() const;

	void SetCurrentState(EGamePlayState NewState);


private:
	EGamePlayState CurrentState;
	void HandleNewState(EGamePlayState NewState);

};
