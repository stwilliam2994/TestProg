#include "base.h"

// Digitial IO encoder Menu
	
// Notes:
// 1. This menu allows you to change the direction of a digital IO port created
//    elsewhere in the program (telop or autonomous modes) so be careful
// 2. Any change to the ports being used for the encoder (or leaving this menu)
//    causes the encoder object to be disabled and deleted to isolate this menu
//    from the other digital IO menus

DigitalIOEncoderMenu::DigitalIOEncoderMenu()
{
	// The Digitial IO encoder menu looks like this (not including the 1st 2 columns):
	// 1:Digitial IO encoder
	// 2: Channel A: #
	// 3: Channel B: #
	// 4: Enabled:
	// 5: Back
	// 6:Count: ##
	
	index_m    = 2;
	maxIndex_m = 5;
	
	// Start out with channel 0 for each
	currentChannelNumA_m   = 0;
	currentChannelNumB_m   = 0;
	
	encoder_mp   = NULL;
	digitalIO_mp = DigitalIO::GetInstance();
}

DigitalIOEncoderMenu::~DigitalIOEncoderMenu()
{
	StopAndDestroyEncoder();
}

void DigitalIOEncoderMenu::CreateAndStartEncoder()
{
	if (!encoder_mp)
	{
		digitalIO_mp->SetToInput(currentChannelNumA_m, true);
		digitalIO_mp->SetToInput(currentChannelNumB_m, true);
	
		DigitalSource * channelA = digitalIO_mp->GetPointer(currentChannelNumA_m);
		DigitalSource * channelB = digitalIO_mp->GetPointer(currentChannelNumB_m);
		
		encoder_mp = new Encoder (channelA, channelB);
		encoder_mp->Start();
	}
}

void DigitalIOEncoderMenu::StopAndDestroyEncoder()
{
	if (encoder_mp)
	{
		encoder_mp->Stop();
		delete encoder_mp;
		encoder_mp = NULL;
	}
}

menuType DigitalIOEncoderMenu::HandleSelectLeft ()
{
	switch (index_m)
	{
		case 2: // Decrement channel A port number 
			StopAndDestroyEncoder();
			currentChannelNumA_m--;
			if (currentChannelNumA_m < MIN_DIO_CHANNEL)
			{
				currentChannelNumA_m = NUM_DIO_CHANNELS - 1;
			}
			break;
		case 3: // Decrement channel B number
			StopAndDestroyEncoder();
			currentChannelNumB_m--;
			if (currentChannelNumB_m < MIN_DIO_CHANNEL)
			{
				currentChannelNumB_m = NUM_DIO_CHANNELS - 1;
			}
			break;
		case 4: //  Enable/disable
			if (!encoder_mp)
			{
				CreateAndStartEncoder();
			}
			else
			{
				StopAndDestroyEncoder();
			}
			break;
		case 5: // Return to previous menu
			StopAndDestroyEncoder();
			return callingMenu_m;
			break;
		default:
			return DIGITAL_IO_ENCODER;
	};
		
	return DIGITAL_IO_ENCODER;
}

menuType DigitalIOEncoderMenu::HandleSelectRight ()
{
	switch (index_m)
	{
		case 2: // Increment channel A number 
			StopAndDestroyEncoder();
			currentChannelNumA_m++;
			if (currentChannelNumA_m >= NUM_DIO_CHANNELS)
			{
				currentChannelNumA_m = MIN_DIO_CHANNEL;
			}
			break;
		case 3: // Decrement channel value
			StopAndDestroyEncoder();
			currentChannelNumB_m++;
			if (currentChannelNumB_m >= NUM_DIO_CHANNELS)
			{
				currentChannelNumB_m = MIN_DIO_CHANNEL;
			}
			break;
		case 4: // Toggle the enabled flag
			if (!encoder_mp)
			{
				CreateAndStartEncoder();
			}
			else
			{
				StopAndDestroyEncoder();
			}
			break;
		case 5: // Only select left is allowed to return to calling menu
			break;
		default:
			return DIGITAL_IO_ENCODER;
	};
	
	return DIGITAL_IO_ENCODER;
}

void DigitalIOEncoderMenu::UpdateDisplay ()
{
	int chanA = currentChannelNumA_m + 1;
	int chanB = currentChannelNumB_m + 1;

	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1, "Digitial IO encoder");
	dsLCD->PrintfLine(LCD2, " Channel A: %d", chanA);
	dsLCD->PrintfLine(LCD3, " Channel B: %d", chanB);
	if (encoder_mp)
	{
		dsLCD->PrintfLine(LCD4, " En: Yes Cnt: %d", encoder_mp->Get());
	}
	else
	{
		dsLCD->PrintfLine(LCD4, " En: No Cnt: --");
	}
	dsLCD->PrintfLine(LCD5, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
