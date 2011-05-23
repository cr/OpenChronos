// *************************************************************************************************
//
//	Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/ 
//	 
//	 
//	  Redistribution and use in source and binary forms, with or without 
//	  modification, are permitted provided that the following conditions 
//	  are met:
//	
//	    Redistributions of source code must retain the above copyright 
//	    notice, this list of conditions and the following disclaimer.
//	 
//	    Redistributions in binary form must reproduce the above copyright
//	    notice, this list of conditions and the following disclaimer in the 
//	    documentation and/or other materials provided with the   
//	    distribution.
//	 
//	    Neither the name of Texas Instruments Incorporated nor the names of
//	    its contributors may be used to endorse or promote products derived
//	    from this software without specific prior written permission.
//	
//	  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//	  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//	  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//	  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//	  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//	  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//	  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//	  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//	  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//	  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//	  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************
// Alarm routines.
// *************************************************************************************************


// *************************************************************************************************
// Include section

// system
#include "project.h"
#ifdef CONFIG_CYCLE_ALARM

// driver
#include "display.h"
#include "buzzer.h"
#include "ports.h"

// logic
#include "cycle_alarm.h"
#include "clock.h"
#include "user.h"


// *************************************************************************************************
// Prototypes section


// *************************************************************************************************
// Defines section


// *************************************************************************************************
// Global Variable section
struct cycle_alarm sCycleAlarm;


// *************************************************************************************************
// Extern section
extern void menu_skip_next(line_t line); //ezchronos.c


// *************************************************************************************************
// @fn          reset_cycle_alarm
// @brief       Resets cycle alarm data to 06:00 (4*90+0)
// @param       none
// @return      none
// *************************************************************************************************
void reset_cycle_alarm(void) 
{
	// Default sleep time 06:00 (4*90+0)
	sCycleAlarm.delay = 00;		// minutes until sleeping in
	sCycleAlarm.cyclelen = 90;	// sleep cycle length
	sCycleAlarm.cycles = 4;		// number of cycles before alarm
	sCycleAlarm.hour = 11;		// dummy alarm time hour
	sCycleAlarm.minute = 11;	// dummy alarm time minute

	// Alarm is initially off
	sCycleAlarm.duration 	= CYCLE_ALARM_ON_DURATION;
	sCycleAlarm.state 	= CYCLE_ALARM_DISABLED;
}


// *************************************************************************************************
// @fn          check_cycle_alarm
// @brief       Check if current time matches alarm time
// @param       none
// @return      none
// *************************************************************************************************
void check_cycle_alarm(void) 
{
	// Return if alarm is not enabled
	if (sCycleAlarm.state != CYCLE_ALARM_ENABLED) return;
	
	// Compare current time and alarm time
	// Start with minutes - only 1/60 probability to match
	
	if (sTime.minute == sCycleAlarm.minute)
	{
		if (sTime.hour == sCycleAlarm.hour)
		{
			// Indicate that alarm is beeping
			sCycleAlarm.state = CYCLE_ALARM_ON;
		}
	}
}


// *************************************************************************************************
// @fn          stop_cycle_alarm
// @brief       Stop active alarm
// @param       none
// @return      none
// *************************************************************************************************
void stop_cycle_alarm(void) 
{
	// Indicate that alarm is disabled and inactive
	sCycleAlarm.state = CYCLE_ALARM_DISABLED;
	
	// Stop buzzer
	stop_buzzer();
}


// *************************************************************************************************
// @fn          sx_cycle_alarm
// @brief       Sx button turns alarm on/off.
// @param       u8 line		LINE1
// @return      none
// *************************************************************************************************
void sx_cycle_alarm(u8 line)
{
	u8 state;
	s32 cycles;
	s32 duration;	
	cycles = 1;
	// UP: Cycle through cycles
	if(button.flag.up)
	{
		// Toggle alarm state
		if (sCycleAlarm.state == CYCLE_ALARM_DISABLED) {
			if (sCycleAlarm.hourly == CYCLE_ALARM_DISABLED) {
				sCycleAlarm.hourly = CYCLE_ALARM_ENABLED;
				// Show "offh" message 
				message.flag.prepare = 1;
				message.flag.type_alarm_off_chime_on = 1;
			} else if (sCycleAlarm.hourly == CYCLE_ALARM_ENABLED) {
				sCycleAlarm.state = CYCLE_ALARM_ENABLED;
				sCycleAlarm.hourly = CYCLE_ALARM_DISABLED;
				// Show " on" message 
				message.flag.prepare = 1;
				message.flag.type_alarm_on_chime_off = 1;
			}
		} else if (sCycleAlarm.state == CYCLE_ALARM_ENABLED) {
			if (sCycleAlarm.hourly == CYCLE_ALARM_DISABLED) {
				sCycleAlarm.hourly = CYCLE_ALARM_ENABLED;
				// Show " onh" message 
				message.flag.prepare = 1;
				message.flag.type_alarm_on_chime_on = 1;
			} else if (sCycleAlarm.hourly == CYCLE_ALARM_ENABLED) {
				sCycleAlarm.state = CYCLE_ALARM_DISABLED;
				sCycleAlarm.hourly = CYCLE_ALARM_DISABLED;
				// Show " off" message 
				message.flag.prepare = 1;
				message.flag.type_alarm_off_chime_off = 1;
			}
		}
	}
}


