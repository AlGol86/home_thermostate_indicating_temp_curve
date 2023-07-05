#include "stm8s.h"

#define PORT_OUT GPIOA

#define PIN_OUT      3
#define BIT_OUT                       (1<<PIN_OUT)

#define PIN_A1       1
#define BIT_A1                        (1<<PIN_A1)
#define PIN_A2       2
#define BIT_A2                        (1<<PIN_A2)

#define PIN_C3       3
#define BIT_C3                        (1<<PIN_C3)
#define PIN_C4       4
#define BIT_C4                        (1<<PIN_C4)

#define PIN_D2       2
#define BIT_D2                        (1<<PIN_D2)
#define PIN_D3       3
#define BIT_D3                        (1<<PIN_D3)
#define PIN_D4       4
#define BIT_D4                        (1<<PIN_D4)
#define PIN_D5       5
#define BIT_D5                        (1<<PIN_D5)
#define PIN_D6       6
#define BIT_D6                        (1<<PIN_D6)

typedef enum {time} menu_t;
typedef enum {set_time, set_date, set_offset, info, set_bell} first_selector_t;

typedef struct {
	      unsigned int temperature;
        char histeresis;
        char inAdvanceDelta;
} Preset_data_t;

void menu_partition(menu_t menu);
void saveTresholdToEeprom(unsigned int threshold, char histeresis);
char menu_selector(void);
int scan_value_at_pos(char pos);
void measureAndPrintTemperature(char print);
void printTemperature(unsigned int val, char mode);
void updateTermostatState(void);
void groundPins(void); 
void pullUpA12Pins(void);