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
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Networking/NetworkedGameMode.h"
#include "Networking/NetworkedPlayerController.h"
#include "Characters/Components/InventorySystemComponent.h"
#include "Net/UnrealNetwork.h"

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

	InventorySystem = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("InventorySystem"));

	TeleportProjectileSpeed = 700.f;
	TeleportProjectileRadius = 10.f;
	TeleportSimulationTime = 2.f;
	MaxNumOfTeleports = 3;
	CurrentNumOfTeleports = 3;
	TeleportFadeDuration = 1;
	TeleportProjectionExtent = FVector(100, 100, 100);

	Health = 1.f;
	bIsPointerActive = false;

	bReplicates = true;
	SetReplicateMovement(true);

	OldLeftPos = FVector();
	OldRightPos = FVector();
	//bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
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

	bool bShouldUpdate = false;
	if (IsLocallyControlled())
	{
		if (LeftController)
		{
			FVector NewPos;
			NewPos = LeftController->GetActorLocation();
			if ((NewPos - OldLeftPos).Size() > 5)
			{
				bShouldUpdate = true;
			}


			OldLeftPos = NewPos;
		}

		if (RightController)
		{

			FVector NewPos;
			NewPos = RightController->GetActorLocation();
			if ((NewPos - OldLeftPos).Size() > 5)
			{
				bShouldUpdate = true;
			}


			OldRightPos = NewPos;


		}

		float diff = 0.f;
		if (KirbyCamera)
		{
			FRotator NewRot(0, KirbyCamera->GetComponentRotation().Yaw, 0);
			diff = OldYaw - KirbyCamera->GetComponentRotation().Yaw;
			if (FMath::Abs(OldYaw - KirbyCamera->GetComponentRotation().Yaw) > 1)
			{
				
				bShouldUpdate = true;
			}

			OldYaw = KirbyCamera->GetComponentRotation().Yaw;
			OldRotation = NewRot;
		}

		/*if(GetLocalRole() != ROLE_Authority)
			AddControllerYawInput(diff);
		UpdateCharacterOnServer(OldLeftPos, OldRightPos, diff);
		KirbyCamera->SetRelativeRotation(KirbyCamera->GetRelativeRotation() + FRotator(0, -diff, 0));*/
	}
}

// Called to bind functionality to input
void AKirbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Pressed, this, &AKirbyCharacter::BeginTeleport);
	PlayerInputComponent->BindAction(TEXT("CancelTeleport"), IE_Pressed, this, &AKirbyCharacter::CancelTeleport);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), IE_Pressed, this, &AKirbyCharacter::GrabLeftHand);
	PlayerInputComponent->BindAction(TEXT("GrabRight"), IE_Pressed, this, &AKirbyCharacter::GrabRightHand);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), IE_Released, this, &AKirbyCharacter::ReleaseLeftHand);
	PlayerInputComponent->BindAction(TEXT("GrabRight"), IE_Released, this, &AKirbyCharacter::ReleaseRightHand);
	PlayerInputComponent->BindAction(TEXT("UIClickLeft"), IE_Pressed, this, &AKirbyCharacter::UILeftClickPressed);
	PlayerInputComponent->BindAction(TEXT("UIClickLeft"), IE_Released, this, &AKirbyCharacter::UILeftClickReleased);
	PlayerInputComponent->BindAction(TEXT("UIClickRight"), IE_Pressed, this, &AKirbyCharacter::UIRightClickPressed);
	PlayerInputComponent->BindAction(TEXT("UIClickRight"), IE_Released, this, &AKirbyCharacter::UIRightClickReleased);
	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Pressed, this, &AKirbyCharacter::TriggerRightPressed);
	PlayerInputComponent->BindAction(TEXT("TriggerLeft"), IE_Pressed, this, &AKirbyCharacter::TriggerLeftPressed);
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
	if (!bTeleporting) // && CurrentNumOfTeleports > 0)
	{
		
		GetWorldTimerManager().SetTimer(TeleportUpdateLocationHandle, this, &AKirbyCharacter::UpdateTeleportMarker, 0.02f, true);
		bTeleporting = true;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Teleport %d"), bTeleporting));
	}
	else if(bTeleporting)
	{
		// Teleport on server with current location
		FVector ZOffset(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TeleportOnServer(TeleportationMarker->GetComponentLocation() + ZOffset);

		// Move this to client function, but call end teleport on server
		//StartFade(0, 1);
		//FTimerHandle FadeHandle;
		//GetWorldTimerManager().SetTimer(FadeHandle, this, &AKirbyCharacter::EndTeleport, TeleportFadeDuration);
	}

}

void AKirbyCharacter::EndTeleport()
{
	//CurrentNumOfTeleports--;
	//FVector ZOffset(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	//SetActorLocation(TeleportationMarker->GetComponentLocation() + ZOffset);

	// Cancel the teleport to signal end and fade back into view
	CancelTeleport();
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
		GrabLeftHandServer();
	}
}

void AKirbyCharacter::GrabLeftHandServer_Implementation()
{
	LeftController->GrabItem(LeftController->GrabbableItem);
}

void AKirbyCharacter::GrabRightHand()
{
	if (RightController->GripState == EGripState::EGS_CanGrab)
	{
		GrabRightHandServer();
	}
}

void AKirbyCharacter::GrabRightHandServer_Implementation()
{
	RightController->GrabItem(RightController->GrabbableItem);
}

