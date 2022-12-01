//==============================================================================
//File name:   "log-file.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:   (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _CLASS_LOG_FILE_H_
#define _CLASS_LOG_FILE_H_

#include <string>

//------------------------------------------------------------------------------
//CDPos class
//------------------------------------------------------------------------------
class CLOGFILE
{
  public:
  //Construction
  CLOGFILE();
 ~CLOGFILE();
 
  //Class Functions
  int Ini(char *pPath, char *pName, char *pExt);
  int NewFile(void);
  int CheckFile(void);
  //int CheckDir(void);
  int ShowFile(void);
  int GetFilesNumber(void);
  int DeleteDirFiles(void);
  int Find_LastNumberOfFile(void);
  int GetLastWriteTime(void);
  int FileRecycle(std::string *pPath);

  //int Write(std::string *pStr);
  int  Write(char *pStr);
  void AddDataStamp(void);
  void TimeStamp(void); 
  int  AddCreateTime(void);

  //path + name + number + extension
  std::string log_file_name;
  std::string last_file_name;
  std::string last_file_time;
  std::string folder_path;
  std::string file_path;
  std::string file_name;
  std::string file_ext;
  std::string data_stamp;
  std::string time_stamp;
  
  int number;
  int files_count;

  protected:
  int Find_NumberFilesInDirectory(std::string &path);

  //----
  int opened;
  int writed;
  CStringArray files;
};

#endif