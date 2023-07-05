#include "stm8s.h"
#define oled_32_128_adr 0x3c
#define DIG_BUF_SIZE 5
#define EMPTY 255;

typedef struct {
	char menu_mode_cach;
  int value[DIG_BUF_SIZE];
	char transperent_view[DIG_BUF_SIZE];
  char position[DIG_BUF_SIZE];
  char spaceRequired[DIG_BUF_SIZE];
} OledDigitBuffer; 

void init_ssd1306(void);
void set_brightness_ssd1306(char brightness);
void sendCmd(char cmd);
void sendData(char* data, char count); //send [count] data byts 
void blankScreen(char n);
void oled_Clear_Screen(void);
void oled_print_giga_space(char pos, char n);
void set_cursor(char x, char y);
void ff_string(char n);
void send_repeat_data(char data_byte, char n);
void send_single_data(char data_byte);


void oled_print_giga_digit(char d, char x, char transparent);   //12*20
void oled_print_giga_transparent_digit(char d, char x);
void oled_print_giga_solid_digit(char d, char x); 

char oled_print_giga_char(char c, char x); //12*20
void oled_print_XXnumber(int n, char pos, char transparent);
void print_save(void); 
void print_graphic(void);
char calculateZeroShift(void);
void shiftGraphicBufferAndInsertNewValue(char val);