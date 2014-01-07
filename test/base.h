#ifndef BASE_H_
#define BASE_H_

#include "WPILib.h"

// The menus shown on the LCD are organized as follows:
//
//                          +---------------------+   
//                          +         Top         +   
//                          +---------------------+   
//                                |    |    |
//            +-------------------+    |    +--------------------+
//            |                        |                         |
// +---------------------+  +---------------------+   +---------------------+
// +       Analog        +  +       Digital       +   +       Solenoid      +
// +---------------------+  +---------------------+   +---------------------+
//                                |    |    |
//            +-------------------+    |    +--------------------+
//            |                        |                         |
// +---------------------+  +---------------------+   +---------------------+
// +     Digital PWM     +  +     Digital IO      +   +    Digital Relay    +
// +---------------------+  +---------------------+   +---------------------+
//                                |    |    |
//            +-------------------+    |    +--------------------+
//            |                        |                         |
// +---------------------+  +---------------------+   +---------------------+
// +  Digital IO State   +  +  Digital IO Clock   +   + Digital IO Encoder  +
// +---------------------+  +---------------------+   +---------------------+

// The complete list of menus in the test program
typedef enum 
{
	TOP,				// Points to analog, digitalTop, and solenoid
	ANALOG, 			// Handles analog module inputs
	DIGITAL_TOP, 		// Points to digitalPWM, digitalIO, and digitalRelay
	SOLENOID,			// Handles solenoid module outputs
	DIGITAL_PWM, 		// Handles digital module PWM outputs
	DIGITAL_IO,			// Points to digitalIOState, digitalIOClock, and digitalIOEncoder
	DIGITAL_RELAY,		// Handles digital module relay outputs
	DIGITAL_IO_STATE,	// Handles digital IOs as ON or OFF
	DIGITAL_IO_CLOCK,	// Handles digital IOs and clocks
	DIGITAL_IO_ENCODER,	// Handles a pair of digital IOs as an encoder input
	NUM_MENU_TYPE
} menuType;

// For convenience when calling DriverStaionLCF::PrintfLine (and etc.)
#define LCD1 DriverStationLCD::kUser_Line1
#define LCD2 DriverStationLCD::kUser_Line2
#define LCD3 DriverStationLCD::kUser_Line3
#define LCD4 DriverStationLCD::kUser_Line4
#define LCD5 DriverStationLCD::kUser_Line5
#define LCD6 DriverStationLCD::kUser_Line6

// ----------------------------------------------------------------------------
// Base class for all of the other menus in the system
// ----------------------------------------------------------------------------

// The menu base class contains all of the basic operations of a menu
// with up to six lines of 20 characters (the LCD display on the driver
// station. Each menu must handle four buttons for navigation and 
// selection:
//   Index Up - move the menu selection asterisk up one line of the menu
//   Index Down - move the menu selection asterisk down one line of the menu
//   Select Right - select the next (incrementally higher) option or
//                  value for a given menu line
//   Select Left - delect the previous (incrementally lower) option or
//                 value for a given menu line.
// The methods provided by this base class are meant to be overriden by
// menu-specific versions in all derived classes. We are creating the base 
// class to allow us to use a base-class pointer to refer to any derived
// class instance.

class BaseMenu
{

public:
	BaseMenu();
	virtual ~BaseMenu ();
	void HandleIndexUp ();
	void HandleIndexDown ();
	virtual  menuType HandleSelectLeft ();
	virtual  menuType HandleSelectRight ();
	virtual void UpdateDisplay ();
	virtual void SetSpeed (float speed);
	virtual void SetTableEntry (int index, Jaguar * pointer);

	void SetCallingMenu (menuType callingMenu);
	DriverStationLCD::Line IndexToLCDLine (int line);
	
	int index_m;
	int maxIndex_m;
	int minIndex_m;
	menuType callingMenu_m;
	DriverStationLCD *dsLCD;
};

// ----------------------------------------------------------------------------
// TOP menu
// ----------------------------------------------------------------------------

class TopMenu : public BaseMenu
{
	
public:
	TopMenu ();
	
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void     UpdateDisplay ();
private:
	menuType HandleSelect ();
};

// ----------------------------------------------------------------------------
// SOLENOID menu
// ----------------------------------------------------------------------------

#define MIN_SOLENOID_CHANNEL 0
#define MAX_SOLENOID_CHANNEL 7

class SolenoidMenu : public BaseMenu
{
	
public:
	         SolenoidMenu ();
	virtual ~SolenoidMenu();
	
	void     doIndexUp ();
	void     doIndexDown ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void     UpdateDisplay ();
	
	int        currentChannelNum_m;
	bool       currentChannelValue_m;
	Solenoid * channel_mp[MAX_SOLENOID_CHANNEL + 1];
};

// ----------------------------------------------------------------------------
// ANALOG menu
// ----------------------------------------------------------------------------

