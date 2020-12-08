// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedGameState.h"
#include "Kismet/GameplayStatics.h"

void ANetworkedGameState::StartMatchWithLevel_Implementation(const FString& LevelName)
{
	GetWorld()->ServerTravel(LevelName);
}