// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Food.generated.h"
class APlayerPawnBase;

UCLASS()
class SNAKEGAME_API AFood : public AActor ,public IInteractable
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite)
		APlayerPawnBase* PlayerBase;

	// Sets default values for this actor's properties
	AFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AActor* Interactor, bool bIsHead) override;

};
