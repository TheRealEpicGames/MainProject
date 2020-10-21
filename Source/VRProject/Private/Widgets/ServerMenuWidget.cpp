// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ServerMenuWidget.h"
#include "Widgets/SServerMenu.h"

UServerMenuWidget::UServerMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameInstance = GetGameInstance();
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
	UE_LOG(LogTemp, Warning, TEXT("Host lobby Clicked"));
}

void UServerMenuWidget::FindLobbyClickedHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Find lobby Clicked"));
}

void UServerMenuWidget::JoinLobbyClickedHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Join lobby Clicked"));
}