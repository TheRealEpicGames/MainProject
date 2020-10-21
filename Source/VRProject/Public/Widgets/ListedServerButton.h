// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ListedServerButton.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UListedServerButton : public UButton
{
	GENERATED_BODY()

	public:
		UListedServerButton();

		void RegisterListener();
	
};
