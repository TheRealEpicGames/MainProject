// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/ServerMenuWidget.h"
#include "Networking/NetworkedPlayerController.h"
#include "NetworkedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UNetworkedGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:

		UPROPERTY(BlueprintReadWrite)
			bool bIsHostingLobby;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Players)
		TArray<ANetworkedPlayerController*> PlayerControllerList;

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void HostLobby();

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void FindLobbies(UServerMenuWidget* Menu);
	
		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void JoinLobby(FBlueprintSessionResult ChosenResult);

		UFUNCTION( BlueprintCallable, BlueprintImplementableEvent)
		void LeaveLobby();

};
