#include "stm8s.h"


#define addr_iic_BMP_280 0x76 

enum
    {
        BME280_REGISTER_DIG_T1              = 0x88,
        BME280_REGISTER_DIG_T2              = 0x8A,
        BME280_REGISTER_DIG_T3              = 0x8C,

        BME280_REGISTER_DIG_P1              = 0x8E,
        BME280_REGISTER_DIG_P2              = 0x90,
        BME280_REGISTER_DIG_P3              = 0x92,
        BME280_REGISTER_DIG_P4              = 0x94,
        BME280_REGISTER_DIG_P5              = 0x96,
        BME280_REGISTER_DIG_P6              = 0x98,
        BME280_REGISTER_DIG_P7              = 0x9A,
        BME280_REGISTER_DIG_P8              = 0x9C,
        BME280_REGISTER_DIG_P9              = 0x9E,

        BME280_REGISTER_DIG_H1              = 0xA1,
        BME280_REGISTER_DIG_H2              = 0xE1,
        BME280_REGISTER_DIG_H3              = 0xE3,
        BME280_REGISTER_DIG_H4              = 0xE4,
        BME280_REGISTER_DIG_H5              = 0xE5,
        BME280_REGISTER_DIG_H6              = 0xE7,

        BME280_REGISTER_CHIPID             = 0xD0,
        BME280_REGISTER_VERSION            = 0xD1,
        BME280_REGISTER_SOFTRESET          = 0xE0,

        BME280_REGISTER_CAL26              = 0xE1,  // R calibration stored in 0xE1-0xF0

        BME280_REGISTER_CONTROLHUMID       = 0xF2,
        BME280_REGISTER_STATUS             = 0XF3,
        BME280_REGISTER_CONTROL            = 0xF4,
        BME280_REGISTER_CONFIG             = 0xF5,
        BME280_REGISTER_PRESSUREDATA       = 0xF7,
        BME280_REGISTER_TEMPDATA           = 0xFA,
        BME280_REGISTER_HUMIDDATA          = 0xFD
    };

/**************************************************************************/
/*! 
    @brief  calibration data
*/
/**************************************************************************/
    
typedef struct
    {
        unsigned int dig_T1; ///< temperature compensation value
        int  dig_T2; ///< temperature compensation value
        int  dig_T3; ///< temperature compensation value

        unsigned int dig_P1; ///< pressure compensation value
        int  dig_P2; ///< pressure compensation value
        int  dig_P3; ///< pressure compensation value
        int  dig_P4; ///< pressure compensation value
        int  dig_P5; ///< pressure compensation value
        int  dig_P6; ///< pressure compensation value
        int  dig_P7; ///< pressure compensation value
        int  dig_P8; ///< pressure compensation value
        int  dig_P9; ///< pressure compensation value

        unsigned int  dig_H1; ///< humidity compensation value
        int  dig_H2; ///< humidity compensation value
        unsigned int  dig_H3; ///< humidity compensation value
        int  dig_H4; ///< humidity compensation value
        int  dig_H5; ///< humidity compensation value
        signed char   dig_H6; ///< humidity compensation value
        char correction;
    } bme280_calib_data;



        // The config register
     typedef   struct  {
            // inactive duration (standby time) in normal mode
            // 000 = 0.5 ms
            // 001 = 62.5 ms
            // 010 = 125 ms
            // 011 = 250 ms
            // 100 = 500 ms
            // 101 = 1000 ms
            // 110 = 10 ms
            // 111 = 20 ms
            unsigned int t_sb : 3;

            // filter settings
            // 000 = filter off
            // 001 = 2x filter
            // 010 = 4x filter
            // 011 = 8x filter
            // 100 and above = 16x filter
            unsigned int filter : 3;

            // unused - don't set
            unsigned int none : 1;
            unsigned int spi3w_en : 1;

     //(t_sb << 5) | (filter << 3) | spi3w_en;

        } config;
        

        
        // The ctrl_meas register
  typedef  struct  {
            // temperature oversampling
            // 000 = skipped
            // 001 = x1
            // 010 = x2
            // 011 = x4
            // 100 = x8
            // 101 and above = x16
            unsigned int osrs_t : 3;

            // pressure oversampling
            // 000 = skipped
            // 001 = x1
            // 010 = x2
            // 011 = x4
            // 100 = x8
            // 101 and above = x16
            unsigned int osrs_p : 3;

            // device mode
            // 00       = sleep
            // 01 or 10 = forced
            // 11       = normal
            unsigned int mode : 2;
        } ctrl_meas;
        

        
        // The ctrl_hum register
    typedef    struct  {
            // unused - don't set
            unsigned int none : 5;

            // pressure oversampling
            // 000 = skipped
            // 001 = x1
            // 010 = x2
            // 011 = x4
            // 100 = x8
            // 101 and above = x16
            unsigned int osrs_h : 3;
            //(osrs_h)
           
        } ctrl_hum;
        




typedef enum  {
            SAMPLING_NONE =0, //0b000,
            SAMPLING_X1   =1, //0b001,
            SAMPLING_X2   =2, //0b010,
            SAMPLING_X4   =3, //0b011,
            SAMPLING_X8   =4, //0b100,
            SAMPLING_X16  =5 //0b101
        }sensor_sampling;

 typedef enum  {
            MODE_SLEEP  =0, // 0b00,
            MODE_FORCED =1, //0b01,
            MODE_NORMAL =3 // 0b11
        }sensor_mode;
 
  typedef enum  {
            FILTER_OFF =0, //0b000,
            FILTER_X2  =1, //0b001,
            FILTER_X4  =2, //0b010,
            FILTER_X8  =3, //0b011,
            FILTER_X16 =4 //0b100
        } sensor_filter;
  typedef enum  {
            STANDBY_MS_0_5  =0, // 0b000,
            STANDBY_MS_10   =6, //0b110,
            STANDBY_MS_20   =7, //0b111,
            STANDBY_MS_62_5 =1, //0b001,
            STANDBY_MS_125  =2, //0b010,
            STANDBY_MS_250  =3, //0b011,
            STANDBY_MS_500  =4, //0b100,
            STANDBY_MS_1000 =5 //0b101
        }standby_duration;
 
 
 	void init_BME280(void);

	void setSampling(sensor_mode mode ,              //  = MODE_NORMAL,
			 sensor_sampling tempSampling ,  // = SAMPLING_X16,
			 sensor_sampling pressSampling , //= SAMPLING_X16,
			 sensor_sampling humSampling ,  // = SAMPLING_X16,
			 sensor_filter filter       ,   // = FILTER_OFF,
			 standby_duration duration      // = STANDBY_MS_0_5
			 );
                   
 void takeForcedMeasurement(void);
 int readTemperature(void);
 void readCoefficients(void);
 char isReadingCalibration(void);
 char spixfer(char x);
 void   write8(char reg, char value);
 char   read8(char reg);
 unsigned int read16_LE(char reg);
 int readS16(char reg);
 int readS16_LE(char reg);
 unsigned int read16(char reg);
 void delay(unsigned int del);
        
        
        
        
        
 
 