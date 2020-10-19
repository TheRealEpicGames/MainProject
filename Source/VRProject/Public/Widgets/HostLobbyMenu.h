// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WorldMenuActor.h"
#include "HostLobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API AHostLobbyMenu : public AWorldMenuActor
{
	GENERATED_BODY()

public:
	virtual void OnButtonClicked() override;
	
};
