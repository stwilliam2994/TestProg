#include "base.h"

DigitalIO * DigitalIO::instance_m = NULL;


// This is a class that should only ever be instantiated once. Its purpose
// is to place control of the digital IO ports in a single place as digital
// ports are required by numerous menus

DigitalIO::DigitalIO ()
{
	for (UINT32 i = 0; i < NUM_DIO_CHANNELS; i++)
	{
		// Need to assign from existing pointers here if any digital IO ports are already
		// allocated in the the main RobotDemo constructor! Use SetDigital...TableEntry to
		// overide any of the entries in this table as required.
		DIOTable_mp[i].DigitalInput_p  = new DigitalInput (i + 1);
		DIOTable_mp[i].DigitalOutput_p = NULL;
	}
};

DigitalIO::~DigitalIO ()
{
	// This will delete any objects added to the table by SetDigital...TableEntry without
	// the original creator knowing about it. However, this destructor won't ever
	// be called in the current program. However, just in case this code gets used
	// elsewhere...

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
};

bool DigitalIO::IsInput (int channel)
{
	return (NULL != DIOTable_mp[channel].DigitalInput_p);
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
	if (!IsInput(channel))
	{
		 DIOTable_mp[channel].DigitalOutput_p->Set(value); 
	}
}

DigitalSource * DigitalIO::GetInputPointer (int channel)
{
	return DIOTable_mp[channel].DigitalInput_p;
}

// This code assumes it will be called after the constructor (which sets all
// digital IOs to inputs) but before any menus are actually accessed (which
// might change the inputs to outputs). If you call this after the menus
// are used, all bets are off.
void DigitalIO::SetDigitalInputTableEntry (int index, DigitalInput * pointer)
{
	// Don't leak the object already pointed to by the table entry...
	delete DIOTable_mp[index - 1].DigitalInput_p;
	
	DIOTable_mp[index - 1].DigitalInput_p  = pointer;
}

// This code assumes it will be called after the constructor (which sets all
// digital IOs to inputs) but before any menus are actually accessed (which
// might change the inputs to outputs). If you call this after the menus
// are used, all bets are off.
void DigitalIO::SetDigitalOutputTableEntry (int index, DigitalOutput * pointer)
{
	// Don't leak the object already pointed to by the table entry...
	delete DIOTable_mp[index - 1].DigitalInput_p;
	DIOTable_mp[index - 1].DigitalInput_p  = NULL;
	
	DIOTable_mp[index - 1].DigitalOutput_p = pointer;
}

DigitalIO * DigitalIO::GetInstance ()
{
	if (instance_m == NULL)
	{
		instance_m = new DigitalIO();
	}
    return instance_m;
}
