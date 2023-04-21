//==============================================================================
//File name:   "app-setup.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef  _APPLICATION_SETUP_H_
#define  _APPLICATION_SETUP_H_

#define APPL_VER_STR  "T962PC v1.0"

//-----------------------------------------------
//Structure 
//-----------------------------------------------
#pragma pack(push, 1)
typedef struct AppSetup_struct
 {     
  unsigned short struct_length;
  char device_name[64];

  //----
  int graph_max_value_x;
  int graph_max_value_y;
  int graph_line_width;
 
  //---- dsp graphic
  char log[10];
  char line_width[10];
  COLORREF line_color[10];
  COLORREF bgnd_color;
  COLORREF scale_color;
  
  //----
  char dsp_chn_On[6];
  char dsp_line_thnks[6];
  char dsp_img_dimension;
  int  dsp_img_heigth;
  int  dsp_img_width;
  char dsp_filter_LPF;
  char dsp_filter_Bezier;
  char dsp_normalize;
  char app_sync;//for sensors debug

  //---- open for user
  char show_img;
  char show_txt;
  char show_csv;//excel;
  char show_svg;//html
  char show_cpp;//for C++ editor

  //---- save img
  char save_gif;
  char save_png;
  char save_jpg;
  char save_bmp; 
  
  //---- save file
  char save_txt;
  char save_csv;
  char save_svg;
  char save_cpp;
  
  //BT
  char bt_enable;
  char bt_connected;
  char bt_dev_address[8];
  char bt_last_com;  

  //----
  char last_transport;  

} ApplSettings;

#pragma pack(pop)

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus

//------------------------------------------------------------------------------
// CBTColor class
//------------------------------------------------------------------------------
class CApplSetup
{
  //Construction
  public:
  CApplSetup();
 ~CApplSetup();
  
  HANDLE hREG;
  ApplSettings *pSetup;
  
  void FirstIni(void);
  void SetDefault(void);
  void Save(void);
  void Read(void);
};

#ifdef __cplusplus
}
#endif //end __cplusplus

#endif //end file
