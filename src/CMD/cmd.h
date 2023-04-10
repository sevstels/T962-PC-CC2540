//==============================================================================
//File name:    "cmd.h"
//Processor:    AVR (Atmel)
//Toolkit:      EWAVR (IAR Systems)
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _COMMAND_HEADER_H_
#define _COMMAND_HEADER_H_


//==============================================================================
//For All devices  SYSTEM CMD
//==============================================================================
#define CMD_NO_OPERATION		    0x00
#define CMD_GETDATA			        0x01
#define CMD_BEEP			        0x02
#define CMD_POWER_ON			    0x03
#define CMD_POWER_OFF			    0x04
#define CMD_READ_INFO_LENGTH		0x05
#define CMD_READ_DEVICE_INFO		0x06       
#define CMD_CHECK_DEVICE		    0x07
#define CMD_READ_MEM			    0x08
#define CMD_READ_EEPROM			    0x09
#define CMD_WRITE_EEPROM		    0x0A
#define CMD_GET_SETUP_LENGTH		0x0B
#define CMD_GET_DEVICE_SETUP		0x0C
#define CMD_SAVE_DEVICE_SETUP		0x0D
#define CMD_GET_DEVICE_VERSION		0x0E
#define CMD_JUMP_TO_BOOTLOADER		0x0F



#define CMD_DEVICE_BLINK		    0x75

//==============================================================================
//PID Controller
//==============================================================================
//#define CMD_SET_SETUP_DEFAULT     0x30
//#define CMD_DEV_SEND_TXT          0x31

//flash write commands
#define CMD_FLASH_WRITE_INI       0xAB
#define CMD_FLASH_FILL_BUFFER     0xAC 
#define CMD_FLASH_WRITE_PAGE      0xAD 
#define CMD_FLASH_WRITE_END       0xAE
//#define CMD_FLASH_WRITE_CRC       0xAF

#define CMD_PCR_CONTROL           0xB0
#define CMD_PCR_READ_ANALYSIS     0xB1
#define CMD_PCR_GET_ANALYSIS      0xB2

#define CMD_TEC_SET_MODE          0xB3
#define CMD_TEC_SET_CURRENT       0xB4
#define CMD_TEC_SET_LIM_CURRENT   0xB5
#define CMD_TEC_SET_LIM_TEMPER    0xB6
#define CMD_TEC_CHECK_DIRECTION   0xB7
#define CMD_TEC_CHECK_PELTIER     0xB8
#define CMD_TEC_TEST_LINERITY	  0xB9
#define CMD_TEC_SW_TEST           0xBA

#define CMD_DEVICE_RESET          0xBB
#define CMD_PWM_ON                0xBC
#define CMD_PWM_OFF               0xBD
#define CMD_PWM_SET_VALUE         0xBE
#define CMD_TEC_TEST_STEP	      0xBF

//#define CMD_SET_DAC_GAIN          0xC1
#define CMD_SET_DAC_OFFSET        0xC2
#define CMD_CHANGE_DAC_VALUE      0xC3
#define CMD_SET_VALUE_DAC0        0xC4
#define CMD_SET_VALUE_DAC1        0xC5
#define CMD_SET_VALUE_DAC2        0xC6
#define CMD_SET_VALUE_DAC3        0xC7

#define CMD_LED_SET_DAC           0xC8
#define CMD_LED_SET_DAC_AND_MODUL 0xC9

#define CMD_GET_OVEN_SETUP        0x30
#define CMD_DEV_SEND_TXT          0x31
#define CMD_GET_HELP	          0x32
#define CMD_GET_ABOUT	          0x33
#define CMD_SOLDERING_RUN	      0x34
#define CMD_SOLDERING_PAUSE	      0x35
#define CMD_SOLDERING_STOP	      0x36
#define CMD_BT_CONNECTED          0x37
#define CMD_BT_DISCONNECTED       0x38
#define CMD_GET_PROFILE_LIST      0x39
#define CMD_GET_SENSORS_LIST      0x3A
#define CMD_GET_SETTINGS_LIST     0x3B
#define CMD_SHOW_EEPROM           0x3C
#define CMD_GET_EEPROM            0x3D
#define CMD_SHOW_SETTINGS         0x3E
#define CMD_EEPROM_TEST			  0x3F
#define CMD_GET_TEMPERATURE		  0x40
#define CMD_SET_OFFSET0           0x41
#define CMD_SET_OFFSET1           0x42
#define CMD_SET_DGAIN0            0x43
#define CMD_SET_DGAIN1            0x44
#define CMD_SENSORS_ENABLE        0x45
#define CMD_SENSORS_CALC_MODE     0x46
#define CMD_EEPROM_ERASE          0x47
#define CMD_SET_MANUAL_MODE       0x48
#define CMD_SET_HEATER            0x49
#define CMD_SET_FAN               0x4A
#define CMD_SET_SETUP_DEFAULT     0x4B
#define CMD_SET_HTR_TOTAL_PWR     0x4C
#define CMD_SET_HTR_POWER_LIM     0x4D
#define CMD_SET_HTR_TMPRT_LIM     0x4E
#define CMD_SET_FAN_SPEED         0x4F
#define CMD_DEV_SEND_REFLOW_LOG   0x50
#define CMD_SET_SENSORS_SETUP     0x51
#define CMD_SET_PROFILE           0x52
#define CMD_PID_SET_P             0x53
#define CMD_PID_SET_I             0x54
#define CMD_PID_SET_D             0x55
#define CMD_PID_SET_P2            0x56
#define CMD_PID_SET_I2            0x57
#define CMD_PID_SET_D2            0x58
#define CMD_PID_SET_REF		      0x59
#define CMD_PID_SET_MODE          0x5A

