// ds1307.h

#ifndef DS1307_H_
#define DS1307_H_

#include "stm32f4.h"
#include "gpio.h"
#include "i2c.h"

#define DS1307_I2C_PORT 			GPIOB
#define DS1307_I2C_SDA				GPIO_PIN_7
#define DS1307_I2C_SCL				GPIO_PIN_6
#define DS1307_I2C_PUPD				GPIO_PUPD_PU
#define DS1307_I2C_SDA_ALTFN		4
#define DS1307_I2C_SCL_ALTFN		4
#define DS1307_I2C_BUS 				I2C1
#define DS1307_I2C_FREQ				I2C_FREQ_SM

#define DS1307_ADDRESS_SECONDS 		0x0
#define DS1307_ADDRESS_MINUTES 		0x1
#define DS1307_ADDRESS_HOURS		0x2
#define DS1307_ADDRESS_DAY			0x3
#define DS1307_ADDRESS_DATE			0x4
#define DS1307_ADDRESS_MONTH		0x5
#define DS1307_ADDRESS_YEAR			0x6

#define DS1307_HOURS_MODE_12AM		0
#define DS1307_HOURS_MODE_12PM		1
#define DS1307_HOURS_MODE_24		2

#define DS1307_DAY_SUNDAY			1
#define DS1307_DAY_MONDAY			2
#define DS1307_DAY_TUESDAY			3
#define DS1307_DAY_WEDNESDAY		4
#define DS1307_DAY_THURSDAY			5
#define DS1307_DAY_FRIDAY			6
#define DS1307_DAY_SATURDAY			7

#define DS1307_MONTH_JANUARY		1
#define DS1307_MONTH_FEBRUARY		2
#define DS1307_MONTH_MARCH			3
#define DS1307_MONTH_APRIL			4
#define DS1307_MONTH_MAY			5
#define DS1307_MONTH_JUNE			6
#define DS1307_MONTH_JULY			7
#define DS1307_MONTH_AUGUST			8
#define DS1307_MONTH_SEPTEMBER		9
#define DS1307_MONTH_OCTOBER		10
#define DS1307_MONTH_NOVEMBER		11
#define DS1307_MONTH_DECEMBER		12

#define DS1307_I2C_ADDRESS			0x68

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t hours_mode;
} time_t;

typedef struct
{
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} calendar_t;

uint8_t ds1307_init();

void ds1307_set_time(time_t * time);
void ds1307_get_time(time_t * time);

void ds1307_set_calendar(calendar_t * calendar);
void ds1307_get_calendar(calendar_t * calendar);

#endif /* DS1307_H_ */
