#if !defined(_SERIALPORT_H_)
#define _SERIALPORT_H_

#include "stdafx.h"


#define SUCC             0      /*�����ɹ�*/
#define FAIL             -1     /*����ʧ��*/

#define EXITFlag         -2     /*��������ֹ�˳�*/
#define NoOpenCom        -100   /*����û��*/
#define TimeOutErr       -101   /*��ʱ*/



extern int OpenPort(int nCOM, int nBaud ,int nByteSize, int nStopBits, int nParity);//�򿪴���
extern int ClosePort( void );
extern int WriteData( unsigned char *buffer, int size );
extern int ReadData( void *buffer, int limit );
extern void ClearBuffer();
extern int GetReturn(unsigned char *buffer, int Least, int OutTime, char* Endflag= NULL, int nflagL=0);
extern int SelectExtPort(char ExPort);


#endif 