void AKirbyCharacter::ReleaseLeftHand()
{
	if (LeftController->GripState == EGripState::EGS_Grab)
	{
		ReleaseLeftHandServer();
	}
}

void AKirbyCharacter::ReleaseLeftHandServer_Implementation()
{
	LeftController->ReleaseItem(LeftController->GrabbedItem);
}

void AKirbyCharacter::ReleaseRightHand()
{
	if (RightController->GripState == EGripState::EGS_Grab)
	{
		ReleaseRightHandServer();
	}
}

void AKirbyCharacter::ReleaseRightHandServer_Implementation()
{
	RightController->ReleaseItem(RightController->GrabbableItem);
}

void AKirbyCharacter::TriggerRightPressed()
{
	if (RightController->GripState == EGripState::EGS_Grab)
	{
		TriggerRightPressedServer();
	}
}

void AKirbyCharacter::TriggerRightPressedServer_Implementation()
{
	RightController->UseItem(RightController->GrabbedItem);
}

void AKirbyCharacter::TriggerLeftPressed()
{
	if (LeftController->GripState == EGripState::EGS_Grab)
	{
		TriggerLeftPressedServer();
	}
}

void AKirbyCharacter::TriggerLeftPressedServer_Implementation()
{
	LeftController->UseItem(LeftController->GrabbedItem);
}

void AKirbyCharacter::UILeftClickPressed()
{
	if (bIsPointerActive)
	{
		if (RightController->bPointerActive)
		{
			RightController->DeactivatePointer();
		}

		LeftController->UsePointer();
	}
	else
	{
		//Disabled til inputs are in order
		/*if (RightController->bInventoryMenuActive)
		{
			RightController->CloseInventoryMenu();
		}*/
		if (LeftController->bInventoryMenuActive)
		{
			LeftController->CloseInventoryMenu();
		}
		else
		{
			LeftController->OpenInventoryMenu();
		}
	}
}

void AKirbyCharacter::UILeftClickReleased()
{
	if (bIsPointerActive)
	{
		LeftController->ReleasePointer();
	}
}

void AKirbyCharacter::UIRightClickPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("RightClick pressed."));
	if (bIsPointerActive)
	{
		if (LeftController->bPointerActive)
		{
			LeftController->DeactivatePointer();
		}

		RightController->UsePointer();
	}
	else
	{
		//Disabled til inputs are in order
		/*if (LeftController->bInventoryMenuActive)
		{
			LeftController->CloseInventoryMenu();
		}
		
		RightController->OpenInventoryMenu();*/
	}
}

void AKirbyCharacter::UIRightClickReleased()
{
	if (bIsPointerActive)
	{
		RightController->ReleasePointer();
	}
}

// Tom's functions

void AKirbyCharacter::EnableGhostStatus()
{
	bIsGhost = true;

	NotifyGhostStatusChanged();

}

void AKirbyCharacter::NotifyGhostStatusChanged_Implementation()
{
	APlayerController* LocalController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AKirbyCharacter* ControlledChar = LocalController->GetPawn<AKirbyCharacter>();

	if (!IsLocallyControlled() && (ControlledChar && !ControlledChar->bIsGhost))
	{
		GetRootComponent()->SetVisibility(false, true);
	}
}

void AKirbyCharacter::TeleportOnServer_Implementation(const FVector& Location)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Do The Teleport")));
	// If player is able to perform the movement
	ANetworkedGameMode* NetGameMode = Cast<ANetworkedGameMode>(UGameplayStatics::GetGameMode(this));
	if (NetGameMode && NetGameMode->CheckAndConsumeMovement(GetController<ANetworkedPlayerController>()))
	{
		// Tell client the teleport is a go
		TeleportResponseClient();

		// Perform teleport after same time as client fades
		FTimerHandle TeleHandle;
		FTimerDelegate TeleportDel;
		TeleportDel.BindUFunction(this, FName("PerformTeleport"), Location);

		GetWorld()->GetTimerManager().SetTimer(TeleHandle, TeleportDel, TeleportFadeDuration, false);
	}
}

void AKirbyCharacter::TeleportResponseClient_Implementation()
{
	// Client simply performs fade and ending the teleport
	StartFade(0, 1);

	FTimerHandle FadeHandle;
	GetWorldTimerManager().SetTimer(FadeHandle, this, &AKirbyCharacter::EndTeleport, TeleportFadeDuration);
}

void AKirbyCharacter::PerformTeleport(const FVector& Location)
{
	// Simple set location that should only happen on server, main function exists in case other functionality is needed
	SetActorLocation(Location);
}

void AKirbyCharacter::PerformActionOnServer_Implementation()
{
	// If player is able to perform the movement
	ANetworkedGameMode* NetGameMode = Cast<ANetworkedGameMode>(UGameplayStatics::GetGameMode(this));
	ANetworkedPlayerController* NetController = GetController<ANetworkedPlayerController>();
	if (NetGameMode && NetGameMode->IsActionAllowed(NetController))
	{
		NetGameMode->PerformAction(NetController);

		//TODO add functionality for an action
	}


}

void AKirbyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKirbyCharacter, LeftHandPos);
	DOREPLIFETIME(AKirbyCharacter, RightHandPos);
	DOREPLIFETIME(AKirbyCharacter, bIsPointerActive);
}

void AKirbyCharacter::UpdateCharacterOnServer_Implementation(const FVector& LeftPos, const FVector& RightPos, float HeadRot)
{
	LeftHandPos = LeftPos;
	RightHandPos = RightPos;
	
	AddControllerYawInput(HeadRot);
}