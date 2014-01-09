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

void RelayMenu::IncrementChannelValue ()
{
	switch (currentChannelValue_m)
	{
		case Relay::kOff:
			currentChannelValue_m =  Relay::kOn;
			break;
		case Relay::kOn:
			currentChannelValue_m =  Relay::kForward;
			break;
		case Relay::kForward:
			currentChannelValue_m =  Relay::kReverse;
			break;
		case Relay::kReverse:
			currentChannelValue_m =  Relay::kOff;
			break;
		default:
			// Should never get here. A swerr here WBN
			currentChannelValue_m =  Relay::kOff;
	}
}

void RelayMenu::DecrementChannelValue ()
{
	switch (currentChannelValue_m)
	{
		case Relay::kOff:
			currentChannelValue_m = Relay::kReverse;
			break;
		case Relay::kOn:
			currentChannelValue_m =  Relay::kOff;
			break;
		case Relay::kForward:
			currentChannelValue_m =  Relay::kOn;
			break;
		case Relay::kReverse:
			currentChannelValue_m =  Relay::kForward;
			break;
		default:
			// Should never get here. A swerr here WBN
			currentChannelValue_m =  Relay::kOff;
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
		case 3: // Decrement channel value
			DecrementChannelValue();
			channel_mp[currentChannelNum_m]->Set(currentChannelValue_m);
			break;
		case 4: // Return to previous menu
			return callingMenu_m;
		default:
			break;
	}
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
			IncrementChannelValue();
			channel_mp[currentChannelNum_m]->Set(currentChannelValue_m);
			break;
		default:
			break;
	}
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
	dsLCD->PrintfLine(LCD2, " Channel: %d", currentChannelNum_m + 1);
	dsLCD->PrintfLine(LCD3, " Set: %s", valueStrings[currentValue]);
	dsLCD->PrintfLine(LCD4, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
