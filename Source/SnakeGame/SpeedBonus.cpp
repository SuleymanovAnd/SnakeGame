// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBonus.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include"SnakeBase.h"


// Sets default values
ASpeedBonus::ASpeedBonus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpeedBonus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpeedBonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpeedBonus::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead) {
		auto Player = Cast<APlayerPawnBase>(PlayerBase);
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake)) {
			if (IsValid(SpeedBonusEatingSound))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, SpeedBonusEatingSound, GetActorLocation());
			}
			Snake->MovementSpeed -= 0.1f;
			Snake->SetActorTickInterval(Snake->MovementSpeed);
			this->Destroy();
		}
	}
	else
	{
		auto Player = Cast<APlayerPawnBase>(PlayerBase);
		this->Destroy();
	}
}


