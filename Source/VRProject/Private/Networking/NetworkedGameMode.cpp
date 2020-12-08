// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedGameMode.h"
#include "Networking/NetworkedGameInstance.h"
#include "Networking/NetworkedGameState.h"
#include "Networking/NetworkedPlayerState.h"
#include "Characters/KirbyCharacter.h"
#include "Widgets/TestMenuCharacterNoVR.h"
#include "Networking/NetworkedPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Characters/GameplayActors/KirbyHandController.h"

ANetworkedGameMode::ANetworkedGameMode() : Super()
{
	DefaultPawnClass = ATestMenuCharacterNoVR::StaticClass();
	bUseSeamlessTravel = true;
	CurrentPlayerMaxTeleports = 5;
}

void ANetworkedGameMode::BeginPlay()
{
	Super::BeginPlay();
	bIsFreeMovementAllowed = true;
	PlayerTurnID = 0;
	MaxTurnTime = 30.f;
	PostActionTurnTime = 5.f;
	HasActionStarted = false;
	GameInProgress = false;
}

void ANetworkedGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Make sure all timers are stopped
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

/***********************************************************************************************************
*
*										TURN HANDLING METHODS
*
*
************************************************************************************************************/




/*
* Starts the cycle of turns
*/
void ANetworkedGameMode::StartTurns()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Starting");

	CurrentPlayerTurn = -1;
	bIsFreeMovementAllowed = false;
	GameInProgress = true;

	// Make sure Turns start
	CountdownStart(10);
}

