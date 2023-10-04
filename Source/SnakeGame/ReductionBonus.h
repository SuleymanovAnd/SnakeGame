// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Sound/SoundBase.h"
#include "ReductionBonus.generated.h"
class APlayerPawnBase;

UCLASS()
class SNAKEGAME_API AReductionBonus : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReductionBonus();
	UPROPERTY(BlueprintReadWrite)
		APlayerPawnBase* PlayerBase;
	UPROPERTY(EditDefaultsOnly)
		bool BonusHalf = false;
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* ReductionBonusEatingSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AActor* Interactor, bool bIsHead) override;
};
