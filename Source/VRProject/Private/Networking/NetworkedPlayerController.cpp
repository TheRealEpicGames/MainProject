// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedPlayerController.h"
#include "Networking/NetworkedPlayerState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ANetworkedPlayerController::KillThisPlayer()
{
	KillOnServer();
	
}

void ANetworkedPlayerController::KillOnServer_Implementation()
{
	ANetworkedPlayerState* NetPlayerState = GetPlayerState<ANetworkedPlayerState>();
	NetPlayerState->bIsPlayerDead = true;

	//TODO, notify server of death
}