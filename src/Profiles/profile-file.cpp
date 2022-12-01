//==============================================================================
//File name:    "profile-file.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cmd.h"
#include "profile-file.h"
#include "files-cpp.h"
#include "device_struct.h"
#include <math.h>


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

#define PARSERGBY_ERROR_TOO_SHORT 			-1
#define PARSERGBY_ERROR_TOO_MANY_TETRADES 	-2
#define PARSERGBY_ERROR_TOO_LONG 			-3
#define PARSERGBY_ERROR_INVALID_Y 			-4
#define PARSERGBY_ERROR_INVALID_SYMBOL      -5

//------------------------------------------------------------------------------
//CPage1 constructor
//------------------------------------------------------------------------------
CTPRF::CTPRF()
{ 

  pProfile = NULL;
  pDevFileBuf = NULL;
  pFileBuf = NULL; 
  FileName.Empty();
  profile_length = 0;
  controller_file_saved = 0;
  controller_file_length = 0;
}

//------------------------------------------------------------------------------
//destructor
//------------------------------------------------------------------------------
CTPRF::~CTPRF()
{
  if(pFileBuf!=NULL){delete [] pFileBuf; pFileBuf = NULL;}
  pProfile = NULL;
  FileName.Empty();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CTPRF::Ini(std::vector<Point2D> *pPCRprofile)
{
  pProfile = pPCRprofile;
}

//------------------------------------------------------------------------------
//To *.csv file
//------------------------------------------------------------------------------
int CTPRF::ToCSV(CString FName)
{
  profile_length = pProfile->size();
  if(profile_length<1) return -1;
  CString txt, dig;
  txt.Empty();

  for(int i=0; i<profile_length; i++)
  {
    int time = (int)pProfile->at(i).x;
    float temperature = (float)pProfile->at(i).y;
       
	//----    
	dig.Format("%d;%d \r\n", time, (int)temperature);	
	txt += dig;
  }

  //file save
  int ret = Save_File(FName, (char*)txt.GetBuffer(), 0,txt.GetLength());

  return ret;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::CheckEmpty(CString string)
{
  if(string.GetLength()<1) return 1;
  return 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::CheckComment(CString string)
{
  int result;
  result = string.Find("#");
  if(result>=0) return 1;
  //----
  result = string.Find(";");
  if(result>=0) return 1;
  //----
  result = string.Find("/");
  if(result>=0) return 1;
  //----
  result = string.Find("*");
  if(result>=0) return 1;

  return 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::CheckParam(CString string, CString keyword, int &value)
{   
  string.Delete(0, keyword.GetLength());

  //if no params
  if(string.GetLength()<1) return -1; 

  //read value in any format
 /// value = (int) StringToDigit(string, -1);

  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::FindKeyWord(CString word, int from, int to, int &row, int &value)
{
  int result, comment;
  CString string;
 /*
  //---- Find keyword: 
  for(int i=from; i<to; i++)
  {
    BSTR txt = XLS.pCells->Cell(i, 1)->GetValueAsString();
    string = txt;
    //----
    comment = CheckComment(string);
    if(comment==0)
    {
      result = string.Find(word);
      if(result>=0)
      {
        row = i;
        result = CheckParam(string, word, value);
        return 1;
      }
    }
  }
  */
  //не найдено
  return -1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::FindEmptyCell(int from, int &row) 
{ /*
  //---- Find keyword: 
  for(int i=from; i<row_count; i++)
  {
    //---- read: 
    BSTR txt = XLS.pCells->Cell(i, 1)->GetValueAsString();
    CString string(txt);
    //проверка
    int result = CheckEmpty(string);
    if(result==1){row = i; return 1;}
  }
 */
  //not found
  return -1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::CheckCycles(int &value)
{
  int result, comment;
  CString string;

  //---- Find keyword: 
 // result = CheckParam(string, word, value);

  //не найдено
  return -1;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::FindEndFile(int from, int &row)
{
  int result, count=0;
  CString string;
  /*
  //---- Find keyword: 
  for(int i=from; i<1024; i++)
  {
    BSTR txt = XLS.pCells->Cell(i, 1)->GetValueAsString();
    string = txt;
    
    //----
    if(string.IsEmpty())
    {
      count++;
      if(count>3)
       { 
         row = i-3;
         return 1;
       }
    }
    else count = 0;
  }
  */
  //не найдено
  return -1;
}

//------------------------------------------------------------------------------
//Ищем ключевую букву и их комбинации
//------------------------------------------------------------------------------ 
/*
следует реализовать разбор текстовой строки для управления включением сенсоров
для красного R, зеленого G,  синего B,  желтого Y

Порядок следования битов управления в тетраде, 4бита управляют 1м семплом:
0b0000
  ybgr

Например аргумент в строке:  "RG Y+G+B Y"
RG - включить последовательно сенсор R, затем G, упаковать в Семплы 0,1 (S0, S1)  
S0: 0b0001
S1: 0b0010

Следующая комбинация через пробел, с знаком +, значит совместно: Y+G+B
S2: 0b1110

Следующая комбинация через пробел: Y
S3: 0b1000  - для этого случая выдать warning

Упаковка итога в 16 битную short переменную, всего 4 семпла:
0b 1000 0000 0000 0000
    S3   S2   S1   S0 

Если в строке более 4х токенов - выводить warning об усечении до 4х
Если в строке найдены не совпадающие буквы - выводить warning с номером строки и символами
Старший бит в short - не занимать, это флаг для устройства
Если в S3 используется позиция (y) - выводить уведомление о усечении до RGВ
*/
int CTPRF::SampleParserRGBY(CString string, short &leds)
{
  int code = 0;
  int pos_R, pos_G, pos_B, pos_Y;
  
  //сбросить приемный регистр
  leds = 0x8000;

  //setlocale(LC_CTYPE, "Russain"); 
  //символы перевести в верхний регистр
  string.MakeUpper();

  //---- Find keyword: 
  pos_R = string.Find("R");
  //----
  pos_G = string.Find("G");
  //----
  pos_B = string.Find("B"); 
  //----
  pos_Y = string.Find("Y");

  //проверяем что нашлось
  if(pos_R>=0){code |= 0x01;}  
  if(pos_G>=0){code |= 0x02;} 
  if(pos_B>=0){code |= 0x04;} 
  if(pos_Y>=0){code |= 0x08;} 
  
  //отправить на выход 
  if(code!=0)
  {
    leds = code;
    
    leds |= 0x8000;
  }
  
  //не найдено
  if(code==0) return -1;  
  //найдено
  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CTPRF::ParserErrors(int error_code) 
{
  CString msg;
    
  switch(error_code) 
  {
    case PARSERGBY_ERROR_TOO_SHORT:
         msg = "ERROR: String too short";
         break;
        
	case PARSERGBY_ERROR_TOO_MANY_TETRADES:
         msg = "ERROR: Too many sections(?)";
         break;
        
	case PARSERGBY_ERROR_TOO_LONG:
         msg = "ERROR: String too long";
         break;
        
	case PARSERGBY_ERROR_INVALID_Y:
         msg = "ERROR: Y not allowed in last section";
         break;
        
	case PARSERGBY_ERROR_INVALID_SYMBOL:
         msg = "ERROR: Unrecognized code";
         break;
        
	default:
         msg = "Unknown error";
         break;
   }
    
  AfxMessageBox(msg);
  return error_code;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::ParserRGBY(CString codestring, short &leds)
{
  char plain_string[64] = {0};
  size_t string_len;
  int tetrade_n = 0;
  int i = 0;
  int return_code = 0;
  bool plus_set = true;
  char *tmp = codestring.GetBuffer();
 
  // Max long string is "R+G+B+Y R+G+B+Y R+G+B+Y R+G+B"
  if(codestring.GetLength() > 29)
     return_code = ParserErrors(PARSERGBY_ERROR_TOO_LONG);
 
  strncpy(plain_string, tmp, codestring.GetLength());
  string_len = strlen(plain_string);
 
  if(string_len < 4)
     return_code = ParserErrors(PARSERGBY_ERROR_TOO_SHORT);
 
  leds |= 0x8000;
 
  //Lowercase all string
  for(i = 0; i < string_len; i++)
   {plain_string[i] = tolower(plain_string[i]);}
 
  i=0;
  do 
  {
    //If this is going to be 5th tetrade, emit error and quit
    if(tetrade_n == 3 && plus_set == false)
	{
      ParserErrors(PARSERGBY_ERROR_TOO_MANY_TETRADES);
      return_code = PARSERGBY_ERROR_TOO_MANY_TETRADES;
      break;
    }
 
    switch (plain_string[i])
	{
      //----      
	  case 'r':
      if(plus_set == false)
         tetrade_n++;
      leds |= 1 << (tetrade_n * 4 + 0);
      plus_set = false;
      break;

      //----         
	  case 'g':
      if(plus_set == false)
         tetrade_n++;
      leds |= 1 << (tetrade_n * 4 + 1);
      plus_set = false;
      break;
      
      //---- 
	  case 'b':
      if(plus_set == false)
         tetrade_n++;
      leds |= 1 << (tetrade_n * 4 + 2);
      plus_set = false;
      break;
      
	  //----         
	  case 'y':
      if(plus_set == false)
         tetrade_n++;
      if(tetrade_n == 3) 
	  {
        return_code = ParserErrors(PARSERGBY_ERROR_INVALID_Y);
      } 
	  else 
	  {
        leds |= 1 << (tetrade_n * 4 + 3);
        plus_set = false;              
      }
      break;
      
	  //----         
	  case '+':
      plus_set = true;
      break;
      
	  //----         
	  case ' ':
      plus_set = false;
      break;

      //----  
	  default:
      return_code = ParserErrors(PARSERGBY_ERROR_INVALID_SYMBOL);
      break;
  }
 
 } while (++i < string_len);
 
 return return_code;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::StringToDigit(CString string, float &out_value)
{
  double value;

  //read value in any format
  int result = sscanf_s(string, "%Lf",(double*)&value);
  
  if(result<1)
  {
	//::AfxMessageBox("This is not digit!\r\nEnter a number...");
	return -1;
  }

  out_value = (float)value;
  return 1;
} 


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::AddPoint(CString *pString_x, CString *pString_y)
{
  Point2D p;
  
  if(CheckEmpty(*pString_x)) return 0;
  if(CheckEmpty(*pString_y)) return 0;
  float x, y;
  int ret;

  //---- data 
  ret = StringToDigit(*pString_x, x);
  if(ret<0) return -1;
  
  ret = StringToDigit(*pString_y, y);
  if(ret<0) return -1;


         
  //Add profile point, calc time 
  ///p.x = profile_time + x;
  
  p.x = x; 
  p.y = y;
    
  profile_time = p.x;
  cycle_index++;

  pNewProfile->push_back(p);

  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CTPRF::AddCycles(int row, int count)
{
  int result, digit=0, length, cnt=0, row_empty;
  int cycle_points = 0;
  Point2D p;
  CString str_x, str_y;
  length = row+count+1;
  
  //---- Find empty
  result = FindEmptyCell(row+1, row_empty); 

  if(result!=1) return -1;

  //add all cycles 
  for(int n=0; n<count; n++)
  {
  pIndex_cycles->Add(cycle_index);
  cycle_points = 0;
  //add 1 cycle: 
  for(int i=row+1; i<row_empty; i++)
  {
    BSTR txt;/// = XLS.pCells->Cell(i, 1)->GetValueAsString();
    str_x = txt;
    
    //----    
    if(CheckComment(str_x)!=1)
    {
      //Check Digit
      if(digit==0)
       { 
         ///txt = XLS.pCells->Cell(i, 2)->GetValueAsString();
         str_y = txt;
         
         //data 
         int x = 1;//// (int) StringToDigit(str_x, i);
         //Ошибка формата в файле - закончить парсинг
         if(x<0) 
            return -1;
         float y = 1;/// (float) StringToDigit(str_y, i);
         //Ошибка формата в файле - закончить парсинг
         if(y<0) 
            return -1;

         //If this first point in the profile
         if(profile_time==0)
         {
           //Add additional zero point
           p.x = 0; p.y = y;
           pNewProfile->push_back(p);
         }

         //Add profile point, calc time 
         p.x = profile_time + x; p.y = y;
         pNewProfile->push_back(p);
         profile_time = p.x;
         cycle_points++;
       } 
     }
   }

   //Add cycle count for draw 
   cycle_index += cycle_points;
   pIndex_cycles->Add(cycle_index);
  }

  row_next = row + cycle_points+2;
  
  //найдено
  return 1;
}

//------------------------------------------------------------------------------
//From *csv file Decoder
//Извлекаем профиль из Ексель файла
//------------------------------------------------------------------------------
int CTPRF::FromCSV(CString FName)
{
  std::vector<Point2D> new_profile;
  new_profile.empty();
  pNewProfile =	&new_profile;
  
  unsigned long length = Get_FileSize(FName);
  if(length<=0 || length>1000) return -1;

  char *pBuf = new char[length];
  //char pBuf[1000];
  memset(pBuf, 0, length);

  //----
  //int result = XLS.OpenFile(&FName);
  int result = Read_File(FName, pBuf, 0, length);
  if(result<=0){ delete [] pBuf; pBuf=NULL; return -1;}

  FileName = FName;

  //---- read data
  int row_number, value, cycle_count;
  CString str(pBuf), digit, dig_x, dig_y;
  profile_time = 0;
  cycle_index = -1;
  int position = 0, sep_pos, end_pos;

  //
  for(;;)
  {
	 int pos_separator = str.Find("\n", position);
	 if(pos_separator>1)
	 {
	   digit = str.Left(pos_separator);
	   
	   //поиск разделителя
	   sep_pos = digit.Find(";", 0);
	   end_pos = digit.Find(' ', sep_pos);

	   if(sep_pos>=1 && end_pos>2)
	   {
	     dig_x = digit.Left(sep_pos);
		 digit.Delete(0, sep_pos+1);
		 digit.Delete(end_pos, digit.GetLength());						 
		 dig_y = digit;	 
		 //TRACE2("%s  %s\n", dig_x, dig_y);

		 //---- Add point
         result = AddPoint(&dig_x, &dig_y);
		 //обнаружена ошибка в файле
         if(result==-1)
		  {delete [] pBuf; pBuf=NULL; return -4;}
	   }

	   //отрезать левую часть строки
	   str.Delete(0, pos_separator+1);
	 }

	 if(pos_separator<0)
	 {
	   break;	 
	 }

	 if(position >= str.GetLength())
	 {
	   break;
	 }
  }
  
  delete [] pBuf; pBuf=NULL;

  //error
  if(new_profile.size()<1) return -4;

  //---------------------------------------
  //copy to display array
  //---------------------------------------
  pProfile->clear();
  for(int i=0; i<new_profile.size(); i++)
  {
    pProfile->push_back(new_profile.at(i));
  }
    
  return 1;
}

//------------------------------------------------------------------------------
//From *csv file Decoder
//Извлекаем профиль из Ексель файла
//------------------------------------------------------------------------------
int CTPRF::FromArray(char *pData, int length)
{   
  Point2D p;
  std::vector<Point2D> new_profile;
  new_profile.empty();
  pNewProfile =	&new_profile;
    
  profile_time = 0;
  cycle_index = -1;
    
  if(length>(NUMPROFILETEMPS<<1)) return -1;

  //temperature_profile tprofile;
  unsigned short buf[NUMPROFILETEMPS];
  memset(buf, 0, NUMPROFILETEMPS<<1);
  memcpy((char*)buf, pData, length); 

  for(int i=0; i<(length>>1); i++)
  {
    //---- Add point
	p.x = i*10; 
    p.y = buf[i];
    
    profile_time = p.x;
    cycle_index++;

    pNewProfile->push_back(p);
  }

  //---------------------------------------
  //copy to display array
  //---------------------------------------
  pProfile->clear();
  for(int i=0; i<new_profile.size(); i++)
  {
    pProfile->push_back(new_profile.at(i));
  }

  return 1;
}

//------------------------------------------------------------------------------
//To *.xls file	Encoder
//------------------------------------------------------------------------------
int CTPRF::FileDefault(void)
{ /*
  profile_length = pProfile->size();
  if(profile_length<1) return -1;
  CString txt;

  //---- Add File header
  XLS.Write_HeaderPCR();
  XLS.Merge_Cells(1, 1, XLS.Setup.xlc_length);
  XLS.Merge_Cells(3, 1, XLS.Setup.xlc_length);
  //---- comment
  txt = "Initial Denaturation";
  XLS.Write_Txt(3, 1, &txt);
  XLS.AlignCenter();

  for(int i=0; i<profile_length; i++)
  {
     if(i==3)
     {
       //---- comment
       XLS.Merge_Cells(4+i, 1, XLS.Setup.xlc_length);
       txt.Format("%d Points", profile_length-i);
       XLS.Write_Txt(4+i, 1, &txt);
       XLS.AlignCenter();
     }
     else
     {
       int time = pProfile->at(i).x;
       float temperature = pProfile->at(i).y;
     
       //----
       XLS.Write_Time(4+i, 1, time);
       XLS.Write_Temperature(4+i, 2, temperature);
     }
  }

  
  file_saved = XLS.Save_File(&FName);
*/
  return 1;//file_saved;
}

//------------------------------------------------------------------------------
// crc
//------------------------------------------------------------------------------
unsigned short CTPRF::shCRC(char *pBuf, int length)
{
  unsigned short temp = 0;

  //---- Calc crc
  do {
       //Get unsigned char variable from mem and load to buffer
       unsigned char byte = *pBuf++;
       temp += byte;
       length--;
     }
  while(length!=0);

  //return crc
  return ~temp;
}

//-----------------------------------------------------------------------------
//Data write
//-----------------------------------------------------------------------------
int CTPRF::ShowFile(CString FName)
{
  int name_length = FName.GetLength();

  HINSTANCE Status = 0;
  CString file_path, msg;
  
  //если пробелы в пути - добавить двойные кавычки
  file_path = "\"";
  file_path += FName;
  file_path += "\"";

  //Open Excel
  Status = ShellExecute(NULL, "open", "excel.exe", file_path, 
                        NULL, SW_SHOWNORMAL); 

  //Excel Not found 
  if(((int)Status)<=32)
  {
    //Open Excel
    Status = ShellExecute(NULL, "open", "notepad.exe", file_path, 
                          NULL, SW_SHOWNORMAL);
  }

  //successful
  if((int)Status > 32) return 1;

  //error case
  switch((int)Status)
  {
    case ERROR_NOT_READY:
    msg = "The specified file: ../";
    msg += file_path;
    msg += " was not ready";
    break;
        
    case ERROR_FILE_NOT_FOUND:
    msg = "The specified file: ../";
    msg += file_path;
    msg += " was not found";
    break;

    case ERROR_PATH_NOT_FOUND:
    msg = "The specified path: ../";
    msg += file_path;
    msg += " was not found";
    break;
        
    default:
    msg = "This file type is not supported.";
  }

  //show error message
  AfxMessageBox(msg);

  return -1;
}