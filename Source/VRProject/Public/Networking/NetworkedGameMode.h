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

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Turn)
		float MaxTurnTime;

		FTimerHandle TurnHandle;

		ANetworkedGameMode();


	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	public:

		UFUNCTION(BlueprintCallable)
		void StartTurns();

		UFUNCTION(BlueprintCallable)
		void NextTurn();

		UFUNCTION(BlueprintCallable)
		void EndAllTurns();

		
};
