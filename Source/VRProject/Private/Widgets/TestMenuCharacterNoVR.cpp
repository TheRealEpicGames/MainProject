// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TestMenuCharacterNoVR.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATestMenuCharacterNoVR::ATestMenuCharacterNoVR()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetInteractor = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction"));
	WidgetInteractor->SetupAttachment(GetRootComponent());
	WidgetInteractor->bShowDebug = true;
	WidgetInteractor->InteractionDistance = 500.f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ATestMenuCharacterNoVR::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestMenuCharacterNoVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestMenuCharacterNoVR::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("KeyboardMoveHorizontal", this, &ATestMenuCharacterNoVR::MoveHorizontal);
	PlayerInputComponent->BindAxis("KeyboardMoveForward", this, &ATestMenuCharacterNoVR::MoveForward);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATestMenuCharacterNoVR::LeftMousePress);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &ATestMenuCharacterNoVR::LeftMouseRelease);
	PlayerInputComponent->BindAxis("Turn", this, &ATestMenuCharacterNoVR::Turn);
	PlayerInputComponent->BindAxis("Tilt", this, &ATestMenuCharacterNoVR::Tilt);
}

void ATestMenuCharacterNoVR::MoveHorizontal(float Scale)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Scale * 3.f);
	}
}

void ATestMenuCharacterNoVR::MoveForward(float Scale)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Scale * 3.f);
	}
}

void ATestMenuCharacterNoVR::LeftMousePress()
{
	WidgetInteractor->PressPointerKey(EKeys::LeftMouseButton);
}

void ATestMenuCharacterNoVR::LeftMouseRelease()
{
	WidgetInteractor->ReleasePointerKey(EKeys::LeftMouseButton);
}

void ATestMenuCharacterNoVR::Turn(float magnitude)
{
	AddControllerYawInput(magnitude * 45.f * GetWorld()->GetDeltaSeconds());
}

void ATestMenuCharacterNoVR::Tilt(float magnitude)
{
	AddControllerPitchInput(magnitude * 45.f * GetWorld()->GetDeltaSeconds());
	WidgetInteractor->AddLocalRotation(FRotator(-2 * magnitude * 45.f * GetWorld()->GetDeltaSeconds(), 0, 0));
}

void ATestMenuCharacterNoVR::EnableGhostStatus()
{
	bIsGhost = true;

	NotifyGhostStatusChanged();
	
}

void ATestMenuCharacterNoVR::NotifyGhostStatusChanged_Implementation()
{
	APlayerController* LocalController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ATestMenuCharacterNoVR* ControlledChar = LocalController->GetPawn<ATestMenuCharacterNoVR>();

	// If not this player's character and this player isn't a ghost, make invisible
	if (!IsLocallyControlled() && (ControlledChar && !ControlledChar->bIsGhost))
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	// If this player is a ghost, make all other ghosts visible
	if (IsLocallyControlled())
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, ATestMenuCharacterNoVR::StaticClass(), Actors);
		
		int i;
		for (i = 0; i < Actors.Num(); i++)
		{
			if (Cast<ATestMenuCharacterNoVR>(Actors[i])->bIsGhost)
			{
				Actors[i]->GetRootComponent()->SetVisibility(true, true);
			}
		}
	}
}