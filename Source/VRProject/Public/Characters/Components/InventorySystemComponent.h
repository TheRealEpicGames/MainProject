// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"

class AItem;

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
		TArray<AItem*> StackedItems;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRPROJECT_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventorySystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	class ACharacter* Char;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	TMap<FName, FItemStack> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	AItem* ActiveOverlappingItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	int InventoryMaxCapacity;

	//UI Dependent Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	bool bIsInventoryOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	AItem* CurrentlySelectedItem;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

	UFUNCTION(BlueprintCallable)
	AItem* AddItemToInventory(AItem* Item);

	UFUNCTION(BlueprintCallable)
	AItem* RemoveItemFromInventory(AItem* Item, bool bDropItem);

	UFUNCTION(BlueprintCallable)
	void ClearInventory(bool bDropInventory);
};
