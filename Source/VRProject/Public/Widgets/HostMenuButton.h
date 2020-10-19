// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "HostMenuButton.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UHostMenuButton : public UButton
{
	GENERATED_BODY()
	
public:

	UHostMenuButton();

	UFUNCTION(BlueprintCallable)
	void HandleClickEvent();
};
