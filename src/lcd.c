#include "lcd.h"
#include "io_ports.h"
#include "system.h"

#define LCD_CONTROL_SIGNAL(EN,RW,RS) ( (EN*(LCD_EN)) | (RW*(LCD_RW)) | (RS*(~LCD_RS)))

#define LCD_DATA_WRITE_SIGNAL  LCD_CONTROL_SIGNAL(1,0,1)
#define LCD_INST_WRITE_SIGNAL  LCD_CONTROL_SIGNAL(1,0,0)

#define LCD_CLEAR            0b00000001
#define LCD_HOME             0b00000010
#define LCD_SHIFT_CUR_LEFT   0b00010000
#define LCD_SHIFT_CUR_RIGHT  0b00010100
#define LCD_SHIFT_DISP_LEFT  0b00011000
#define LCD_SHIFT_DISP_RIGHT 0b00011100

#define LCD_BYTES_PER_LINE      16
#define LCD_START_ADDRESS_LINE2 0x40
#define LCD_END_ADDRESS_LINE2   0x67

void lcd_nibble_write(char nibble,char ctrl_signal);
void lcd_data_write(char data, char address);
void lcd_set_cursor_address(char address);
char lcd_get_cursor_address(char line);
char lcd_get_cursor_line(char position);

char lcd_cursor_position = 0;
char lcd_shift_offset = 0;

void init_lcd(void)           
{

	lcd_nibble_write(0b0011, LCD_INST_WRITE_SIGNAL);
	delay_ms(2);
	lcd_nibble_write(0b0011, LCD_INST_WRITE_SIGNAL);
	delay_ms(2);
	lcd_nibble_write(0b0011, LCD_INST_WRITE_SIGNAL);
	delay_ms(2);
	
	lcd_nibble_write(0b0010, LCD_INST_WRITE_SIGNAL);                          
	delay_ms(2);
	
	lcd_nibble_write(0b0010, LCD_INST_WRITE_SIGNAL);
	delay_ms(2);
	lcd_nibble_write(0b1000, LCD_INST_WRITE_SIGNAL);     // Function Set ( 0 0 - 0 0 1 DL N F * * )
	delay_ms(2);
	lcd_nibble_write(0b1110, LCD_INST_WRITE_SIGNAL);        // Display ON/OFF control ( 0 0 - 0 0 0 0 1 D C B )
	delay_ms(2);
	lcd_instruction_write(LCD_CLEAR); // Clear display
	delay_ms(2);
	lcd_instruction_write(0b00000110); // Entry Mode Set ( 0 0 0 0 0 0 0 1 I/D S )
	delay_ms(2);

	lcd_cursor_position = 0;
	lcd_shift_offset = 0;
}

void lcd_nibble_write(char nibble,char ctrl_signal)
{
	char lcd_out;

	lcd_out = nibble & (~LCD_NIBBLE_MASK);
	lcd_out |= ctrl_signal;
	write_sreg2(LCD_CONTROL_MASK&LCD_NIBBLE_MASK, lcd_out);

	bit_clear_reg2(LCD_EN);
	
}


/* writes a charcter to the LCD and increments the cursor position */
void lcd_character_write(char character)
{	
	char address_line_0, address_line_1;
	
	address_line_0 = lcd_get_cursor_address(0);
	address_line_1 = lcd_get_cursor_address(1);
	
	if (lcd_get_cursor_line(lcd_cursor_position + 1) == 0)
	{
		lcd_data_write(character, address_line_1);
		lcd_data_write(character, address_line_0);
	}
	else
	{
		lcd_data_write(character, address_line_0);
		lcd_data_write(character, address_line_1);
	}
}

void lcd_data_write(char data, char address)
{
	if((address & 0b0111111) < 40)
	{
		lcd_set_cursor_address(address);
		
		lcd_nibble_write(data >> 4, LCD_DATA_WRITE_SIGNAL);
		lcd_nibble_write(data     , LCD_DATA_WRITE_SIGNAL);
		lcd_cursor_position++;
	}
}

void lcd_set_cursor_address(char address)
{
	
	lcd_nibble_write(0b1000 | (address >> 4), LCD_INST_WRITE_SIGNAL);
	lcd_nibble_write(address                , LCD_INST_WRITE_SIGNAL);
	
	if (address & 0b1000000) // In second line.
	{
		address &= 0b0111111;
		if(address > 19)
		{
			lcd_cursor_position = address + LCD_BYTES_PER_LINE - 40;
		}
		else
		{
			lcd_cursor_position = address + LCD_BYTES_PER_LINE;
		}
	}
	else // In first line.
	{
		if(address > 19)
		{
			lcd_cursor_position = address - 40;
		}
		else
		{
			lcd_cursor_position = address;
		}
	}
}

void lcd_set_position (signed char position)
{
	char address;
	lcd_cursor_position = position;
	address = lcd_get_cursor_address(lcd_get_cursor_line(position));
	lcd_set_cursor_address(address);
}

char lcd_get_cursor_address(char line)
{
	if(line == 0)
	{
		if(lcd_cursor_position < 0)
		{
			return 40 + lcd_cursor_position;
		}
		else
		{
			return lcd_cursor_position;
		}
	}
	else
	{
		if(lcd_cursor_position < LCD_BYTES_PER_LINE)
		{
			return (40 + lcd_cursor_position - LCD_BYTES_PER_LINE) | 0b1000000;
		}
		else
		{
			return (lcd_cursor_position - LCD_BYTES_PER_LINE) | 0b1000000;
		}
	}
}

char lcd_get_cursor_line(char position)
{
	signed char position_on_screen =  (position + lcd_shift_offset);
	return ( position_on_screen < LCD_BYTES_PER_LINE)? 0 : 1;
}

/* writes an instruction to the LCD */
void lcd_instruction_write(char instruction)
{	
	lcd_nibble_write(instruction>>4, LCD_INST_WRITE_SIGNAL);
	lcd_nibble_write(instruction,    LCD_INST_WRITE_SIGNAL);
}

/* scrolls the chatacters on the LCD one position to the left */
void lcd_left_scroll(void)
{  
	lcd_instruction_write(LCD_SHIFT_DISP_LEFT);
	lcd_shift_offset--;
}

/* scrolls the chatacters on the LCD one position to the right */
void lcd_right_scroll(void)
{  
	lcd_instruction_write(LCD_SHIFT_DISP_RIGHT);
	lcd_shift_offset++;
}

void lcd_shift_cursor_left(void)
{
	lcd_instruction_write(LCD_SHIFT_CUR_LEFT);
	lcd_cursor_position--;
}

void lcd_shift_cursor_right(void)
{
	lcd_instruction_write(LCD_SHIFT_CUR_RIGHT);
	lcd_cursor_position++;
}

/* clears the LCD and moves the cursor to the start position */
void lcd_scrn_clear(void)
{
	lcd_instruction_write(LCD_CLEAR);
	lcd_cursor_position = 0;
	lcd_shift_offset = 0;
}

void lcd_backspace (void)
{
	lcd_shift_cursor_left();
	delay_us(37);
	lcd_character_write(' ');
	delay_us(37);
	lcd_shift_cursor_left();
}

void lcd_print_str(char* str)
{
	while( (*str) != '\0' ){
       lcd_character_write(*str);
       str++;
       delay_us(37);
    }
}
