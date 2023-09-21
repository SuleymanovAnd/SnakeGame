// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Food.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

static FORCEINLINE int32 RandRange(int32 Min, int32 Max)
{
	const int32 Range = (Max - Min) + 1;
	return Min + FMath::RandHelper(Range);
}

// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	RootComponent = PawnCamera;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(-90, 0, 0));
	CreateSnakeAktor();
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

void APlayerPawnBase::CreateSnakeAktor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeAktorClass, FTransform());
}

void APlayerPawnBase::CreateFoodActor() 
{
	bool findLocation;
	FTransform NewTransform;
	do {
		findLocation = true;
		auto X = RandRange(-500, 500);
		auto Y = RandRange(-500, 500);
		FVector NewLocation(X, Y, 0);
		FTransform TempTransforrm(NewLocation);
		NewTransform = TempTransforrm;
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
}

void APlayerPawnBase::HandlePlayerVerticalInput(float value)
{
	if (IsValid(SnakeActor)) {
		if (value > 0 && SnakeActor->LastMovementDirection != EMovementDirection::DOWN) {
			SnakeActor->LastMovementDirection = EMovementDirection::UP;
		}
		if (value < 0 && SnakeActor->LastMovementDirection != EMovementDirection::UP){
			SnakeActor->LastMovementDirection = EMovementDirection::DOWN;
		}

	}
}

void APlayerPawnBase::HandlePlayerHorizontalInput(float value)
{
	if (IsValid(SnakeActor)) {
		if (value < 0 && SnakeActor->LastMovementDirection != EMovementDirection::LEFT) {
			SnakeActor->LastMovementDirection = EMovementDirection::RIGHT;
		}
		if (value > 0 && SnakeActor->LastMovementDirection != EMovementDirection::RIGHT) {
			SnakeActor->LastMovementDirection = EMovementDirection::LEFT;
		}

	}
}

