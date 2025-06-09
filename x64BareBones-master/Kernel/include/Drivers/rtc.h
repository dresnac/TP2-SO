
#include <stdint.h>

#ifndef TP_SO_RTC_H
#define TP_SO_RTC_H



#define RTC_SECONDS         0x00
#define RTC_SECONDS_ALARM   0x01
#define RTC_MINUTES         0x02
#define RTC_MINUTES_ALARM   0x03
#define RTC_HOURS           0x04
#define RTC_HOURS_ALARM     0x05
#define RTC_DAY_OF_WEEK     0x06
#define RTC_DAY_OF_MONTH    0x07
#define RTC_MONTH           0x08
#define RTC_YEAR            0x09

uint8_t getSecondsRTC();
uint8_t getMinutesRTC();
uint8_t getHoursRTC();
uint8_t getDayOfWeekRTC();
uint8_t getDayOfMonthRTC();
uint8_t getMonthRTC();
uint8_t getYearRTC();


#endif 
