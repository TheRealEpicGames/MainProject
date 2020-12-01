// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/UnrealNetwork.h"
#include "Networking/NetworkedPlayerState.h"

ANetworkedPlayerState::ANetworkedPlayerState()
{
	Username = "User0";
	bReplicates = true;
	bIsPlayerDead = false;
	NumAllowedMovements = 0;
}

void ANetworkedPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkedPlayerState, bIsPlayerDead);
	DOREPLIFETIME(ANetworkedPlayerState, Username);
	DOREPLIFETIME(ANetworkedPlayerState, NumAllowedMovements);
}