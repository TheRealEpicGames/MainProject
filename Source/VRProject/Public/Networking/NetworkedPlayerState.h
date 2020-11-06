// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetworkedPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API ANetworkedPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
		UPROPERTY(Replicated, BlueprintReadWrite)
		bool bIsPlayerDead;

		ANetworkedPlayerState();

		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
