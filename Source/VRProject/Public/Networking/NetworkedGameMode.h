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

		UPROPERTY(BlueprintReadOnly)
		TArray<APlayerStart*> PlayerSpawns;

		UPROPERTY(BlueprintReadOnly)
		class APlayerStart* GhostRespawnZone;

		UPROPERTY(BlueprintReadWrite, Category = Turn)
		bool bIsFreeMovementAllowed;

		// Represents the player's turn from the index of all player states
		UPROPERTY(BlueprintReadOnly, Category = Turn)
		uint8 CurrentPlayerTurn;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Turn)
		uint8 CurrentPlayerMaxTeleports;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Turn)
		int MaxTurnTime;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Turn)
		int PostActionTurnTime;

		UPROPERTY(BlueprintReadOnly)
		bool HasActionStarted;

		FTimerHandle TurnHandle;

		FTimerHandle CountdownHandle;

		FTimerHandle FireworkHandle;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

		int32 PlayerTurnID;

		bool GameInProgress;

	public:

		ANetworkedGameMode();

		UFUNCTION(BlueprintCallable)
		void StartTurns();

		UFUNCTION(BlueprintCallable)
		void NextTurn();

		UFUNCTION(BlueprintCallable)
		void EndAllTurns();

		UFUNCTION(BlueprintCallable)
		void ForceNextTurn();

		UFUNCTION(BlueprintCallable)
		bool CheckAndConsumeMovement(ANetworkedPlayerController* Controller);

		UFUNCTION(BlueprintCallable)
		bool IsActionAllowed(ANetworkedPlayerController* Controller);

		UFUNCTION(BlueprintCallable)
		bool PerformAction(ANetworkedPlayerController* Controller);

		UFUNCTION(BlueprintCallable)
		int32 GetRemainingPlayerCount();

		UFUNCTION(BlueprintCallable)
		bool CheckGameOver();

		UFUNCTION(BlueprintCallable)
		void PlayerDied(ANetworkedPlayerController* Controller);

		UFUNCTION(BlueprintCallable)
		APlayerStart* GetGhostSpawnZone();

		UFUNCTION(BlueprintCallable)
		void SetGhostSpawnZone(APlayerStart* NewRespawnZone);

		UFUNCTION(BlueprintCallable)
		void RegisterPlayerSpawns(TArray<APlayerStart*> Spawns);

		UFUNCTION(BlueprintCallable)
		void SpawnPlayers();

		UFUNCTION(BlueprintCallable)
		void HandleAllSpawnSetup(APlayerStart* NewGhostZone);

		UFUNCTION()
		void TriggerEndGame();

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnPlayerDied(ANetworkedPlayerController* Controller);

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void LaunchFireworks();

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnCountdownUpdated(uint8 TimesRemaining);

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnGameEnd();

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ReturnToLobby();

		UFUNCTION(BlueprintCallable)
		void PlayerLeft(APlayerController* Controller);

		UFUNCTION()
		void CountdownStart(uint8 TimesRemaining);

		UFUNCTION()
		void CountdownEnd(uint8 TimesRemaining);
};
