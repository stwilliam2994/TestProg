#include "base.h"

DigitalIO * DigitalIO::instance_m = NULL;


// This is a class that should only ever be instantiated once. Its purpose
// is to place control of the digital IO ports in a single place as digital
// ports are required by numerous menus

DigitalIO::DigitalIO ()
{
	for (UINT32 i = 0; i < NUM_DIO_CHANNELS; i++)
	{
		// Need to assign from existing pointers here if and digital IO ports 
		// are already allocated in the the main RobotDemo constructor!

		DIOTable_mp[i].DigitalInput_p  = new DigitalInput (i + 1);
		DIOTable_mp[i].DigitalOutput_p = NULL;
	}
};

DigitalIO::~DigitalIO ()
{
	// Paranoia. Even though only one OR the other should ever be
	// nonNULL, we check both.
	
	for (UINT32 i = 0; i < NUM_DIO_CHANNELS; i++)
	{
		if (NULL != DIOTable_mp[i].DigitalInput_p)
		{
			delete DIOTable_mp[i].DigitalInput_p;
		}
		
		if (NULL != DIOTable_mp[i].DigitalOutput_p)
		{
			delete DIOTable_mp[i].DigitalOutput_p;
		}	
	}
	
	// Don't forget to delete instance_m
};

bool DigitalIO::IsInput (int channel)
{
	return (NULL == DIOTable_mp[channel].DigitalOutput_p);
}

void DigitalIO::SetToInput (int channel, bool input)
{
	if (input)
	{
		if (!IsInput(channel))
		{
			delete DIOTable_mp[channel].DigitalOutput_p;
			DIOTable_mp[channel].DigitalOutput_p = NULL;
			DIOTable_mp[channel].DigitalInput_p  = new DigitalInput (channel + 1);
		}
	}
	else
	{
		if (IsInput(channel))
		{
			delete DIOTable_mp[channel].DigitalInput_p;
			DIOTable_mp[channel].DigitalInput_p   = NULL;
			DIOTable_mp[channel].DigitalOutput_p  = new DigitalOutput (channel + 1);
		}
	}
}
bool DigitalIO::GetValue (int channel)
{
	if (IsInput(channel))
	{
		return DIOTable_mp[channel].DigitalInput_p->Get();
	}
	else
	{
		return false; 
	}
}

void DigitalIO::SetValue (int channel, bool value)
{
	if (IsInput(channel))
	{
		return;
	}
	else
	{
		return DIOTable_mp[channel].DigitalOutput_p->Set(value); 
	}
}

DigitalSource * DigitalIO::GetPointer (int channel)
{
	if (IsInput(channel))
	{
		return DIOTable_mp[channel].DigitalInput_p; ;
	}
	else
	{
		return DIOTable_mp[channel].DigitalOutput_p; 
	}
}

DigitalIO * DigitalIO::GetInstance ()
{
	if (instance_m == NULL)
	{
		instance_m = new DigitalIO();
	}
    return instance_m;
}
