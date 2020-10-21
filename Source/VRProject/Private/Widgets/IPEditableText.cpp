// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/IPEditableText.h"
#include "Internationalization/Text.h"

UIPEditableText::UIPEditableText() : Super()
{
	IP_buffer = "";
}

void UIPEditableText::AddToIP(FString sym)
{
	// Don't add if already at max length for an IP
	if (IP_buffer.Len() == 15)
		return;

	IP_buffer.Append(sym);
}

void UIPEditableText::DeleteFromIP()
{
	if (IP_buffer.Len() == 0)
		return;

	IP_buffer.RemoveAt(IP_buffer.Len() - 1, 1, false);
}

bool UIPEditableText::IsIPValid()
{
	// Needs 3 periods with numbers (< 256) between them to represent an IP
	FString num = TEXT("");
	uint8 periodCount = 0;
	uint8 i;
	for (i = 0; i < IP_buffer.Len(); i++)
	{
		if (IP_buffer[i] == '.')
		{
			// Only three periods in dotted decimal, and there needs to be a number between them
			if (++periodCount > 3 || num.Equals(TEXT("")))
				return false;

			// Make char array from string
			TCHAR* charStr = new TCHAR[num.Len()];
			int j;
			for (j = 0; j < num.Len(); j++)
			{
				charStr[j] = num[j];
			}

			// IP values only between 0 through 255
			uint32 val = FCString::Atoi(charStr);
			if (val > 255)
				return false;

			num = TEXT("");
			delete charStr;
		}
		else
			num.AppendChar(IP_buffer[i]);
	}

	// There should be 3 periods
	if (periodCount != 3)
		return false;

	// Handle last num
	// Make char array from string
	TCHAR* charStr = new TCHAR[num.Len()];
	int j;
	for (j = 0; j < num.Len(); j++)
	{
		charStr[j] = num[j];
	}

	// IP values only between 0 through 255
	uint32 val = FCString::Atoi(charStr);
	if (val > 255)
		return false;

	delete charStr;

	return true;
}