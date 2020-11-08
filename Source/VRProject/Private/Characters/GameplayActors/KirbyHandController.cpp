// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GameplayActors/KirbyHandController.h"


// Sets default values
AKirbyHandController::AKirbyHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
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
}

void AKirbyHandController::HandEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}

