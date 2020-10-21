// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


/**
 * 
 */
class VRPROJECT_API Weapon : public AActor
{
public:
	Weapon();
	UPROPERTY(EditAnywhere)
		class USkletalMeshComponent* MeshComp;

	~Weapon();

protected:
	//virtual void BeginPlay() override;

public:
	void Fire();
};
