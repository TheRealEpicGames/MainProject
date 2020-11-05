// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SSlateMidGameMenu.h"
#include "Widgets/MidGameMenuWidget.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "LobbyMenu"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlateMidGameMenu::Construct(const FArguments& InArgs)
{
	WidgetOwner = InArgs._WidgetOwner;
	bCanSupportFocus = true;

	const FText LeaveLobbyText = LOCTEXT("LeaveLobby", "Leave Lobby");
	const FText SettingsText = LOCTEXT("Settings", "Settings");
	
	FSlateFontInfo ButtonFont = FCoreStyle::Get().GetFontStyle("Roboto");
	ButtonFont.Size = 48.f;

	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this, &SSlateMidGameMenu::OnLeaveGameClicked)
			[
				SNew(STextBlock)
				.Font(ButtonFont)
				.Text(LeaveLobbyText)
				.Justification(ETextJustify::Center)
			]
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this, &SSlateMidGameMenu::OnSettingsClicked)
			[
				SNew(STextBlock)
				.Font(ButtonFont)
				.Text(SettingsText)
				.Justification(ETextJustify::Center)
			]
		]
	];
	
}

FReply SSlateMidGameMenu::OnLeaveGameClicked() const
{
	WidgetOwner->LeaveLobbyClickedHandler();
	return FReply::Handled();
}

FReply SSlateMidGameMenu::OnSettingsClicked() const
{
	WidgetOwner->SettingsClickedHandler();
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE