// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ServerListPanel.h"
#include "Widgets/ListedServerButton.h"
#include "Components/CanvasPanelSlot.h"

UServerListPanel::UServerListPanel()
{
	//Add buttons to the panel
	int i;
	for (i = 0; i < 10; i++)
	{
		
		
		// Set up each individual button, and format them on the canvas
		FName buttonName = MakeUniqueObjectName(GetWorld(), UListedServerButton::StaticClass());

		
		//UCanvasPanelSlot* panel = AddChildToCanvas(button);
		//panel->SetSize(FVector2D(800, 60));
		//panel->SetPosition(FVector2D(0, i * 100.f));

		//Buttons.Add(button);
		//ChildrenSlots.Add(panel);
	}
}