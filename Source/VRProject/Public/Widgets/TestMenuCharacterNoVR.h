// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestMenuCharacterNoVR.generated.h"

UCLASS()
class VRPROJECT_API ATestMenuCharacterNoVR : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestMenuCharacterNoVR();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = WidgetInteraction)
	class UWidgetInteractionComponent* WidgetInteractor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStatus)
	bool bIsGhost;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveHorizontal(float Scale);
	void MoveForward(float Scale);
	virtual void Turn(float magnitude);
	virtual void Tilt(float magnitude);
	void LeftMousePress();
	void LeftMouseRelease();

	void EnableGhostStatus();

	UFUNCTION(NetMulticast, Unreliable)
	void NotifyGhostStatusChanged();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateName(const FString& NewName);
};
