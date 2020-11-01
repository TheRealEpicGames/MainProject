// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SSlateLobbySettings.h"
#include "Widgets/LobbySettingsWidget.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "LobbyMenu"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlateLobbySettings::Construct(const FArguments& InArgs)
{
	WidgetOwner = InArgs._WidgetOwner;
	bCanSupportFocus = true;

	FSlateFontInfo ButtonFont = FCoreStyle::Get().GetFontStyle("Roboto");
	ButtonFont.Size = 48.f;

	const FText StartLevelText = LOCTEXT("StartLevel", "Start Game");
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			// Start match
			SNew(SButton)
			.OnClicked(this, &SSlateLobbySettings::StartLevelHandler)
			[
				SNew(STextBlock)
				.Font(ButtonFont)
				.Text(StartLevelText)
				.Justification(ETextJustify::Center)
			]
		]
	];
	
}

FReply SSlateLobbySettings::StartLevelHandler() const
{
	UE_LOG(LogTemp, Warning, TEXT("Slate Start"))
	WidgetOwner.Get()->StartLevelHandler();

	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE