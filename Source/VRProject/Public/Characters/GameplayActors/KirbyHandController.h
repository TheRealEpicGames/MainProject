// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "KirbyHandController.generated.h"

UENUM(BlueprintType)
enum class EGripState :uint8
{
	EGS_Grab UMETA(DisplayName = "Grab"),
	EGS_Open UMETA(DisplayName = "Open"),
	EGS_CanGrab UMETA(DisplayName = "CanGrab"),

	EGS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class VRPROJECT_API AKirbyHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKirbyHandController();

	void SetHand(EControllerHand Hand) { MotionController->SetTrackingSource(Hand); }

	void SetGrabbableItem(class AItem* Item);

	void GrabItem(class AItem* Item);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAbility")
	EGripState GripState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAbility")
	class AItem* GrabbableItem;

	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	class UHapticFeedbackEffect_Base* GrabHapticEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//Default subobject
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMotionControllerComponent* MotionController;

	void HandBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void HandEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
