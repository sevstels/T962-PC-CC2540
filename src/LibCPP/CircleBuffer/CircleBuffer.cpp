//==============================================================================
//File name:    "CircleBuffer.cpp"
//Purpose:      Source File, Circle Memory Buffer class
//Version:      3.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "CircleBuffer.h"

//---- Mem Leakage Debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//----

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------------------ 
//Constructor 
//------------------------------------------------------------------------------
CCircleBuffer::CCircleBuffer(int nLength, int nExtra)
{
  m_Length = nLength;
  m_Extra = nExtra;
  m_DataLength = m_Head = m_Tail = m_Size = 0;
  pBuffer = new char[m_Length];
  pBegin = pBuffer;
  memset((void*)pBuffer, 0, m_Length);
  TRACE1("Circle Buffer open pTr: 0x%X\n", pBuffer);
}

//------------------------------------------------------------------------------
//Destructor 
//------------------------------------------------------------------------------
CCircleBuffer::~CCircleBuffer()
{
  CSingleLock Lock(&m_Lock, TRUE);
  ev_bufferNotEmpty.ResetEvent();
  TRACE1("Circle Buffer close pTr: 0x%X\n", pBuffer);
  delete [] pBuffer;
  pBuffer = NULL;
  m_Lock.Unlock();
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
void CCircleBuffer::Clear(void)
{
  CSingleLock Lock(&m_Lock, TRUE);
  memset((void*)pBuffer,0, m_Length);
  ev_bufferNotEmpty.ResetEvent();

  //set pointer to zero position
  m_DataLength = m_Tail = m_Head = 0;
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
void CCircleBuffer::Empty(void)
{
  CSingleLock Lock(&m_Lock, TRUE);
  ev_sendBlock.ResetEvent();
  ev_bufferNotEmpty.ResetEvent();

  //set pointer to zero position
  m_DataLength = m_Tail = m_Head = 0;
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
int CCircleBuffer::ReadByte(unsigned char &byte)
{
  CSingleLock Lock(&m_Lock, TRUE);
  //TRACE("\nCB Read Byte\n");
  
  //Exit, if data finished
  if(m_Head == m_Tail)
  {
	//ev_bufferNotEmpty.ResetEvent();
	return 0;
  }
   int t = m_Head;
  //������ ����
  byte = pBuffer[m_Head++];
 
  //���� �� ������� - �������� ���������
  if(m_Head == m_Tail){m_Head = 0; m_Tail = 0;}
  
  m_DataLength = m_Tail - m_Head;

  TRACE2("CB Read Byte \t[%2d]\tHead\t%d\n",t, m_Head);
  return 1;
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
void CCircleBuffer::WriteByte(unsigned char byte)
{
  CSingleLock Lock(&m_Lock, TRUE);
  //TRACE("\nCB Write Byte\n");
   
  int t = m_Tail;
  
  //�������� ����� � ������	�� ��������� ������, � ��������� �����
  //�������� ���� ���� 
  pBuffer[m_Tail++] = byte; 
  
  //�������� ��� ����� �� ��������� �� ������ ������
  //If current buffer space is finished, add next space	  m_Head == 
  if(m_Tail == m_Length)
  {
    char *pBuff = new char[m_Length+m_Extra];
	//����������� ������ ������ � ����� ������� ������
	//������������� � ������� ������ �� ������ ����� �� ������ 
	memcpy(pBuff, &pBuffer[m_Head], m_Tail-m_Head);
		
	//��� ������ ������ ������:
	m_Tail = m_Tail-m_Head;
	m_Head = 0;
    m_Length += m_Extra;
    
	delete [] pBuffer;
	pBuffer = pBuff;
	pBuff = NULL;
	TRACE1("CB Write Byte Mem Reallok pTr: 0x%X\n", pBuffer);
  }
  
  m_DataLength = m_Tail - m_Head;

  TRACE2("CB Write Byte \t[%2d]\tTail\t%d\n",t, m_Tail);
  
  //������ ��� ����� �� ������
  ev_bufferNotEmpty.SetEvent();
}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
int CCircleBuffer::ReadBlock(char *pRxBuf, int max_length)
{
  CSingleLock Lock(&m_Lock, TRUE);
  
  TRACE("\nCB ReadBlock: \n");

  int length;

  //Exit, if data finished
  if (m_Head == m_Tail)	return 0;

  //Calc data size for read
  length = m_Tail - m_Head;

  //������������ ������������ ������ ��������
  if(length > max_length) length = max_length; 
  
  //Copy data to rx buffer 
  memcpy(pRxBuf, &pBuffer[m_Head], length);

  //�������������� ���������
  m_Head += length;

  //���� �� ������� - �������� ���������
  if(m_Head >= m_Tail){m_Head = m_Tail = 0;}

  TRACE2("CBuf ReadBlock length: %d Head = Tail: %d\n", length, m_Tail);

  //�������� 	
  //ev_bufferNotEmpty.ResetEvent();

  return length;
}

//------------------------------------------------------------------------------
//Function: Write block
//------------------------------------------------------------------------------
void CCircleBuffer::WriteBlock(char *pTxBuf, int length)
{ 
  //close access to buffer
  CSingleLock Lock(&m_Lock, TRUE);
  
  TRACE1("\nCBuf Write Block, Length %d", length);

  //If current buffer space is finished, add next space
  if(m_Tail+length > m_Length)
   {
	 //��������� ������ ������
     char *pBuff = new char[m_Tail-m_Head];
	 //����������� ������ ������ � ����� ������
	 //������������� � ������� ������ �� ������ ����� �� ������ 
	 memcpy(pBuff, &pBuffer[m_Head], m_Tail-m_Head);

	 //������� ������ �������� �����
	 delete [] pBuffer;
	 pBuffer = NULL;
	 
	 int add_length;

	 //��������� ������
	 if(m_Extra<=length) add_length = length;
	 else add_length = m_Extra;

	 //������� �� ��� ����� ����� 
	 pBuffer = new char[m_Length+add_length];

	 //����������� �� ���������� ������ ������
	 memcpy(pBuffer, pBuff, m_Tail-m_Head);
	 //������� ��������� �����
	 delete [] pBuff;
	 pBuff = NULL;

	 //��� ������ ������ ������:
	 m_Tail = m_Tail-m_Head;
	 m_Head = 0;
     m_Length = m_Length + add_length;

	 TRACE1("\nCBuf Write Block Mem Reallok pTr: 0x%X\n", pBuffer);
  }

  //������ ������ ����� ������ 
  pBegin = &pBuffer[m_Head];
  m_DataLength = (m_Tail-m_Head) + length;
  
  //��������� ����
  memcpy(&pBuffer[m_Tail], pTxBuf, length);
  
  m_Tail += length;

  ev_sendBlock.SetEvent();
}