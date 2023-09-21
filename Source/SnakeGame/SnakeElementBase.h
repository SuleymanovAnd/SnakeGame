// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "SnakeElementBase.generated.h"
class UStaticMeshComponent;
class ASnakeBase;

UCLASS()
class SNAKEGAME_API ASnakeElementBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeElementBase();
	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent;	

	UPROPERTY()
		ASnakeBase* SnakeOwner; 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void SetFirstElementType();
	void SetFirstElementType_Implementation();
	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	UFUNCTION()
		void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void ToggleCollision();
};
