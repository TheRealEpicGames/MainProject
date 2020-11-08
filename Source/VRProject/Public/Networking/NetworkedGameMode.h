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

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Players)
		TArray<class ANetworkedPlayerController*> PlayerControllerList;

		UPROPERTY(BlueprintReadWrite, Category = Turn)
		bool bIsFreeMovementAllowed;

		// Represents the player's turn from the index of all player states
		UPROPERTY(BlueprintReadOnly, Category = Turn)
		uint8 CurrentPlayerTurn;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Turn)
		float MaxTurnTime;

		FTimerHandle TurnHandle;

		ANetworkedGameMode();


	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

		void RegisterPlayers();

		int32 PlayerTurnID;

	public:

		UFUNCTION(BlueprintCallable)
		void StartTurns();

		UFUNCTION(BlueprintCallable)
		void NextTurn();

		UFUNCTION(BlueprintCallable)
		void EndAllTurns();

		UFUNCTION(BlueprintCallable)
		void ForceNextTurn();

		UFUNCTION(BlueprintCallable)
		bool IsActionAllowed(ANetworkedPlayerController* Controller);

		UFUNCTION(BlueprintCallable)
		int32 GetRemainingPlayerCount();

		UFUNCTION(BlueprintCallable)
		bool CheckGameOver();
};
