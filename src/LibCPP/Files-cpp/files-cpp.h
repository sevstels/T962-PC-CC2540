//==============================================================================
//File name:   "files-cpp.h"
//Purpose:      Header File
//Version:      2.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================

#ifndef _FILES_CPP_H_
#define _FILES_CPP_H_

//Save binary data to file, функция сохранения данных в файле  
int Save_File(CString filename, char *pBuf,
			   unsigned long start_addr, unsigned long data_size);

//Add and Save binary data to file, функция сохранения данных в файле  
int Add_to_File(CString filename, char *pBuf, unsigned long data_size);

//Read binary data from file, функция чтения данных из файла 
int Read_File(CString filename, char *pBuf, 
			  unsigned long start_addr, unsigned long data_size);

//Check file exists
int CheckFile(CString filename);
  
int CheckFolder(CString foldername);

int CreateFolder(CString foldername);

//Get File Size
unsigned long Get_FileSize(CString filename);

//Remove(delete) file, функция стирания файла
int Delete_File(CString filename);

#endif
