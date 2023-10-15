// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHUD.h"
#include "Runtime\CoreUObject\Public\UObject\ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "SnakeBase.h"
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
void ASnakeHUD::ShowScoreMenu()
{
	if (IsValid(EndMenuWidget))
	{
		EndMenuWidget->RemoveFromViewport();
	}
	if (!IsValid(CurrentWidget))
	{
		CurrentWidget = CreateWidget <UUserWidget>(GetWorld(), HUDWidgetClass);
	}
	else 
	{
		CurrentWidget->AddToViewport();
		FInputModeGameOnly InputMode;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	}
}
void ASnakeHUD::ShowEndMenu() 
{
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->RemoveFromViewport();
	}
	if (GameMode->BasePawn->NameChanged) 
	{
		GameMode->BasePawn->SnakeActor->SaveScore(); 
	}
	if (IsValid(EndMenuWidgetClass))
	{
		if (!IsValid(EndMenuWidget))
		{
			EndMenuWidget = CreateWidget <UUserWidget>(GetWorld(), EndMenuWidgetClass);
		}
		
		if (IsValid(EndMenuWidget))
		{
			EndMenuWidget->AddToViewport();
			
			APlayerController* Controller = Cast<APlayerController>(GameMode->BasePawn->GetController());
			FInputModeUIOnly UiOnlyMode;
			EndMenuWidget->SetCursor(Controller->GetMouseCursor());
			UiOnlyMode.SetWidgetToFocus(EndMenuWidget->TakeWidget());
			UiOnlyMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			Controller->SetInputMode(UiOnlyMode);
			Controller->bShowMouseCursor = true;
			

		}
	}
}