// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "SnakeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawnBase.h"
// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFood::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead) {
		auto Player = Cast<APlayerPawnBase>(PlayerBase);
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake)) {
			if (IsValid(FoodEatingSound))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, FoodEatingSound, GetActorLocation());
			}
			Player->CreateFoodActor();
			Player->ScoreSturct.Score++;
			Snake->AddSnakeElement();
			this->Destroy();
		}
	}
	else 
	{
		auto Player = Cast<APlayerPawnBase>(PlayerBase);
			Player->CreateFoodActor();
		this->Destroy();
	}
}