// *************************************************************************************************
// @fn          mx_cycle_alarm
// @brief       Set cycle alarm parameters
// @param       u8 line		LINE1
// @return      none
// *************************************************************************************************
void mx_cycle_alarm(u8 line)
{
	u8 select;
	s32 delay;
	s32 cyclelen;
	s32 hour;
	s32 minutes;
	u8 * str;
	
	// Clear display
	clear_display_all();

	// Keep global values in case new values are discarded
	cyclelen	= sCycleAlarm.cyclelen;
	delay		= sCycleAlarm.delay;
		
	// Init value index
	select = 0;
		
	// Loop values until all are set or user breaks	set
	while(1) 
	{
	  // Idle timeout: exit without saving
	  if (sys.flag.idle_timeout) break;

	  // STAR (short): save, then exit
	  if (button.flag.star)
	  {
	    // Store local variables in global alarm time
	    sCycleAlarm.delay = delay;
	    sCycleAlarm.cyclelen = cyclelen;
	    // Set display update flag
	    display.flag.line1_full_update = 1;
	    break;
	  }

	  switch (select)
	  {
	  case 0:		// Set delay
	    display_chars(LCD_SEG_L2_4_0, (u8 *)"C-LEN", SEG_ON);
	    set_value(&cyclelen, 4, 3, 1, 180, SETVALUE_DISPLAY_VALUE + SETVALUE_NEXT_VALUE + SETVALUE_FAST_MODE, LCD_SEG_L1_3_0, display_value1);
	    select = 1;
	    break;

	  case 1:		// Set cycle length
	    display_chars(LCD_SEG_L2_4_0, (u8 *)"DELAY", SEG_ON);
	    set_value(&delay, 4, 3, 0, 60, SETVALUE_DISPLAY_VALUE + SETVALUE_NEXT_VALUE + SETVALUE_FAST_MODE, LCD_SEG_L1_3_0, display_value1);
	    select = 0;
	    break;
	  }
	}

	// Clear button flag
	button.all_flags = 0;

	// Indicate to display function that new value is available
	display.flag.update_cycle_alarm = 1;
}
// *************************************************************************************************
// @fn          nx_cycle_alarm
// @brief       Disable alarm before advancing to next menu item
// @param       u8 line	LINE1, LINE2
// @return      none
// *************************************************************************************************
void nx_cycle_alarm(u8 line)
{
	sCycleAlarm.state == CYCLE_ALARM_DISABLED;
	menu_skip_next(line);
}

// *************************************************************************************************
// @fn          display_cycle_alarm
// @brief       Display alarm time. 24H / 12H time format.
// @param       u8 line	LINE1, LINE2
//		u8 update	DISPLAY_LINE_UPDATE_FULL, DISPLAY_LINE_CLEAR
// @return      none
// *************************************************************************************************
void display_cycle_alarm(u8 line, u8 update)
{
	
	if (update == DISPLAY_LINE_UPDATE_FULL)			
	{
	  display_hours_12_or_24(switch_seg(line, LCD_SEG_L1_3_2, LCD_SEG_L2_3_2), sCycleAlarm.hour, 2, 1, SEG_ON);
	  display_chars(switch_seg(line, LCD_SEG_L1_1_0, LCD_SEG_L2_1_0), itoa(sCycleAlarm.minute, 2, 0), SEG_ON);
	  display_symbol(switch_seg(line, LCD_SEG_L1_COL, LCD_SEG_L2_COL0), SEG_ON);

	  // Show blinking alarm icon
	  display_symbol(LCD_ICON_ALARM, SEG_ON_BLINK_ON);
	}
	else if (update == DISPLAY_LINE_CLEAR)			
	{
	  // Clean up function-specific segments before leaving function
	  display_symbol(LCD_SYMB_AM, SEG_OFF);

	  // Clear / set alarm icon
	  if (sCycleAlarm.state == CYCLE_ALARM_DISABLED)
	  {
	    display_symbol(LCD_ICON_ALARM, SEG_OFF_BLINK_OFF);
	  }
	  else
	  {
	    display_symbol(LCD_ICON_ALARM, SEG_ON_BLINK_OFF);
	  }
	}
}
#endif /* CONFIG_CYCLE_ALARM */
