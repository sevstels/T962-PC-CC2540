#include "stdafx.h"
#include "Page4.h"

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

//==============================================================================
//Thread function: Monitoring and receive device data
//==============================================================================
DWORD PAGE5_THREAD(LPVOID lParam)
{
 //Get pointer to Class
 CPage4 *pPage4 = (CPage4*)lParam;
 ASSERT(pPage4->IsKindOf(RUNTIME_CLASS(CPage4)));

 //Synchronisation objects
 CSyncObject *pSyncObj[4];
 pSyncObj[0] = &pPage4->DSPL.ev_Redraw;
 pSyncObj[1] = &pPage4->DSPL.ev_ScrollLeft;
 pSyncObj[2] = &pPage4->DSPL.ev_ScrollRight;
 pSyncObj[3] = &pPage4->ev_Quit;

 //---- launch Tread loop 
 for(;;)
  {
  //waiting event
  CMultiLock Lock(pSyncObj, 4);
				
  switch (Lock.Lock(INFINITE, FALSE))
   {			
     //------ EVENT from ev_Redraw
	case WAIT_OBJECT_0:
	pPage4->DSPL.Draw_Scales();
	pPage4->DSPL.Draw_Profile();
    pPage4->DSPL.DisplayShow();
	break;
	
	//------ EVENT from ev_ScrollLeft
	case WAIT_OBJECT_0+1:
	pPage4->DSPL.Scroll_Left();
	break;
			
	//------ EVENT from ev_ScrollRight
	case WAIT_OBJECT_0+2:
	pPage4->DSPL.Scroll_Right();
	break;	

	//------ EVENT from ev_DeviceEvent
	//case WAIT_OBJECT_0+1:
	//break;												  
			
/*	//------ EVENT from ev_RedrawGraph
	case WAIT_OBJECT_0+2:
	break;
					  
	//------ EVENT from ev_SignalProcessing
    case WAIT_OBJECT_0+5:
	break;
			
	//------ EVENT from ev_Oscilloscope
   case WAIT_OBJECT_0+6:
   break;
			
   //------ EVENT from ev_Quit
   case WAIT_OBJECT_0+7:  */

   //==================================================
   //Exit from thread by "Quit" event ev_Quit
   //==================================================
   case WAIT_OBJECT_0+3:
   //------ EVENT from ev_Quit
   default:
   //---- End Tread
   AfxEndThread(0, TRUE);
   return 0;
  }
 }		
 
 //---- End Tread loop
 AfxEndThread(0, TRUE);
 return 0;
}

