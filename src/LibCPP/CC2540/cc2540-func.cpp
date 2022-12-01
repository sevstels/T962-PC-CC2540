//==============================================================================
//File name:   "cc2540.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cc2540.h"
#include "trace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CC2540::AddEventsHandler(void(*pEventsCallback)(int, char*, int))
{
  #ifdef BLE_DEBUG
  TRACE1("%s\n", __FUNCTION__);
  #endif

  //Add Events callback
  pEventsHandler = pEventsCallback;
  //----
  if(pEventsCallback!=NULL) return 1;
  else return -1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CC2540::AddFilter_DeviceName(char *pDeviceName)
{
   dev_name = pDeviceName;
   return 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CC2540::ReverseArray(char *pArray, int len) 
{
  if(len <= 1)
	 return;

  int begin = 0;
  int end = len - 1;
  char buf;
  while (1) 
  {
	if(pArray[begin] != pArray[end]) 
	{
	  buf = pArray[end];
	  pArray[end] = pArray[begin];
	  pArray[begin] = buf;
	}
	if(++begin >= end--) 
	{
	  break;
	}
  }
}

//------------------------------------------------------------------------------
//Function:	Format: XX:XX:XX:XX:XX:XX
//------------------------------------------------------------------------------
void CC2540::BD_AddrToString(char *pAddrArr, std::string *pTxt) 
{
  unsigned char bd_address[6];
  memcpy(bd_address, pAddrArr, 6);

  ReverseArray((char*)bd_address, 6);

  std::string txt;
  char buf[4];

  for(int i=0; i<6; i++)
  {
    memset(buf, 0, sizeof(buf));
    
	if(i<5) _snprintf(buf, sizeof(buf), "%0.2X:", bd_address[i]); 
    else _snprintf(buf, sizeof(buf), "%0.2X", bd_address[i]); 
    txt += buf;      
  }

  *pTxt = txt;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CC2540::BD_StringToAddr(std::string *pTxt, char *pAddrArr)
{
  if(pTxt==NULL) return -1;
  
  std::string txt, dig;
  txt = *pTxt;
 // txt = "C6:04:9C:27:9A:BD";

  int length = txt.length();
  int value;
  unsigned char buf[6];

  for(int i=0; i<6; i++)
  {
    int pos = txt.find(":");
	if(pos!=2){if(i<5) return -1;}
	
	if(i<5) dig = txt.substr(pos-2, 2);
	else dig = txt.substr(0, 2);
	if(i<5) txt.erase(0, 3);

	//read value in any format
	int result = sscanf_s(dig.c_str(), "%X",(int*)&value);
	
	//если число правильно считано
	if(result==1)
    {
	  //сохранить во временном буфере 
	  buf[i] = (unsigned char) value;
	}
	else return -1;
  }
  
  ReverseArray((char*)buf, 6); 

  if(pAddrArr!=NULL) memcpy(pAddrArr, buf, 6);
  return 1;
}