/*
* Moves the turn to the next available player, and resets everyone's turn variables
*/
void ANetworkedGameMode::NextTurn()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Next Turn");
	ANetworkedGameState* NetGameState = GetGameState<ANetworkedGameState>();
	CurrentPlayerTurn++;
	if (CurrentPlayerTurn >= NetGameState->PlayerArray.Num())
		CurrentPlayerTurn = 0;

	// Handle that next player may be dead
	ANetworkedPlayerState* NextState = Cast<ANetworkedPlayerState>(NetGameState->PlayerArray[CurrentPlayerTurn]);
	while(!NextState || NextState->bIsPlayerDead)
	{
		CurrentPlayerTurn++;
		if (CurrentPlayerTurn >= NetGameState->PlayerArray.Num())
			CurrentPlayerTurn = 0;

		NextState = Cast<ANetworkedPlayerState>(NetGameState->PlayerArray[CurrentPlayerTurn]);
	}

	PlayerTurnID = GameState->PlayerArray[CurrentPlayerTurn]->GetPlayerId();
	HasActionStarted = false;

	int i;
	for (i = 0; i < NetGameState->PlayerArray.Num(); i++)
	{
		ANetworkedPlayerState* CurrState = Cast<ANetworkedPlayerState>(NetGameState->PlayerArray[i]);
		if (CurrState->GetPlayerId() == PlayerTurnID)
		{
			CurrState->NumAllowedMovements = CurrentPlayerMaxTeleports;
		}
		else
		{
			CurrState->NumAllowedMovements = 1;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(TurnHandle, this, &ANetworkedGameMode::NextTurn, MaxTurnTime, false);
}

/*
* Forces the turn to move immediately
*/
void ANetworkedGameMode::ForceNextTurn()
{
	GetWorld()->GetTimerManager().ClearTimer(TurnHandle);
	NextTurn();
}

/*
* Ends all turns
*/
void ANetworkedGameMode::EndAllTurns()
{

	// Make sure all timers are stopped
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


/***********************************************************************************************************
*
*										INPUT HANDLING METHODS
*
*
************************************************************************************************************/

/*
* Reports whether a controller can perform some action, like firing a gun or equipping an item
*/
bool ANetworkedGameMode::IsActionAllowed(ANetworkedPlayerController* Controller)
{
	ANetworkedPlayerState* PlayerState = Cast<ANetworkedPlayerState>(Controller->PlayerState);
	if (PlayerState)
		return bIsFreeMovementAllowed || PlayerState->GetPlayerId() == PlayerTurnID || PlayerState->bIsPlayerDead;
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Uh oh");
		return bIsFreeMovementAllowed;
	}
}

/*
* Attempts to perform an action for the controller, reporting whether the action is either illegal or already performing
* Moves the game to the Post-Action intermission before the next turn
*/
bool ANetworkedGameMode::PerformAction(ANetworkedPlayerController* Controller)
{
	if (!GameInProgress || !IsActionAllowed(Controller) || HasActionStarted)
		return false;

	GetWorld()->GetTimerManager().ClearTimer(TurnHandle);
	GetWorld()->GetTimerManager().SetTimer(TurnHandle, this, &ANetworkedGameMode::NextTurn, PostActionTurnTime, false);

	HasActionStarted = true;

	return true;
}

/*
* Reports if a player can perform a movement action, and updates their player state if possible
*/
bool ANetworkedGameMode::CheckAndConsumeMovement(ANetworkedPlayerController* Controller)
{
	if (bIsFreeMovementAllowed)
		return true;

	ANetworkedPlayerState* PlayerState = Cast<ANetworkedPlayerState>(Controller->PlayerState);
	if (PlayerState && PlayerState->NumAllowedMovements > 0)
	{
		PlayerState->NumAllowedMovements = PlayerState->NumAllowedMovements - 1;
		return true;
	}
	return false;
}


/***********************************************************************************************************
*
*										WIN/LOSE/PLAYER STATUS METHODS
*
*
************************************************************************************************************/


int32 ANetworkedGameMode::GetRemainingPlayerCount()
{
	int32 PlayersLeft = 0;
	ANetworkedGameState* NetGameState = GetGameState<ANetworkedGameState>();

	if (NetGameState)
	{
		int i;
		for (i = 0; i < NetGameState->PlayerArray.Num(); i++)
		{
			ANetworkedPlayerState* CurrState = Cast<ANetworkedPlayerState>(NetGameState->PlayerArray[i]);
			if (CurrState && !CurrState->bIsPlayerDead)
				PlayersLeft++;
		}
	}

	return PlayersLeft;
}

bool ANetworkedGameMode::CheckGameOver()
{
	return GetRemainingPlayerCount() <= 1;
}

/*
* Updates a given controller's player state to dead, and checks for a game over
*/
void ANetworkedGameMode::PlayerDied(ANetworkedPlayerController* Controller)
{

	if (!GameInProgress)
		return;

	ANetworkedPlayerState* State = Cast<ANetworkedPlayerState>(Controller->PlayerState);

	if (!State)
		return;
	else
		State->bIsPlayerDead = true;

	// Ghost spawning now in blueprints due to issues with BP versions of characters
	OnPlayerDied(Controller);

	/*
	// Create a ghost for the player and spawn it in
	if (DefaultPawnClass->IsChildOf(ATestMenuCharacterNoVR::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Death");
		ATestMenuCharacterNoVR* NewGhost;
		if (GhostRespawnZone)
			NewGhost = GetWorld()->SpawnActor<ATestMenuCharacterNoVR>(ATestMenuCharacterNoVR::StaticClass(),
				GhostRespawnZone->GetActorLocation(), GhostRespawnZone->GetActorRotation());
		else
			NewGhost = GetWorld()->SpawnActor<ATestMenuCharacterNoVR>(ATestMenuCharacterNoVR::StaticClass(), FVector(), FRotator());

		Controller->Possess(NewGhost);
		NewGhost->EnableGhostStatus();
		NewGhost->UpdateName(State->GetPlayerName());
	}
	else if (DefaultPawnClass->IsChildOf(AKirbyCharacter::StaticClass()))
	{

		AKirbyCharacter* NewGhost;
		if (GhostRespawnZone)
			NewGhost = GetWorld()->SpawnActor<AKirbyCharacter>(AKirbyCharacter::StaticClass(),
				GhostRespawnZone->GetActorLocation(), GhostRespawnZone->GetActorRotation());
		else
			NewGhost = GetWorld()->SpawnActor<AKirbyCharacter>(AKirbyCharacter::StaticClass(), FVector(), FRotator());

		Controller->Possess(NewGhost);
		NewGhost->EnableGhostStatus();
		NewGhost->UpdateName(State->GetPlayerName());
	}
	*/
	

	// End match if game over
	if (CheckGameOver())
		TriggerEndGame();
}

/*
* End of match function
*/
void ANetworkedGameMode::TriggerEndGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Game over!");

	// End turns and trigger game end
	EndAllTurns();
	bIsFreeMovementAllowed = true;
	GameInProgress = false;
	OnGameEnd();

	UNetworkedGameInstance* NetGameInstance = GetGameInstance<UNetworkedGameInstance>();

	ANetworkedGameState* NetGameState = GetGameState<ANetworkedGameState>();

	if (NetGameState)
	{
		int i;
		for (i = 0; i < NetGameState->PlayerArray.Num(); i++)
		{
			ANetworkedPlayerState* CurrState = Cast<ANetworkedPlayerState>(NetGameState->PlayerArray[i]);
			if (CurrState && !CurrState->bIsPlayerDead)
			{
				NetGameInstance->WinningPlayer = CurrState->GetPlayerName();
			}
		}
	}


	// Launch fireworks as victory until time to go back to lobby
	LaunchFireworks();
	GetWorld()->GetTimerManager().SetTimer(FireworkHandle, this, &ANetworkedGameMode::LaunchFireworks, 3, true);

	// Perform countdown after certain time is elapsed
	FTimerDelegate CountdownDel;
	CountdownDel.BindUFunction(this, FName("CountdownEnd"), 10);
	GetWorld()->GetTimerManager().SetTimer(CountdownHandle, CountdownDel, 10, false);
}

