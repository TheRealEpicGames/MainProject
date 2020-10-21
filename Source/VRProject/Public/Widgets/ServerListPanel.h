// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "ServerListPanel.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UServerListPanel : public UCanvasPanel
{
	GENERATED_BODY()
	
	public:
		UServerListPanel();

		UPROPERTY(BlueprintReadWrite)
		TArray<class UListedServerButton*> Buttons;

	private:
		TArray<class UCanvasPanelSlot*> ChildrenSlots;
};
