//==============================================================================
//File name:    "cmd.h"
//Processor:    AVR (Atmel)
//Toolkit:      EWAVR (IAR Systems)
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _COMMAND_HEADER_H_
#define _COMMAND_HEADER_H_


//==============================================================================
//For All devices  SYSTEM CMD
//==============================================================================
#define CMD_NO_OPERATION		    0x00
#define CMD_BEEP			        0x02
#define CMD_POWER_ON			    0x03
#define CMD_POWER_OFF			    0x04
#define CMD_READ_INFO_LENGTH		0x05
#define CMD_READ_DEVICE_INFO		0x06       
#define CMD_CHECK_DEVICE		    0x07
#define CMD_GET_SETUP_LENGTH		0x0B
#define CMD_GET_DEVICE_SETUP		0x0C
#define CMD_SAVE_DEVICE_SETUP		0x0D
#define CMD_GET_DEVICE_VERSION		0x0E

//==============================================================================
//
//==============================================================================
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
#define CMD_CHECK_BOOTLOADER      0x6D
#define CMD_LED_BLINK		      0x6E

//CMD for Nordic chip
#define CMD_NRF_OVEN_RESET              0xB0
#define CMD_NRF_OVEN_PROG_DATA          0xB1
#define CMD_NRF_OVEN_ISP_MODE           0xB2
#define CMD_NRF_OVEN_JUMP_TO_BOOTLOADER 0xB3
#define CMD_NRF_GET_BT_INFO             0xB4
#define CMD_NRF_GET_POWER_VOLTAGE       0xB5
#define CMD_NRF_LED_BLINK               0xB6
#define CMD_NRF_READ_I2C	            0xB7
#define CMD_NRF_WRITE_I2C		        0xB8
#define CMD_NRF_SET_SETUP_DEFAULT       0xB9
#define CMD_NRF_READ_INFO_LENGTH        0xBA
#define CMD_NRF_READ_DEVICE_INFO	    0xBB
#define CMD_NRF_CHECK_DEVICE            0xBC
#define CMD_NRF_GET_DEVICE_SETUP        0xBD
#define CMD_NRF_GET_SERIAL_NUMBER       0xBE
#define CMD_NRF_JUMP_TO_BOOTLOADER      0xBF

#define EVT_REFLOW_DONE           0
#define EVT_CMD_ACK				  1
#define EVT_SAVE_EEPROM			  2
#define EVT_WRITE_PROFILE	      3
#define EVT_BOOT_MODE			  4
#define LOG_REFLOW_MASK           0x01

//==============================================================================
//Device data send for state monitoring 
//==============================================================================
#define PROTOCOL_START_BYTE1	   0x0F
#define PROTOCOL_START_BYTE2	   0xAC

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

#define OVEN_ERROR                 0x11
#define PCR_PREPARATION            0x12
#define PCR_MEASUREMENT            0x14
#define REFLOW_COMPLETED           0x15
#define PCR_PAUSE                  0x16
#define PCR_CONTINUE               0x17
#define PCR_ENABLE                 0x1B
#define PCR_DISABLE                0x1C
#define PCR_SENSOR_ERROR	       0x1F
#define PCR_SAMPLE_BLOCK_END	   0x20
 

#endif