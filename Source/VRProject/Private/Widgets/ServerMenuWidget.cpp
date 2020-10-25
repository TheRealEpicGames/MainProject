// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ServerMenuWidget.h"
#include "Widgets/SServerMenu.h"
#include "OnlineSessionSettings.h"
#include "Networking/NetworkedGameInstance.h"

UServerMenuWidget::UServerMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//static ConstructorHelpers::FObjectFinder<UClass> NetworkedGameClass(TEXT("Class'/Game/Core/Networking/BPNetworkedGameInstance.BPNetworkedGameInstance_C'"));
	//TSubclassOf<class AActor> aclass = (UClass*)NetworkedGameClass.Object->GeneratedClass;
	GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance());
	FocusedMenu = 0;
}

TSharedRef<SWidget> UServerMenuWidget::RebuildWidget()
{
	SlateServerMenu = SNew(SServerMenu).WidgetOwner(this);

	return SlateServerMenu.ToSharedRef();
}

void UServerMenuWidget::FocusMenu(uint8 MenuIndex)
{ 
	this->FocusedMenu = MenuIndex; 
}

void UServerMenuWidget::HostLobbyClickedHandler()
{
	GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->HostLobby();
	}
	UE_LOG(LogTemp, Warning, TEXT("Host lobby Clicked"));
}

void UServerMenuWidget::FindLobbyClickedHandler()
{
	GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->FindLobbies(this);
	}
	UE_LOG(LogTemp, Warning, TEXT("Find lobby Clicked"));
}

void UServerMenuWidget::JoinLobbyClickedHandler()
{
	if (LobbyResults.Num() == 0)
		return;

	GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->JoinLobby(LobbyResults[FocusedMenu]);
	}
	UE_LOG(LogTemp, Warning, TEXT("Join lobby Clicked"));
}

void UServerMenuWidget::PopulateLobbyButtons(TArray<FBlueprintSessionResult> result)
{
	LobbyResults = result;
	FChildren* children = SlateServerMenu->GetChildren();
	UE_LOG(LogTemp, Warning, TEXT("Result %f"), result.Num());
	int i;
	for (i = 0; i < children->Num(); i++)
	{

	}
}

//TODO Write container for found lobbies, set updated lobby buttons on changes
// Write event that can be subscribed to for when find finishes?