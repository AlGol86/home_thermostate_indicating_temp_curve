#include "stm8s.h"
#include "main.h"
#include "encoder.h"
#include "iic_embedded_tx.h"
#include "oled.h"
#include "bmp_280.h"

//globals
encoder_t  encoder;

//global block for BME_280 sensor
long int          t_fine;
bme280_calib_data _bme280_calib;
ctrl_meas         _measReg; 
ctrl_hum          _humReg;
config            _configReg;
unsigned int temp_10;
unsigned int temperature;
unsigned int pastTemperature;
unsigned int deltaTemperature;
char globalCntr;
char printGraficFlag;
unsigned int resetCondition;

Preset_data_t   preset_eeprom_data @0x4100;         //store setting variables (in EEPROM)()

OledDigitBuffer oledBuffer;

//graphic buffer
char graphicBuf[128];

int main() { 
  char i;
	groundPins();
	delay(1000);
  init_iic_emb_tx();
  init_encoder(&encoder);
	init_ssd1306();
	init_BME280();
  
  encoder.but_data_lim = 1;
  encoder_setter(0, 10, 5);
	printGraficFlag = 1;
	
  temperature = readTemperature() * 10;
  while(1) { 
	 //set brightness
		if(encoder.transient_counter & 0x1000) {
			//i = get_ADC(5);       //adapted
			i = encoder.enc_data*5; //manual
		  set_brightness_ssd1306(i >= 50 ? 255 : i*5);
   }
	 
	  if (i == 50 && printGraficFlag) {
	  	oled_Clear_Screen();
	    print_graphic();
	    printGraficFlag = 0;
	  } else if (i != 50 && !printGraficFlag) {
			oled_Clear_Screen();
			printGraficFlag = 1;
	  } else if	(i != 50 && printGraficFlag) {
	    measureAndPrintTemperature(1);
			updateTermostatState();
    } else {
		  measureAndPrintTemperature(0);
	  }

	  //fall into adjusting parameters
	  if (encoder.but_data != 0) { 	
		
		  encoder.but_data = 0;    
      menu_selector();
		
		  oled_Clear_Screen();
			i=40;
	  	encoder_setter(0, 10, i/5);
	  }
		//reset iic on condition
		if (resetCondition > 5000) {
			resetCondition = 0;
			pullUpA12Pins();
			delay(2000);
			groundPins();
			delay(1000);
			init_ssd1306();
	    init_BME280();
		}
  }
}

char menu_selector() {
	
	unsigned int integerVal;
	unsigned int decimalVal;
	Preset_data_t preset;
	preset.temperature= preset_eeprom_data.temperature;
	preset.histeresis= preset_eeprom_data.histeresis;
	oled_Clear_Screen();
	//set threshold	- integer
	printTemperature(preset.temperature, 0);
	encoder_setter(10, 50, preset.temperature / 1000);
	integerVal = scan_value_at_pos(0);
	
	preset.temperature = integerVal * 1000 + preset.temperature % 1000;
	printTemperature(preset.temperature, 0);
	
	//set threshold	- fractional
	encoder_setter(0, 99, (preset.temperature % 1000) / 10);
	decimalVal = scan_value_at_pos(56);
	preset.temperature = (preset.temperature/1000) * 1000 + decimalVal * 10;
	
	//set histeresis
	oled_Clear_Screen();
	encoder_setter(0, 99, preset.histeresis);
	preset.histeresis = scan_value_at_pos(80);
		
	saveTresholdToEeprom(preset.temperature, preset.histeresis);



}

int scan_value_at_pos(char pos) {
	char blinking;
	while(encoder.but_data == 0) {
		blinking = (encoder.transient_counter >> 10) & 1;
		oled_print_XXnumber(encoder.enc_data, pos, blinking);
		measureAndPrintTemperature(0);
	}
	encoder.but_data = 0;
	return encoder.enc_data;
}

 

void saveTresholdToEeprom(unsigned int threshold, char histeresis) { 
  char i;
	sim();
  if (!((FLASH->IAPSR) & (FLASH_IAPSR_DUL))) {  // unlock EEPROM
       FLASH->DUKR = 0xAE;
       FLASH->DUKR = 0x56;
  }
  rim();
  while (!((FLASH->IAPSR) & (FLASH_IAPSR_DUL))) ;
  
  preset_eeprom_data.temperature = threshold;
	preset_eeprom_data.histeresis = histeresis;
    
  FLASH->IAPSR &= ~(FLASH_IAPSR_DUL);  // lock EEPROM
}  

void measureAndPrintTemperature(char print) {
	extern char globalCntr;
	extern unsigned int temp_10;
	extern unsigned int temperature;
	temp_10 += readTemperature();
	temp_10 -= temp_10 / 10;
	 if (++globalCntr / 10){
	   temperature = temp_10;
		 if (print) {
	     printTemperature(temperature, 1);
	   }
		 globalCntr = 0;
   }
	 
	 
}

void printTemperature(unsigned int val, char mode) {
	   oled_print_XXnumber(val / 1000, 0, mode);
	   oled_print_giga_char('.', 40);
	   oled_print_XXnumber((val / 10) % 100, 56, mode);
}


void groundPins(void) {
  GPIOA->CR2 &= ~BIT_A1;
  GPIOA->CR2 &= ~BIT_A2;
  GPIOA->CR1 |=  BIT_A1;
  GPIOA->CR1 |=  BIT_A2;
  GPIOA->DDR |=  BIT_A1;
  GPIOA->DDR |=  BIT_A2;
  GPIOA->ODR &= ~BIT_A1;
  GPIOA->ODR &= ~BIT_A2;

  GPIOC->CR2 &= ~BIT_C3;
  GPIOC->CR2 &= ~BIT_C4;
  GPIOC->CR1 |=  BIT_C3;
  GPIOC->CR1 |=  BIT_C4;
  GPIOC->DDR |=  BIT_C3;
  GPIOC->DDR |=  BIT_C4;
  GPIOC->ODR &= ~BIT_C3;
  GPIOC->ODR &= ~BIT_C4;

  GPIOD->CR2 &= ~BIT_D2;
  GPIOD->CR2 &= ~BIT_D3;
  GPIOD->CR2 &= ~BIT_D4;
  GPIOD->CR2 &= ~BIT_D5;
  GPIOD->CR2 &= ~BIT_D6;
  GPIOD->CR1 |=  BIT_D2;
  GPIOD->CR1 |=  BIT_D3;
  GPIOD->CR1 |=  BIT_D4;
  GPIOD->CR1 |=  BIT_D5;
  GPIOD->CR1 |=  BIT_D6;
  GPIOD->DDR |=  BIT_D2;
  GPIOD->DDR |=  BIT_D3;
  GPIOD->DDR |=  BIT_D4;
  GPIOD->DDR |=  BIT_D5;
  GPIOD->DDR |=  BIT_D6;
  GPIOD->ODR &= ~BIT_D2;
  GPIOD->ODR &= ~BIT_D3;
  GPIOD->ODR &= ~BIT_D4;
  GPIOD->ODR &= ~BIT_D5;
  GPIOD->ODR &= ~BIT_D6;
}

void pullUpA12Pins(void) {
	GPIOA->ODR |= BIT_A1;
  GPIOA->ODR |= BIT_A2;
}