#define MIN_ANALOG_CHANNEL 0
// The last channel is always used for batt voltage so we do not have 
// access to it (hence our max channel is 6 not 7)
#define MAX_ANALOG_CHANNEL 6 

class AnalogMenu : public BaseMenu
{
	
public:
	         AnalogMenu ();
	virtual ~AnalogMenu();
	void     doIndexUp ();
	void     doIndexDown ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void     UpdateDisplay ();
	
	int             currentChannelNum_m;
	// float           currentChannelValue_m;
	AnalogChannel * channel_mp[MAX_ANALOG_CHANNEL + 1];
};

// ----------------------------------------------------------------------------
// A class to handle the digital IO ports for the DIGITAL_* menus
// ----------------------------------------------------------------------------

#define NUM_DIO_CHANNELS 14
#define MIN_DIO_CHANNEL  0

// To get around the fact that we do not have a common base class through which
// we can have a single pointer to point to either an input or an output we have
// one for each direction as follows:
// 	Direction DigitalInput DigitalOutput
//	  Input     NonNULL        Null
//	  Output     NULL         NonNULL

typedef struct {
	DigitalInput  * DigitalInput_p;
	DigitalOutput * DigitalOutput_p;
} DigitalIOEntry;

class DigitalIO
{
public:
	DigitalIO ();
	~DigitalIO ();
	
	bool IsInput (int channel);
	void SetToInput (int channel, bool input);
	void SetDirection (int channel, bool input);
	DigitalSource * GetPointer (int channel);
	bool GetValue (int channel);
	void SetValue (int channel, bool value);
	
	static DigitalIO * GetInstance ();
	
private:
	static DigitalIO * instance_m;
	
	DigitalIOEntry DIOTable_mp[NUM_DIO_CHANNELS];
};

// ----------------------------------------------------------------------------
// DIGITAL_TOP menu
// ----------------------------------------------------------------------------

class DigitalMenu : public BaseMenu
{
	
public:
	DigitalMenu ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void UpdateDisplay ();
};

// ----------------------------------------------------------------------------
// DIGITAL_PWM menu
// ----------------------------------------------------------------------------

#define MIN_PWM_CHANNEL 0
#define MAX_PWM_CHANNEL 9

class PWMMenu : public BaseMenu
{
typedef enum {
	NEITHER,
	A_ONLY,
	B_ONLY,
	BOTH
} enabledType;


public:
	PWMMenu ();
	virtual ~PWMMenu();
	void doIndexUp ();
	void doIndexDown ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void UpdateDisplay ();
	void SetSpeed (float speed);
	void SetTableEntry (int index, Jaguar * pointer);

	
	int currentChannelNumA_m;
	int currentChannelNumB_m;
	int enabled_m;
	
	Jaguar * channel_mp[MAX_PWM_CHANNEL + 1];

};

// ----------------------------------------------------------------------------
// DIGITAL_IO menu
// ----------------------------------------------------------------------------

class DigitalIOMenu : public BaseMenu
{
	
public:
	DigitalIOMenu ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void UpdateDisplay ();
};

// ----------------------------------------------------------------------------
// RELAY menu
// ----------------------------------------------------------------------------

#define MIN_RELAY_CHANNEL 0
#define MAX_RELAY_CHANNEL 7

class RelayMenu : public BaseMenu
{
	
public:
	         RelayMenu ();
	virtual ~RelayMenu();
	
	void     doIndexUp ();
	void     doIndexDown ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void     UpdateDisplay ();
	
	void IncrementChannelValue ();
	void DecrementChannelValue ();
	int  RelayValueToInt (Relay::Value value);

	
	int          currentChannelNum_m;
	Relay::Value currentChannelValue_m;
	Relay *      channel_mp[MAX_RELAY_CHANNEL + 1];
};

// ----------------------------------------------------------------------------
// Digital IO State menu
// ----------------------------------------------------------------------------

class DigitalIOStateMenu : public BaseMenu
{	
public:
	         DigitalIOStateMenu ();
	virtual ~DigitalIOStateMenu();
	
	void     doIndexUp ();
	void     doIndexDown ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void     UpdateDisplay ();
	
	int  currentChannelNum_m;
	bool currentChannelValue_m;
	
	DigitalIO * digitalIO_mp;
};

// ----------------------------------------------------------------------------
// DIGITAL_IO_ENCODER menu
// ----------------------------------------------------------------------------

class DigitalIOEncoderMenu : public BaseMenu
{
public:
	DigitalIOEncoderMenu ();
	virtual ~DigitalIOEncoderMenu();
	
	void     doIndexUp ();
	void     doIndexDown ();
	menuType HandleSelectLeft ();
	menuType HandleSelectRight ();
	void     UpdateDisplay ();
	
	void CreateAndStartEncoder();
	void StopAndDestroyEncoder();

	int currentChannelNumA_m;
	int currentChannelNumB_m;
	
	Encoder *   encoder_mp;
	DigitalIO * digitalIO_mp;
};

#endif

