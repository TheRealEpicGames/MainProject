// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState :uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Inventory UMETA(DisplayName = "Inventory"),

	EIS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class VRPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	class USphereComponent* BaseCollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystemComponent* IdleParticlesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	bool bIdleParticleActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystem* OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystem* GrabbedParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
	class USoundCue* OnPickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Properties")
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Properties")
	float RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Interaction")
	EItemState CurrentItemState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Interaction")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Interaction")
	bool bCanStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Interaction")
	bool bEquipable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Interaction")
	int MaxStackCapacity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent)
	void Grab(AActor* GrabbingActor);

	UFUNCTION(BlueprintNativeEvent)
	void Release(AActor* GrabbingActor);

	UFUNCTION(BlueprintNativeEvent)
	void Use(AActor* GrabbingActor);

};
