#include "stm8s.h"
#include "encoder.h"


void init_tim2 (void) {//encoder_poller
  TIM2->IER|=TIM2_IER_UIE;
	//TIM2->IER|=TIM1_IER_CC1IE;
  TIM2->PSCR=0;
	TIM2->ARRH=1; //2
  TIM2->ARRL=20;//20
	TIM2->CNTRH=0;
	TIM2->CNTRL=0;
	rim();
	TIM2->CR1|=TIM2_CR1_CEN;
  }

void init_encoder(encoder_t* enc){
	
	init_tim2();
	
	PORT_ENCODER_R_B->CR2&=~(bit_enc_l|bit_enc_r|bit_enc_b);
	PORT_ENCODER_R_B->DDR&=~(bit_enc_l|bit_enc_r|bit_enc_b);
	PORT_ENCODER_R_B->CR1|=(bit_enc_l|bit_enc_r|bit_enc_b);
	PORT_ENCODER_L->CR2&=~(bit_enc_l|bit_enc_r|bit_enc_b);
	PORT_ENCODER_L->DDR&=~(bit_enc_l|bit_enc_r|bit_enc_b);
	PORT_ENCODER_L->CR1|=(bit_enc_l|bit_enc_r|bit_enc_b);
	
  enc->but_data_lim=1;        
  enc->but_data_lim_long=1;
}

char check_encoder_button(encoder_t* enc){
      
        if(PORT_ENCODER_R_B->IDR & bit_enc_b) return 0;
        else return 1;

}

void encoder_handler(encoder_t* enc){ 
  char cnt_lim=7;
	int cnt_button_lim=10000;
//encoder
	if(((PORT_ENCODER_L->IDR&bit_enc_l)==0) && (enc->cnt < cnt_lim)) 
	  enc->cnt++;
  else	if (enc->cnt>0) enc->cnt--;
//button
	if(((PORT_ENCODER_R_B->IDR&bit_enc_b)==0) && (enc->cnt_button < cnt_button_lim)) 
	  enc->cnt_button++;
  else	if (enc->cnt_button > 0) 
	        if(enc->cnt_button > cnt_lim) 
					    enc->cnt_button = cnt_lim; 
					else
					 enc->cnt_button--;
//encoder
	if ((enc->cnt >(cnt_lim-2))&&(enc->f_push==0)) {
		  enc->f_push=1;
			if(((PORT_ENCODER_R_B->IDR&bit_enc_r)==0)&&(enc->enc_data < enc->enc_data_lim_h)) enc->enc_data++; 
			else if(((PORT_ENCODER_R_B->IDR&bit_enc_r)!=0)&&(enc->enc_data > enc->enc_data_lim_l)) enc->enc_data--;
	    }
   if ((enc->cnt==0)&&(enc->cnt_button==0))	
	       {enc->f_push=0;enc->f_long_push=0;}

//button long
   if ((enc->cnt_button >(cnt_button_lim-2))&&(enc->f_long_push==0)){
		enc->f_long_push=1;
		if (enc->but_data_long < enc->but_data_lim_long) enc->but_data_long++;
		else enc->but_data_long=0;

		enc->but_data=0;
	}
        
//button short	
	if ((enc->cnt_button >(cnt_lim-2))&&(enc->f_push==0)) {
		enc->f_push=1;
		if (enc->but_data < enc->but_data_lim) enc->but_data++;
		else enc->but_data=0;
	}			
	
}
	
void encoder_setter(int lim_l,int lim_h,int first_value){
		extern encoder_t encoder;	
		encoder.enc_data_lim_l=lim_l;
		encoder.enc_data_lim_h=lim_h;
		encoder.enc_data=first_value;
}