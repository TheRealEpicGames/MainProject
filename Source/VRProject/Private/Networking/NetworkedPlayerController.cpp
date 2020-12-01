// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedPlayerController.h"
#include "Networking/NetworkedPlayerState.h"
#include "Networking/NetworkedGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/TestMenuCharacterNoVR.h"

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

void ANetworkedPlayerController::ClientRegisterNameOfPlayer_Implementation()
{
	UNetworkedGameInstance* NetGameInstance = GetGameInstance<UNetworkedGameInstance>();
	if(NetGameInstance)
		RegisterNameOnServer(NetGameInstance->PlayerName);
}

void ANetworkedPlayerController::RegisterNameOnServer_Implementation(const FString& Name)
{
	ANetworkedPlayerState* NetPlayerState = GetPlayerState<ANetworkedPlayerState>();
	if(NetPlayerState)
		NetPlayerState->SetPlayerName(Name);
	ATestMenuCharacterNoVR* TestCharacter = GetPawn<ATestMenuCharacterNoVR>();
	if (TestCharacter)
	{
		TestCharacter->UpdateName(Name);
	}

	//TODO Add kirby implementation
}