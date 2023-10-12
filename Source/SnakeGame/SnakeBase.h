// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "PlayerPawnBase.h"
#include "SnakeHUD.h"
#include "GameFramework/SaveGame.h"
#include "SnakeBase.generated.h"
class ASnakeElementBase;
class UScoreSave;
class ASnakeHUD;

UENUM()
enum class EMovementDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

UCLASS()
class SNAKEGAME_API ASnakeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ASnakeElementBase> SnakeElementClass;
	UPROPERTY(BlueprintReadOnly)
		TArray <ASnakeElementBase*> SnakeElements;
	UPROPERTY(EditDefaultsOnly)
		float MovementSpeed;
	UPROPERTY(EditDefaultsOnly)
		float ElementSize;
	UPROPERTY()
		EMovementDirection LastMovementDirection;
	UPROPERTY(EditAnywhere, category = "Sound")
		USoundBase* MovementSound;
	UPROPERTY(EditAnywhere, category = "Sound")
		USoundBase* DestroySnakeSound;
	UPROPERTY(BlueprintReadWrite, category = "Save")
		UScoreSave* SaveSlot;

	bool TikAxisChanged = false; // limitation on accepting keyboard events for snake movement

	FTimerHandle Timer; // timer for end game

	ASnakeGameModeBase* CurrentGameMode;

	APlayerPawnBase* CurrentPlayer;

	UPROPERTY(BlueprintReadOnly)
	bool SnakeDestroy = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddSnakeElement(int ElementsNum = 1);

	void RemoveSnakeElement(int Count);

	void Move();

	void DestroySnake();

	UFUNCTION()
		void SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other);
	UFUNCTION(BlueprintPure)
		float GetMovementSpeed();
	UFUNCTION(BlueprintCallable)
	void SetEndGame();
	UFUNCTION(BlueprintPure)
		UUserWidget* GetEndWidget();

	bool FillSaveSlot(UScoreSave* SaveSlot);

};
