// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHUD.h"
#include "Runtime\CoreUObject\Public\UObject\ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "SnakeGameModeBase.h"
#include "PlayerPawnBase.h"

ASnakeHUD::ASnakeHUD() {
	 static ConstructorHelpers::FClassFinder<UUserWidget> ScoreBarObj(TEXT("/Game/UI/UI_ScoreWidget"));
	 static ConstructorHelpers::FClassFinder<UUserWidget> EndMenuObj(TEXT("/Game/UI/UI_EndMenu"));
	 HUDWidgetClass = ScoreBarObj.Class;
	 EndMenuWidgetClass = EndMenuObj.Class;
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

	GameMode = Cast <ASnakeGameModeBase> (UGameplayStatics::GetGameMode(this));

	if (IsValid(GameMode))
	{
		GameMode->SnakeHud = this;
	}
}
void ASnakeHUD::ShowEndMenu() 
{
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->RemoveFromViewport();
	}
	if (IsValid(EndMenuWidgetClass))
	{
		EndMenuWidget = CreateWidget <UUserWidget>(GetWorld(), EndMenuWidgetClass);
		if (IsValid(EndMenuWidget))
		{
			EndMenuWidget->AddToViewport();
			
			APlayerController* Controller = Cast<APlayerController>(GameMode->BasePawn->GetController());
			FInputModeUIOnly UiOnlyMode;
			UiOnlyMode.SetWidgetToFocus(EndMenuWidget->TakeWidget());
			UiOnlyMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			Controller->SetInputMode(UiOnlyMode);
			Controller->bShowMouseCursor = true;
		
		}
	}
}