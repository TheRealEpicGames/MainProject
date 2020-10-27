// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class VRPROJECT_API SSlateMidGameMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlateMidGameMenu)
	{}

	SLATE_ARGUMENT(TWeakObjectPtr<class UMidGameMenuWidget>, WidgetOwner)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<class UMidGameMenuWidget> WidgetOwner;

	// Allows widget focus
	virtual bool SupportsKeyboardFocus() const override { return true; };

	FReply OnLeaveGameClicked() const;
	FReply OnSettingsClicked() const;
};
