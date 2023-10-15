// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeHUD.h"
#include "PlayerPawnBase.h"
#include "UObject/ConstructorHelpers.h"
#include "SnakeBase.h"

ASnakeGameModeBase::ASnakeGameModeBase() : Super()
{
	HUDClass = ASnakeHUD::StaticClass();
	ConstructorHelpers::FClassFinder <APlayerPawnBase> PlayerFinder (TEXT("/Game/Blueprints/BP_PlayerPawn"));
	DefaultPawnClass = PlayerFinder.Class;
}

void ASnakeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentState(EGamePlayState::EPlaying);
	BasePawn = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(this, 0));
}

EGamePlayState ASnakeGameModeBase::GetCurrentState() const
{
	return CurrentState;
}

void ASnakeGameModeBase::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void ASnakeGameModeBase::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
		{
		if (UGameplayStatics::IsGamePaused(this))
		{
			if (IsValid(SnakeHud))
			{
				SnakeHud->ShowScoreMenu();
			}
			UGameplayStatics::SetGamePaused(this, false);
		}
		} break;
	case EGamePlayState::EGameOver:
	{
		if (IsValid(SnakeHud))
		{
			SnakeHud->ShowEndMenu();
		}
		else
		{
			UGameplayStatics::OpenLevel(this, TEXT("LEVEL1"), false);
		}
		
		
	} break;
	default:
	case EGamePlayState::EPause:
	{
		if (!UGameplayStatics::IsGamePaused(this)) 
		{
			UGameplayStatics::SetGamePaused(this, true);
			if (IsValid(SnakeHud))
			{
				SnakeHud->ShowEndMenu();
			}
		}
		
	}break;
	}
}
