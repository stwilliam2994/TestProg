#include "base.h"

// Solenoid Menu
		
SolenoidMenu::SolenoidMenu()
{
	// The solenoid menu looks like this (not including the 1st 2 columns):
	// 1: Solenoid: [Single|Dual]
	// 2: Channel: #
	// 3: Set: $$$
	// 4: Back
	// 5:
	// 6:

	minIndex_m = 1;
	index_m    = 2;
	maxIndex_m = 5;
	mode_m     = SINGLE;
	
	// Always start pointing at Channel and solenoid channels initialize to off
	currentChannelNumA_m   = 0;
	currentChannelNumB_m   = 0;
	currentChannelValue_m = FALSE;
	
	// Create a solenoid object for every channel on the module (we start out in single mode)
	// (note that solenoid channels are numbered 1->8 in the API but 0->7 on the module)
	for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
	{
		channel_mp[i] = new Solenoid(i + 1);
	}
	
	// Don't allocate one yet, we start out in single mode
	doubleSolenoid_mp = NULL;
}

void SolenoidMenu::SwitchMode ()
{
	if (SINGLE == mode_m)
	{
		// Delete all of the single solenoid objects
		for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
		{
			channel_mp[i]->Set(false);
			delete channel_mp[i];
		}

		// Set channels A and B back to 0 and 1, respectively
		currentChannelNumA_m   = 0;
		currentChannelNumB_m   = 1;

		// Allocate a dual solenoid object
		doubleSolenoid_mp = new DoubleSolenoid (currentChannelNumA_m + 1, currentChannelNumB_m + 1);
	}
	else
	{
		// Delete the double solenoid object
		doubleSolenoid_mp->Set(DoubleSolenoid::kOff);
		delete doubleSolenoid_mp;
		
		// Allocate a table of solenoid objects
		for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
		{
			channel_mp[i] = new Solenoid(i + 1);
		}
	}
	mode_m = mode_m ^ DUAL;
}

SolenoidMenu::~SolenoidMenu()
{
	if (SINGLE == mode_m)
	{
		for (int i=0; i <= MAX_SOLENOID_CHANNEL; i++)
		{
			delete channel_mp[i];
		}
	}
	else
	{
		delete doubleSolenoid_mp;
	}
}

menuType SolenoidMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 1: // Change mode
			SwitchMode();
			break;
		case 2: // Decrement channel A pointer 
			currentChannelNumA_m--;
			if (currentChannelNumA_m == currentChannelNumB_m)
			{
				currentChannelNumA_m--;
			}
			if (currentChannelNumA_m < MIN_SOLENOID_CHANNEL)
			{
				currentChannelNumA_m = MAX_SOLENOID_CHANNEL;
				if (currentChannelNumA_m == currentChannelNumB_m)
				{
					currentChannelNumA_m--;
				}
			}
			if (DUAL == mode_m)
			{
				doubleSolenoid_mp->Set(DoubleSolenoid::kOff);
				delete doubleSolenoid_mp;
				doubleSolenoid_mp = new DoubleSolenoid(currentChannelNumA_m + 1, currentChannelNumB_m + 1);
			}
			break;
		case 3: // Decrement channel B pointer 
			if (DUAL == mode_m)
			{
				currentChannelNumB_m--;
				if (currentChannelNumB_m == currentChannelNumA_m)
				{
					currentChannelNumB_m--;
				}
				if (currentChannelNumB_m < MIN_SOLENOID_CHANNEL)
				{
					currentChannelNumB_m = MAX_SOLENOID_CHANNEL;
					if (currentChannelNumB_m == currentChannelNumA_m)
					{
						currentChannelNumB_m--;
					}
				}
			}
			if (DUAL == mode_m)
			{
				doubleSolenoid_mp->Set(DoubleSolenoid::kOff);
				delete doubleSolenoid_mp;
				doubleSolenoid_mp = new DoubleSolenoid(currentChannelNumA_m + 1, currentChannelNumB_m + 1);
			}
			break;
		case 4: // Set ...
			if (SINGLE == mode_m)
			{
				// Toggle channel value (it can only be true of false)
				channel_mp[currentChannelNumA_m]->Set(!channel_mp[currentChannelNumA_m]->Get());
			}
			else
			{
				switch (doubleSolenoid_mp->Get())
				{
					case DoubleSolenoid::kOff:
						doubleSolenoid_mp->Set(DoubleSolenoid::kReverse);
						break;
					case DoubleSolenoid::kForward:
						doubleSolenoid_mp->Set(DoubleSolenoid::kOff);
						break;
					case DoubleSolenoid::kReverse:
						doubleSolenoid_mp->Set(DoubleSolenoid::kForward);
						break;
					default:
						break;
				}
			}
			break;
		case 5: // Return to previous menu
			return callingMenu_m;
		default:
			break;
	}
	return SOLENOID;
}

