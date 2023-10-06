// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeHUD.h"
#include "PlayerPawnBase.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "SnakeBase.h"

ASnakeGameModeBase::ASnakeGameModeBase() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
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

void ASnakeGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (BasePawn->SnakeActor->SnakeDestroy && !EndGame) // End Game Delay
	{
		GetWorldTimerManager().SetTimer(Timer, 3, false);
		EndGame = true;
	}
	if (!GetWorldTimerManager().IsTimerActive(Timer) && BasePawn->SnakeActor->SnakeDestroy) {
		SetCurrentState(EGamePlayState::EGameOver);
	}
	
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
	case EGamePlayState::EUnknown:
	{
		
	}break;
	}
}
