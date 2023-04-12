//==============================================================================
//File name:   "device_struct.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _DEVICE_STRUCT_H_
#define _DEVICE_STRUCT_H_
      
#define DEVICE_PCR_CONTROLLER         0
#define DEVICE_LED_CONTROLLER         1  
#define DEVICE_TECv2_CONTROLLER       2
#define DEVICE_LED_CONTROLLER_ARM_V1  3

//Device monitoring channels
//Channels for data Block1
#define CHN_HEATSINK          0 
#define CHN_PWR_VOLTAGE       1
#define CHN_LED_CURRENT       2

//Channels for data Block2
#define CHN_TEC_TEMPER        3
#define CHN_TEC_VOLTAGE       4
#define CHN_TEC_CURRENT       5

//Device current sensing hardware
//#define LED_CURRENT_SHUNT     0.020 //Ohm
//#define LED_CURRENT_RESISTOR  30000 //Ohm

//----
#define TEC_CURRENT_SHUNT     0.020 //Ohm
#define TEC_CURRENT_RESISTOR  30000//Ohm

//-----------------------------------------------
//Structure 
//-----------------------------------------------
#pragma pack(push, 1)
typedef struct p_Setup
{
  //---- PID factors
  float P; //factor   
  float I; //factor 
  float D; //factor
  float REF;  //Ref
  
  //----
  unsigned char period;//ms
} PID_Setup;
#pragma pack(pop)

//-----------------------------------------------
//Bitfield Structure 
//-----------------------------------------------
#pragma pack(push, 1)
typedef struct {

  //Byte 1
  unsigned ADC_OffsetCorrection: 1;
  unsigned TSensor_Polarity:     1;
  unsigned Manual_MeasurementON: 1;  
  unsigned PCR_InProgress:       1;
  unsigned JTAG_enabled:         1;
  unsigned TEC_DCDC_enable:      1; 
  unsigned StopProcess1:         1;  
  unsigned TEC_SW_direction:     1; //Normal = 0, wrong connection = 1
  
  //Byte 2  
  unsigned TEC_PeltierError:     1;
  unsigned HeaterType:           1; //Resistor=1 Peltier=0
  unsigned PID_ModeManual:       1; //Manual mode = 1 
  unsigned PID_locked:           1; //Locked = 1	
  unsigned MSR_InProgress:       1; //Flag
  unsigned TEC_TestMode:         1; //Flag
  unsigned AMS_sensorOK:         1; //Sensor state
  unsigned AMS_OnBoard:          1; //
    
  //Byte 3	
  unsigned DS18B20_OnBoard:      1;
  unsigned LED_AMS_or_CAN:       1;
  unsigned dLED3_ModulationON:   1;
  unsigned dLED4_ModulationON:   1;
  unsigned dLED1_Calibrated:     1;
  unsigned dLED2_Calibrated:     1;
  unsigned dLED3_Calibrated:     1;
  unsigned dLED4_Calibrated:     1;
  
  //Byte 4
  unsigned PCR_SampleManual:     1;
  unsigned PC_monitoringON:      1;
  unsigned PC_monitoringACK:     1;   
  unsigned MSR_SW_ManualControl: 1;
  unsigned MSR_SwapPolarity:     1;  
  unsigned PID_TEST_enable:      1;
  unsigned PID_Run:              1; 
  unsigned LED_protectionON:     1;
/*
  //Byte 5 
  unsigned MonitoringPeriod:     8;
  
  //Byte 6  
  unsigned AlignReserved1:       8;
  
  //Byte 7 
  unsigned AlignReserved2:       8;

  //Byte 8 
  unsigned AlignReserved3:       8;    
*/

} Device_Mode;
#pragma pack(pop)

typedef enum eTempSensor 
{
  TC_COLD_JUNCTION = 0,
  TC_HEATER_ALL_SENSORS,
  TC_HEATER_SENSOR_LEFT,
  TC_HEATER_SENSOR_RIGHT,
  TC_PCB_ALL_SENSORS,
  TC_PCB1,
  TC_PCB2,
  TC_PCB3,
  TC_PCB4,
  TC_PCB,
  TC_TMPR_HEATER_FAN,  
  TC_COUNT_END //terminator

} TempSensor_t;

typedef enum eNVItem 
{
  REFLOW_BEEP_DONE_LEN=0,
  REFLOW_PROFILE=1,
  TC_LEFT_GAIN,
  TC_LEFT_OFFSET,
  TC_RIGHT_GAIN,
  TC_RIGHT_OFFSET,
  REFLOW_MIN_FAN_SPEED,
  REFLOW_BAKE_SETPOINT_H,
  REFLOW_BAKE_SETPOINT_L,
  NVITEM_NUM_ITEMS // Last value
} NVItem_t;

#pragma pack(push)
#pragma pack(1)
typedef struct 
{
  unsigned short magic;
  unsigned char numitems;
  unsigned char config[NVITEM_NUM_ITEMS];
} NV_t;
#pragma pack(pop)

// Number of temperature settings in a reflow profile
#define NUMPROFILETEMPS (48)
typedef struct 
{
  char* name;
  unsigned char temperatures[NUMPROFILETEMPS];
} temperature_profile;

//-----------------------------------------------
//Structure Temperature Sensor setup
//-----------------------------------------------
#pragma pack(push, 1)
typedef struct TSensor_struct
{  
  //----
  float calibration;
  char sensor_type;
  char filter;
  char mode;
  char burst_samples;
  char measure_resolution;
  char ambient_resolution;

} MCP9600_Setup;
#pragma pack(pop)
//-----------------------------------------------

