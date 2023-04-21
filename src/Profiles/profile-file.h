//==============================================================================
//File name:   "profile-file.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "afxcmn.h"
#include "afxwin.h"
#include "arrays.h"

#ifndef _PCR_FILES_H_
#define _PCR_FILES_H_

unsigned short GetSampleSetup(float code);
float SetSampleSetup(unsigned short leds);

//------------------------------------------------------------------------------
// CLASS
//------------------------------------------------------------------------------
class CTPRF
{ 
  //Construction
  public:
  CTPRF();
 ~CTPRF();

  ///CTMPRT *pTMPR;
  std::vector<PointFLT> *pProfile;
  std::vector<PointFLT> *pNewProfile;
  CByteArray *pIndex_cycles;
  CString FileName;

  //----
  void Ini(std::vector<PointFLT> *pPCRprofile);

  //----
  //to *.csv file
  int ToCSV(CString FName);  
  //from *.csv file
  int FromCSV(CString FName);
  int FromArray(char *pData, int length);
  //user template
  int FileDefault(void);  
  int ShowFile(CString FName);


  //Controller PCR file
  int CreateControllerFile(void);
  unsigned short shCRC(char *pBuffer, int length);

  //----
  int FindKeyWord(CString word, int from, int to, int &row, int &value);
  int FindEndFile(int from, int &row);
  int FindEmptyCell(int from, int &row); 
  int StringToDigit(CString string, float &out);
  
  int SampleParserRGBY(CString string, short &leds);
  int ParserErrors(int error_code); 
  int ParserRGBY(CString codestring, short &leds);
  int CheckComment(CString string);
  int CheckParam(CString string, CString keyword, int &value);
  int CheckCycles(int &value);
  int CheckEmpty(CString string);
  int AddCycles(int row, int count);
  int AddPoint(CString *pString_x, CString *pString_y);

  int row_begin;
  int row_next;
  int row_end;
  int row_count;

  CByteArray cycle_string, cycle_count; 
  int cycle_index;

  //----
  int profile_length;
  int profile_time;
  int file_saved;
  int file_opened;
  
  //----
  int controller_file_saved;
  int controller_file_length;
  unsigned short *pDevFileBuf;

  protected:
  unsigned short *pFileBuf;
};

#endif