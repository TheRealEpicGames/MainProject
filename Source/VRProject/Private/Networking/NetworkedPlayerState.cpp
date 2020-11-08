// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/UnrealNetwork.h"
#include "Networking/NetworkedPlayerState.h"

ANetworkedPlayerState::ANetworkedPlayerState()
{
	bReplicates = true;
	bIsPlayerDead = false;
}

void ANetworkedPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(ANetworkedPlayerState, bIsPlayerDead);
}