//==============================================================================
//File name:    "log-file.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "log-file.h"
#include "files-cpp.h"
#include <shellapi.h>

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
//Class	Constructor
//------------------------------------------------------------------------------
CLOGFILE::CLOGFILE()
{
  opened = 0;
  number = 0;
  writed = 0;
  files_count = 0;

  //----
  files.RemoveAll();
  folder_path.empty();
  file_path.empty();
  file_name.empty();
  file_ext.empty();
  log_file_name.empty();
  last_file_time.empty();
}

//------------------------------------------------------------------------------
//Class	Destructor
//------------------------------------------------------------------------------ 
CLOGFILE::~CLOGFILE(){}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
int CLOGFILE::Ini(char *pPath, char *pName, char *pExt)
{   
  file_path = pPath;
  int length = file_path.length();
  if(length!=0) file_path += "\\";

  //проверить наличие директории
  CString dir_path;
  TCHAR buffer[MAX_PATH];
  GetCurrentDirectory(sizeof(buffer),buffer);
  dir_path.Format("%s\\%s", buffer, pPath);
 
  int ret = PathFileExistsA(dir_path);

  if(ret!=1)
  {
     //директори€ нет - создать
     int ret = CreateDirectoryA(dir_path, NULL);
  }

  folder_path = dir_path;
  file_name = pName;
  file_ext = pExt;
  
  return ret;
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CLOGFILE::AddDataStamp(void)
{ 
  SYSTEMTIME wtime;
  GetLocalTime(&wtime);
  CString txt;

  //---- create stamp
  txt.Format("%d ",wtime.wDay); 
  data_stamp = txt;

  switch(wtime.wMonth)
  {
    case 1:  txt = "January";  break;
    case 2:  txt = "February"; break;    
    case 3:  txt = "March";    break;  
    case 4:  txt = "April";    break;  
    case 5:  txt = "May";      break;
    case 6:  txt = "June";     break;
    case 7:  txt = "July";     break;
    case 8:  txt = "August";   break;
    case 9:  txt = "September";break;
    case 10: txt = "October";  break;
    case 11: txt = "November"; break;
    case 12: txt = "December"; break;
  }
  data_stamp += txt + " ";

  //----
  txt.Format("%04d ",wtime.wYear); 
  data_stamp += txt;
  
  //----  
  txt.Format("%02d:",wtime.wHour);
  data_stamp += txt;
  
  //----
  txt.Format("%02d:",wtime.wMinute);
  data_stamp += txt;
  
  //----  
  txt.Format("%02d",wtime.wSecond);
  data_stamp += txt;
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CLOGFILE::TimeStamp(void)
{ 
  SYSTEMTIME wtime;
  GetLocalTime(&wtime);
  CString txt;

  //---- create stamp
  txt.Format("%02d:",wtime.wHour);
  time_stamp = txt;
  
  //----
  txt.Format("%02d:",wtime.wMinute);
  time_stamp += txt;
  
  //----  
  txt.Format("%02d",wtime.wSecond);
  time_stamp += txt;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CLOGFILE::Find_NumberFilesInDirectory(std::string &path)
{
  int length, counter = 0;
  WIN32_FIND_DATA FIND_DATA;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string find;

  length = path.length();
  if(length<1) {find.empty();} 
  else {find = path + "\\";}

  //LONG ret = GetCurrentDirectoryW(path.length(), (LPSTR)path.c_str());

  //find += "\\*.*";
  find += file_name + "*." + file_ext;

  //Start iterating over the files in the path directory
  hFind = ::FindFirstFileA(find.c_str(), &FIND_DATA);
  //hFind = ::FindFirstFileW(L"\\pictures\\", &ffd);

  if(hFind==INVALID_HANDLE_VALUE)
  {
     TRACE1("Failed to find: %s", path.c_str());
	 return -1;
  }
  
  //Save first file name
  files.RemoveAll();
  files.Add(FIND_DATA.cFileName);

  //Managed to locate and create an handle to that folder
  for(;;) 
  { 
    counter++;
	//find next file
	BOOL result = ::FindNextFileA(hFind, &FIND_DATA);
	if(result==FALSE) break;
    
	//Save next file name
    files.Add(FIND_DATA.cFileName);
  }

  //----  
  ::FindClose(hFind);

  return counter;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
int CLOGFILE::Find_LastNumberOfFile(void)
{
  int value, length, temp = 0;

  //если файлов нет, следующий будет 0
  length = files.GetCount();
  if(length<1) return -1; 
  
  //Managed to locate and create an handle to that folder
  for(int i=0; i<length; i++) 
  { 
	//Get file name
	std::string str_name = files.GetAt(i);
	last_file_name = str_name;

	//delete file extension
	int pos = str_name.find(file_ext);	
	str_name.erase(pos-1, file_ext.length()+1);

	//delete file name
	pos = str_name.find(file_name);	
	str_name.erase(pos, file_name.length());

	//read value in any format
	int result = sscanf_s(str_name.c_str(), "%d",(int*)&value);
	
	//если число правильно считано
	if(result>=1)
    {
	  //сохранить во временном буфере 
	  //если число больше предидущего
	  if(value>temp)
	  { 
		temp = value;
		last_file_name = files.GetAt(i);
	  }
	}
  }
  
  return temp;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CLOGFILE::NewFile(void)
{
  CFile file;
  CString info;
  CFileException fileException;
  CFileStatus status;
  std::string txt, new_name;
  
  int count = 0;
  BOOL b_OK;
    
  //провер€ем число существующих лог файлов
  txt = folder_path;
  count = Find_NumberFilesInDirectory(txt);
  if(count>0)
  {
    //ищем наибольший номер лог файла
    number = Find_LastNumberOfFile();
  }
  else number = -1;

  //установить номер текущего файла
  if(number<0) number = 0;
  else number++;

  //========================================================
  //если используетс€ дополнительна€ директори€, проверить еЄ наличие
  int dir = CheckFolder(folder_path.c_str());
  if(dir<0)
  { 
	//dir = CreateFolder(folder_path.c_str());
	dir = CreateDirectory(folder_path.c_str(), NULL);
	if(dir<0) return -1;
  }
  
  //========================================================  
  //собрать полное им€ нового файла 
  new_name = file_path + file_name;
  new_name += std::to_string((long long)number);
  new_name += '.' + file_ext;
     
  //cоздаЄм файл и открываем его
  b_OK = file.Open(new_name.c_str(), CFile::typeBinary|
                                     CFile::modeCreate| 
                                     CFile::modeWrite);
                                     //&fileException);
  //error create file
  if(b_OK!=1) {opened = 0; return 0;}

  log_file_name = new_name;

  //Set first position 
  file.Seek(0, CFile::begin);
   
  //====================================================
  //Write first format header string
  txt = "sep=; \r\n";
  file.Write(txt.c_str(), txt.length());
/*
  //Write Time stamp header
  DataStamp();
  txt = "File created: " + data_stamp + "\r\n\r\n";
  file.Write(txt.c_str(), txt.length());
*/  
  file.Close();
  
  opened = 1;

  return 1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CLOGFILE::AddCreateTime(void)
{
  if(opened!=1) return -1;
  
  std::string txt,

  //Write created Time stamp header
  AddDataStamp();
  txt = "File created: " + data_stamp + "\r\n\r\n";
  
  //Write to file
  int ret = Add_to_File(txt.c_str(),(char*)txt.c_str(), txt.length());

  return ret;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CLOGFILE::CheckFile(void)
{
  CFileStatus status;
  
  if(log_file_name.length()<4) return 0;

  //смотрим есть ли файл на диске  
  BOOL ret = CFile::GetStatus(log_file_name.c_str(), status);

  return (int) ret;
}

//-----------------------------------------------------------------------------
//Data write
//-----------------------------------------------------------------------------
int CLOGFILE::Write(char *pStr)
{
  int name_length = log_file_name.length();
  if(name_length<1) return -1;

  //Txt to file
  std::string txt = pStr;
  int txt_length = txt.length();
  if(txt_length<=0) return 0;
  
  //Write to file
  int ret = Add_to_File(log_file_name.c_str(),(char*)txt.c_str(), txt_length);
  
  //если не удалось записать потому что файл удалили
  if(ret<0)
  {
    //создать новый файл
    CheckFile();

    //записать 
    Add_to_File(log_file_name.c_str(),(char*)txt.c_str(), txt_length);
  }

  writed = 1;
  return 1;
}

//-----------------------------------------------------------------------------
//Data write
//-----------------------------------------------------------------------------
int CLOGFILE::ShowFile(void)
{
  int name_length = log_file_name.length();
  if(name_length<1) {AfxMessageBox("File not opened"); return -1;}

  HINSTANCE Status = 0;
  CString file_path, msg;
  
  //если пробелы в пути - добавить двойные кавычки
  file_path = "\"";
  file_path += log_file_name.c_str();
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

  const char *pTxt = "The specified file: ../";

  //error case
  switch((int)Status)
  {
    case ERROR_NOT_READY:
    msg = pTxt;
    msg += file_path;
    msg += " was not ready";
    break;
        
    case ERROR_FILE_NOT_FOUND:
    msg = pTxt;
    msg += file_path;
    msg += " was not found";
    break;

    case ERROR_PATH_NOT_FOUND:
    msg = pTxt;
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

//-----------------------------------------------------------------------------
//Files number in directory
//-----------------------------------------------------------------------------
int CLOGFILE::GetFilesNumber(void)
{
  int length, counter = 0;
  BOOL result;
  WIN32_FIND_DATA FIND_DATA;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string find;  
	
  //если не было инициализировано
  length = folder_path.length();
  if(length<4) return 0;
  
  //проверить наличие директории
  int ret = PathFileExistsA(folder_path.c_str());
  if(ret!=1)
  {
	 //директори€ пуста€, файлов нет
	 //файлов нет
	 files_count = 0;
	 return 0;
  }

  find = folder_path + "\\" + "*." + file_ext;

  //Start iterating over the files in the path directory
  hFind = ::FindFirstFile(find.c_str(), &FIND_DATA);

  if(hFind==INVALID_HANDLE_VALUE)
  {
     //файлов нет
	 files_count = 0;
	 return 0;
  }
  
  //Save first file name
  files.RemoveAll();
  files.Add(FIND_DATA.cFileName);

  //Managed to locate and create an handle to that folder
  for(;;) 
  { 
	counter++;
	//find next file
	result = ::FindNextFile(hFind, &FIND_DATA);
	if(result==FALSE) break;
    
	//Save next file name
    files.Add(FIND_DATA.cFileName);
  }

  //----  
  ::FindClose(hFind);
  //----------------------------------------------------

  files_count = counter;

  return files_count;
}

//-----------------------------------------------------------------------------
//получить файловую дату и врем€
//-----------------------------------------------------------------------------
int CLOGFILE::GetLastWriteTime(void)
{
  FILETIME ftCreate, ftWrite;
  SYSTEMTIME stUTC, stLocal;
  HANDLE hFile;
  int length, counter = 0;
  BOOL result;
  WIN32_FIND_DATA FIND_DATA;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string find;  
	
  //если не было инициализировано
  length = folder_path.length();
  if(length<4) return 0;
  
  //проверить наличие директории
  int ret = PathFileExistsA(folder_path.c_str());
  if(ret!=1)
  {
	 //директори€ пуста€, файлов нет
	 files_count = 0;
	 return 0;
  }

  //find = folder_path + "\\" + "*." + file_ext;
  find = log_file_name;
    
  length = find.length();
  if(length<4) return 0;

  //Start iterating over the files in the path directory
  hFind = ::FindFirstFile(find.c_str(), &FIND_DATA);
  if(hFind==NULL) return 0;

  ftWrite = FIND_DATA.ftLastWriteTime;

  //преобразовать врем€ модификации в локальное врем€
  FileTimeToSystemTime(&ftWrite, &stUTC);
   //дата и врем€ последней записи в файл
  SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
  last_file_time = CTime(stLocal).Format("%H:%M %d.%m.%Y");

  return 1;
}

/*
//https://www.codeproject.com/Articles/294/Using-the-IE-5-built-in-progress-dialog
void ProcessStuff(void)
{
  SHProgressWnd dlg;
  const DWORD  dwMax = 100;
  TCHAR szMsg[256];

  dlg.SetTitle ( _T("Hang on a sec...") );
  dlg.SetAnimation(IDR_PROGRESS_AVI);
  dlg.SetCancelMessage ( _T("Cancelling this operation...") );
  dlg.SetLineText ( 1, _T("Unzipping files...") );

  if(FAILED(dlg.ShowModal(this)))
     return;

  dlg.UpdateProgress ( 0, dwMax );

  for(DWORD dwProgress = 0;
      dwProgress < dwMax && !dlg.HasUserCanceled();
      dwProgress += 5 )
  {
     //DoSomeSlowProcessing();
     wsprintf(szMsg, _T("I'm %lu%% done"), dwProgress );

     dlg.SetLineText ( 2, szMsg );
     dlg.UpdateProgress ( dwProgress );
  }

  dlg.EndDialog();
}
*/

//-----------------------------------------------------------------------------
//удаление файла в корзину
//-----------------------------------------------------------------------------
int CLOGFILE::FileRecycle(std::string *pPath)
{
  SHFILEOPSTRUCT SHFileOp;
  ZeroMemory(&SHFileOp, sizeof(SHFILEOPSTRUCT));
  //NULL (диалога прогреса, не используем)
  SHFileOp.hwnd = NULL;
  //FO_DELETE (операци€ удалени€)
  SHFileOp.wFunc = FO_DELETE;
  
  //им€ файла(ов) дл€ удалени€
  std::string path;
  path = *pPath;
  path.push_back('\0');
  path.push_back('\0');
  //----
  SHFileOp.pFrom = path.c_str();

  //NULL (дл€ удалени€ не используетс€)
  SHFileOp.pTo = NULL;
  
  //#define FOF_SILENT      0x0004    // не показывать процесс удалени€
  //#define FOF_NOERRORUI   0x0400    // не выводить ошибки
  //#define FOF_ALLOWUNDO   0x0040    // ќЅя«ј“≈Ћ№Ќќ дл€ корзины!!!
  //#define FOF_NOCONFIRMATION 0x0010 // Ќе спрашивать пользовател€ OK
  SHFileOp.fFlags = FOF_NOCONFIRMATION | FOF_ALLOWUNDO | FOF_SIMPLEPROGRESS;
  //BOOL fAnyOperationsAborted;// (возвращает TRUE если пользователь прервал) не используем
  SHFileOp.lpszProgressTitle = "Delete"; // дл€ удалени€ не используетс€
  //LPVOID hNameMappings;      // дл€ удалени€ не используетс€
  
  int result = SHFileOperation(&SHFileOp);
  if (result != 0) 
  { 
    // Delete failed
	return 0;
  }

  return 1;
}

//-----------------------------------------------------------------------------
//Delete files in directory
//-----------------------------------------------------------------------------
int CLOGFILE::DeleteDirFiles(void)
{  
  int count;
  BOOL result;
  std::string find;  
	
  count = GetFilesNumber();
  if(count<=0) return 0;

  //----
  files_count =	files.GetCount();
  count = files.GetCount();

  //теперь удал€ем файлы
  for(int i=0; i<count; i++)
  {
  	//Remove file
	find = folder_path + "\\";
	find +=  files.GetAt(i);
	
	//----
	///result = ::DeleteFileA(find.c_str());
  	///if(result==TRUE) files_count--; 

    //Delete the file by sending to the recycle bin.
    result = FileRecycle(&find);
	if(result==TRUE) files_count--; 
  }

  return files_count;
}
