//==============================================================================
//File name:   "lpc21isp.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _LPC21_ISP_H_
#define _LPC21_ISP_H_

#include "cmd.h"
#include "Page6.h"
#include <ctype.h>    
#include <stdio.h>  
#include <time.h>
#include <fcntl.h>

// The Required features can be enabled / disabled here
#define LPC_SUPPORT
#define INTEGRATED_IN_WIN_APP

typedef enum
{
  NXP_ARM,
  ANALOG_DEVICES_ARM
} TARGET;

typedef enum
{
  PROGRAM_MODE,
  RUN_MODE
} TARGET_MODE;

typedef enum
{
  FORMAT_BINARY,
  FORMAT_HEX
} FILE_FORMAT_TYPE;

/** Used to create list of files to read in. */
typedef struct file_list FILE_LIST;

#define ERR_RECORD_TYPE_LOADFILE  55  /**< File record type not yet implemented. */
#define ERR_ALLOC_FILE_LIST       60  /**< Error allocation file list. */
#define ERR_FILE_OPEN_HEX         61  /**< Couldn't open hex file. */
#define ERR_FILE_SIZE_HEX         62  /**< Unexpected hex file size. */
#define ERR_FILE_ALLOC_HEX        63  /**< Couldn't allocate enough memory for hex file. */
#define ERR_MEMORY_RANGE          69  /**< Out of memory range. */

typedef struct
{
  //The type of micro that will be programmed.
  TARGET micro;                
  FILE_FORMAT_TYPE FileFormat;
  //Normally set
  unsigned char ProgramChip;   
  int DoNotStart;
  int BootHold;
  unsigned char WriteDelay;
  unsigned char DetectOnly;
  unsigned char WipeDevice;
  unsigned char Verify;
  //index in LPCtypes[] array
  int DetectedDevice;     
  
  //Binary image of the microcontroller's memory.
  unsigned char *BinaryContent;   
                           
  unsigned long BinaryLength;
  unsigned long BinaryOffset;
  unsigned long StartAddress;
  unsigned long BinaryMemSize;

} ISP_ENVIRONMENT;

extern int debug_level;

#if defined INTEGRATED_IN_WIN_APP

#define DebugPrintf AppDebugPrintf
void AppDebugPrintf(const char *fmt, ...);

#define exit(val)   AppException(val)
void AppException(int exception_level);

#define Exclude_kbhit 1
int AppSyncing(int trials);
void AppWritten(int size);

#else
void DebugPrintf(const char *fmt, ...);
//#define DebugPrintf(level, ...) if (level <= debug_level) { TRACE( __VA_ARGS__ ); }
#endif

extern CPage6 *pCPage6; 
extern char * sendbuf[20];
extern char uuencode_table[64];

/*
debug levels
0 - very quiet          - Nothing gets printed at this level
1 - quiet               - Only error messages should be printed
2 - indicate progress   - Add progress messages
3 - first level debug   - Major level tracing
4 - second level debug  - Add detailed debugging
5 - log comm's          - log serial I/O
*/

void PrepareKeyboardTtySettings(void);
void ResetKeyboardTtySettings(void);
void ResetTarget(ISP_ENVIRONMENT *IspEnvironment, TARGET_MODE mode);

void DumpString(int level, const void *s, size_t size, const char *prefix_string);
int  NxpDownload(ISP_ENVIRONMENT *IspEnvironment);

int  Chip_Erase(ISP_ENVIRONMENT *IspEnvironment);
int  Sector_Erase(int sector);
int  Sector_CheckEmpty(int sector);
int  Sector_Prepare(int sector);
int  Sector_Verify(char *pCMD);
int  Write_ToRAM(unsigned long ram_addr, int length);
int  Copy_ToRAM(ISP_ENVIRONMENT *IspEnvironment, unsigned long pos, int &line, unsigned long &bl_crc);
int  Copy_ToFlash(unsigned long flash_addr, unsigned long ram_addr, int length);
int  Check_CRC(unsigned long crc);

void SetTXpointer(void *pBT);
int  BT_Send(const char *pString);
int  BT_Receive(void *block, int size, unsigned timeout);

unsigned long ReturnValueLpcRamStart(ISP_ENVIRONMENT *IspEnvironment);
unsigned long ReturnValueLpcRamBase(ISP_ENVIRONMENT *IspEnvironment);

#endif
