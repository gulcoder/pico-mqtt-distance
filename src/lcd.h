#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void lcd_init();
void lcd_clear();
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char* str);

#endif

