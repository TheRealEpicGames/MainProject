// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MidGameMenuWidget.h"
#include "Widgets/SSlateMidGameMenu.h"
#include "Networking/NetworkedGameInstance.h"

UMidGameMenuWidget::UMidGameMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

TSharedRef<SWidget> UMidGameMenuWidget::RebuildWidget()
{
	SlateMidGameMenu = SNew(SSlateMidGameMenu).WidgetOwner(this);

	return SlateMidGameMenu.ToSharedRef();
}

void UMidGameMenuWidget::LeaveLobbyClickedHandler()
{
	UNetworkedGameInstance* GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->LeaveLobby();
	}
}

void UMidGameMenuWidget::SettingsClickedHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Settings Clicked"))
}