menuType SolenoidMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 1: // Change mode
			SwitchMode();
			break;
		case 2: // Increment channel A pointer 
			currentChannelNumA_m++;
			if (currentChannelNumA_m == currentChannelNumB_m)
			{
				currentChannelNumA_m++;
			}
			if (currentChannelNumA_m > MAX_SOLENOID_CHANNEL)
			{
				currentChannelNumA_m = MIN_SOLENOID_CHANNEL;
				if (currentChannelNumA_m == currentChannelNumB_m)
				{
					currentChannelNumA_m++;
				}
			}
			if (DUAL == mode_m)
			{
				doubleSolenoid_mp->Set(DoubleSolenoid::kOff);
				delete doubleSolenoid_mp;
				doubleSolenoid_mp = new DoubleSolenoid(currentChannelNumA_m + 1, currentChannelNumB_m + 1);
			}
			break;
		case 3: // Increment channel B pointer 
			if (DUAL == mode_m)
			{
				currentChannelNumB_m++;
				if (currentChannelNumB_m == currentChannelNumA_m)
				{
					currentChannelNumB_m++;
				}
				if (currentChannelNumB_m > MAX_SOLENOID_CHANNEL)
				{
					currentChannelNumB_m = MIN_SOLENOID_CHANNEL;
					if (currentChannelNumB_m == currentChannelNumA_m)
					{
						currentChannelNumB_m++;
					}
				}
			}
			if (DUAL == mode_m)
			{
				doubleSolenoid_mp->Set(DoubleSolenoid::kOff);
				delete doubleSolenoid_mp;
				doubleSolenoid_mp = new DoubleSolenoid(currentChannelNumA_m + 1, currentChannelNumB_m + 1);
			}
			break;
		case 4: // Set ...
			if (SINGLE == mode_m)
			{
				// Toggle channel value (it can only be true of false)
				channel_mp[currentChannelNumA_m]->Set(!channel_mp[currentChannelNumA_m]->Get());
			}
			else
			{
				switch (doubleSolenoid_mp->Get())
				{
					case DoubleSolenoid::kOff:
						doubleSolenoid_mp->Set(DoubleSolenoid::kForward);
						break;
					case DoubleSolenoid::kForward:
						doubleSolenoid_mp->Set(DoubleSolenoid::kReverse);
						break;
					case DoubleSolenoid::kReverse:
						doubleSolenoid_mp->Set(DoubleSolenoid::kOff);
						break;
					default:
						break;
				}
			}
			break;
		default:
			break;
	}
	return SOLENOID;
}

void SolenoidMenu::UpdateDisplay ()
{
	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, " Solenoid: [%s]", mode_m ? "Dual" : "Single");
	dsLCD->PrintfLine(LCD2, " Channel A: %d", currentChannelNumA_m);
	if (SINGLE == mode_m)
	{
		dsLCD->PrintfLine(LCD3, "                    ");
	}
	else
	{
		dsLCD->PrintfLine(LCD3, " Channel B: %d", currentChannelNumB_m);
	}
	if (SINGLE == mode_m)
	{
		dsLCD->PrintfLine(LCD4, " Set: %s", channel_mp[currentChannelNumA_m]->Get() ? "ON" : "OFF");
	}
	else
	{	
		char * string = NULL;
		switch (doubleSolenoid_mp->Get())
		{
			case DoubleSolenoid::kOff:
				string = "Off";
				break;
			case DoubleSolenoid::kForward:
				string = "For";
				break;
			case DoubleSolenoid::kReverse:
				string = "Rev";
				break;
			default:
				string = "Error";
				break;
		}
		
		dsLCD->PrintfLine(LCD4, " Set: %s", string);
	
	}
	dsLCD->PrintfLine(LCD5, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
