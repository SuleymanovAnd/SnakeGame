// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnBase.generated.h"
class UCameraComponent;
class ASnakeBase;
class AFood;
class ASpeedBonus;
class AReductionBonus;

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
	UPROPERTY(BlueprintReadWrite)
		AReductionBonus* ReductionBonus;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AReductionBonus> ReductionBonusClass;
	UPROPERTY(BlueprintReadWrite)
		AReductionBonus* ReductionHalfBonus;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AReductionBonus> ReductionHalfBonusClass;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
		int32 Score;
	/** Half the full width of the field is indicated. (extreme limit relative to zero). The value must be divisible by 60 without a remainder. */
	UPROPERTY(EditAnywhere, Category = "Field")
		int32 FieldWidth = 480;
	/** Half the full length of the field is indicated. (extreme limit relative to zero). The value must be divisible by 60 without a remainder. */
	UPROPERTY(EditAnywhere, Category = "Field")
		int32 FieldLength = 480;

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
	void CreateReductionBonusActor(bool HalfBonus);

	UFUNCTION()
		void HandlePlayerVerticalInput(float value);
	UFUNCTION()
		void HandlePlayerHorizontalInput(float value);
	UFUNCTION(BlueprintPure, Category = "Score")
		int32 GetScore();
};
