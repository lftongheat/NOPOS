#include "stdafx.h"
#include "enumser.h"





///////////////////////////// Statics / Locals ////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
#define new DEBUG_NEW
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif




///////////////////////////// Implementation //////////////////////////////////

void EnumerateSerialPorts(CUIntArray& ports)
{
  //Make sure we clear out any elements which may already be in the array
  ports.RemoveAll();

  //Up to 255 COM ports are supported so we iterate through all of them seeing
  //if we can open them or if we fail to open them, get an access denied error.
  //Both of these cases indicate that there is a COM port at that number. The 
  //beauty of this technique is that it is portable across 95, 98, NT and CE.
  for (UINT i=1; i<20; i++)
  {
    //Form the Raw device name
    CString sPort;
    sPort.Format(_T("\\\\.\\COM%d"), i);

    //Try to open the port
    BOOL bSuccess = FALSE;
    HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hPort == INVALID_HANDLE_VALUE)
    {
      DWORD dwError = GetLastError();

      //Check to see if the error was because some other app had the port open
      if (dwError == ERROR_ACCESS_DENIED)
        bSuccess = TRUE;
    }
    else
    {
      //The port was opened successfully
      bSuccess = TRUE;

      //Don't forget to close the port, since we are going to do nothing with it anyway
      CloseHandle(hPort);
    }

    //Add the port number to the array which will be returned
    if (bSuccess)
      ports.Add(i);
  }
}