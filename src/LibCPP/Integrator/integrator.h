//==============================================================================
//File name:    "integrator.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus
  
//void Integrator_change(unsigned char div);
//void Integrator_Reset(void);

//---------------------------------------------
//Class:
//---------------------------------------------
class CINTG
{
  public:
  
  CINTG();
 ~CINTG();
 
  unsigned char  integrator_divisor;
  unsigned short integrator_length;
  unsigned short result;
  
  //Function definitions:
  void ResetZero(void);
  void Reinit(void);
  int  IntDivChange(int div_value);
  void IniBuffer(unsigned short value);
  void AddInputValue(unsigned short value);
  char Integration(void);
  char data_ready; 
 
  protected: 
  unsigned short Buf[1024]; //256
  unsigned short input_value;
  unsigned short calc_counter;
  unsigned short *pBuf;
  int index;
};

#ifdef __cplusplus
}
#endif //end __cplusplus

#endif //end file
