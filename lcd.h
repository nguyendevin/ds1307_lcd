// lcd.h

#ifndef LCD_H_
#define LCD_H_

#include "stm32f4.h"
#include "gpio.h"

#define LCD_GPIO_PORT 			GPIOD
#define LCD_GPIO_RS				GPIO_PIN_0
#define LCD_GPIO_RW				GPIO_PIN_1
#define LCD_GPIO_EN				GPIO_PIN_2
#define LCD_GPIO_D4				GPIO_PIN_3
#define LCD_GPIO_D5				GPIO_PIN_4
#define LCD_GPIO_D6				GPIO_PIN_5
#define LCD_GPIO_D7				GPIO_PIN_6

#define LCD_CODE_CLEAR_DISPLAY	0x8
#define LCD_CODE_RETURN_HOME 	0xC

void lcd_init();

void lcd_send_code(uint8_t code);

void lcd_print_char(uint8_t data);
void lcd_print_string(char * msg);

void lcd_clear_display();
void lcd_return_home();
void lcd_set_cursor(uint8_t row, uint8_t column);

#endif /* LCD_H_ */
