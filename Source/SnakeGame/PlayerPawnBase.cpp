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
#include "ReductionBonus.h"



static FORCEINLINE int32 RandRange(int32 Min, int32 Max)
{
	const int32 Range = (Max - Min) + 1;
	int32 RandomValue = FMath::RandHelper(Range);
	return Min + (RandomValue - RandomValue%60);
}


FTransform FindTransform(ASnakeBase* SnakeActor, int32 FLength, int32 FWidth) {
	bool findLocation;
	FTransform NewTransform;
	do {
		findLocation = true;
		auto X = RandRange(-FWidth, FWidth);
		auto Y = RandRange(-FLength, FLength);
		FVector NewLocation(X, Y, 0);
		NewTransform = FTransform(NewLocation);
		for (auto element : SnakeActor->SnakeElements)
		{
			if (IsValid(element))
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
	return NewTransform;
}
FTransform FindTransform(ASnakeBase* SnakeActor, AFood* SnakeFood,int32 FLength, int32 FWidth) {
	bool findLocation;
	FTransform NewTransform;
	do {
		findLocation = true;
		auto X = RandRange(-FWidth, FWidth);
		auto Y = RandRange(-FLength, FLength);
		FVector NewLocation(X, Y, 0);
		NewTransform = FTransform(NewLocation);
		for (auto element : SnakeActor->SnakeElements)
		{
			if (IsValid(element))
			{
				FVector ElementLocation = element->GetActorLocation();
				FVector FoodLocation = SnakeFood->GetActorLocation();
				if (NewLocation.X >= ElementLocation.X - 50 && NewLocation.X <= ElementLocation.X + 50
					&& NewLocation.X >= FoodLocation.X - 50 && NewLocation.X <= FoodLocation.X + 50)
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
	return NewTransform;
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
	return APlayerPawnBase::ScoreSturct.Score;
}

void APlayerPawnBase::SetPlayerName(FString name)
{
	ScoreSturct.Name = name;
	NameChanged = true;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	;
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
	SnakeActor->CurrentPlayer = this;
}

void APlayerPawnBase::CreateFoodActor() 
{
	SnakeFood = GetWorld()->SpawnActor<AFood>(FoodClass, FindTransform(SnakeActor,FieldLength,FieldWidth));
	SnakeFood->PlayerBase = this;
	int32 ChanceOfBonus = FMath::RandHelper(10);

	if (!IsValid(SpeedBonus)) // Create SpeedBonus
	{
		if ((ChanceOfBonus == 3 || ChanceOfBonus == 2 ) && SnakeActor->MovementSpeed > 0.2f)
		{
			this->CreateSpeedBonusActor();
		}
	}
	if (!IsValid(SpeedAntiBonus)) // Create SpeedBonus
	{
		if (ChanceOfBonus == 1 && SnakeActor->MovementSpeed < 0.8f)
		{
			this->CreateSpeedAntiBonusActor();
		}else if (SnakeActor->MovementSpeed <= 0.3f)
		{
			this->CreateSpeedAntiBonusActor();
		}
	}
	if (!IsValid(ReductionBonus))  // Create ReductionBonus
	{
		if (ChanceOfBonus > 3 && ChanceOfBonus < 7 && SnakeActor->SnakeElements.Num()>3)
		{
			this->CreateReductionBonusActor(false);
		}
	}
	if(!IsValid(ReductionHalfBonus)) // Create ReductionHalfBonus
	{ 
		if ((ChanceOfBonus == 7 || ChanceOfBonus == 8) && SnakeActor->SnakeElements.Num() > 10)
		{
			this->CreateReductionBonusActor(true);
		}
	}
	
}

void APlayerPawnBase::CreateSpeedBonusActor()
{
	SpeedBonus = GetWorld()->SpawnActor<ASpeedBonus>(SpeedBonusClass, FindTransform(SnakeActor,SnakeFood, FieldLength, FieldWidth));
	if(IsValid (SpeedBonus))
	SpeedBonus->PlayerBase = this;
}

void APlayerPawnBase::CreateSpeedAntiBonusActor()
{
	SpeedAntiBonus = GetWorld()->SpawnActor<ASpeedBonus>(SpeedAntiBonusClass, FindTransform(SnakeActor, SnakeFood, FieldLength, FieldWidth));
	if (IsValid(SpeedAntiBonus))
		SpeedAntiBonus->PlayerBase = this;
}

void APlayerPawnBase::CreateReductionBonusActor(bool HalfBonus)
{
	if (HalfBonus) 
	{
		ReductionHalfBonus = GetWorld()->SpawnActor<AReductionBonus>(ReductionHalfBonusClass, FindTransform(SnakeActor, SnakeFood, FieldLength, FieldWidth));
		if(IsValid(ReductionHalfBonus))
		{
			ReductionHalfBonus->PlayerBase = this;
		}
	}
	else
	{
		ReductionBonus = GetWorld()->SpawnActor<AReductionBonus>(ReductionBonusClass, FindTransform(SnakeActor, SnakeFood, FieldLength, FieldWidth));
		if (IsValid(ReductionBonus))
		{
			ReductionBonus->PlayerBase = this;
		}
			
	}
	

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
