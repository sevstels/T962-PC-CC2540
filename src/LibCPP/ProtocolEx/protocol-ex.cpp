//==============================================================================
//File name:   "protocol-ex.cpp"
//Purpose:      Source File
//Version:      2.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cmd.h"
#include "protocol-ex.h"
#include <string.h>

#define SEARCH_HEADER         0
#define READ_DATA_BLOCK       1
#define CHECK_DATA_BLOCK_CRC  2

//---- Mem Leakage Debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//----
 /*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

//---- If need Debug
//#define  PRT_DEBUG

//------------------------------------------------------------------------------
//CUSBProtocol, construction
//------------------------------------------------------------------------------
CProtocolEx::CProtocolEx()
{
  cmd = 0; 
  data_length = 0;
  data_counter = 0;
  header_accepted = 0;
  byte_counter = 0;
  pDataBuf = NULL;
  pDataBlock = NULL;
  pData = (char*)rx_data;
  operation = SEARCH_HEADER;
  memset(rx_data, 0, sizeof(rx_data));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CProtocolEx::~CProtocolEx()
{
  FreeMem();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CProtocolEx::FreeMem(void)
{
 if(pDataBlock!=NULL)
 {
   delete [] pDataBlock; 
   pDataBlock = NULL;
 } 

 data_length = 0;
 data_counter = 0;
 header_accepted = 0;
 operation = SEARCH_HEADER;
}

//------------------------------------------------------------------------------
//CRC calculation, ������ ����������� �����
//------------------------------------------------------------------------------
unsigned char CProtocolEx::CRC(char *pBuffer, unsigned short size)
{
  unsigned char temp = 0; 
  unsigned short i = 0;
  
  //----- Create CRC for data block
  do {temp += *pBuffer++; i++;}
  while (i< size);

  //Return crc
  return ~temp;
}

//------------------------------------------------------------------------------
//Create serial exchange data block
//------------------------------------------------------------------------------
int CProtocolEx::CreateDataBlock(char *pOutBuf,
                                 char *pDataIn,
								 unsigned char cmd,
                                 unsigned short data_length)
{
  unsigned char crc;
  unsigned char header[6];
  //memset(pOutBuf, 0xCC, 4 + data_length);

  //Preparation protocol header  
  header[0] = PROTOCOL_START_BYTE1;//Protocol start byte 1        	
  header[1] = PROTOCOL_START_BYTE2;//Protocol start byte 2
  header[2] = cmd;                           //Command code
  header[3] =(unsigned char)(data_length>>8);//Data length
  header[4] =(unsigned char)data_length;     //Data length
  header[5] = CRC((char*)&header[0],5);      //CRC for control command   	
  //pBuff[6]			                     //First data byte position here
  //......					                 //Data Block Body
  //pBuff[6+data_size]                       //Data Block CRC here
  memcpy(pOutBuf, header, sizeof(header));
  /*
  TRACE1("\n 0x%X ",header[0]);
  TRACE1(" %X ", header[1]);
  TRACE1(" %X ", header[2]);
  TRACE1(" %X ", header[3]);
  TRACE1(" %X ", header[4]);
  TRACE1(" %X ", header[5]);
  */
  if(data_length!=0)
  {
    
    //Reload main Data Block to output buffer
    if(pDataIn != NULL) memcpy(&pOutBuf[6], pDataIn, data_length);

    //Calc CRC for transmitted data 
    crc = CRC(&pOutBuf[6], data_length);
  
    //Put CRC to buffer
    pOutBuf[6+data_length] = crc;
   	
	//data + data_crc 
	data_length++;
  }

  //Header 6 + data + crc 
  return (6+data_length);
} 

//------------------------------------------------------------------------------
//Create serial exchange data block
//------------------------------------------------------------------------------
int CProtocolEx::CalcBlockLength(unsigned short data_length)
{ /*
  int length = 0;
  //---- Preparation protocol header  
  length++;    //0xAA Protocol start byte 1        	
  length++;    //Command code
  length++;    //data_length; //Message data length (0-16) 
  length++;    //CRC for control command   	
  //pBuff[4]   //First data byte position here
  //......	   //Data Block Body
  length++;    //Data Block CRC here
   */

  if(data_length!=0) return (6+data_length+1);
  else return 6;
} 

