// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedGameMode.h"
#include "Widgets/TestMenuCharacterNoVR.h"

ANetworkedGameMode::ANetworkedGameMode() : Super()
{
	DefaultPawnClass = ATestMenuCharacterNoVR::StaticClass();
}

void ANetworkedGameMode::BeginPlay()
{
	Super::BeginPlay();

}