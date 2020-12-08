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
		FString Username;

		UPROPERTY(Replicated, BlueprintReadWrite)
		bool bIsPlayerDead;

		UPROPERTY(Replicated, BlueprintReadWrite)
		bool bIsPlayerTurn;

		UPROPERTY(Replicated, BlueprintReadWrite)
		uint8 NumAllowedMovements;

		ANetworkedPlayerState();

		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
