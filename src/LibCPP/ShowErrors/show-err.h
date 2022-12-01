//==============================================================================
//File name:   "show-err.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _SHOW_ERROR_MSG_H_
#define _SHOW_ERROR_MSG_H_

#define  __func__ __FUNCTION__

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus

 void ShowError(LPTSTR lpszFunction); 

#ifdef __cplusplus
}
#endif //end __cplusplus

#endif
