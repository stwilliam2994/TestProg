#include "base.h"

// RELAY Menu
		
	
RelayMenu::RelayMenu()
{
	// The relay menu looks like this (not including the 1st 2 columns):
	// 1:RELAY
	// 2: Channel: #
	// 3: Set: $$$
	// 4: Back
	// 5:
	// 6:

	index_m    = 2;
	maxIndex_m = 4;
	
	currentChannelNum_m   = 0;
	currentChannelValue_m = Relay::kOff;
	
	// Create a relay object for every channel on the module
	// (note that relay channels are numbered 1->8 in the API but 0->7 here)
	for (int i=0; i <= MAX_RELAY_CHANNEL; i++)
	{
		// Need to assign from existing pointers here if and relay ports are already
		// allocated in the the main RobotDemo constructor!

		channel_mp[i] = new Relay(i + 1);
	}
}

RelayMenu::~RelayMenu()
{
	for (int i=0; i <= MAX_RELAY_CHANNEL; i++)
	{
		delete channel_mp[i];
	}
}

Relay::Value RelayMenu::IncrementChannelValue ()
{
	switch (currentChannelValue_m)
	{
		case Relay::kOff:
			return Relay::kOn;
			break;
		case Relay::kOn:
			return Relay::kForward;
			break;
		case Relay::kForward:
			return Relay::kReverse;
			break;
		case Relay::kReverse:
			return Relay::kOff;
			break;
		default:
			// Should never get here. A swerr here WBN
			return Relay::kOff;
	}
}

Relay::Value RelayMenu::DecrementChannelValue ()
{
	switch (currentChannelValue_m)
	{
		case Relay::kOff:
			return Relay::kReverse;
			break;
		case Relay::kOn:
			return Relay::kOff;
			break;
		case Relay::kForward:
			return Relay::kOn;
			break;
		case Relay::kReverse:
			return Relay::kForward;
			break;
		default:
			// Should never get here. A swerr here WBN
			return Relay::kOff;
	}
}

int RelayMenu::RelayValueToInt (Relay::Value value)
{
	switch (value)
	{
		case Relay::kOff:
			return 0;
			break;
		case Relay::kOn:
			return 1;
			break;
		case Relay::kForward:
			return 2;
			break;
		case Relay::kReverse:
			return 3;
			break;
		default:
			// Should never get here. A swerr here WBN
			return 0;
	}
}
menuType RelayMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 2: // Decrement channel pointer 
			currentChannelNum_m--;
			if (currentChannelNum_m < MIN_RELAY_CHANNEL)
			{
				currentChannelNum_m = MAX_RELAY_CHANNEL;
			}
			currentChannelValue_m = channel_mp[currentChannelNum_m]->Get();
			break;
		case 3: // Toggle channel value (it can only be true of false)
			IncrementChannelValue();
			channel_mp[RelayValueToInt(currentChannelValue_m)]->Set(currentChannelValue_m);
			break;
		case 4: // Return to previous menu
			return callingMenu_m;
		default:
			return DIGITAL_RELAY;
	};
	
	// This is now done in the main loop
	// UpdateDisplay();
	
	return DIGITAL_RELAY;
}

menuType RelayMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 2: // Increment channel pointer 
			currentChannelNum_m++;
			if (currentChannelNum_m > MAX_RELAY_CHANNEL)
			{
				currentChannelNum_m = MIN_RELAY_CHANNEL;
			}
			currentChannelValue_m = channel_mp[currentChannelNum_m]->Get();
			break;
		case 3: // Increment channel value
			DecrementChannelValue();
			channel_mp[RelayValueToInt(currentChannelValue_m)]->Set(currentChannelValue_m);
			break;
		case 4: // We only allow a select left to return to the previous menu
			// return callingMenu_m;
		default:
			return DIGITAL_RELAY;
	};
	
	// This is now done in the main loop
	// UpdateDisplay();
	
	return DIGITAL_RELAY;
}

void RelayMenu::UpdateDisplay ()
{
	const char * valueStrings[] = {
			"Off",
			"On",
			"Forward",
			"Reverse"
	};
	
	int currentValue = RelayValueToInt(channel_mp[currentChannelNum_m]->Get());
	
	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, "RELAY");
	dsLCD->PrintfLine(LCD2, " Channel: %d", currentChannelNum_m);
	dsLCD->PrintfLine(LCD3, " Set: %s", valueStrings[currentValue]);
	dsLCD->PrintfLine(LCD4, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
