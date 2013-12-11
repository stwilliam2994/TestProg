#include "base.h"

DigitalIO::DigitalIO ()
{
	for (UINT32 i = 0; i < NUM_DIO_CHANNELS; i++)
	{
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
};

bool DigitalIO::IsInput (int channel)
{
	return (NULL == DIOTable_mp[channel].DigitalInput_p);
}

void DigitalIO::SetDirection (int channel, bool input)
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
