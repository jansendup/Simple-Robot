#ifndef LCD_H
#define LCD_H

void init_lcd(void);

void lcd_character_write(char character);
void lcd_instruction_write(char instruction);
void lcd_set_position (signed char position);
void lcd_left_scroll(void);
void lcd_right_scroll(void);
void lcd_shift_cursor_left(void);
void lcd_shift_cursor_right(void);
void lcd_scrn_clear(void);
void lcd_backspace(void);
void lcd_print_str(char* str);

#endif