//------------------------------------------------------------------------------
//Create serial exchange data block
//------------------------------------------------------------------------------
int CProtocolEx::Decoder(unsigned char byte)
{ 
 //====== protocol parser =====
 switch(operation)
 {      
  //-- ����� ������
  case SEARCH_HEADER:
       
  rx_data[0] = rx_data[1]; //Header, Protocol start byte 1 
  rx_data[1] = rx_data[2]; //Header, Protocol start byte 2
  rx_data[2] = rx_data[3]; //Command code
  rx_data[3] = rx_data[4]; //Data length
  rx_data[4] = rx_data[5]; //Data length
  rx_data[5] = byte;        
  
  if(rx_data[0]== PROTOCOL_START_BYTE1 &&//Protocol start byte 1  
     rx_data[1]== PROTOCOL_START_BYTE2)	 //Protocol start byte 2
  {
     //�������� crc	
     //CRC for control command 
     crc = CRC((char*)&rx_data[0], 5);
           
     //���� crc ������
     if(crc == byte)
     { 
       cmd = rx_data[2];
       //----
       block_length = (unsigned char)rx_data[3];
       block_length <<= 8;
       block_length |= (unsigned char)rx_data[4];

       //���� ����� ������ ������ ������ ����
       if(block_length>0)
       {         
         //user info
         data_counter = 0;
         data_length = block_length;
         header_accepted = 1;

         //�������� ������ ��� ���� ������
         //������� ������ � ���������������� ��� ������ ������
         if(pDataBlock!=NULL){delete [] pDataBlock; pDataBlock=NULL;} 
         pDataBlock =  new unsigned char [block_length];
		 if(pDataBlock==NULL) 
		 {
		   //��� ��� �� ���� ���������� ������ ��������� ������ !!!
		   FreeMem();
		   return 0;
		 }
         ///pDataBlock = (char*) Data;
         
         //��������� �������� ������ ����� ������
         operation = READ_DATA_BLOCK;
         pData = (char*)pDataBlock;
         byte_counter = block_length; 
       }
       else 
       {
         //��������� �������� ������ ��������� �������
         operation = SEARCH_HEADER;
         //�������� ������� � �������
         return 1; 
       }
     }
     else{operation = SEARCH_HEADER;}
     break;
          
     //-- ������ �������, ��������� �������� ����� ���� ����� 1
     case READ_DATA_BLOCK:
             
          //�������� ������ � ������� �����
          *pData++ = byte;
          data_counter++;

          //������� ��������� �����
          byte_counter--;

		  #ifdef PRT_DEBUG
		  TRACE1("n: 0x%d\n", byte_counter);
          #endif

          //���� �� ���������
          if(byte_counter==0) 
             operation = CHECK_DATA_BLOCK_CRC;
          break;
        
     //-- ������ ��������� ���� ���������� crc
     case CHECK_DATA_BLOCK_CRC:
             
          //�������� ���� crc � ������� �����
          crc = byte;
          unsigned char blockcrc = CRC((char*)pDataBlock,(unsigned short)block_length);

          #ifdef PRT_DEBUG
          TRACE2("DEC: crc PC: 0x%X DEV: 0x%X\n", blockcrc, crc);
          #endif
          
          //�������� crc ��� ����� ������
          if(crc == blockcrc)
          {	   
            //������� ��� ������
            pDataBuf = (char*)pDataBlock;
            return 1;
          }
          else
          {
            //crc is wrong, ���������� ������
            if(pDataBlock!=NULL)
            { 
		      memset(rx_data, 0, sizeof(rx_data));
			  FreeMem();
			}
          }      
       }
            
    //����� ��������� �����
    FreeMem();
    break;
  }

 return 0;
}
