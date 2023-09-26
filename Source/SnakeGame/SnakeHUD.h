// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SnakeHUD.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API ASnakeHUD : public AHUD
{
	GENERATED_BODY()
public:
	ASnakeHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf <class UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere)
		class UUserWidget* CurrentWidget;
	
};
