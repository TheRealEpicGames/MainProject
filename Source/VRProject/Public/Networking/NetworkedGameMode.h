// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NetworkedGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API ANetworkedGameMode : public AGameMode
{
	GENERATED_BODY()

	public:
		
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Players)
		TArray<APlayerController*> PlayerControllerList;

		ANetworkedGameMode();


	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
};
