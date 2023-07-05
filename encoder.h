#include "stm8s.h"

#define PORT_ENCODER_L   GPIOC

#define PORT_ENCODER_R_B GPIOC


#define pin_enc_l      6
#define bit_enc_l                       (1<<pin_enc_l)
#define pin_enc_r      7
#define bit_enc_r                       (1<<pin_enc_r)
#define pin_enc_b      5
#define bit_enc_b                       (1<<pin_enc_b)


typedef struct {
	char cnt;
	int cnt_button;
	char f_push :1;
	char f_long_push :1;
        
	int first_enc_data;
	int enc_data;
	int enc_data_lim_h;
	int enc_data_lim_l;
	char but_data;
	char but_data_long;
	char but_data_lim;
	char but_data_lim_long;
	unsigned int transient_counter;
	unsigned int transient_long_counter;
} encoder_t;

//encoder
@far @interrupt void TIM2Interrupt (void);
void init_tim2 (void);
void init_encoder(encoder_t* enc);
void encoder_handler(encoder_t* enc);
void encoder_setter(int lim_l,int lim_h,int first_value);
char check_encoder_button(encoder_t* enc);
