// ds1307.c

#include "ds1307.h"

static void ds1307_i2c_cfg_pin();
static void ds1307_i2c_cfg_peripheral();

static void ds1307_i2c_write_data(uint8_t addr, uint8_t data);
static uint8_t ds1307_i2c_read_data(uint8_t addr);

static uint8_t binary_to_bcd(uint8_t data);
static uint8_t bcd_to_binary(uint8_t data);

i2c_cfg_t g_i2c;

uint8_t ds1307_init()
{
	ds1307_i2c_cfg_pin();

	ds1307_i2c_cfg_peripheral();

	ds1307_i2c_write_data(DS1307_ADDRESS_SECONDS, 0x0);

	return (ds1307_i2c_read_data(DS1307_ADDRESS_SECONDS) >> 7);
}

void ds1307_set_time(time_t * time)
{
	uint8_t hours = binary_to_bcd(time->hours);

	ds1307_i2c_write_data((binary_to_bcd(time->seconds) & ~(0x1 << 7)), DS1307_ADDRESS_SECONDS);

	ds1307_i2c_write_data(binary_to_bcd(time->minutes), DS1307_ADDRESS_MINUTES);

	if (time->hours_mode == DS1307_HOURS_MODE_24)
	{
		hours &= ~(0x1 << 6);
	}
	else
	{
		hours |= (0x1 << 6);
		hours = (time->hours_mode == DS1307_HOURS_MODE_12PM) ? (hours | (0x1 << 5)) : (hours & ~(0x1 << 5));
	}

	ds1307_i2c_write_data(hours, DS1307_ADDRESS_HOURS);
}

void ds1307_get_time(time_t * time)
{
	uint8_t hours = ds1307_i2c_read_data(DS1307_ADDRESS_HOURS);

	time->seconds = bcd_to_binary(ds1307_i2c_read_data(DS1307_ADDRESS_SECONDS) & ~(0x1 << 7));
	time->minutes = bcd_to_binary(ds1307_i2c_read_data(DS1307_ADDRESS_MINUTES));

	if (hours & (0x1 << 6))
	{
		time->hours_mode = ((hours & (0x1 << 5)) != 0);
		hours &= ~(0x3 << 5);
	}
	else
	{
		time->hours_mode = DS1307_HOURS_MODE_24;
	}

	time->hours = bcd_to_binary(hours);
}

void ds1307_set_calendar(calendar_t * calendar)
{
	ds1307_i2c_write_data(binary_to_bcd(calendar->day), DS1307_ADDRESS_DAY);

	ds1307_i2c_write_data(binary_to_bcd(calendar->date), DS1307_ADDRESS_DATE);

	ds1307_i2c_write_data(binary_to_bcd(calendar->month), DS1307_ADDRESS_MONTH);

	ds1307_i2c_write_data(binary_to_bcd(calendar->year), DS1307_ADDRESS_YEAR);
}

void ds1307_get_calendar(calendar_t * calendar)
{
	calendar->day =  bcd_to_binary(ds1307_i2c_read_data(DS1307_ADDRESS_DAY));

	calendar->date = bcd_to_binary(ds1307_i2c_read_data(DS1307_ADDRESS_DATE));

	calendar->month = bcd_to_binary(ds1307_i2c_read_data(DS1307_ADDRESS_MONTH));

	calendar->year = bcd_to_binary(ds1307_i2c_read_data(DS1307_ADDRESS_YEAR));
}

static void ds1307_i2c_cfg_pin()
{
	gpio_cfg_t sda;
	gpio_cfg_t scl;

	sda.pin = DS1307_I2C_SDA;
	sda.mode = GPIO_MODE_ALTFN;
	sda.type = GPIO_TYPE_OD;
	sda.speed = GPIO_SPEED_HIGH;
	sda.pupd = DS1307_I2C_PUPD;
	sda.altfn = DS1307_I2C_SDA_ALTFN;

	scl.pin = DS1307_I2C_SCL;
	scl.mode = GPIO_MODE_ALTFN;
	scl.type = GPIO_TYPE_OD;
	scl.speed = GPIO_SPEED_HIGH;
	scl.pupd = DS1307_I2C_PUPD;
	scl.altfn = DS1307_I2C_SCL_ALTFN;

	gpio_init(DS1307_I2C_PORT, &sda);
	gpio_init(DS1307_I2C_PORT, &scl);
}

static void ds1307_i2c_cfg_peripheral()
{
	g_i2c.ack = I2C_ACK_ENABLE;
	g_i2c.freq = DS1307_I2C_FREQ;

	i2c_init(DS1307_I2C_BUS, &g_i2c);

	i2c_peripheral_control(DS1307_I2C_BUS, ENABLE);
}

static void ds1307_i2c_write_data(uint8_t addr, uint8_t data)
{
	uint8_t tx[2];

	tx[0] = addr;
	tx[1] = data;

	i2c_master_transmit_data(DS1307_I2C_BUS, tx, 2, DS1307_I2C_ADDRESS, 0);
}

static uint8_t ds1307_i2c_read_data(uint8_t addr)
{
	uint8_t data;

	i2c_master_transmit_data(DS1307_I2C_BUS, &addr, 1, DS1307_I2C_ADDRESS, 0);

	i2c_master_receive_data(DS1307_I2C_BUS, &g_i2c, &data, 1, DS1307_I2C_ADDRESS, 0);

	return data;
}

static uint8_t binary_to_bcd(uint8_t data)
{
	return (data < 10 ? data : (((data / 10) << 4) | (data % 10)));
}

static uint8_t bcd_to_binary(uint8_t data)
{
	return (((data >> 4) * 10) + (data & 0xF));
}
