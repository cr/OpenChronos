// *************************************************************************************************
// Include section


// *************************************************************************************************
// Prototypes section

// internal functions
extern void reset_cycle_alarm(void);
extern void check_cycle_alarm(void);
extern void stop_cycle_alarm(void);

// menu functions
extern void sx_cycle_alarm(u8 line);
extern void mx_cycle_alarm(u8 line);
extern void nx_cycle_alarm(u8 line);
extern void display_cycle_alarm(u8 line, u8 update);


// *************************************************************************************************
// Defines section

// Alarm states
#define CYCLE_ALARM_DISABLED 	(0u)
#define CYCLE_ALARM_ENABLED	(1u)
#define CYCLE_ALARM_ON		(2u)

// Keep alarm for 10 on-off cycles
#define CYCLE_ALARM_ON_DURATION	(10u)


// *************************************************************************************************
// Global Variable section
struct cycle_alarm
{
	u8 hourly; //dummy
	// CYCLE_ALARM_DISABLED, CYCLE_ALARM_ENABLED, CYCLE_ALARM_ON
	u8 state;
	// Alarm duration
	u8 duration;
	// Alarm hour
	u8 hour;
	// Alarm minute
	u8 minute;
	// Sleep-in delay
	u8 delay;
	// Cycle lenght
	u8 cyclelen;
	// Number of cycles before wake
	u8 cycles;
};
extern struct cycle_alarm sCycleAlarm;


// *************************************************************************************************
// Extern section
