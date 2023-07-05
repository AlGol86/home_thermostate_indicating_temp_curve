/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */
 
#include "encoder.h"
#include "oled.h"
#include "main.h"

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

@far @interrupt void TIM2Interrupt (void)
{
	extern encoder_t encoder;
	extern unsigned int temperature;
	extern unsigned int pastTemperature;
	extern int deltaTemperature;
	extern char printGraficFlag;
	extern Preset_data_t   preset_eeprom_data;
	
	TIM2->SR1&=~TIM2_SR1_UIF;//flag "0"
	encoder_handler(&encoder);
	encoder.transient_counter++;
	if(!encoder.transient_counter) {
		encoder.transient_long_counter++;
		if(!(encoder.transient_long_counter & 0x0f)) {
			//period = 150 sec = 2,5 min
	    deltaTemperature = temperature - pastTemperature;
		  pastTemperature = temperature;
			shiftGraphicBufferAndInsertNewValue((temperature / 100) % 100);
			printGraficFlag = 1;
	  }
		updateTermostatState();
	}
	
	return;
}


void updateTermostatState(void) {
	
	extern unsigned int temperature;
	extern Preset_data_t   preset_eeprom_data @0x4100; ;
	
	PORT_OUT->CR2 &= ~BIT_OUT;
	PORT_OUT->CR1 |=  BIT_OUT;
	PORT_OUT->DDR |=  BIT_OUT;
	if (preset_eeprom_data.temperature + (preset_eeprom_data.histeresis * 5) < temperature) {
		PORT_OUT->ODR |= BIT_OUT;
	} else if (preset_eeprom_data.temperature > temperature + (preset_eeprom_data.histeresis * 5)) {
	  PORT_OUT->ODR &= ~BIT_OUT;
  }
	
}

void shiftGraphicBufferAndInsertNewValue(char val){
	extern char graphicBuf[128];
	char i;
	for (i=0; i<128; i++) {
		graphicBuf[i] = graphicBuf[i+1];
	}
	graphicBuf[127] = val;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, NonHandledInterrupt}, /* irq3  */
	{0x82, NonHandledInterrupt}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, TIM2Interrupt}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
