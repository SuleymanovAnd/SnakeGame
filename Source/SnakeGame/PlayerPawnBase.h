// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnBase.generated.h"
class UCameraComponent;
class ASnakeBase;
class AFood;
class ASpeedBonus;

UCLASS()
class SNAKEGAME_API APlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawnBase();
	UPROPERTY(BlueprintReadWrite)
		UCameraComponent* PawnCamera;
	UPROPERTY(BlueprintReadWrite)
		ASnakeBase* SnakeActor;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ASnakeBase> SnakeAktorClass;
	UPROPERTY(BlueprintReadWrite)
		AFood* SnakeFood;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AFood> FoodClass;
	UPROPERTY(BlueprintReadWrite)
		ASpeedBonus* SpeedBonus;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ASpeedBonus> SpeedBonusClass;
	UPROPERTY(BlueprintReadOnly)
		int32 Score;
	UFUNCTION(BlueprintPure, Category = "Health")
		int32 GetScore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void CreateSnakeActor();
	void CreateFoodActor();
	void CreateSpeedBonusActor();
	UFUNCTION()
		void HandlePlayerVerticalInput(float value);
	UFUNCTION()
		void HandlePlayerHorizontalInput(float value);
};