#define CMD_PID_TUNE              0x5B
#define CMD_PID_TEST_ENABLE       0x5C
#define CMD_PID_TEST_SETUP        0x5D
#define CMD_PID_SET_PERIOD        0x5E
#define CMD_PID_SET_TO_DEFAULT    0x5F
#define CMD_PID_MANUAL_ENABLE     0x60
#define CMD_SET_FAN_SPEED_GAIN    0x61
#define CMD_SHOW_CURR_PROFILE	  0x62
#define CMD_DEV_SEND_EVENT        0x63
#define CMD_DEVICE_DETECT         0x64
#define CMD_GET_PROFILE           0x65
#define CMD_DEV_SEND_REFLOW_STEP  0x66
#define CMD_SET_LOG_CONFIG        0x67
#define CMD_WRITE_PROFILE		  0x68
#define CMD_ERASE_CUSTOM_PROFILE  0x69
#define CMD_SET_PAUSE             0x6A
#define CMD_CHECK_PROFILE_CRC	  0x6B
#define CMD_BOOT_SEND_TXT         0x6C

//CMD for Nordic chip
#define CMD_NRF_OVEN_RESET        0xB0
#define CMD_NRF_OVEN_PROG_DATA    0xB1
#define CMD_NRF_OVEN_PROG_MODE    0xB2
#define CMD_NRF_GET_BT_INFO       0xB3


#define EVT_REFLOW_DONE           0
#define EVT_CMD_ACK				  1
#define EVT_SAVE_EEPROM			  2
#define EVT_WRITE_PROFILE	      3
#define EVT_BOOT_MODE			  4
#define LOG_REFLOW_MASK           0x01


#define CMD_GET_REFLOW            0xE3
#define CMD_GET_LIST_SETT         0xE4
#define CMD_GET_STOP              0xE5
#define CMD_GET_QUET              0xE6  
#define CMD_GET_VALUES            0xE7 


//---- AMS
#define CMD_MSR_SET_MODE          0xD7
#define CMD_MSR_MEASUREMENT       0xD8 
#define CMD_SET_MONITORING_PERIOD 0xD9
#define CMD_MONITORING_ENABLE     0xDA
#define CMD_MONITORING_ACK        0xDB

#define CMD_ALL_DEFAULT_SETUP     0xDC
#define CMD_CHANNEL_DEFAULT_SETUP 0xDD
#define CMD_SET_REF               0xDE
#define CMD_SET_LOW_PASS_FILTER   0xDF

//---- ADC
#define CMD_ADC_GET_VALUE         0xE0
#define CMD_ADC_SET_GAIN          0xE1
#define CMD_ADC_SET_OVERSAMPLING  0xE2
#define CMD_ADC_NOISE_CANCEL_MODE 0xE3
#define CMD_ADC_OFFSET_CORRECTION 0xE4
//----
#define CMD_GET_INTENSITY_SAMPLE  0xE5

//----
#define CMD_MSR_SET_BIND_TIME     0xE6
#define CMD_MSR_SET_SAMPLES_NUM   0xE7
#define CMD_MSR_SET_PERIODS_NUM   0xE8
#define CMD_MSR_SET_INTERVAL      0xE9
#define CMD_MSR_SWAP_POLARITY     0xEA
#define CMD_MSR_CALIBRATION       0xEB
#define CMD_MSR_SET_CALIBR_DC     0xC0
#define CMD_MSR_SET_CALIBR_AC     0xC1

//---- TSensor
#define CMD_TSN_SET_SENSOR_TYPE   0xEC
#define CMD_TSN_WRITE_COEFF       0xED

//---- Sensors Switch
#define CMD_SET_SWITCH            0xEF

