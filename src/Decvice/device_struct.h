//==============================================================================
//File name:   "device_struct.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _DEVICE_STRUCT_H_
#define _DEVICE_STRUCT_H_

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
  float REF;//Ref
  
  //----
  unsigned char period;//ms
} PID_Setup;
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

#pragma pack(push, 1)
typedef struct Setup_struct
{
  //RAM copy of the NV data
  NV_t NV; 
  
  //----
  unsigned short heater_total_power;  
  unsigned short heater_power_limit;
  unsigned short heater_tempr_limit;
  unsigned char fan_speed;
  unsigned char beep_ms;
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

}Device_Setup;
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
