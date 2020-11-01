// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "LobbySettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API ULobbySettingsWidget : public UWidget
{
	GENERATED_BODY()
	
	protected:
		virtual TSharedRef<SWidget> RebuildWidget() override;

		// The Slate server menu
		TSharedPtr<class SSlateLobbySettings> SlateLobbyMenu;

	public:

		UPROPERTY(BlueprintReadWrite)
		FString ChosenLevel;

		ULobbySettingsWidget(const FObjectInitializer& ObjectInitializer);

		void StartLevelHandler();
};
