#ifndef BASICDEFINES_H_
#define BASICDEFINES_H_

// Types
typedef enum
{
	kEventErr, kEventNone, kEventOpened, kEventClosed
} EventType;
	
typedef enum
{
	kStateOpen, kStateClosed, kStateErr
} StateType;

typedef struct
{
	StateType state; 
	EventType event;
	bool enabled;
} ButtonEntry;

#endif
