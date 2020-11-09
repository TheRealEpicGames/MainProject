// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/GameplayActors/KirbyHandController.h"
#include "MotionControllerComponent.h"
#include "KirbyHandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UKirbyHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hands)
	EGripState GripState;
};
