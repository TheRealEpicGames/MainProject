// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetworkedPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API ANetworkedPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
		UFUNCTION(BlueprintCallable)
		void KillThisPlayer();

		UFUNCTION(BlueprintCallable, Client, Unreliable)
		void ClientRegisterNameOfPlayer();

		UFUNCTION(BlueprintCallable, Server, Unreliable)
		void RegisterNameOnServer(const FString& Name);

protected:
	UFUNCTION(Server, Unreliable)
		void KillOnServer();
	
};
