// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Food.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpeedBonus.h"

static FORCEINLINE int32 RandRange(int32 Min, int32 Max)
{
	const int32 Range = (Max - Min) + 1;
	int32 RandomValue = FMath::RandHelper(Range);
	return Min + (RandomValue - RandomValue%60);
}

// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	RootComponent = PawnCamera;
}

int32 APlayerPawnBase::GetScore()
{
	return APlayerPawnBase::Score;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	Score = 0;
	SetActorRotation(FRotator(-90, 0, 0));
	CreateSnakeActor();
	CreateFoodActor();
	FInputModeGameOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
}

// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	
}

// Called to bind functionality to input
void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Vertical",this,&APlayerPawnBase::HandlePlayerVerticalInput);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawnBase::HandlePlayerHorizontalInput);
}

void APlayerPawnBase::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeAktorClass, FTransform());
}

void APlayerPawnBase::CreateFoodActor() 
{
	bool findLocation;
	FTransform NewTransform;
	do {
		findLocation = true;
		auto X = RandRange(-480, 480);
		auto Y = RandRange(-480, 480);
		FVector NewLocation(X, Y, 0);
		NewTransform = FTransform(NewLocation);
		for (auto element : SnakeActor->SnakeElements)
		{
			if(IsValid(element))
			{ 
			FVector ElementLocation = element->GetActorLocation();
			if (NewLocation.X >= ElementLocation.X - 50 && NewLocation.X <= ElementLocation.X + 50) 
			{
				if (NewLocation.Y >= ElementLocation.Y - 50 && NewLocation.Y <= ElementLocation.Y + 50)
				{
					findLocation = false;
				}
			}
			}
		}
	} while (!findLocation);
	
	SnakeFood = GetWorld()->SpawnActor<AFood>(FoodClass, NewTransform);
	SnakeFood->PlayerBase = this;
	if (!IsValid(SpeedBonus))
	{
		int32 ChanceOfBonus = FMath::RandHelper(10);
		if (ChanceOfBonus == 3  && SnakeActor->MovementSpeed > 0.2f)
		{
			this->CreateSpeedBonusActor();
		}
	}
	
}

void APlayerPawnBase::CreateSpeedBonusActor()
{
	bool findLocation;
	FTransform NewTransform;
	do {
		findLocation = true;
		auto X = RandRange(-480, 480);
		auto Y = RandRange(-480, 480);
		FVector NewLocation(X, Y, 0);
		NewTransform = FTransform(NewLocation);
		for (auto element : SnakeActor->SnakeElements)
		{
			if (IsValid(element))
			{
				FVector ElementLocation = element->GetActorLocation();
				FVector FoodLocation = SnakeFood->GetActorLocation();
				if (NewLocation.X >= ElementLocation.X - 50 && NewLocation.X <= ElementLocation.X + 50 
					&& NewLocation.X >= FoodLocation.X -50 && NewLocation.X <= FoodLocation.X + 50)
				{
					if (NewLocation.Y >= ElementLocation.Y - 50 && NewLocation.Y <= ElementLocation.Y + 50
						&& NewLocation.Y >= FoodLocation.Y - 50 && NewLocation.Y <= FoodLocation.Y + 50)
					{
						findLocation = false;
					}
				}
			}
		}
	} while (!findLocation);
	SpeedBonus = GetWorld()->SpawnActor<ASpeedBonus>(SpeedBonusClass, NewTransform);
	SpeedBonus->PlayerBase = this;
}

void APlayerPawnBase::HandlePlayerVerticalInput(float value)
{
	if (IsValid(SnakeActor)) {
		if (value > 0 && SnakeActor->LastMovementDirection != EMovementDirection::DOWN && SnakeActor->TikAxisChanged) {
			SnakeActor->LastMovementDirection = EMovementDirection::UP;
			SnakeActor->TikAxisChanged = false;
		}
		if (value < 0 && SnakeActor->LastMovementDirection != EMovementDirection::UP && SnakeActor->TikAxisChanged){
			SnakeActor->LastMovementDirection = EMovementDirection::DOWN;
			SnakeActor->TikAxisChanged = false;
		}

	}
}

void APlayerPawnBase::HandlePlayerHorizontalInput(float value)
{
	if (IsValid(SnakeActor)) {
		if (value < 0 && SnakeActor->LastMovementDirection != EMovementDirection::LEFT && SnakeActor->TikAxisChanged) {
			SnakeActor->LastMovementDirection = EMovementDirection::RIGHT;
			SnakeActor->TikAxisChanged = false;
		}
		if (value > 0 && SnakeActor->LastMovementDirection != EMovementDirection::RIGHT && SnakeActor->TikAxisChanged) {
			SnakeActor->LastMovementDirection = EMovementDirection::LEFT;
			SnakeActor->TikAxisChanged = false;
		}

	}
}

