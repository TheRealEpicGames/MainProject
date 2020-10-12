// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KirbyCharacter.h"

// Sets default values
AKirbyCharacter::AKirbyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKirbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKirbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKirbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

