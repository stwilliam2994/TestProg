#ifndef BASE_H_
#define BASE_H_

#include "WPILib.h"


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

#define LCD1 DriverStationLCD::kUser_Line1
#define LCD2 DriverStationLCD::kUser_Line2
#define LCD3 DriverStationLCD::kUser_Line3
#define LCD4 DriverStationLCD::kUser_Line4
#define LCD5 DriverStationLCD::kUser_Line5
#define LCD6 DriverStationLCD::kUser_Line6

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
	void SetCallingMenu (menuType callingMenu);
	DriverStationLCD::Line IndexToLCDLine (int line);
	
	int index_m;
	int maxIndex_m;
	int minIndex_m;
	menuType callingMenu_m;
	DriverStationLCD *dsLCD;

};

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
	Solenoid * channel_mp[8];
};

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
	float           currentChannelValue_m;
	AnalogChannel * channel_mp[8];
};

#define NUM_DIO_CHANNELS 14

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
	void SetDirection (int channel, bool input);
	bool GetValue (int channel);
	void SetValue (int channel, bool value);
	
	DigitalIOEntry DIOTable_mp[NUM_DIO_CHANNELS];
};
#endif

