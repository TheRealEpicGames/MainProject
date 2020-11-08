// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GameplayActors/KirbyHandController.h"
#include "Items/Item.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Haptics/HapticFeedbackEffect_Base.h"


// Sets default values
AKirbyHandController::AKirbyHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);

	GripState = EGripState::EGS_Open;
}

void AKirbyHandController::SetGrabbableItem(AItem* Item)
{
	GrabbableItem = Item; 
	if (GrabbableItem)
	{
		GripState = EGripState::EGS_CanGrab;
	}
	else
	{
		GripState = EGripState::EGS_Open;
	}
}

void AKirbyHandController::GrabItem(AItem* Item)
{
	//TODO: Destroy Item, then add to inventory.
}

// Called when the game starts or when spawned
void AKirbyHandController::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AKirbyHandController::HandBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AKirbyHandController::HandEndOverlap);
}

// Called every frame
void AKirbyHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKirbyHandController::HandBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//TODO: Query for Items for (Grab item on field functionality)
	AItem* Item = Cast<AItem>(OtherActor);
	if (Item)
	{
		APawn* Pawn = Cast<APawn>(GetAttachParentActor());
		if (Pawn)
		{
			APlayerController* Controller = Cast<APlayerController>(Pawn->GetController());
			if (Controller)
			{
				Controller->PlayHapticEffect(GrabHapticEffect, MotionController->GetTrackingSource());
				SetGrabbableItem(Item);
			}
		}
	}
}

void AKirbyHandController::HandEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AItem* Item = Cast<AItem>(OtherActor);
	if (Item)
	{
		SetGrabbableItem(nullptr);
	}
}

