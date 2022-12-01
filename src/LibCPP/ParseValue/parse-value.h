//==============================================================================
//File name:   "parse-value.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _PARSE_VALUE_H_
#define _PARSE_VALUE_H_

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus  

 int ParseValueFl(CEdit *pEdit, float &out, float min, float max);
 int ParseValueSh(CEdit *pEdit, short &out, short min, short max);
 int ParseValueCh(CEdit *pEdit, char &out, char min, char max);

 int ParseValueUCh(CEdit *pEdit, unsigned char &out, 
	               unsigned char min, unsigned char max);

#ifdef __cplusplus
}
#endif //end __cplusplus

#endif
