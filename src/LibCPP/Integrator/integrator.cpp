//==============================================================================
//File name:    "integrator.cpp"
//Processor:    Cortex-M0 (nRF51822x)
//Toolkit:      EWARM (IAR Systems)
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include <string.h>
#include "integrator.h"

//------------------------------------------------------------------------------
//Function: constructor
//------------------------------------------------------------------------------
CINTG::CINTG()
{
  result = 0;
  input_value = 0;

  integrator_divisor = 10; //max
  if(integrator_divisor>10) integrator_divisor = 10;  
  integrator_length = (1<<integrator_divisor);
  ResetZero();

}

//------------------------------------------------------------------------------
//Function: constructor
//------------------------------------------------------------------------------
CINTG::~CINTG()
{
  //if(pChBuf!=NULL) delete [] pChBuf;
  //if(pShBuf!=NULL) delete [] pShBuf;
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CINTG::ResetZero(void)
{
  memset(Buf, 0, sizeof(Buf));
  data_ready = 0;
  calc_counter = 0;
  pBuf = Buf;
  index = 0;
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void Integrator_Reset(void)
{
 // pINT->ResetZero();
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CINTG::AddInputValue(unsigned short value)
{
  input_value = value;
}
//------------------------------------------------------------------------------
//Function: New buffer size, изменение параметров и переинициализаци€
//------------------------------------------------------------------------------
int CINTG::IntDivChange(int div_value)
{  
  if(div_value>10) div_value = 10;
  pBuf = Buf;
  index = 0;

  //инициализировать 
  integrator_divisor = (unsigned char)div_value;
  integrator_length = (1<<integrator_divisor);
  return integrator_length;
}

//------------------------------------------------------------------------------
//Function: «аливка буфера
//------------------------------------------------------------------------------
void CINTG::IniBuffer(unsigned short value)
{  
  data_ready = 0;
  
  //инициализировать буфер заданным значением 
  for(int i=0; i<integrator_length; i++)
   {  
     Buf[i] = value;
   }
}

//------------------------------------------------------------------------------
//Function: New buffer size, изменение параметров и переинициализаци€
//------------------------------------------------------------------------------
void CINTG::Reinit(void)
{
  integrator_divisor = 10;///GLASSES.Integrator_divisor;
  if(integrator_divisor>10) integrator_divisor = 10;  
  integrator_length = (1<<integrator_divisor)-1;
  pBuf = Buf;
  index = 0;

  //проинициализировать буфер новыми значени€ми 
  for(short i=0; i<integrator_length; i++)
   {  
     Buf[i] = result;
   }
}
//------------------------------------------------------------------------------
//Function: Set switch position 
//------------------------------------------------------------------------------
//#pragma optimize=none
char CINTG::Integration(void)
{
  unsigned long long buffer = 0;
     
  //интегрирование
  if(integrator_divisor>0)
  {
    //проверка переполнени€
	if(index > (integrator_length-1))
	{
	  pBuf = Buf;
	  index = 0; 	
	}
	
	//положить входное значение в массив
	*pBuf++ = input_value;
	 index++;

	//сумма всех значений 
    for(int i=0; i<integrator_length; i++)
     {
       //Add all 
       buffer += Buf[i];
     }

    //div all
    buffer >>= integrator_divisor;
  
    result = (unsigned short)buffer;
  
    //данные готовы
    if(calc_counter < integrator_length) calc_counter++;
    else data_ready = 1;
  }
  else 
   {
     result = input_value;
   }
  
  return data_ready;
}