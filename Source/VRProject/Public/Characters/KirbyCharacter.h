// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KirbyCharacter.generated.h"

UCLASS()
class VRPROJECT_API AKirbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKirbyCharacter();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* KirbyCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	class UStaticMeshComponent* TeleportationMarker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	float MaxTeleportDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	int MaxNumOfTeleports;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	int CurrentNumOfTeleports;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	float TeleportFadeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	FVector TeleportProjectionExtent;

private:

	bool bTeleporting;

	FTimerHandle TeleportUpdateLocationHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void BeginTeleport();

	void EndTeleport();

	void CancelTeleport();

	bool FindTeleportationDestination(FVector& OutLocation);

	void UpdateTeleportMarker();

	void StartFade(float FromAlpha, float ToAlpha);
};
