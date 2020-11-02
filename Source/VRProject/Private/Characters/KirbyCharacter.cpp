// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KirbyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"

// Sets default values
AKirbyCharacter::AKirbyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	KirbyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("KirbyCamera"));
	KirbyCamera->SetupAttachment(VRRoot);

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(VRRoot);
	LeftMotionController->SetTrackingSource(EControllerHand::Left);

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHand->SetupAttachment(LeftMotionController);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(VRRoot);
	RightMotionController->SetTrackingSource(EControllerHand::Right);

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHand->SetupAttachment(RightMotionController);

	TeleportationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportationMarker"));
	TeleportationMarker->SetupAttachment(GetRootComponent());

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportPath"));
	TeleportPath->SetupAttachment(RightMotionController);

	TeleportProjectileSpeed = 700.f;
	TeleportProjectileRadius = 10.f;
	TeleportSimulationTime = 2.f;
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

bool AKirbyCharacter::FindTeleportationDestination(TArray<FVector> &OutPath, FVector &OutLocation)
{
	FVector Start = RightMotionController->GetComponentLocation();
	FVector Look = RightMotionController->GetForwardVector();

	FPredictProjectilePathParams Params(
		TeleportProjectileRadius, 
		Start,
		Look * TeleportProjectileSpeed,
		TeleportSimulationTime,
		ECollisionChannel::ECC_Visibility,
		this
		);
	Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;

	FPredictProjectilePathResult Result;
	bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, Result);

	if (!bHit) return false;

	for (FPredictProjectilePathPointData PointData : Result.PathData)
	{
		OutPath.Add(PointData.Location);
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("No NavSystem"));
		return false;
	}

	FNavLocation NavLocation;
	bool bValidNavProjection = NavSystem->ProjectPointToNavigation(Result.HitResult.Location, NavLocation, TeleportProjectionExtent);

	if (!bValidNavProjection) return false;

	OutLocation = NavLocation.Location;

	return bValidNavProjection && bHit;
}

void AKirbyCharacter::UpdateTeleportSpline(const TArray<FVector> &Path)
{
	TeleportPath->ClearSplinePoints(false);
	for (int i = 0; i < Path.Num(); ++i)
	{
		FVector LocalPos = TeleportPath->GetComponentTransform().InverseTransformPosition(Path[i]);
		FSplinePoint Point(i, LocalPos, ESplinePointType::Curve);
		TeleportPath->AddPoint(Point, false);
	}

	TeleportPath->UpdateSpline();
}

void AKirbyCharacter::UpdateTeleportMarker()
{
	FVector TeleportLocation;
	TArray<FVector> Path;

	if (FindTeleportationDestination(Path, TeleportLocation))
	{
		TeleportationMarker->SetVisibility(true);

		TeleportationMarker->SetWorldLocation(TeleportLocation);

		UpdateTeleportSpline(Path);
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

