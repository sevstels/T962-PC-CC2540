//==============================================================================
//File name:   "display.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "afxcmn.h"
#include "afxwin.h"
#include "arrays.h"

#define  SELECT_HORIZLINE  1
#define  SELECT_RAMP       2
#define  SELECT_DRAGPOINT  3
#define  SELECT_SENSOR     4
#define  SB_RESET		   200

#define  SCALE_X_CELLS_NUM 8

#ifndef _T_DISPLAY_H_
#define _T_DISPLAY_H_

//------------------------------------------------------------------------------
// CLASS
//------------------------------------------------------------------------------
class CDSPL
{ 
  //Construction
  public:
  CDSPL();
 ~CDSPL();
  
  //----
  int object_selected;
  int object_multiselect;
  int object_type;
  int object_index;
  int scroll_index;
  
  //Profile data points array, graph curves
  std::vector<PointFLT> profile;
  //Profile cycles arrov array
  CByteArray index_cycles;

  //----
  CDWordArray index_selected;
  CEdit *pMonitor;
  CScrollBar *pScroll;

  CEvent ev_Redraw;
  CEvent ev_ScrollLeft;
  CEvent ev_ScrollRight;

  //----
  void DisplayIni(HANDLE hHWnd, int offset_x, int offset_y, int height, int width);
  void DisplayShow(void);
  void ScaleOffset(int x0, int y0);
  void ScaleRange(int x0, int x, int cells_x, int y0, int y, int cells_y);
  void ScaleRangeX(int x0, int x, int cells_x);
  void ScaleSize(int x, int y);
  void GraphIni(int offset_x, int offset_y, int width, int height);
  void Draw_BackGround(void);
  //----
  void Draw_Line(CDC *pDC, char width, int x0, int y0, 
                 int x, int y, COLORREF color);
  //----
  void Draw_Frame(CDC *pDC, char width, int x0, int y0, 
                  int x, int y, COLORREF color);
  //----
  void Draw_StringAt(CDC *pDC, int x, int y, CString txt, int height, 
	                 int angle, COLORREF txt_color, COLORREF bgd_color);
  //----
  void Draw_ScaleX(CDC *pCDC, int x0, int y0, int width, int height,
                              int line_number, COLORREF color);
  //----
  void Draw_ScaleY(CDC *pCDC, int x0, int y0, int width, int height, 
                   int line_number, COLORREF color);
  
  void Draw_Box(CDC *pCDC, int x0, int y0, int dx, int dy, COLORREF color);
  void Draw_MaxPoint(CDC *pDC, float *pBuffer, int size);
  void Draw_CycleArrow(CDC *pCDC);

  //----
  void Draw_Profile(void);
  void Draw_Scales(void);
  void Draw_FocusRect(void);
  void Draw_DragPoints(int index);
  void Draw_SensorSQR(int x0, COLORREF color);
  void Draw_Ramp(int index);  
  void Draw_Duration(int index);
  void Draw_Temperature(int index);
  //----
  void Load_Profile(int profile_number);
  void Save_Profile(const char *pFileName);
  void Select_Focused(int x, int y);
  void Add_Focused(void);
  void Scroll_Setup(void);
  void Scroll_Handler(UINT nSBCode, UINT nPos);
  void Scroll_Left(void);
  void Scroll_Right(void);
  void ShowMenu(CMenu *pMenu);
  float Convert_ToScaleY(int y);
  void  Convert_ToDisplay_XY(int &y, int &x, float fl_y);
  int   Convert_ToDisplay_Y(float fl_y);
  int   round(double value);

  //----
  void Profile_DefaultIni(void);

  //----
  float CalcLeftX(float x0, float x1, float y0, float y1, float fl_delta);
  float CalcRightX(float x2, float x3, float y2, float y3, float fl_delta);
  void  Calc_LinePoints(int x1, int fl_y1, int x2, int fl_y2);
  void  Calc_LinePointsT(int x1, float fl_y1, int x2, float fl_y2);
  int   Object_Detector(int x, int y);
  float RecalcMousePosition(int ms_x, int ms_y, int &x, int &y);
  
  //---- event handlers
  void MouseWheel(UINT nFlags, short zDelta, CPoint pt);
  void MouseMove(UINT nFlags, CPoint point);
  void RButtonDown(UINT nFlags, CPoint point);
  void RButtonUp(UINT nFlags, CPoint point);
  void LButtonDown(UINT nFlags, CPoint point);
  void LButtonUp(UINT nFlags, CPoint point);

  //----
  void ChangeTemperature(float fl_delta, int mode);
  void ChangeDuration(int delta);
  void ChangeRamp(int delta);
  void DragTemperature(int x, int y);
  void DragDuration(int x, int y);
  void DragRamp(int x, int y);
  void Hold_Ramp(void);

  //----
  void AddVertex(void);
  void DelVertex(void);
  void DelSegment(void);
  void AddSensor(int x, int channel);

  //Device Contexts for paint 
  CDC BGND;//Only for Bgnd
  CDC SCL; //
  CDC IMG; 
  
  //Message box for user info
  void MsgBox(const char *pTxt, RECT rect, COLORREF color, int shw_time);
  
  short  ScreenBuf[8048];
  float  ProfileBuf[8048];
  int    ProfileLength;

  protected: 
  //----
  HWND hWnd;
  HDC  hDC;
  CDC *pDC;
  
  float *pPrBuf;

  //----
  int scroll_position;
  int scroll_step;

  //Bitmaps for pictures
  CBitmap bmp_BkGnd;
  CBitmap bmp_Scale;
  CBitmap bmp_Img;
  CRect rect_selection;

  //----
  CFile file_monitor;
  int map_mode;
  int opened_bgnd;
  int opened_img;
  int pcr_profile;
  int drag_mode;
  int mouse_x;
  int mouse_y;
  int mouse_Lclick_x;
  int mouse_Lclick_y;
  int mouse_Rclick_x;
  int mouse_Rclick_y;
  int object_index_old;

  //Dimensions
  int get_profile_point(int index, int &x, float &y);

  //---- graph
  int graph_offset_x, graph_offset_y;
  int graph_width, graph_height;
  int graph_x0, graph_y0;
  double graph_factor_x, graph_factor_y;

  //---- display 
  int display_offset_x, display_offset_y;
  int display_width, display_height;

  //---- scale
  int scale_offset_x, scale_offset_y;
  int scale_width_x, scale_height_y;
  int scale_range_x0, scale_range_x;
  int scale_range_y0, scale_range_y;
  int scale_cell_x, scale_cell_y;
};

#endif