//==============================================================================
//Device data send for state monitoring 
//==============================================================================
#define MONITORING_CHN             0xFE
#define HEADER_MONITORING          0xFE
#define HEADER_ERROR_CODE          0xFA
#define DEVICE_ADC_DATA            0xFD
#define DEVICE_ADC_DATA2           0xFC
#define PAGE_PCR                   0xFB
#define PAGE_TEC	               0xF9
#define PAGE_LED	               0xF8
#define PAGE_PID	               0xF7
#define PAGE_MAIN_DLG   	       0xF6

//---- PID
#define PID_ERROR                  0x01
#define PID_LOCKED                 0x02
#define PID_LOST_LOCK              0x03
#define PID_SET_NEW_REF		       0x04
#define PID_SAMPLES                0x05
#define PID_SAMPLES_END            0x06
#define PID_MODE_MANUAL            0x07
#define PID_MODE_AUTO              0x08
#define PID_MODE_ENABLE            0x09
#define PID_MODE_DISABLE           0xA0
#define PID_TEST_PERIOD            0xA1
#define PID_TEST_MAX_TEMPER        0xA2
#define PID_TEST_MIN_TEMPER        0xA3
#define PID_TEST_CYCLES            0xA4
#define PID_TEST_FUNCTION          0xA5

//---- TEC
#define TEC_CALC_CURRENT_LIMIT     0
#define TEC_PELTIER_DISCONNECTED   1
#define TEC_PELTIER_CURRENT        2
#define TEC_DIRECTION_DETECTED     3
#define TEC_DIRECTION_ERR          4
#define TEC_PELTIER_TEST_POS       6
#define TEC_PELTIER_TEST_NEG       7
#define TEC_PELTIER_TEST_END       8
#define TEC_DISABLE 0
#define TEC_POS  1
#define TEC_NEG  2
#define TEC_OFF  3

//----
#define TEC_ERROR                  0x21
#define TEC_SET_SW_DIRECTION       0x22
#define TEC_CHECK_PELTIER          0x23
#define TEC_CHECK_DIRECTION        0x24
#define TEC_HEATER_PELTIER         0x25
#define TEC_HEATER_RESISTOR        0x26
//----
#define PCR_HEAD_RGBY_SAMPLE       0x45

//----  LEDS
#define LED_SET_SCALE_RANGE        0x01
#define LED_SET_CAN_ID             0x02
#define LED_SET_AMC_LED            0x03
#define LED_SET_CAN_LED            0x04

//----	MEASUREMENT
#define MEASURE_SET_ZERO           1
#define MEASURE_SET_GAIN           2
#define MEASURE_SET_ALGORITHM      3


#define DEV_SEND_ERROR_EVENT       0x01
#define DEV_SEND_RECOGNIZE_POINT   0x02
#define DEV_COMPLETE_RECOGNIZE     0x03
#define DEV_RECOGNIZE_MAX          0x04 
#define DEV_JTAG_CONNECTED         0x05
#define DEV_COMPLETE_SEND_SCAN     0x06
#define DEV_COMPLETE_OSCSCAN_LEFT  0x07
#define DEV_COMPLETE_OSCSCAN_RIGHT 0x08
#define DEV_SETUP_UPDATE_REQUEST   0x09
#define DEV_TEC_DCDC_ERROR         0x0A
#define DEV_SET_DEFAULT_COMLETED   0x0B

//Devices CAN address range 
#define CANID_FIRST                0x90 //Address begin 
#define CANID_LAST                 0x9F //Address end
#define MONITORING_CHN             0xFE

#define CH1_ON					   0x01
#define CH2_ON					   0x02
#define CH3_ON					   0x04
#define CH4_ON					   0x08

#define PCR_ERROR                  0x11
#define PCR_PREPARATION            0x12
#define PCR_SET_TEMPERATURE        0x13
#define PCR_MEASUREMENT            0x14
#define REFLOW_COMPLETED           0x15
#define PCR_PAUSE                  0x16
#define PCR_CONTINUE               0x17
#define PCR_SINGLE_SAMPLE          0x18
#define PCR_SAMPLE_PROFILE         0x19
#define PCR_SAMPLE_MANUAL          0x1A
#define PCR_ENABLE                 0x1B
#define PCR_DISABLE                0x1C
#define PCR_LED_CALIBRATION	       0x1D
#define PCR_LED_LINEARIZE          0x1E
#define PCR_SENSOR_ERROR	       0x1F
#define PCR_SAMPLE_BLOCK_END	   0x20

#define CHN_MODE_OFF			   0x00
#define CHN_MODE_PID			   0x01
#define CHN_MODE_DAC			   0x02
#define PID_SCALING_FACTOR		   128
 
#define PROTOCOL_START_BYTE1	   0x0F
#define PROTOCOL_START_BYTE2	   0xAC

#endif