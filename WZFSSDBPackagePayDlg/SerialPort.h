#if !defined(_SERIALPORT_H_)
#define _SERIALPORT_H_

#include "stdafx.h"


#define SUCC             0      /*操作成功*/
#define FAIL             -1     /*操作失败*/

#define EXITFlag         -2     /*接收中中止退出*/
#define NoOpenCom        -100   /*串口没打开*/
#define TimeOutErr       -101   /*超时*/



extern int OpenPort(int nCOM, int nBaud ,int nByteSize, int nStopBits, int nParity);//打开串口
extern int ClosePort( void );
extern int WriteData( unsigned char *buffer, int size );
extern int ReadData( void *buffer, int limit );
extern void ClearBuffer();
extern int GetReturn(unsigned char *buffer, int Least, int OutTime, char* Endflag= NULL, int nflagL=0);
extern int SelectExtPort(char ExPort);


#endif 
