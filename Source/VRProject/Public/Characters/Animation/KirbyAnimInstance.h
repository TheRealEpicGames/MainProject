// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/GameplayActors/KirbyHandController.h"
#include "KirbyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UKirbyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AKirbyCharacter* Kirby;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hands)
	EGripState LeftGripState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hands)
	EGripState RightGripState;
};
