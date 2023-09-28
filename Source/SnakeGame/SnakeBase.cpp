// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 1.f;
	LastMovementDirection = EMovementDirection::DOWN;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
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

void ASnakeBase::Move()
{
	
	FVector MovementVector(ForceInitToZero);

	switch (LastMovementDirection) {
	case EMovementDirection::UP :
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::DOWN :
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::LEFT :
		MovementVector.Y -= ElementSize;
		break;
	case EMovementDirection::RIGHT :
		MovementVector.Y += ElementSize;
		break;

	}
	//AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();

	for (int i = SnakeElements.Num() - 1; i > 0; i--) {
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i-1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->MeshComponent->SetVisibility(1); 
		CurrentElement->SetActorLocation(PrevLocation);
		}
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
}

void ASnakeBase::DestroySnake()
{
		MovementSpeed = 10000.f;
		SetActorTickInterval(MovementSpeed);
		SnakeDestroy = true;
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

