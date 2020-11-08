// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animation/KirbyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/KirbyCharacter.h"


void UKirbyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Kirby = Cast<AKirbyCharacter>(Pawn);
		}
	}
}

void UKirbyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		

		if (Kirby == nullptr)
		{
			Kirby = Cast<AKirbyCharacter>(Pawn);
		}

		
	}
}
