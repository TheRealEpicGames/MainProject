// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableText.h"
#include "IPEditableText.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UIPEditableText : public UEditableText
{
	GENERATED_BODY()
	
	public:
		UIPEditableText();

		UPROPERTY(BlueprintReadOnly)
		FString IP_buffer;
		
		UFUNCTION(BlueprintCallable)
		void AddToIP(FString sym);

		UFUNCTION(BlueprintCallable)
		void DeleteFromIP();

		UFUNCTION(BlueprintCallable)
		bool IsIPValid();
};
