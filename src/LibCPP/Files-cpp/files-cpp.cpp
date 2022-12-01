//==============================================================================
//File name:   "files-cpp.cpp"
//Purpose:      Source File, Memory Buffer class
//Version:      2.00
//Copyright:   (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "files-cpp.h"

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

//-----------------------------------------------------------------------------
//Save binary data to file, ������� ���������� ������ � ����� 
//-----------------------------------------------------------------------------
int Save_File(CString filename, char *pBuf, 
	          unsigned long start_addr, unsigned long data_size)
{
   int result;
   TRY
        {	
            
            //Open and check status
            //file.Open(filename, CFile::modeRead, )
                
            //��������� ���� ��� ������
            CFile file(filename, CFile::modeCreate|		//
                                 CFile::modeWrite|		//Only for record
                                 CFile::shareDenyNone|	//
                                 CFile::typeBinary);	//binary mode

            //������������� ������� ������ ����� ������ ������������ ������ �����
            file.Seek(start_addr, CFile::begin);
                        
            //���������� ���� ������ � ���� �� ������ 
            file.Write(pBuf, data_size);
			//file.Flush();
            file.Close();
            result = 1;					
        }

    CATCH( CFileException, pEx)
                    
        {
            #ifdef _DEBUG
            afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
            pEx->Delete();
            #endif
            AfxMessageBox(_T("Can not write file: \n") + filename);
          	result = -1;
        }
                
  END_CATCH

  return result;
}

//-----------------------------------------------------------------------------
//Add and Save binary data to file, ������� ���������� ������ � ����� 
//-----------------------------------------------------------------------------
int Add_to_File(CString filename, char *pBuf, unsigned long data_size)
{
  TRY
   {	
      //��������� ���� ��� ������
      CFile file(filename, 
                 CFile::modeNoTruncate|	//Not delite data
                 CFile::modeWrite|		//Only for record
                 CFile::shareDenyNone|	//share all
                 CFile::typeBinary);    //binary mode

	   //!!! ��� �����, �� ���������� ���������� �����
       //������������� ������� ������ ����� ������ 
	   //������������ ������ �����
       ///file.Seek(start_addr,CFile::begin);

	  //������ �� ����� ������������� �����
	  file.Seek(0, CFile::end);
	 
      //���������� ���� ������ � ���� �� ������ 
      file.Write(pBuf, data_size);
	  //file.Flush();
      file.Close();
      return 1;					
   }

  CATCH(CFileException, pEx)			
   {
      #ifdef _DEBUG
      afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
      pEx->Delete();
      #endif
      AfxMessageBox(_T("Can not write file: \n") + filename);
      return -1;
   }			
  END_CATCH	
}

//-----------------------------------------------------------------------------                
//Read file size, ������� ������ ������� ����� 
//-----------------------------------------------------------------------------
unsigned long Get_FileSize(CString filename)
{
  TRY
  {	
     //Open file for read, ��������� ���� ��� ������
     CFile fileuser(filename, CFile::modeRead | 
                              CFile::shareDenyNone | 
                              CFile::typeBinary);
    
     //Get file size 
     unsigned long size = (unsigned long)fileuser.GetLength();
     //Close					
     fileuser.Close();
     return size;	
  }

 CATCH( CFileException, pEx)
  {
    #ifdef _DEBUG
    afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
    pEx->Delete();
    #endif
    AfxMessageBox(_T("Can not reade file:\n") + filename);
    return 0;
  }
 END_CATCH	
}

//-----------------------------------------------------------------------------                
//Read binary data from file, ������� ������ ������ �� ����� 
//-----------------------------------------------------------------------------
int Read_File(CString filename, char *pBuf, unsigned long start_addr, 
                                             unsigned long data_length)
{
  int result;

  TRY
   {	
     //��������� ���� ��� ������
     //open file for read
     CFile fileuser (filename, CFile::modeRead | 
                               CFile::shareDenyNone | 
                               CFile::typeBinary);
                            
     //������������� ������� ������ ����� ������ ������������ ������ �����
     fileuser.Seek(start_addr,CFile::begin);
                        
     //������ ���� ������ �� ���� � ����� 
     result = fileuser.Read(pBuf, data_length);
     fileuser.Close();
  }

  CATCH(CFileException, pEx)				
   {
     #ifdef _DEBUG
     afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
     pEx->Delete();
     #endif
     AfxMessageBox(_T("Can`t reade file:\n") + filename);
     result = -1;
   }			
  END_CATCH	

  return result;
}

//-----------------------------------------------------------------------------                
//Check file, �������� ������� ����� 
//-----------------------------------------------------------------------------
int CheckFile(CString filename)
{
  return::GetFileAttributes((LPCTSTR)filename) != DWORD(-1);
}

//-----------------------------------------------------------------------------                
//Delete file, ������� �������� ����� 
//-----------------------------------------------------------------------------
int Delete_File(CString filename)
{
    TRY
        {		
            //Remove file
            CFile::Remove(filename);
            return true;	
        }

    CATCH( CFileException, pEx)
                    
        {
            #ifdef _DEBUG
            afxDump << _T("File could not be removed ") << pEx->m_cause << "\n";
            pEx->Delete();
            #endif
            AfxMessageBox(_T("Can not remove file:\n") + filename);
            return false;
        }
                
    END_CATCH	
}

//-----------------------------------------------------------------------------                
//
//-----------------------------------------------------------------------------
int CheckFolder(CString foldername)
{
  if(GetFileAttributes(foldername) == INVALID_FILE_ATTRIBUTES)
  {
    //CreateDirectory(newPath, NULL);
	return -1;
  }

  return 1;
}

//-----------------------------------------------------------------------------                
//
//-----------------------------------------------------------------------------
int CreateFolder(CString foldername)
{  /*
   CString year, mon;
    CString current_dir,open_dir;
    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer),buffer);
 
    CTime time = CTime::GetCurrentTime();
 
    year.Format("%d",time.GetYear());
    mon.Format("%s\\%s", year,months[time.GetMonth()]);
 
    CreateDirectory(year,NULL);
    CreateDirectory(mon,NULL);
 
    current_dir.Format("%s",buffer);
    open_dir.Format("%s\\%s",buffer,mon);
 
    ShellExecute(NULL,"open",open_dir,"","",SW_SHOW);
    ShellExecute(NULL,"close",current_dir,"","",SW_HIDE);
	  */
  return 1;
}
