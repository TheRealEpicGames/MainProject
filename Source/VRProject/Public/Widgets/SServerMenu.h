// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SServerMenu : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SServerMenu) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class UServerMenuWidget>, WidgetOwner)

	SLATE_END_ARGS()

	void Construct(const FArguments& Args);

	TWeakObjectPtr<class UServerMenuWidget> WidgetOwner;

	// Allows widget focus
	virtual bool SupportsKeyboardFocus() const override { return true; };

	FReply OnHostLobbyClicked() const;
	FReply OnFindLobbyClicked() const;
	FReply OnJoinLobbyClicked() const;

	// Lobby button click handlers, only serves to focus which lobby will be used
	FReply OnLobby0Clicked() const;
	FReply OnLobby1Clicked() const;
	FReply OnLobby2Clicked() const;
	FReply OnLobby3Clicked() const;
	FReply OnLobby4Clicked() const;
	FReply OnLobby5Clicked() const;
	FReply OnLobby6Clicked() const;
	FReply OnLobby7Clicked() const;
	FReply OnLobby8Clicked() const;
	FReply OnLobby9Clicked() const;
	
};
