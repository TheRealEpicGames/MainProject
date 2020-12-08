// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "Components/WidgetInteractionComponent.h"
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

	void SetGrabbedItem(class AItem* Item);

	void GrabItem(class AItem* Item);

	void ReleaseItem(class AItem* Item);

	void UseItem(class AItem* Item);

	UFUNCTION(BlueprintCallable)
	void SetGripState(EGripState State) { GripState = State; UpdateAnimState(); }

	void UsePointer();

	void ReleasePointer();

	void DeactivatePointer();

	void OpenInventoryMenu();

	void CloseInventoryMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void InventoryMenuOpened();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USkeletalMeshComponent* HandMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemCollision")
	class USphereComponent* BaseCollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere, Category = "UI Interaction")
	UWidgetInteractionComponent* Pointer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction")
	class UWidgetComponent* InventoryMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAbility")
	EGripState GripState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAbility")
	class AItem* GrabbableItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAbility")
	class AItem* GrabbedItem;

	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	class UHapticFeedbackEffect_Base* GrabHapticEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAbility")
	bool bPointerActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAbility")
	bool bInventoryMenuActive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//Default subobject
	

	UFUNCTION()
	void HandBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void HandEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void UpdateAnimState();
};
