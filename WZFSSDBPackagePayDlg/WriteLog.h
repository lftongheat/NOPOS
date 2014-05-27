#if !defined(_WRITELOG_H_)
#define _WRITELOG_H_
#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 



//#define DebugLog

//extern  int  WriteLog(char *fmt);
extern int WriteLog(const char *Format, ...);

extern  int  WriteLog_uchar(BYTE *fmt,int FLen);
extern  void GetTime(char *Date,char *Time);
extern  int WriteLog_APDU(BYTE *fmt,int FLen);

extern int OpenLog(char *File);
extern int CloseLog();




#endif 
