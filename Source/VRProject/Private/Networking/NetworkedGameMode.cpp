// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedGameMode.h"
#include "Networking/NetworkedGameInstance.h"
#include "Networking/NetworkedGameState.h"
#include "Networking/NetworkedPlayerState.h"
#include "Widgets/TestMenuCharacterNoVR.h"
#include "Networking/NetworkedPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

ANetworkedGameMode::ANetworkedGameMode() : Super()
{
	DefaultPawnClass = ATestMenuCharacterNoVR::StaticClass();
	bUseSeamlessTravel = true;
}

void ANetworkedGameMode::BeginPlay()
{
	Super::BeginPlay();
	bIsFreeMovementAllowed = true;
	PlayerTurnID = 0;
}

void ANetworkedGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Make sure all timers are stopped
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ANetworkedGameMode::StartTurns()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Starting");

	CurrentPlayerTurn = -1;
	bIsFreeMovementAllowed = false;

	// Make sure Turns start
	NextTurn();
}

void ANetworkedGameMode::NextTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Next Turn");
	ANetworkedGameState* NetGameState = GetGameState<ANetworkedGameState>();
	CurrentPlayerTurn++;
	if (CurrentPlayerTurn == NetGameState->PlayerArray.Num())
		CurrentPlayerTurn = 0;

	
	PlayerTurnID = GameState->PlayerArray[CurrentPlayerTurn]->GetPlayerId();

	GetWorld()->GetTimerManager().SetTimer(TurnHandle, this, &ANetworkedGameMode::NextTurn, 20.f, false);
}

void ANetworkedGameMode::ForceNextTurn()
{
	GetWorld()->GetTimerManager().ClearTimer(TurnHandle);
	NextTurn();
}

void ANetworkedGameMode::EndAllTurns()
{

	// Make sure all timers are stopped
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


bool ANetworkedGameMode::IsActionAllowed(ANetworkedPlayerController* Controller)
{
	return bIsFreeMovementAllowed || Controller->PlayerState->GetPlayerId() == PlayerTurnID;
}

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