//-----------------------------------------------
//Structure 
//-----------------------------------------------
#pragma pack(push, 1)
typedef struct Setup_struct
 {
   //do not change this variables
   unsigned short setup_length;
   unsigned char  Device_type;

   //ADC
   float          ADC_REF;
   //unsigned char  REF_setup;
   signed short   ADC_value[6];
   unsigned char  ADC_chn_mode[6];
   unsigned char  ADC_oversample[6];
   unsigned char  ADC_div[6];
   
   //LED DACs
   unsigned short LED_DAC_value[4];
   unsigned short LED_ScaleRange;
   unsigned char  LED_CurrentShuntResistor;
   unsigned short LED_CurrentSensorResistor;
   unsigned char  LED_Modulation_value[4];
   
   //LEDs
   unsigned char LEDs_Enable;   
   unsigned char LEDs_Modulation;
   unsigned char LEDs_Calibrated;
   unsigned char LEDs_Group;
   unsigned char LED_ControllerID;

   //Measure setup
   unsigned char  MSR_Algorithm;
   unsigned char  MSR_SW_mode;
   unsigned char  MSR_ZeroBind;
   unsigned char  MSR_Samples;
   unsigned char  MSR_Periods;
   unsigned char  MSR_Interval;
   unsigned short MSR_Matrix;
   unsigned short MSR_CalibrDC;
   unsigned short MSR_CalibrAC;

   //TEC 
   float TEC_temperature;
   float TEC_voltage_ResDivisor;
   signed char TEC_temperature_limit_max;
   signed char TEC_temperature_limit_min;
   unsigned short TEC_current;
   unsigned short TEC_current_limit;
   unsigned short TEC_current_max;
   unsigned short TEC_max_dac_value;   
   unsigned short TEC_shunt_value; 

   //Device
   float Power;
   float Power_ResDivisor;
   unsigned char  MonitoringPeriod;
   
   //PID
   PID_Setup PID;
   unsigned char  PID_TEST_function;
   unsigned char  PID_TEST_max_temperature;
   unsigned char  PID_TEST_min_temperature;
   unsigned char  PID_TEST_period_sec;
   unsigned char  PID_TEST_cycles;
   
   //T-Sensor
   unsigned char  TSensor_id;
   unsigned char  TSensor_channel;
   signed short	  TSensor_Dallas;

   //LPF Filter
   unsigned char  LPF_filter[4];
   unsigned short period;
     
   //Files
   unsigned short Analysis_length;
   unsigned char  Analysis_crc;
   unsigned short LastSample_number;
   unsigned char  Profile_number;   
   
   //PCR
   unsigned short PCR_period; 
   unsigned short PCR_File_writed;   
   unsigned char  PCR_File_crc;
   
   unsigned short DevSetupAddress;
   
   unsigned char  Bootloader;
   unsigned char  Firmware_version;
   unsigned long  Firmware_build_data;
   //----
   Device_Mode Mode;
   
 }Device_Setup;
#pragma pack(pop)
//-----------------------------------------------

#pragma pack(push, 1)
typedef struct Setup_struct2
{
  //RAM copy of the NV data
  NV_t NV; 
  
  //----
  unsigned short heater_total_power;  
  unsigned short heater_power_limit;
  unsigned short heater_tempr_limit;
  unsigned char fan_speed;
  float fan_gain;

  //User setup
  char  tsensor_type;
  
  //T-Sensors calibration value
  char adc_offset[2];
  float adc_gain[2];
  
  //T-Sensors enable for use
  char  use_sensor_heater;
  char  use_sensor_pcb;
  char  calc_method_heater;
  char  calc_method_sensors;
  char  cold_junction_heater;

  //Soldering mode
  unsigned char soldering_mode;
  unsigned char reflow_profile;
  unsigned char profile_length;
  unsigned char custom_profile_crc;
  char log_config;

  //PID parameters
  PID_Setup PID;
  
  //MCP9600 T-sensors setup
  MCP9600_Setup TS[4];
    
  //System setup
  unsigned short firmware_version;   
  unsigned long  firmware_build_data;
  unsigned char  bootloader;
  unsigned char  CRC;

}Device_Setup2;
#pragma pack(pop)
//-----------------------------------------------

//-----------------------------------------------
//Structure 
//-----------------------------------------------
#pragma pack(push, 1)
typedef struct App_struct
{
   //
   float TEC_current;
   float TEC_voltage;
   float Heater_Temperature;
   unsigned char pwm_heater;
   unsigned char pwm_fan;
   unsigned char pwm_enabled;

 } Device_Params;
#pragma pack(pop)
//-----------------------------------------------


//-----------------------------------------------
//Structure 
//-----------------------------------------------
#define RFLW_MODE_STANDBY      0
#define RFLW_MODE_BAKE         1
#define RFLW_MODE_REFLOW       2
#define RFLW_MODE_BAKE_PREHEAT 3
#define RFLW_MODE_UNKNOWN      4

#pragma pack(push, 1)
typedef struct TLog_struct
{  
  //----
  char min;
  char sec; 
  char msec;
  
  //----
  float sensor_heater[2];
  float sensor_pcb[4];
  
  //---- 
  unsigned short set_point;
  float cold_junction_temp;
  float temperature_heater;
  float temperature_pcb;
  
  //----
  unsigned char heat; 
  unsigned char fan; 
  unsigned char mode;
  
} ReFlowData;
#pragma pack(pop)
//-----------------------------------------------


#endif
