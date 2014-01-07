#include "base.h"

// PWM Menu
	
	
PWMMenu::PWMMenu()
{
	// The PWM menu looks like this (not including the 1st 2 columns):
	// 1:PWM
	// 2: Channel A: #
	// 3: Channel B: #
	// 4: Enabled:
	// 5: Back
	// 6:
	
	index_m    = 2;
	maxIndex_m = 5;
	
	// Start out with channel 0 for each
	currentChannelNumA_m   = 0;
	currentChannelNumB_m   = 1;
	enabled_m = NEITHER;
	
	
	// Create a PWM object for every channel on the module
	// (note that PWM channels are numbered 1->10 in the API and 1->10 on the module)
	// We are using 0->9
	
	for (int i=0; i <= MAX_PWM_CHANNEL; i++)
	{
		// Need to assign from existing pointers here if and PWM ports are already
		// allocated in the the main RobotDemo constructor! See SetTableEntry
		channel_mp[i] = new Jaguar(i + 1);
		channel_mp[i]->SetExpiration(0.2); 
	}
}

PWMMenu::~PWMMenu()
{
	for (int i=0; i <= MAX_PWM_CHANNEL; i++)
	{
		delete channel_mp[i];
	}
}

void PWMMenu::SetTableEntry (int index, Jaguar * pointer)
{
	channel_mp[index] = pointer;
	channel_mp[index]->SetExpiration(0.2);
};

menuType PWMMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 2: // Decrement channel A number 
			currentChannelNumA_m--;
			if (currentChannelNumA_m == currentChannelNumB_m)
			{
				currentChannelNumA_m--;
			}
			if (currentChannelNumA_m < MIN_PWM_CHANNEL)
			{
				currentChannelNumA_m = MAX_PWM_CHANNEL;
				if (currentChannelNumA_m == currentChannelNumB_m)
				{
					currentChannelNumA_m--;
				}
			}
			break;
		case 3: // Decrement channel B number
			currentChannelNumB_m--;
			if (currentChannelNumB_m == currentChannelNumA_m)
			{
				currentChannelNumB_m--;
			}
			if (currentChannelNumB_m < MIN_PWM_CHANNEL)
			{
				currentChannelNumB_m = MAX_PWM_CHANNEL;
				if (currentChannelNumB_m == currentChannelNumA_m)
				{
					currentChannelNumB_m--;
				}
			}
			break;
		case 4: // Decrement enabled flag
			enabled_m--;
			if (enabled_m < NEITHER)
			{
				enabled_m = BOTH;
			}
			break;
		case 5: // Return to previous menu
			return callingMenu_m;
			break;
		default:
			return DIGITAL_PWM;
	};
		
	return DIGITAL_PWM;
}

menuType PWMMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 2: // Increment channel A number 
			currentChannelNumA_m++;
			if (currentChannelNumA_m == currentChannelNumB_m)
			{
				currentChannelNumA_m++;
			}
			if (currentChannelNumA_m > MAX_PWM_CHANNEL)
			{
				currentChannelNumA_m = MIN_PWM_CHANNEL;
				if (currentChannelNumA_m == currentChannelNumB_m)
				{
					currentChannelNumA_m++;
				}
			}
			break;
		case 3: // Decrement channel value
			currentChannelNumB_m++;
			if (currentChannelNumB_m == currentChannelNumA_m)
			{
				currentChannelNumB_m++;
			}
			if (currentChannelNumB_m > MAX_PWM_CHANNEL)
			{
				currentChannelNumB_m = MIN_PWM_CHANNEL;
				if (currentChannelNumB_m == currentChannelNumA_m)
				{
					currentChannelNumB_m++;
				}
			}
			break;
		case 4: // Increment the enabled flag
			enabled_m++;
			if (enabled_m > BOTH)
			{
				enabled_m = NEITHER;
			}
		case 5: // Only select left is allowed to return to calling menu
			break;
		default:
			return DIGITAL_PWM;
	};
	
	return DIGITAL_PWM;
}

void PWMMenu::SetSpeed (float speed)
{
	// To keep motor safety tmeouts from occuring we need to set all of the motor
	// controllers each pass of the main program loop (and this method is called 
	// in each pass of the main program loop)
	
	bool alreadySet = false;
	for (int i = MIN_PWM_CHANNEL; i <= MAX_PWM_CHANNEL; i++)
	{
		alreadySet = false;
		if ((i == currentChannelNumA_m) && ((A_ONLY == enabled_m) || (BOTH == enabled_m)))
		{
			channel_mp[i]->Set(speed);
			alreadySet = true;
		}
		if ((i == currentChannelNumB_m) && ((B_ONLY == enabled_m) || (BOTH == enabled_m)))
		{
			channel_mp[i]->Set(speed);
			alreadySet = true;
		}

		if (!alreadySet)
		{
			channel_mp[i]->Set(0.0);
		}
	}
}

void PWMMenu::UpdateDisplay ()
{
	const char * enabledStrings[] = 
		{
			"Neither",
			"A Only",
			"B Only",
			"Both"
		};
	int chanA = currentChannelNumA_m + 1;
	int chanB = currentChannelNumB_m + 1;

	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, "PWM");
	dsLCD->PrintfLine(LCD2, " Channel A: %d %5.2f", chanA, channel_mp[currentChannelNumA_m]->Get());
	dsLCD->PrintfLine(LCD3, " Channel B: %d %5.2f", chanB, channel_mp[currentChannelNumB_m]->Get());
	dsLCD->PrintfLine(LCD4, " Enabled: %s", enabledStrings[enabled_m]);
	dsLCD->PrintfLine(LCD5, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
