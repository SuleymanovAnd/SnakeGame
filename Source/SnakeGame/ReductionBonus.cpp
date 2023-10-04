// Fill out your copyright notice in the Description page of Project Settings.


#include "ReductionBonus.h"
#include "PlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include"SnakeBase.h"

// Sets default values
AReductionBonus::AReductionBonus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AReductionBonus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReductionBonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReductionBonus::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead) {
		auto Player = Cast<APlayerPawnBase>(PlayerBase);
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake)) {
			if (BonusHalf)
			{
				if (IsValid(ReductionBonusEatingSound))
				{
					UGameplayStatics::SpawnSoundAtLocation(this, ReductionBonusEatingSound, GetActorLocation());
				}
				int HalfElement =  (Snake->SnakeElements.Num() / 2);
				Snake->RemoveSnakeElement(HalfElement);
				this->Destroy();
			}
			else {
				if (IsValid(ReductionBonusEatingSound))
				{
					UGameplayStatics::SpawnSoundAtLocation(this, ReductionBonusEatingSound, GetActorLocation());
				}
				Snake->RemoveSnakeElement(2);
				this->Destroy();
			}
		}
	}
}

