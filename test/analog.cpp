#include "base.h"

// Analog Menu
		
// The analog menu looks like this (not including the 1st 2 columns):
// 1:Analog
// 2: Channel: # #.##
// 3: Back
// 4:
// 5:
// 6:

AnalogMenu::AnalogMenu()
{
	index_m    = 2;
	maxIndex_m = 3;
	
	// Always start pointing at Channel 0 and analog channel values initialize to zero
	currentChannelNum_m   = 0;
	// currentChannelValue_m = 0.0;
	
	// Create a analog channel object for every channel on the module
	// (note that analog channels are numbered 1->8 in the WPILib API and on the module
	// but are 0->... here)
	for (int i=0; i <= MAX_ANALOG_CHANNEL; i++)
	{
		// Need to assign from existing pointers here if and analog ports are already
		// allocated in the the main RobotDemo constructor!
		channel_mp[i] = new AnalogChannel(i + 1);
	}
}

AnalogMenu::~AnalogMenu()
{
	for (int i=0; i <= MAX_ANALOG_CHANNEL; i++)
	{
		delete channel_mp[i];
	}
}

menuType AnalogMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 2: // Decrement channel pointer 
			currentChannelNum_m--;
			if (currentChannelNum_m < MIN_ANALOG_CHANNEL)
			{
				currentChannelNum_m = MAX_ANALOG_CHANNEL;
			}
			//currentChannelValue_m = channel_mp[currentChannelNum_m]->GetVoltage();
			break;
		case 3: // Return to previous menu
			return callingMenu_m;
		default:
			return ANALOG;
	};
		
	return ANALOG;
}

menuType AnalogMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 2: // Increment channel pointer 
			currentChannelNum_m++;
			if (currentChannelNum_m > MAX_ANALOG_CHANNEL)
			{
				currentChannelNum_m = MIN_ANALOG_CHANNEL;
			}
			//currentChannelValue_m = channel_mp[currentChannelNum_m]->GetVoltage();
			break;
		case 3: // We only allow a select left to return to a previous menu
			break;
		default:
			return ANALOG;
	};
	
	return ANALOG;
}

void AnalogMenu::UpdateDisplay ()
{
	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, "Analog");
	dsLCD->PrintfLine(LCD2, " Channel: %d %5.2f", currentChannelNum_m + 1, channel_mp[currentChannelNum_m]->GetVoltage());
	dsLCD->PrintfLine(LCD3, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
