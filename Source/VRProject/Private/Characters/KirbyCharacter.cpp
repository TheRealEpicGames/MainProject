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
#include "Components/SplineMeshComponent.h"
#include "Characters/GameplayActors/KirbyHandController.h"
#include "HeadMountedDisplayFunctionLibrary.h"


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

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportPath"));
	TeleportPath->SetupAttachment(VRRoot);

	TeleportProjectileSpeed = 700.f;
	TeleportProjectileRadius = 10.f;
	TeleportSimulationTime = 2.f;
	MaxNumOfTeleports = 3;
	CurrentNumOfTeleports = 3;
	TeleportFadeDuration = 1;
	TeleportProjectionExtent = FVector(100, 100, 100);

	Health = 100.f;
}

// Called when the game starts or when spawned
void AKirbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	TeleportationMarker->SetVisibility(false);

	LeftController = GetWorld()->SpawnActor<AKirbyHandController>(HandControllerClass);
	if (LeftController)
	{
		LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		LeftController->SetOwner(this);
		LeftController->SetHand(EControllerHand::Left);

		LeftController->HandMesh->SetWorldScale3D(FVector(1.f, 1.f, -1.f));
	}

	RightController = GetWorld()->SpawnActor<AKirbyHandController>(HandControllerClass);
	if (RightController)
	{
		RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		RightController->SetOwner(this);
		RightController->SetHand(EControllerHand::Right);
	}
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
	//PlayerInputComponent->BindAction(TEXT("GrabLeft"), IE_Pressed, this, &AKirbyCharacter::GrabLeftHand);
	//PlayerInputComponent->BindAction(TEXT("GrabRight"), IE_Pressed, this, &AKirbyCharacter::GrabRightHand);
	PlayerInputComponent->BindAction(TEXT("UIClickLeft"), IE_Pressed, this, &AKirbyCharacter::UILeftClickPressed);
	PlayerInputComponent->BindAction(TEXT("UIClickLeft"), IE_Released, this, &AKirbyCharacter::UILeftClickReleased);
	PlayerInputComponent->BindAction(TEXT("UIClickRight"), IE_Pressed, this, &AKirbyCharacter::UIRightClickPressed);
	PlayerInputComponent->BindAction(TEXT("UIClickRight"), IE_Released, this, &AKirbyCharacter::UIRightClickReleased);
}

void AKirbyCharacter::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

float AKirbyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DecrementHealth(DamageAmount);

	return DamageAmount;
}

void AKirbyCharacter::Die()
{
	//TODO: Death
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
	for (USplineMeshComponent* SplineMesh : TeleportPathMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}
	GetWorldTimerManager().ClearTimer(TeleportUpdateLocationHandle);

	StartFade(1, 0);
}

void AKirbyCharacter::CancelTeleport()
{
	bTeleporting = false;
	TeleportationMarker->SetVisibility(false);
	for (USplineMeshComponent* SplineMesh : TeleportPathMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}
	GetWorldTimerManager().ClearTimer(TeleportUpdateLocationHandle);
}

bool AKirbyCharacter::FindTeleportationDestination(TArray<FVector> &OutPath, FVector &OutLocation)
{
	FVector Start = RightController->GetActorLocation();
	FVector Look = RightController->GetActorForwardVector();

	FPredictProjectilePathParams Params(
		TeleportProjectileRadius, 
		Start,
		Look * TeleportProjectileSpeed,
		TeleportSimulationTime,
		ECollisionChannel::ECC_Visibility,
		this
		);
	//Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;

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

void AKirbyCharacter::DrawTeleportPath(const TArray<FVector>& Path)
{
	UpdateTeleportSpline(Path);

	for (USplineMeshComponent* SplineMesh : TeleportPathMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}

	for (int i = 0; i < Path.Num() - 1; ++i)
	{
		if (TeleportPathMeshPool.Num() <= i)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStaticMesh(TeleportArcMesh);
			SplineMesh->SetMaterial(0, TeleportArcMaterial);
			SplineMesh->RegisterComponent();

			TeleportPathMeshPool.Add(SplineMesh);
		}

		USplineMeshComponent* SplineMesh = TeleportPathMeshPool[i];
		SplineMesh->SetVisibility(true);

		FVector StartPos, StartTangent, EndPos, EndTangent;
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i, StartPos, StartTangent);
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent);
		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
	}
}

void AKirbyCharacter::UpdateTeleportMarker()
{
	FVector TeleportLocation;
	TArray<FVector> Path;

	if (FindTeleportationDestination(Path, TeleportLocation))
	{
		TeleportationMarker->SetVisibility(true);

		TeleportationMarker->SetWorldLocation(TeleportLocation);

		DrawTeleportPath(Path);
	}
	else
	{
		TeleportationMarker->SetVisibility(false);
		
		TArray<FVector> EmptyPath;
		DrawTeleportPath(EmptyPath);
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

void AKirbyCharacter::GrabLeftHand()
{
	if (LeftController->GripState == EGripState::EGS_CanGrab)
	{
		LeftController->GrabItem(LeftController->GrabbableItem);
	}
}

void AKirbyCharacter::GrabRightHand()
{
	if (RightController->GripState == EGripState::EGS_CanGrab)
	{
		RightController->GrabItem(RightController->GrabbableItem);
	}
}

void AKirbyCharacter::UILeftClickPressed()
{
	if (RightController->bPointerActive)
	{
		RightController->DeactivatePointer();
	}

	LeftController->UsePointer();
}

void AKirbyCharacter::UILeftClickReleased()
{
	LeftController->ReleasePointer();
}

void AKirbyCharacter::UIRightClickPressed()
{
	if (LeftController->bPointerActive)
	{
		LeftController->DeactivatePointer();
	}

	RightController->UsePointer();
}

void AKirbyCharacter::UIRightClickReleased()
{
	RightController->ReleasePointer();
}

