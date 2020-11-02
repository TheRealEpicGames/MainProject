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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
	class UMotionControllerComponent* LeftMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
	class UMotionControllerComponent* RightMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
	class USkeletalMeshComponent* LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
	class USkeletalMeshComponent* RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	class UStaticMeshComponent* TeleportationMarker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleportation")
	class USplineComponent* TeleportPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	float TeleportProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	float TeleportProjectileRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	float TeleportSimulationTime;

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

	bool FindTeleportationDestination(TArray<FVector> &OutPath, FVector& OutLocation);

	void UpdateTeleportSpline(const TArray<FVector> &Path);

	void UpdateTeleportMarker();

	void StartFade(float FromAlpha, float ToAlpha);
};
