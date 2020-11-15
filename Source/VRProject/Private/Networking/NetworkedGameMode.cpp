// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedGameMode.h"
#include "Networking/NetworkedGameInstance.h"
#include "Networking/NetworkedGameState.h"
#include "Networking/NetworkedPlayerState.h"
#include "Widgets/TestMenuCharacterNoVR.h"
#include "Networking/NetworkedPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Starting");

	CurrentPlayerTurn = -1;
	bIsFreeMovementAllowed = false;
	GameInProgress = true;

	// Make sure Turns start
	NextTurn();
}

/*
* Moves the turn to the next available player, and resets everyone's turn variables
*/
void ANetworkedGameMode::NextTurn()
{
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
	return bIsFreeMovementAllowed || Controller->PlayerState->GetPlayerId() == PlayerTurnID;
}

/*
* Attempts to perform an action for the controller, reporting whether the action is either illegal or already performing
* Moves the game to the Post-Action intermission before the next turn
*/
bool ANetworkedGameMode::PerformAction(ANetworkedPlayerController* Controller)
{
	if (!IsActionAllowed(Controller) || HasActionStarted)
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

	int i;
	for (i = 0; i < NetGameState->PlayerArray.Num(); i++)
	{
		ANetworkedPlayerState* CurrState = Cast<ANetworkedPlayerState>(NetGameState->PlayerArray[i]);
		if (CurrState && !CurrState->bIsPlayerDead)
			PlayersLeft++;
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

	Cast<ANetworkedPlayerState>(Controller->PlayerState)->bIsPlayerDead = true;


	//TODO add same code for kirby character

	ATestMenuCharacterNoVR* NewGhost;
	if (GhostRespawnZone)
		NewGhost = GetWorld()->SpawnActor<ATestMenuCharacterNoVR>(ATestMenuCharacterNoVR::StaticClass(), 
			GhostRespawnZone->GetActorLocation(), GhostRespawnZone->GetActorRotation());
	else
		NewGhost = GetWorld()->SpawnActor<ATestMenuCharacterNoVR>(ATestMenuCharacterNoVR::StaticClass(), FVector(), FRotator());

	Controller->Possess(NewGhost);
	NewGhost->EnableGhostStatus();

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
	EndAllTurns();
	bIsFreeMovementAllowed = true;
	GameInProgress = false;

	GetWorld()->ServerTravel(TEXT("Lobby"));
}

/*
* Updates game mode data according to the controller of a leaving player
*/
void  ANetworkedGameMode::PlayerLeft(APlayerController* Controller)
{
	if (Controller->PlayerState->GetPlayerId() == PlayerTurnID)
		ForceNextTurn();
}


/***********************************************************************************************************
*
*										OTHER METHODS
*
*
************************************************************************************************************/

APlayerStart* ANetworkedGameMode::GetGhostSpawnZone()
{
	return GhostRespawnZone;
}

void ANetworkedGameMode::SetGhostSpawnZone(APlayerStart* NewRespawnZone)
{
	GhostRespawnZone = NewRespawnZone;
}


















//DONT USE, here for in case we need to get all controllers, but not the best for networking
void ANetworkedGameMode::RegisterPlayers()
{
	TArray< AActor*> Controllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetworkedPlayerController::StaticClass(), Controllers);

	PlayerControllerList.Empty();

	int i;
	for (i = 0; i < Controllers.Num(); i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Trying add");
		ANetworkedPlayerController* CurrentController = Cast< ANetworkedPlayerController>(Controllers[i]);
		if (CurrentController)
		{
			PlayerControllerList.Add(CurrentController);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Bad controller");
		}

	}
}