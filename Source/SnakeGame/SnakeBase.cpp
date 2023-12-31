// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeGameModeBase.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "ScoreSave.h"
#include "SnakeHUD.h"
#include "Interactable.h"

void ASnakeBase::SetEndGame() {
	if (SnakeDestroy) {
		CurrentGameMode->SetCurrentState(EGamePlayState::EGameOver);
	}
}
// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 1.f;
	LastMovementDirection = EMovementDirection::DOWN;
	CurrentGameMode = Cast <ASnakeGameModeBase>(UGameplayStatics::GetGameMode(this));
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
	SaveSlot = Cast<UScoreSave>(UGameplayStatics::LoadGameFromSlot(TEXT("ScoreSave"), 0));
	if (!IsValid(SaveSlot) )
	{
		SaveSlot = Cast <UScoreSave>(UGameplayStatics::CreateSaveGameObject(UScoreSave::StaticClass()));
	}
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	Move();
	TikAxisChanged = true;

}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; i++) {
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform;
		if (SnakeElements.Num()>1) 
		{ 
			FVector LastLocation = SnakeElements.Last()->GetActorLocation(); 
			NewTransform = FTransform(LastLocation);
		}
		else
		{ 
			NewTransform = FTransform(NewLocation);
		}
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0) {
			NewSnakeElem->SetFirstElementType();
			NewSnakeElem->MeshComponent->SetVisibility(1);
		}
	}
}

void ASnakeBase::RemoveSnakeElement(int Count)
{
	int SnakeLength = SnakeElements.Num() - 1;
	for (int i = SnakeLength; i > (SnakeLength - Count); i--)
	{
		auto LastElement = SnakeElements.Last();
		SnakeElements.RemoveAt(i);
		if (IsValid(LastElement))
		{
			LastElement->Destroy();
		}
	}
	

}

void ASnakeBase::Move()
{
	if (!SnakeDestroy)
	{
		FVector MovementVector(ForceInitToZero);

		switch (LastMovementDirection) {
		case EMovementDirection::UP:
			MovementVector.X += ElementSize;
			break;
		case EMovementDirection::DOWN:
			MovementVector.X -= ElementSize;
			break;
		case EMovementDirection::LEFT:
			MovementVector.Y -= ElementSize;
			break;
		case EMovementDirection::RIGHT:
			MovementVector.Y += ElementSize;
			break;

		}
		if (IsValid(MovementSound))
		{
			UGameplayStatics::SpawnSoundAtLocation(this, MovementSound, GetActorLocation());
		}

		TikAxisChanged = true;
		SnakeElements[0]->ToggleCollision();

		for (int i = SnakeElements.Num() - 1; i > 0; i--) {
			auto CurrentElement = SnakeElements[i];
			auto PrevElement = SnakeElements[i - 1];
			FVector PrevLocation = PrevElement->GetActorLocation();
			CurrentElement->MeshComponent->SetVisibility(1);
			CurrentElement->SetActorLocation(PrevLocation);
		}
		SnakeElements[0]->AddActorWorldOffset(MovementVector);
		SnakeElements[0]->ToggleCollision();
	}
}

UUserWidget* ASnakeBase::GetEndWidget() 
{
	return CurrentGameMode->SnakeHud->EndMenuWidget;
}

bool ASnakeBase::FillSaveSlot(UScoreSave* SaveSlot)
{
	// Find place in SaveClot
	CurrentPlace = 0;
	for (FScoreStruct FStruct : SaveSlot->ScoreArr)
	{
		if (FStruct.Score < CurrentPlayer->ScoreSturct.Score) {
			CurrentPlace = FStruct.Place;
			break;
		}
	}
	// Add Score and Name in Slot
	if (CurrentPlace)
	{
		CurrentPlayer->SaveScore = true;
		APlayerController* Controller = Cast<APlayerController>(CurrentPlayer->GetController());
		FInputModeUIOnly UiOnlyMode;
		UiOnlyMode.SetWidgetToFocus(CurrentGameMode->SnakeHud->CurrentWidget->TakeWidget());
		UiOnlyMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Controller->SetInputMode(UiOnlyMode);
		Controller->bShowMouseCursor = true;
		//
		// Waiting player name and SaveScore
		//
		return true;
	}
	else 
	{
		return false;
	}
}

void ASnakeBase::SaveScore()
{
	CurrentPlayer->ScoreSturct.Place = CurrentPlace;

	SaveSlot->ScoreArr.EmplaceAt((CurrentPlace - 1), CurrentPlayer->ScoreSturct);
	if (SaveSlot->ScoreArr.Num() > 10)
	{
		for (int i = (SaveSlot->ScoreArr.Num()-1); i >= 10; i--)
		{
			SaveSlot->ScoreArr.RemoveAt(i);
		}
		
	}
	for (int32 i = (CurrentPlace - 1); i < SaveSlot->ScoreArr.Num(); i++)
	{
		SaveSlot->ScoreArr[i].Place = (i + 1);
	}
	UGameplayStatics::SaveGameToSlot(SaveSlot, TEXT("ScoreSave"), 0);
}

void ASnakeBase::DestroySnake()
{
	if (IsValid(DestroySnakeSound)) 
	{
		UGameplayStatics::SpawnSoundAtLocation(this, DestroySnakeSound, GetActorLocation());
	}
		SnakeDestroy = true;
		SaveSlot = Cast<UScoreSave>(UGameplayStatics::LoadGameFromSlot(TEXT("ScoreSave"), 0));
		bool NewScore = false;
		if (IsValid(SaveSlot)) {
			NewScore = FillSaveSlot(SaveSlot);
		}
		else {
			// create SaveSlot
			SaveSlot = Cast <UScoreSave>(UGameplayStatics::CreateSaveGameObject(UScoreSave::StaticClass()));
			NewScore = FillSaveSlot(SaveSlot);
		}
		
		if (!NewScore) {
			GetWorld()->GetTimerManager().SetTimer(Timer, this, &ASnakeBase::SetEndGame, 3, false);// End Game Delay
		}
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElementIndex;
		SnakeElements.Find(OverlappedElement, ElementIndex);
		bool bIsFirs = ElementIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, bIsFirs);
		}
	}
}

float ASnakeBase::GetMovementSpeed()
{
	return MovementSpeed;
}

