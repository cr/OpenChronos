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

// Cycle alarm states
#define CYCLE_ALARM_UNSET	(0u)
#define CYCLE_ALARM_ACTIVE	(1u)
#define CYCLE_ALARM_RINGING	(2u)
#define CYCLE_ALARM_CONFIG	(3u)


// Keep alarm for 64 on-off cycles
#define CYCLE_ALARM_ON_DURATION	(64u)


// *************************************************************************************************
// Global Variable section
struct cycle_alarm
{
	u8 state;
	// Sleep-in delay
	s8 delay;
	// Cycle length
	u8 cyclelen;
	// Number of cycles before alarm
	u8 cycles;
	// Alarm hour
	u8 hour;
	// Alarm minute
	u8 minute;
	// Alarm duration
	u8 duration;
};
extern struct cycle_alarm sCycleAlarm;


// *************************************************************************************************
// Extern section
