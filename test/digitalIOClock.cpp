#include "base.h"

// Digitial IO Clock Menu
	
// Notes:
// 1. This menu allows you to change the direction of a digital IO port created
//    elsewhere in the program so be careful
// 2. Any change to the ports being used for the clock (or leaving this menu)
//    causes the clock object to be disabled and deleted to isolate this menu
//    from the other digital IO menus

DigitalIOClockMenu::DigitalIOClockMenu()
{
	// The Digitial IO clock menu looks like this (not including the 1st 2 columns):
	// 1:Digitial IO Clock
	// 2: Channel: #
	// 3: Enable: $$    
	// 4: Count: ####
	// 5: Back
	// 6:Count: ##
	
	index_m    = 2;
	maxIndex_m = 5;
	
	// Start out with channel 0
	currentChannelNum_m   = 0;
	
	counter_mp   = NULL;
	digitalIO_mp = DigitalIO::GetInstance();
}

DigitalIOClockMenu::~DigitalIOClockMenu()
{
	StopAndDestroyCounter();
}

void DigitalIOClockMenu::CreateAndStartCounter()
{
	if (!counter_mp)
	{
		digitalIO_mp->SetToInput(currentChannelNum_m, true);
	
		DigitalSource * channelA = digitalIO_mp->GetInputPointer(currentChannelNum_m);
		
		// Really should check for NULL channel and SWERR (as well as not
		// enable the counter)
		counter_mp = new Counter (channelA);
		counter_mp->Start();
	}
}

void DigitalIOClockMenu::StopAndDestroyCounter()
{
	if (counter_mp)
	{
		counter_mp->Stop();
		delete counter_mp;
		counter_mp = NULL;
	}
}

menuType DigitalIOClockMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 2: // Decrement channel port number 
			StopAndDestroyCounter();
			currentChannelNum_m--;
			if (currentChannelNum_m < MIN_DIO_CHANNEL)
			{
				currentChannelNum_m = NUM_DIO_CHANNELS - 1;
			}
			break;
		case 3: //  Toggle the enabled state
			if (!counter_mp)
			{
				CreateAndStartCounter();
			}
			else
			{
				StopAndDestroyCounter();
			}
			break;
		case 5: // Return to previous menu
			StopAndDestroyCounter();
			return callingMenu_m;
			break;
		default:
			break;
	}
	return DIGITAL_IO_CLOCK;
}

menuType DigitalIOClockMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 2: // Increment channel number 
			StopAndDestroyCounter();
			currentChannelNum_m++;
			if (currentChannelNum_m >= NUM_DIO_CHANNELS)
			{
				currentChannelNum_m = MIN_DIO_CHANNEL;
			}
			break;
		case 3: // Toggle the enabled state
			if (!counter_mp)
			{
				CreateAndStartCounter();
			}
			else
			{
				StopAndDestroyCounter();
			}
			break;
		default:
			break;
	}
	return DIGITAL_IO_CLOCK;
}

void DigitalIOClockMenu::UpdateDisplay ()
{
	int chan = currentChannelNum_m + 1;

	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, "Digitial IO Clock");
	dsLCD->PrintfLine(LCD2, " Channel %d", chan);
	dsLCD->PrintfLine(LCD3, " Enable: %s", counter_mp ? "Yes" : "No");
	if (counter_mp)
	{
		dsLCD->PrintfLine(LCD4, " Count: %d", counter_mp->Get());
	}
	else
	{
		dsLCD->PrintfLine(LCD4, " Count: --");
	}
	dsLCD->PrintfLine(LCD5, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}

