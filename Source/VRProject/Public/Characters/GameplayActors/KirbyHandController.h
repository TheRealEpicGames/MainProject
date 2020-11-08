// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "KirbyHandController.generated.h"

UCLASS()
class VRPROJECT_API AKirbyHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKirbyHandController();

	void SetHand(EControllerHand Hand) { MotionController->SetTrackingSource(Hand); }

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
