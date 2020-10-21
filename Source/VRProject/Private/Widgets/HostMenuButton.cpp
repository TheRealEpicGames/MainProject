// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HostMenuButton.h"
#include "Kismet/GameplayStatics.h"

UHostMenuButton::UHostMenuButton() : Super()
{

}

void UHostMenuButton::HandleClickEvent()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("MotionControllerMap")), true, "listen");
}