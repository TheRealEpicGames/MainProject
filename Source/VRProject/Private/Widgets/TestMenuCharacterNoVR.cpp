// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TestMenuCharacterNoVR.h"
#include "Components/WidgetInteractionComponent.h"

// Sets default values
ATestMenuCharacterNoVR::ATestMenuCharacterNoVR()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetInteractor = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction"));
	WidgetInteractor->SetupAttachment(GetRootComponent());
	WidgetInteractor->bShowDebug = true;
	WidgetInteractor->InteractionDistance = 500.f;
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