/*
* Updates game mode data according to the controller of a leaving player
*/
void  ANetworkedGameMode::PlayerLeft(APlayerController* Controller)
{
	if (GameInProgress && Controller->PlayerState->GetPlayerId() == PlayerTurnID)
		ForceNextTurn();
}


/***********************************************************************************************************
*
*										Spawn METHODS
*
*
************************************************************************************************************/

void ANetworkedGameMode::HandleAllSpawnSetup(APlayerStart* NewGhostZone)
{
	TArray<AActor*> SpawnActors;
	SetGhostSpawnZone(NewGhostZone);
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), SpawnActors);

	TArray<APlayerStart*> SpawnZones;
	int i;
	for ( i = 0; i < SpawnActors.Num(); i++)
	{
		APlayerStart* CurrZone = Cast<APlayerStart>(SpawnActors[i]);
		if (CurrZone && CurrZone != NewGhostZone)
		{
			SpawnZones.Add(CurrZone);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Spawn Zones: %d"), SpawnZones.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Spawn Actors: %d"), SpawnActors.Num()));

	RegisterPlayerSpawns(SpawnZones);
	SpawnPlayers();
}

APlayerStart* ANetworkedGameMode::GetGhostSpawnZone()
{
	return GhostRespawnZone;
}

void ANetworkedGameMode::SetGhostSpawnZone(APlayerStart* NewRespawnZone)
{
	GhostRespawnZone = NewRespawnZone;
}

void ANetworkedGameMode::RegisterPlayerSpawns(TArray<APlayerStart*> Spawns)
{
	PlayerSpawns = Spawns;
}

/*
* Spawns all players at a random spawn point, given they all fit within the spawns
*/
void ANetworkedGameMode::SpawnPlayers()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "SPAWN PLAYERS");
	ANetworkedGameState* NetGameState = GetGameState<ANetworkedGameState>();

	if (NetGameState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "NetgameState Exists");
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Player Spawns: %d"), PlayerSpawns.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Player Array: %d"), NetGameState->PlayerArray.Num()));
	if (NetGameState && PlayerSpawns.Num() >= NetGameState->PlayerArray.Num())
	{
		int i;
		for (i = 0; i < NetGameState->PlayerArray.Num(); i++)
		{
			ANetworkedPlayerState* CurrState = Cast<ANetworkedPlayerState>(NetGameState->PlayerArray[i]);
			CurrState->bIsPlayerDead = false;
			APawn* CurrPawn = CurrState->GetPawn();

			if (CurrPawn)
			{
				ACharacter* Character = Cast<ACharacter>(CurrPawn);
				if (Character)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "CHARACTER CASTED CORRECTLY");
					APlayerStart* CurrentStart = PlayerSpawns[i];
					Character->SetActorLocationAndRotation(CurrentStart->GetActorLocation(), CurrentStart->GetActorRotation());

					AKirbyCharacter* Kirb = Cast<AKirbyCharacter>(Character);
					if (Kirb)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "POINTER");
						Kirb->bIsPointerActive = false;
						if (Kirb->LeftController)
						{
							Kirb->LeftController->DeactivatePointer();
						}

						if (Kirb->RightController)
						{
							Kirb->RightController->DeactivatePointer();
						}
					}
				}
			}
		}
	}
}

/***********************************************************************************************************
*
*										OTHER METHODS
*
*
************************************************************************************************************/

void ANetworkedGameMode::ReturnToLobby()
{
	GetWorld()->ServerTravel(TEXT("Lobby"));
}

void ANetworkedGameMode::CountdownStart(uint8 TimesRemaining)
{
	OnCountdownUpdated(TimesRemaining);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Countdown");
	if (TimesRemaining <= 0)
	{
		NextTurn();
	}
	else
	{
		FTimerDelegate CountdownDel;
		CountdownDel.BindUFunction(this, FName("CountdownStart"), TimesRemaining - 1);

		GetWorld()->GetTimerManager().SetTimer(CountdownHandle, CountdownDel, 1, false);
	}
}


void ANetworkedGameMode::CountdownEnd(uint8 TimesRemaining)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Countdown");
	OnCountdownUpdated(TimesRemaining);
	if (TimesRemaining <= 0)
	{
		ReturnToLobby();
	}
	else
	{
		FTimerDelegate CountdownDel;
		CountdownDel.BindUFunction(this, FName("CountdownEnd"), TimesRemaining - 1);

		GetWorld()->GetTimerManager().SetTimer(CountdownHandle, CountdownDel, 1, false);
	}
}


