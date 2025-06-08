#include <rtc.h>
#include <libasm.h>

static uint8_t bcdDecimal ( uint8_t bcd )
{
	return ( bcd >> 4 ) * 10 + ( bcd & 0x0f );
}

static uint8_t getRTC ( uint8_t mode )
{
	return bcdDecimal ( rtc ( mode ) );
}

uint8_t getSecondsRTC()
{
	return getRTC ( RTC_SECONDS );
}

uint8_t getMinutesRTC()
{
	return getRTC ( RTC_MINUTES );
}

uint8_t getHoursRTC()
{
	return getRTC ( RTC_HOURS );
}

uint8_t getDayOfWeekRTC()
{
	return getRTC ( RTC_DAY_OF_WEEK );
}

uint8_t getDayOfMonthRTC()
{
	return getRTC ( RTC_DAY_OF_MONTH );
}

uint8_t getMonthRTC()
{
	return getRTC ( RTC_MONTH );
}

uint8_t getYearRTC()
{
	return getRTC ( RTC_YEAR );
}


