#include "base.h"

// Digital IO State Menu
		
	
DigitalIOStateMenu::DigitalIOStateMenu()
{
	// The relay menu looks like this (not including the 1st 2 columns):
	// 1:Digital IO State
	// 2: Channel: #
	// 3: Direction: $$$
	// 4: Set|Value: #
	// 5: Back
	// 6:

	index_m    = 2;
	maxIndex_m = 5;
	
	currentChannelNum_m   = 0;
	currentChannelValue_m = false;
	digitalIO_mp = DigitalIO::GetInstance();
}

DigitalIOStateMenu::~DigitalIOStateMenu()
{
}

menuType DigitalIOStateMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 2: // Decrement channel pointer 
			currentChannelNum_m--;
			if (currentChannelNum_m < MIN_DIO_CHANNEL)
			{
				currentChannelNum_m = NUM_DIO_CHANNELS - 1;
			}
			break;
		case 3: // Toggle channel direction
			if (digitalIO_mp->IsInput(currentChannelNum_m))
			{
				digitalIO_mp->SetToInput(currentChannelNum_m, false);
			}
			else
			{
				digitalIO_mp->SetToInput(currentChannelNum_m, true);
			}
			break;
		case 4: // Toggle channel value (it can only be true or false)
			if (!digitalIO_mp->IsInput(currentChannelNum_m))
			{
				currentChannelValue_m = !currentChannelValue_m;
				digitalIO_mp->SetValue(currentChannelNum_m, currentChannelValue_m);
			}
			break;
		case 5: // Return to previous menu
			return callingMenu_m;
		default:
			return DIGITAL_IO_STATE;
	};
	
	return DIGITAL_IO_STATE;
}

menuType DigitalIOStateMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 2: // Increment channel index 
			currentChannelNum_m++;
			if (currentChannelNum_m >= NUM_DIO_CHANNELS)
			{
				currentChannelNum_m = MIN_DIO_CHANNEL;
			}
			break;
		case 3: // Toggle channel direction
			if (digitalIO_mp->IsInput(currentChannelNum_m))
			{
				digitalIO_mp->SetToInput(currentChannelNum_m, false);
			}
			else
			{
				digitalIO_mp->SetToInput(currentChannelNum_m, true);
			}
			break;
		case 4: // Toggle channel value (it can only be true or false)
			if (!digitalIO_mp->IsInput(currentChannelNum_m))
			{
				currentChannelValue_m = !currentChannelValue_m;
				digitalIO_mp->SetValue(currentChannelNum_m, currentChannelValue_m);
			}
			break;
		case 5: // We only allow a select left to return to the previous menu
		default:
			return DIGITAL_IO_STATE;
	};
		
	return DIGITAL_IO_STATE;
}

void DigitalIOStateMenu::UpdateDisplay ()
{	
	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, "Digital IO State");
	dsLCD->PrintfLine(LCD2, " Channel: %d", currentChannelNum_m + 1);
	
	if (digitalIO_mp->IsInput(currentChannelNum_m))
	{
		dsLCD->PrintfLine(LCD3, " Direction: IN");
		dsLCD->PrintfLine(LCD4, " Value: %d", digitalIO_mp->GetValue(currentChannelNum_m));
	}
	else
	{
		dsLCD->PrintfLine(LCD3, " Direction: OUT");
		dsLCD->PrintfLine(LCD4, " Set: %d", currentChannelValue_m);
	}
	
	dsLCD->PrintfLine(LCD5, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
