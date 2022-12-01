//==============================================================================
//File name:   "CircleBuffer.h"
//Purpose:      Header File
//Version:      3.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include <afxmt.h>	  //MFC: CCriticalSection, CEvent

#ifndef _CIRCLE_BUFFER_H_
#define _CIRCLE_BUFFER_H_

//------------------------------------------------------------------------------
// CUSBbuffer class
//------------------------------------------------------------------------------
class CCircleBuffer
{
 //Construction
 public:							//1024 512
  CCircleBuffer(int nLength = 1024, int nExtra = 512); 	
 ~CCircleBuffer();
	
  char *pBuffer;
  char *pBegin;
  unsigned int m_Size;
  unsigned int m_DataLength;

  CEvent ev_bufferNotEmpty;
  CEvent ev_sendBlock;

  //---- Operations
  public:
  
  //clear buffer
  void Clear(void);
  void Empty(void);

  //Write 1 byte
  void WriteByte(unsigned char Byte); 
  //Read  1 byte
  int ReadByte (unsigned char &Byte);
	
  //Write block
  void WriteBlock(char *pTxBuf, int size);
  //Read  block
  int ReadBlock(char *pRxBuf, int max_length);

  //---- Attributes
  private:
  int m_Length, m_Extra;
  int m_Head, m_Tail; 
  CCriticalSection m_Lock;
};
#endif
