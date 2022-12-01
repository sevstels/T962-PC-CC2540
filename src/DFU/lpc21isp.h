//==============================================================================
//File name:   "lpc21isp.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#define COMPILE_FOR_WINDOWS
#define COMPILED_FOR "Windows"

// The Required features can be enabled / disabled here
#define LPC_SUPPORT
#define INTEGRATED_IN_WIN_APP

#ifndef COMPILE_FOR_LPC21
#define AD_SUPPORT
#define TERMINAL_SUPPORT
#endif

#if defined COMPILE_FOR_WINDOWS
#include <windows.h>
#include <io.h>
#endif // defined COMPILE_FOR_WINDOWS

#if defined COMPILE_FOR_WINDOWS
#include <conio.h>
#define TRACE(x) OutputDebugString(x)
//#define TRACE(x) printf("%s",x)
#endif // defined COMPILE_FOR_WINDOWS

#include <ctype.h>      // isdigit()
#include <stdio.h>      // stdout
#include <stdarg.h>
#include <time.h>

#if defined COMPILE_FOR_LPC21
#include <stdlib.h>
#include <string.h>
//#include <lpc_ioctl.h>  // if using libc serial port communication
#else
#include <fcntl.h>
#endif

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

typedef unsigned char BINARY;               // Data type used for microcontroller

/** Used to create list of files to read in. */
typedef struct file_list FILE_LIST;

#define ERR_RECORD_TYPE_LOADFILE  55  /**< File record type not yet implemented. */
#define ERR_ALLOC_FILE_LIST       60  /**< Error allocation file list. */
#define ERR_FILE_OPEN_HEX         61  /**< Couldn't open hex file. */
#define ERR_FILE_SIZE_HEX         62  /**< Unexpected hex file size. */
#define ERR_FILE_ALLOC_HEX        63  /**< Couldn't allocate enough memory for hex file. */
#define ERR_MEMORY_RANGE          69  /**< Out of memory range. */

/** Structure used to build list of input files. */
struct file_list
{
    const char *name;       /**< The name of the input file.	*/
    FILE_LIST *prev;        /**< The previous file name in the list.*/
    char hex_flag;          /**< True if the input file is hex.	*/
};

typedef struct
{
#if !defined COMPILE_FOR_LPC21
    TARGET micro;                                // The type of micro that will be programmed.
    FILE_FORMAT_TYPE FileFormat;
    unsigned char ProgramChip;                // Normally set

    unsigned char ControlLines;
    unsigned char ControlLinesSwapped;
    unsigned char ControlLinesInverted;
    unsigned char LogFile;
    FILE_LIST *f_list;                  // List of files to read in.
    int nQuestionMarks; // how many times to try to synchronise
    int DoNotStart;
    int BootHold;
    char *serial_port;                  // Name of the serial port to use to
                                        // communicate with the microcontroller.
                                        // Read from the command line.
#endif // !defined COMPILE_FOR_LPC21

    unsigned char TerminalOnly;         // Declared here for lazyness saves ifdef's
#ifdef TERMINAL_SUPPORT
    unsigned char TerminalAfterUpload;
    unsigned char LocalEcho;
#endif

    unsigned char HalfDuplex;           // Only used for LPC Programming
    unsigned char WriteDelay;
    unsigned char DetectOnly;
    unsigned char WipeDevice;
    unsigned char Verify;
    int           DetectedDevice;       /* index in LPCtypes[] array */
    char *baud_rate;                    /**< Baud rate to use on the serial
                                           * port communicating with the
                                           * microcontroller. Read from the
                                           * command line.                        */

    char StringOscillator[6];           /**< Holds representation of oscillator
                                           * speed from the command line.         */

    BINARY *FileContent;
    BINARY *BinaryContent;              /**< Binary image of the                  */
                                          /* microcontroller's memory.            */
    unsigned long BinaryLength;
    unsigned long BinaryOffset;
    unsigned long StartAddress;
    unsigned long BinaryMemSize;

#if defined COMPILE_FOR_WINDOWS
    HANDLE hCom;
#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#ifdef INTEGRATED_IN_WIN_APP
    unsigned char NoSync;
#endif

#if defined COMPILE_FOR_WINDOWS
    unsigned long serial_timeout_count;   /**< Local used to track timeouts on serial port read. */
#else
    unsigned serial_timeout_count;   /**< Local used to track timeouts on serial port read. */
#endif

} ISP_ENVIRONMENT;

#if defined COMPILE_FOR_LPC21

#define DebugPrintf(in, ...)

#else
extern int debug_level;

#if defined INTEGRATED_IN_WIN_APP

#define DebugPrintf AppDebugPrintf
void AppDebugPrintf(const char *fmt, ...);

#define exit(val)   AppException(val)
void AppException(int exception_level);

int AppDoProgram(int argc, char *argv[]);

#define Exclude_kbhit 1
int AppSyncing(int trials);
void AppWritten(int size);

#else
void DebugPrintf(const char *fmt, ...);
//#define DebugPrintf(level, ...) if (level <= debug_level) { TRACE( __VA_ARGS__ ); }
#endif

void ClearSerialPortBuffers(ISP_ENVIRONMENT *IspEnvironment);
void ControlXonXoffSerialPort(ISP_ENVIRONMENT *IspEnvironment, unsigned char XonXoff);

#endif

/*
debug levels
0 - very quiet          - Nothing gets printed at this level
1 - quiet               - Only error messages should be printed
2 - indicate progress   - Add progress messages
3 - first level debug   - Major level tracing
4 - second level debug  - Add detailed debugging
5 - log comm's          - log serial I/O
*/

void ReceiveBT(ISP_ENVIRONMENT *IspEnvironment,
                    const char *Ans, unsigned long MaxSize,
                    unsigned long *RealSize, unsigned long WantedNr0x0A,
                    unsigned timeOutMilliseconds);
void PrepareKeyboardTtySettings(void);
void ResetKeyboardTtySettings(void);
void ResetTarget(ISP_ENVIRONMENT *IspEnvironment, TARGET_MODE mode);

void DumpString(int level, const void *s, size_t size, const char *prefix_string);
//void SendComPort(ISP_ENVIRONMENT *IspEnvironment, const char *s);
//void SendComPortBlock(ISP_ENVIRONMENT *IspEnvironment, const void *s, size_t n);
int  ReceiveComPortBlockComplete(ISP_ENVIRONMENT *IspEnvironment, void *block, size_t size, unsigned timeout);
void ClearSerialPortBuffers(ISP_ENVIRONMENT *IspEnvironment);
void ControlXonXoffSerialPort(ISP_ENVIRONMENT *IspEnvironment, unsigned char XonXoff);
	
int NxpDownload(ISP_ENVIRONMENT *IspEnvironment);

void SetTXpointer(void *pBT);
int BT_Send(const char *pString);
int BT_Receive(void *block, int size, unsigned timeout);
