// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldMenuActor.generated.h"

UCLASS()
class VRPROJECT_API AWorldMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMenuActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	class UWidgetComponent* MenuWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// An event to be called whenever the lone button of this menu is called
	UFUNCTION(BlueprintCallable)
	virtual void OnButtonClicked();

};
