#include "base.h"

// Solenoid Menu
		
	
SolenoidMenu::SolenoidMenu()
{
	// The solenoid menu looks like this (not including the 1st 2 columns):
	// 1:Solenoid
	// 2: Channel: #
	// 3: Set: $$$
	// 4: Back
	// 5:
	// 6:

	index_m    = 2;
	maxIndex_m = 4;
	
	// Always start pointing at Channel and solenoid channels initialize to off
	currentChannelNum_m   = 0;
	//currentChannelValue_m = FALSE; // this can be removed (just set to !get)
	
	// Create a solenoid object for every channel on the module
	// (note that solenoid channels are numbered 1->8 in the API but 0->7 on the module)
	for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
	{
		// Need to assign from existing pointers here if any solenoid ports are already
		// allocated in the the main RobotDemo constructor! Use SetSolenoidTableEntry to
		// overide any of the entries in this table as required.
		channel_mp[i] = new Solenoid(i + 1);
	}
}

SolenoidMenu::~SolenoidMenu()
{
	for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
	{
		// This will delete any objects added to the table by SetSolenoidTableEntry without
		// the original creator knowing about it. However, this destructor won't ever
		// be called
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
			//currentChannelValue_m = channel_mp[currentChannelNum_m]->Get();
			break;
		case 3: // Toggle channel value (it can only be true of false)
			//currentChannelValue_m = !currentChannelValue_m;
			//channel_mp[currentChannelNum_m]->Set(currentChannelValue_m);
			channel_mp[currentChannelNum_m]->Set(!channel_mp[currentChannelNum_m]->Get());
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
			//currentChannelValue_m = channel_mp[currentChannelNum_m]->Get();
			break;
		case 3: // Increment channel value
			//currentChannelValue_m = !currentChannelValue_m;
			//channel_mp[currentChannelNum_m]->Set(currentChannelValue_m + 1);
			channel_mp[currentChannelNum_m]->Set(!channel_mp[currentChannelNum_m]->Get());
			break;
		case 4: // We only allow a select left to return to the previous menu
			// return callingMenu_m;
		default:
			return SOLENOID;
	};
	
	return SOLENOID;
}

void SolenoidMenu::SetSolenoidPWMTableEntry (int index, Solenoid * pointer)
{
	// Don't leak the object already pointed to by the table entry...
	delete channel_mp[index - 1];
	
	channel_mp[index - 1] = pointer;
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
