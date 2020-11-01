// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class VRPROJECT_API SSlateLobbySettings : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlateLobbySettings){}
	SLATE_ARGUMENT(TWeakObjectPtr<class ULobbySettingsWidget>, WidgetOwner)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& Args);

	TWeakObjectPtr<class ULobbySettingsWidget> WidgetOwner;

	// Allows widget focus
	virtual bool SupportsKeyboardFocus() const override { return true; };

	FReply StartLevelHandler() const;
};
