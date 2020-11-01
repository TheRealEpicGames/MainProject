// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LobbySettingsWidget.h"
#include "Widgets/SSlateLobbySettings.h"
#include "Networking/NetworkedGameState.h"

ULobbySettingsWidget::ULobbySettingsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ChosenLevel = TEXT("TestLevel");
}

TSharedRef<SWidget> ULobbySettingsWidget::RebuildWidget()
{
	SlateLobbyMenu = SNew(SSlateLobbySettings).WidgetOwner(this);

	return SlateLobbyMenu.ToSharedRef();
}

void ULobbySettingsWidget::StartLevelHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Clicked"))
	ANetworkedGameState* GameState = Cast<ANetworkedGameState>(GetWorld()->GetGameState());
	if(GameState)
		GameState->StartMatchWithLevel(ChosenLevel);
}