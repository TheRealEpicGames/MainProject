// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SServerMenu.h"
#include "Widgets/ServerMenuWidget.h"

#define LOCTEXT_NAMESPACE "LobbyMenu"

void SServerMenu::Construct(const FArguments& Args)
{
	WidgetOwner = Args._WidgetOwner;
	bCanSupportFocus = true;

	const FText HostLobbyText = LOCTEXT("HostLobby", "Host Lobby");
	const FText FindLobbyText = LOCTEXT("FindLobby", "Find Lobby");
	const FText JoinLobbyText = LOCTEXT("JoinLobby", "Join Lobby");

	FSlateFontInfo ButtonFont = FCoreStyle::Get().GetFontStyle("Roboto");
	ButtonFont.Size = 48.f;

	FSlateFontInfo LobbyFont = ButtonFont;
	LobbyFont.Size = 32.f;

	// Construct widget
	ChildSlot
		[
			// Horizontal Box containing both halves of the menu
			SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					// Vertical box for Lobby Buttons
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Host Lobby Button
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnHostLobbyClicked)
						[
							SNew(STextBlock)
							.Font(ButtonFont)
							.Text(HostLobbyText)
							.Justification(ETextJustify::Center)
						]
						
					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Find Lobby Button
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnFindLobbyClicked)
						[
							SNew(STextBlock)
							.Font(ButtonFont)
							.Text(FindLobbyText)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Join Lobby Button
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnJoinLobbyClicked)
						[
							SNew(STextBlock)
							.Font(ButtonFont)
							.Text(JoinLobbyText)
							.Justification(ETextJustify::Center)
						]

					]
					
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					// Vertical box for Lobby Choices
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 0
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby0Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 1
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby1Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 2
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby2Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 3
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby3Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 4
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby4Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 5
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby5Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 6
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby6Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 7
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby7Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 8
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby8Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]
					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						// Lobby 9
						SNew(SButton)
						.OnClicked(this, &SServerMenu::OnLobby9Clicked)
						[
							SNew(STextBlock)
							.Font(LobbyFont)
							.Justification(ETextJustify::Center)
						]

					]
					


				]
		];
}



/**********************************************************************

						BUTTON CLICK HANDLERS

***********************************************************************/

FReply SServerMenu::OnHostLobbyClicked() const
{
	// Let the widget owner do all the work
	WidgetOwner->HostLobbyClickedHandler();
	return FReply::Handled();
}

FReply SServerMenu::OnFindLobbyClicked() const
{
	WidgetOwner->FindLobbyClickedHandler();
	return FReply::Handled();
}

FReply SServerMenu::OnJoinLobbyClicked() const
{
	WidgetOwner->JoinLobbyClickedHandler();
	return FReply::Handled();
}

FReply SServerMenu::OnLobby0Clicked() const
{
	WidgetOwner->FocusMenu(0);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby1Clicked() const
{
	WidgetOwner->FocusMenu(1);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby2Clicked() const
{
	WidgetOwner->FocusMenu(2);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby3Clicked() const
{
	WidgetOwner->FocusMenu(3);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby4Clicked() const
{
	WidgetOwner->FocusMenu(4);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby5Clicked() const
{
	WidgetOwner->FocusMenu(5);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby6Clicked() const
{
	WidgetOwner->FocusMenu(6);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby7Clicked() const
{
	WidgetOwner->FocusMenu(7);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby8Clicked() const
{
	WidgetOwner->FocusMenu(8);
	return FReply::Handled();
}

FReply SServerMenu::OnLobby9Clicked() const
{
	WidgetOwner->FocusMenu(9);
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE