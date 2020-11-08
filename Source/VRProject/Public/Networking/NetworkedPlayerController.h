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

protected:
	UFUNCTION(Server, Unreliable)
		void KillOnServer();
	
};
