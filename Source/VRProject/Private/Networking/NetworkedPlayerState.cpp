// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedPlayerState.h"
#include "Net/UnrealNetwork.h"

ANetworkedPlayerState::ANetworkedPlayerState()
{
	Username = "User0";
	bReplicates = true;
	bIsPlayerDead = false;
	bIsPlayerTurn = false;
	NumAllowedMovements = 0;
}

void ANetworkedPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkedPlayerState, bIsPlayerDead);
	DOREPLIFETIME(ANetworkedPlayerState, bIsPlayerTurn);
	DOREPLIFETIME(ANetworkedPlayerState, Username);
	DOREPLIFETIME(ANetworkedPlayerState, NumAllowedMovements);
}