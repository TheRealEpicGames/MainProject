// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/NetworkedGameMode.h"
#include "Widgets/TestMenuCharacterNoVR.h"
#include "Kismet/GameplayStatics.h"

ANetworkedGameMode::ANetworkedGameMode() : Super()
{
	DefaultPawnClass = ATestMenuCharacterNoVR::StaticClass();
	bUseSeamlessTravel = true;
}

void ANetworkedGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ANetworkedGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Make sure all timers are stopped
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ANetworkedGameMode::StartTurns()
{

	// Make sure Turns start
	NextTurn();
}

void ANetworkedGameMode::NextTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Next Turn"))

	GetWorld()->GetTimerManager().SetTimer(TurnHandle, this, &ANetworkedGameMode::NextTurn, 60.f, false);
}

void ANetworkedGameMode::EndAllTurns()
{

	// Make sure all timers are stopped
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}
