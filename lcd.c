// lcd.c

#include "lcd.h"

static void lcd_cfg_pin();
static void lcd_set_pin(uint8_t data);
static void lcd_enable();
static void delay_ms(uint8_t time);
static void delay_us(uint8_t time);

void lcd_init()
{
	lcd_cfg_pin();

	lcd_set_pin(0x0);

	delay_ms(40);

	lcd_set_pin(0xC);

	delay_ms(5);

	lcd_set_pin(0xC);

	delay_us(150);

	lcd_set_pin(0xC);

	lcd_set_pin(0x4);
}

void lcd_send_code(uint8_t code)
{
	lcd_set_pin((code >> 4) & 0xF);

	lcd_set_pin(code & 0xF);
}

void lcd_print_char(uint8_t data)
{
	lcd_set_pin(((data >> 4) & 0xF) | (0x1 << 5));

	lcd_set_pin((data & 0xF) | (0x1 << 5));
}

void lcd_print_string(char * msg)
{
    do
    {
        lcd_print_char((uint8_t) *msg++);
    }
    while (*msg != '\0');
}

void lcd_clear_display()
{
	lcd_send_code(LCD_CODE_CLEAR_DISPLAY);

	delay_ms(2);
}

void lcd_return_home()
{
	lcd_send_code(LCD_CODE_RETURN_HOME);

	delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t column)
{
	column--;

	switch (row)
	{
		case 1:
			lcd_send_code((column |= 0x80));
		break;

		case 2:
			lcd_send_code((column |= 0xC0));
		break;

		default:
		break;
	}
}

static void lcd_cfg_pin()
{
	gpio_cfg_t lcd;

	lcd.mode = GPIO_MODE_OUTPUT;
	lcd.type = GPIO_TYPE_PP;
	lcd.speed = GPIO_SPEED_HIGH;
	lcd.pupd = GPIO_PUPD_NO;

	lcd.pin = LCD_GPIO_RS;
	gpio_init(LCD_GPIO_PORT, &lcd);

	lcd.pin = LCD_GPIO_RW;
	gpio_init(LCD_GPIO_PORT, &lcd);

	lcd.pin = LCD_GPIO_EN;
	gpio_init(LCD_GPIO_PORT, &lcd);

	lcd.pin = LCD_GPIO_D4;
	gpio_init(LCD_GPIO_PORT, &lcd);

	lcd.pin = LCD_GPIO_D5;
	gpio_init(LCD_GPIO_PORT, &lcd);

	lcd.pin = LCD_GPIO_D6;
	gpio_init(LCD_GPIO_PORT, &lcd);

	lcd.pin = LCD_GPIO_D7;
	gpio_init(LCD_GPIO_PORT, &lcd);
}

static void lcd_set_pin(uint8_t data)
{
	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_RS, ((data >> 5) & 0x1));

	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_RW, ((data >> 4) & 0x1));

	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_D4, ((data >> 3) & 0x1));

	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_D5, ((data >> 2) & 0x1));

	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_D6, ((data >> 1) & 0x1));

	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_D7, ((data >> 0) & 0x1));

	lcd_enable();
}

static void lcd_enable()
{
	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_EN, 0x1);

	delay_us(10);

	gpio_write_pin(LCD_GPIO_PORT, LCD_GPIO_EN, 0x0);

	delay_us(100);
}

static void delay_ms(uint8_t time)
{
	for (uint32_t i = 0; i < (time * 1000); i++);
}

static void delay_us(uint8_t time)
{
	for (uint32_t i = 0; i < (time * 1); i++);
}
