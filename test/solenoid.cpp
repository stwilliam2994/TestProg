#include "base.h"

// Solenoid Menu
	
#define MIN_SOLENOID_CHANNEL 0
#define MAX_SOLENOID_CHANNEL 7
	
// The solenoid menu looks like this (not including the 1st 2 columns):
// 1:Solenoid
// 2: Channel: #
// 3: Set: $$$
// 4: Back
// 5:
// 6:
	
SolenoidMenu::SolenoidMenu()
{
	// The index control can point at Channel, Set, and Back
	index_m    = 2;
	maxIndex_m = 4;
	
	// Always start pointing at Channel and solenoid channels initialize to off
	currentChannelNum_m   = 0;
	currentChannelValue_m = FALSE;
	
	// Create a solenoid object for every channel on the module
	// (note that solenoid channels are numbered 1->8 in the API but 0->7 on the module)
	for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
	{
		channel_mp[i] = new Solenoid(i + 1);
	}
}

SolenoidMenu::~SolenoidMenu()
{
	for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
	{
		delete channel_mp[i];
	}
}

menuType SolenoidMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 2: // Decrement channel pointer 
			currentChannelNum_m--;
			if (currentChannelNum_m < MIN_SOLENOID_CHANNEL)
			{
				currentChannelNum_m = MAX_SOLENOID_CHANNEL;
			}
			currentChannelValue_m = channel_mp[currentChannelNum_m]->Get();
			break;
		case 3: // Toggle channel value (it can only be true of false)
			currentChannelValue_m = !currentChannelValue_m;
			channel_mp[currentChannelNum_m]->Set(currentChannelValue_m);
			break;
		case 4: // Return to previous menu
			return callingMenu_m;
		default:
			return SOLENOID;
	};
	
	// This is now done in the main loop
	// UpdateDisplay();
	
	return SOLENOID;
}

menuType SolenoidMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 2: // Increment channel pointer 
			currentChannelNum_m++;
			if (currentChannelNum_m > MAX_SOLENOID_CHANNEL)
			{
				currentChannelNum_m = MIN_SOLENOID_CHANNEL;
			}
			currentChannelValue_m = channel_mp[currentChannelNum_m]->Get();
			break;
		case 3: // Increment channel value
			currentChannelValue_m = !currentChannelValue_m;
			channel_mp[currentChannelNum_m]->Set(currentChannelValue_m + 1);
			break;
		case 4: // We only allow a select left to return to the previous menu
			// return callingMenu_m;
		default:
			return SOLENOID;
	};
	
	// This is now done in the main loop
	// UpdateDisplay();
	
	return SOLENOID;
}

void SolenoidMenu::UpdateDisplay ()
{
	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, "Solenoid");
	dsLCD->PrintfLine(LCD2, " Channel: %d", currentChannelNum_m);
	dsLCD->PrintfLine(LCD3, " Set: %s", channel_mp[currentChannelNum_m]->Get() ? "ON" : "OFF");
	dsLCD->PrintfLine(LCD4, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
