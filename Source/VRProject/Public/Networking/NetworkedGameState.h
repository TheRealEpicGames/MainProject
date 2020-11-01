// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NetworkedGameState.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API ANetworkedGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Players)
	TArray<APlayerController*> PlayerControllerList;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void StartMatchWithLevel(const FString & LevelName);

};
