#include "stm8s.h"

#define PIN_sda     5   //port B
#define PIN_clk     4   //port B

#define BIT_sda    (1<<PIN_sda)
#define BIT_clk    (1<<PIN_clk)


#define RX_ADDR         0x06

#define TRANSFERED_SIZE 6



void init_iic_emb_tx(void);
void i2c_wr_reg(char address, char reg_addr, char * data, char length);
void i2c_write_repeated(char address, char reg_addr, char val, char length);
void i2c_read(char address, char * data, char length);
void disable_iic_emb_tx(void);