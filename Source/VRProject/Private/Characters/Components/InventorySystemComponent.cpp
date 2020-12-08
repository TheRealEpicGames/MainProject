// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/InventorySystemComponent.h"
#include "Items/Item.h"

// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	InventoryMaxCapacity = 5;
	bIsInventoryOpen = false;
}


// Called when the game starts
void UInventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventorySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AItem* UInventorySystemComponent::AddItemToInventory(AItem* Item)
{
	AItem* CopiedItem;
	//If the item already exists in our inventory and cannot be stacked then this fails to add another to the inventory.
	if (Inventory.Contains(Item->ItemID) && (!Item->bCanStack || Inventory[Item->ItemID].StackedItems.Num() >= Item->MaxStackCapacity))
	{
		//TODO:Notify player that Item was not added to Inventory.
		UE_LOG(LogTemp, Warning, TEXT("Item was not added because capacity."));
		return Item;
	}
	else if (!Inventory.Contains(Item->ItemID))
	{
		if (Inventory.Num() < InventoryMaxCapacity)
		{
			CopiedItem = DuplicateObject(Item, NULL);

			Item->Destroy();

			TArray<AItem*> ItemsList;
			ItemsList.Add(CopiedItem);

			FItemStack Items;
			Items.StackedItems = ItemsList;

			Inventory.Add(CopiedItem->ItemID, Items);
		}
		else
		{
			//TODO:Notify player that Item was not added to Inventory.
			UE_LOG(LogTemp, Warning, TEXT("Item was not added."));
			return Item;
		}
	}
	else
	{
		CopiedItem = DuplicateObject(Item, NULL);

		Item->Destroy();

		Inventory[CopiedItem->ItemID].StackedItems.Add(CopiedItem);
	}

	return CopiedItem;
}

AItem* UInventorySystemComponent::RemoveItemFromInventory(AItem* Item, bool bDropItem)
{
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item is null."));
		return nullptr;
	}
	if (!Inventory.Contains(Item->ItemID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item not in inventory"));
		return nullptr;
	}

	if (!bDropItem)
	{
		Inventory[Item->ItemID].StackedItems.RemoveSingle(Item);
	}
	else
	{
		FActorSpawnParameters ItemSpawnParam;
		//ItemSpawnParam.Template = Item;

		//Behavior for future use
		AItem* CopiedItem = GetWorld()->SpawnActor<AItem>(Item->GetClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), ItemSpawnParam);
		Inventory[Item->ItemID].StackedItems.RemoveSingle(Item);
		Item = CopiedItem;
	}

	if (Inventory[Item->ItemID].StackedItems.Num() == 0)
	{
		Inventory.Remove(Item->ItemID);
	}
	

	return Item;
}

void UInventorySystemComponent::ClearInventory(bool bDropInventory)
{
	if (!bDropInventory)
	{
		Inventory.Empty();
	}
	else
	{
		//TODO: Spawn items in some sort of package or as individual items and drop on ground in front of player
		// Then empty Inventory.
		Inventory.Empty();
	}

}