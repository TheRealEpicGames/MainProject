// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WorldMenuActor.h"
#include "Components/WidgetComponent.h"

// Sets default values
AWorldMenuActor::AWorldMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MenuWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Menu Widget"));
	MenuWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWorldMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldMenuActor::OnButtonClicked()
{

}
