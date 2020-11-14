// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/UnrealNetwork.h"
#include "Networking/NetworkedPlayerState.h"

ANetworkedPlayerState::ANetworkedPlayerState()
{
	bReplicates = true;
	bIsPlayerDead = false;
	NumAllowedMovements = 0;
}

void ANetworkedPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkedPlayerState, bIsPlayerDead);
	DOREPLIFETIME(ANetworkedPlayerState, NumAllowedMovements);
}