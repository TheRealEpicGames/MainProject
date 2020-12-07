// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GameplayActors/KirbyHandController.h"
#include "Items/Item.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Haptics/HapticFeedbackEffect_Base.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/GameplayActors/Animations/KirbyHandAnimInstance.h"
#include "Components/SphereComponent.h"
#include "InputCoreTypes.h"
#include "Characters/KirbyCharacter.h"
#include "Components/WidgetComponent.h"


// Sets default values
AKirbyHandController::AKirbyHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);

	BaseCollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("BaseCollisionVolume"));
	BaseCollisionVolume->SetupAttachment(GetRootComponent());

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(GetRootComponent());

	Pointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Pointer"));
	Pointer->SetupAttachment(GetRootComponent());

	InventoryMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("InventoryMenu"));
	InventoryMenu->SetupAttachment(GetRootComponent());

	SetGripState(EGripState::EGS_Open);
	bPointerActive = false;
	bInventoryMenuActive = false;
}

void AKirbyHandController::SetGrabbableItem(AItem* Item)
{
	GrabbableItem = Item; 
	if (GrabbableItem)
	{
		SetGripState(EGripState::EGS_CanGrab);
	}
	else
	{
		SetGripState(EGripState::EGS_Open);
	}
}

void AKirbyHandController::SetGrabbedItem(AItem* Item)
{
	GrabbedItem = Item;
	if (GrabbedItem)
	{
		SetGripState(EGripState::EGS_Grab);
	}
	else
	{
		SetGripState(EGripState::EGS_Open);
	}
}

void AKirbyHandController::GrabItem(AItem* Item)
{
	SetGrabbedItem(Item);

	Item->Grab(this);
}

void AKirbyHandController::ReleaseItem(AItem* Item)
{
	SetGrabbedItem(nullptr);

	Item->Release(this);
}

void AKirbyHandController::UseItem(AItem* Item)
{
	Item->Use(this);
}

// Called when the game starts or when spawned
void AKirbyHandController::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AKirbyHandController::HandBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AKirbyHandController::HandEndOverlap);

	Pointer->SetActive(false);

	InventoryMenu->SetActive(false);
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

void AKirbyHandController::UpdateAnimState()
{
	UKirbyHandAnimInstance* AnimInstance = Cast<UKirbyHandAnimInstance>(HandMesh->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->GripState = GripState;
	}
}

void AKirbyHandController::UsePointer()
{
	if (!bPointerActive)
	{
		Pointer->SetActive(true);
		bPointerActive = true;
	}
	else
	{
		Pointer->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void AKirbyHandController::ReleasePointer()
{
	if (bPointerActive)
	{
		Pointer->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

void AKirbyHandController::DeactivatePointer()
{
	Pointer->ReleasePointerKey(EKeys::LeftMouseButton);
	bPointerActive = false;
	Pointer->SetActive(false);
}

void AKirbyHandController::OpenInventoryMenu()
{
	InventoryMenuOpened();
	InventoryMenu->SetActive(true);
}

void AKirbyHandController::CloseInventoryMenu()
{
	InventoryMenu->SetActive(false);
}

