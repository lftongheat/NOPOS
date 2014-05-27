#include "SerialPort.h"
#include "stdafx.h"


int EXIT=0;

HANDLE m_hIDComDev;   //串口句柄
OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
BOOL m_bOpened;  //串口打开标志

int OpenPort(int nCOM, int nBaud ,int nByteSize, int nStopBits, int nParity)//打开串口
{
	char szPort[15];
	DCB dcb;
	wsprintf(szPort, "COM%d", nCOM );
	m_hIDComDev =CreateFile(szPort,  //COM1口
				 GENERIC_READ|GENERIC_WRITE, //允许读和写
				 0,  //独占方式
				 NULL,
				 OPEN_EXISTING,  //打开而不是创建
				 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, //重叠方式
				 NULL);
	if(m_hIDComDev ==INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("打开串口失败!");
		return FAIL;
	}
	
	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	
	SetupComm(m_hIDComDev,1024,1024); //输入缓冲区和输出缓冲区的大小都是1024
	
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( m_hIDComDev, &CommTimeOuts );

	GetCommState( m_hIDComDev, &dcb );
	dcb.BaudRate = nBaud;
	dcb.ByteSize = nByteSize;
	dcb.StopBits = nStopBits-1;//0-1位停止位，1-1.5位停止位，2-2位停止位
	dcb.Parity = nParity;

	if(!SetCommState(m_hIDComDev,&dcb))
	{
		//AfxMessageBox("打开串口失败!");
		return FAIL;
	}

	PurgeComm(m_hIDComDev,PURGE_TXCLEAR|PURGE_RXCLEAR);
	
	m_bOpened = TRUE;//打开串口标志
	return SUCC;

}

int ClosePort( void )//关闭串口
{

	if( !m_bOpened || m_hIDComDev == NULL ) return SUCC;//串口没有打开

	if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent!= NULL ) CloseHandle( m_OverlappedWrite.hEvent);
	CloseHandle( m_hIDComDev );
	
	m_bOpened = FALSE;
	m_hIDComDev = NULL;

	return SUCC;

}

int ReadData( void *buffer, int limit )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;
   	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
	if( !bReadStatus )
	{
		if( GetLastError() == ERROR_IO_PENDING )
		{
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
		}
		return( 0 );
	}

	return( (int) dwBytesRead );

}

int WriteData(unsigned char *Data,int length )//写Data中length个数据到串口
{
	DWORD dwBytesWritten;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	COMSTAT ComStat;
	if( !m_bOpened || m_hIDComDev == NULL ) return NoOpenCom ;
	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );//清除错误标志

	bWriteStat=WriteFile(m_hIDComDev,Data,(DWORD)length,&dwBytesWritten,&m_OverlappedWrite);
	if(!bWriteStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			//WaitForSingleObject(m_OverlappedWrite.hEvent,1000);
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten,TRUE);
			//函数会一直等待，直到写操作完成或由于错误而返回
			return dwBytesWritten;
		}
		return FAIL;
	}
	return dwBytesWritten;

}

void ClearBuffer()//清空缓冲区
{
	if( !m_bOpened || m_hIDComDev == NULL ) return;

	DWORD dwErrorFlags;
	COMSTAT ComStat;
	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	int nBuffer=(int) ComStat.cbInQue;//取缓冲区数据个数
	if(nBuffer == 0) return;

	unsigned char* buffer = new BYTE[nBuffer];
	ReadData(buffer, nBuffer);
	delete[] buffer;

}
//有条件获取缓冲区数据
int GetReturn(unsigned char *buffer, int Least, int OutTime, char* Endflag, int nflagL)
	//Endflag结束符缓冲区，nflagL结束符个数，若没有结束符，则收到Least个数据以上或超时返回
{	
	long int ot = OutTime * 30;
	int nRead = 0, n = 0 , t = 0;
	BOOL bTimeover = FALSE, bFinish = FALSE;	
    MSG message;
    EXIT=0;
	for(; !bTimeover && !bFinish ; t++)
	{
		n = ReadData(&buffer[nRead] ,1024);
		nRead += n;

		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))	
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		
		if(EXIT)//判断是否退出
		{
			EXIT=0;
			return EXITFlag;
          
		}

		if(Endflag != NULL && nflagL > 0)//判断是否有结束符
		{
			for(int i = 0 ; i < nRead ; i++)
			{
				for(int j = 0 ; j < nflagL ; j++)
				{
					if(buffer[i + j] != Endflag[j])//判断接收到的数据是否与结束符相符
					break;
				}

				if(j == nflagL)
				{
					bFinish = TRUE;//相符即设置结束标志，然后退出
					break;
				}
			}			
		}

		else//若没有结束符及外部退出命令，就判断接收到的数据个数是否已达到最小要求的个数
		{
			if(nRead >= Least && n == 0)
			{	bFinish = TRUE;
		        break;
			}
		}		

		if(t > ot) bTimeover = TRUE;//超时退出
		Sleep(20);
	
	}
	
	if(bTimeover) return TimeOutErr;//超时
	return nRead;//返回接收到的数据个数
}
//选择扩展口
int SelectExtPort(char ExPort)
{
	int iRet;
	BYTE cmd[3]={0x1B, 0x25, 0x41};
    cmd[2]=ExPort;
    iRet=WriteData(cmd, 3);
	if(iRet!=3) return FAIL;	
	return SUCC;
}