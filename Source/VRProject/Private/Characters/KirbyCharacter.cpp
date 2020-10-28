// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KirbyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AKirbyCharacter::AKirbyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	KirbyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("KirbyCamera"));
	KirbyCamera->SetupAttachment(VRRoot);

	TeleportationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportationMarker"));
	TeleportationMarker->SetupAttachment(GetRootComponent());

	MaxTeleportDistance = 400.f;
	MaxNumOfTeleports = 3;
	CurrentNumOfTeleports = 3;
}

// Called when the game starts or when spawned
void AKirbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	TeleportationMarker->SetVisibility(false);
}

// Called every frame
void AKirbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Sync colliders with playspace location
	FVector NewCameraOffset = KirbyCamera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);

}

// Called to bind functionality to input
void AKirbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Pressed, this, AKirbyCharacter::Teleport);
	PlayerInputComponent->BindAction(TEXT("CancelTeleport"), IE_Pressed, this, AKirbyCharacter::Teleport);

}

void AKirbyCharacter::Teleport()
{
	if (!bTeleporting && CurrentNumOfTeleports > 0)
	{
		GetWorldTimerManager().SetTimer(TeleportUpdateLocationHandle, this, &AKirbyCharacter::UpdateTeleportMarker, 0.02f, true);
		bTeleporting = true;
	}
	else
	{
		CurrentNumOfTeleports--;
		SetActorLocation(TeleportationMarker->GetComponentLocation());
		bTeleporting = false;
		TeleportationMarker->SetVisibility(false);
		GetWorldTimerManager().ClearTimer(TeleportUpdateLocationHandle);
	}

}

void AKirbyCharacter::CancelTeleport()
{
	bTeleporting = false;
	TeleportationMarker->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(TeleportUpdateLocationHandle);
}

void AKirbyCharacter::UpdateTeleportMarker()
{
	FHitResult HitResult;
	FVector Start = KirbyCamera->GetComponentLocation();
	FVector End = Start + KirbyCamera->GetForwardVector() * MaxTeleportDistance;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	if (bHit)
	{
		TeleportationMarker->SetVisibility(true);
		TeleportationMarker->SetWorldLocation(HitResult.Location);
	}
	else
	{
		TeleportationMarker->SetVisibility(false);
	}
}

