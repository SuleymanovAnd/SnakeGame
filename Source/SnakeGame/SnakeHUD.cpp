// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHUD.h"
#include "Runtime\CoreUObject\Public\UObject\ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


ASnakeHUD::ASnakeHUD() {
	 static ConstructorHelpers::FClassFinder<UUserWidget> ScoreBarObj(TEXT("/Game/UI/UI_ScoreWidget"));

	 HUDWidgetClass = ScoreBarObj.Class;
}


void ASnakeHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ASnakeHUD::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(HUDWidgetClass)) {
		CurrentWidget = CreateWidget <UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget) {
			CurrentWidget->AddToViewport();
		}
	}
}
