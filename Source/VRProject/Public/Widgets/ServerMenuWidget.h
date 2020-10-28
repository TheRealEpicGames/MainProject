// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "OnlineSessionSettings.h"
#include "FindSessionsCallbackProxy.h"
#include "ServerMenuWidget.generated.h"


/**
 * 
 */
UCLASS()
class VRPROJECT_API UServerMenuWidget : public UWidget
{
	GENERATED_BODY()

	private:
		UPROPERTY()
		class UNetworkedGameInstance* GameInstance;
	
	protected:
		virtual TSharedRef<SWidget> RebuildWidget() override;

		// The Slate server menu
		TSharedPtr<class SServerMenu> SlateServerMenu;

		TArray<FBlueprintSessionResult> LobbyResults;

	public:
		UServerMenuWidget(const FObjectInitializer& ObjectInitializer);

		UPROPERTY(BlueprintReadOnly)
		uint8 FocusedMenu;

		void FocusMenu(uint8 MenuIndex);

		void HostLobbyClickedHandler();
		void FindLobbyClickedHandler();
		void JoinLobbyClickedHandler();

		UFUNCTION(BlueprintCallable)
		void PopulateLobbyButtons(TArray<FBlueprintSessionResult> result);
};
