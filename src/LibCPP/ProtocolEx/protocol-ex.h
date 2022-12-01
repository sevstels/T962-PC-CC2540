//==============================================================================
//File name:   "protocol-ex.h"
//Purpose:      Header File
//Version:      2.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _CLASS_PROTOCOL_EX_H_
#define _CLASS_PROTOCOL_EX_H_

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus

class CProtocolEx
{
  public:
  
  //Construction
  CProtocolEx();
 ~CProtocolEx();

  unsigned char CRC(char *pBuffer, unsigned short size);
  int CalcBlockLength(unsigned short data_length);
  int CreateDataBlock(char *pOutBuf, char *pDataIn,
	                  unsigned char cmd,
	                  unsigned short data_length);

  int Decoder(unsigned char byte);
  void FreeMem(void);

  //---- user buffer access
  char *pDataBuf;
  int  data_length;
  int  data_counter;
  int  header_accepted;
  char cmd;

  protected:
  //----
  unsigned char  crc;
  unsigned char  mode;
  unsigned char  operation;
  unsigned char  data_CRC;
  unsigned char *pBuffer;
  unsigned char *pBuffer_Limiter;
    
  //Pointer for data buffer array 
  char *pData;
  unsigned char *pDataBlock;
  unsigned char  rx_data[8];
  unsigned int block_length;
  unsigned int byte_counter;
};

#ifdef __cplusplus
}
#endif //end __cplusplus

#endif
