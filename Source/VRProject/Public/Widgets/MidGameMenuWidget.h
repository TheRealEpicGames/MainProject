// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MidGameMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UMidGameMenuWidget : public UWidget
{
	GENERATED_BODY()
	
	protected:
		virtual TSharedRef<SWidget> RebuildWidget() override;

		// The Slate menu
		TSharedPtr<class SSlateMidGameMenu> SlateMidGameMenu;

	public:
		UMidGameMenuWidget(const FObjectInitializer& ObjectInitializer);

		void LeaveLobbyClickedHandler();
		void SettingsClickedHandler();
};
