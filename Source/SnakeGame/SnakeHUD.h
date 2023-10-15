// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SnakeHUD.generated.h"
class ASnakeGameModeBase;
class UUserWidget;

UCLASS()
class SNAKEGAME_API ASnakeHUD : public AHUD
{
	GENERATED_BODY()
public:

	ASnakeHUD();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, category = "Widget")
		TSubclassOf <UUserWidget> HUDWidgetClass;
	UPROPERTY()
		UUserWidget* CurrentWidget;
	UPROPERTY(EditAnywhere, category = "Widget")
		TSubclassOf <UUserWidget> EndMenuWidgetClass;
	UPROPERTY()
		UUserWidget* EndMenuWidget;
	UPROPERTY(EditAnywhere)
		ASnakeGameModeBase* GameMode;

	
	void ShowEndMenu();
	void ShowScoreMenu();
};
