// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KirbyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"

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

	MaxTeleportDistance = 3000.f;
	MaxNumOfTeleports = 3;
	CurrentNumOfTeleports = 3;
	TeleportFadeDuration = 1;
	TeleportProjectionExtent = FVector(100, 100, 100);
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

	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Pressed, this, &AKirbyCharacter::BeginTeleport);
	PlayerInputComponent->BindAction(TEXT("CancelTeleport"), IE_Pressed, this, &AKirbyCharacter::CancelTeleport);

}

void AKirbyCharacter::BeginTeleport()
{
	if (!bTeleporting && CurrentNumOfTeleports > 0)
	{
		GetWorldTimerManager().SetTimer(TeleportUpdateLocationHandle, this, &AKirbyCharacter::UpdateTeleportMarker, 0.02f, true);
		bTeleporting = true;
	}
	else if(bTeleporting)
	{
		StartFade(0, 1);

		FTimerHandle FadeHandle;
		GetWorldTimerManager().SetTimer(FadeHandle, this, &AKirbyCharacter::EndTeleport, TeleportFadeDuration);
	}

}

void AKirbyCharacter::EndTeleport()
{
	CurrentNumOfTeleports--;

	FVector ZOffset(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	SetActorLocation(TeleportationMarker->GetComponentLocation() + ZOffset);
	bTeleporting = false;
	TeleportationMarker->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(TeleportUpdateLocationHandle);

	StartFade(1, 0);
}

void AKirbyCharacter::CancelTeleport()
{
	bTeleporting = false;
	TeleportationMarker->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(TeleportUpdateLocationHandle);
}

bool AKirbyCharacter::FindTeleportationDestination(FVector& OutLocation)
{
	FVector Start = KirbyCamera->GetComponentLocation();
	FVector End = Start + KirbyCamera->GetForwardVector() * MaxTeleportDistance;

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	if (!bHit) return false;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("No NavSystem"));
		return false;
	}

	FNavLocation NavLocation;
	bool bValidNavProjection = NavSystem->ProjectPointToNavigation(HitResult.Location, NavLocation, TeleportProjectionExtent);

	if (!bValidNavProjection) return false;

	OutLocation = NavLocation.Location;

	return bValidNavProjection && bHit;
}

void AKirbyCharacter::UpdateTeleportMarker()
{
	FVector TeleportLocation;

	if (FindTeleportationDestination(TeleportLocation))
	{
		TeleportationMarker->SetVisibility(true);

		TeleportationMarker->SetWorldLocation(TeleportLocation);
	}
	else
	{
		TeleportationMarker->SetVisibility(false);
	}
}

void AKirbyCharacter::StartFade(float FromAlpha, float ToAlpha)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->StartCameraFade(FromAlpha, ToAlpha, TeleportFadeDuration, FLinearColor::Black);
	}
}

