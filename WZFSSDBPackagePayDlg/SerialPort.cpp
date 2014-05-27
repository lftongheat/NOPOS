#include "SerialPort.h"
#include "stdafx.h"


int EXIT=0;

HANDLE m_hIDComDev;   //���ھ��
OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
BOOL m_bOpened;  //���ڴ򿪱�־

int OpenPort(int nCOM, int nBaud ,int nByteSize, int nStopBits, int nParity)//�򿪴���
{
	char szPort[15];
	DCB dcb;
	wsprintf(szPort, "COM%d", nCOM );
	m_hIDComDev =CreateFile(szPort,  //COM1��
				 GENERIC_READ|GENERIC_WRITE, //�������д
				 0,  //��ռ��ʽ
				 NULL,
				 OPEN_EXISTING,  //�򿪶����Ǵ���
				 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, //�ص���ʽ
				 NULL);
	if(m_hIDComDev ==INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("�򿪴���ʧ��!");
		return FAIL;
	}
	
	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	
	SetupComm(m_hIDComDev,1024,1024); //���뻺����������������Ĵ�С����1024
	
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
	dcb.StopBits = nStopBits-1;//0-1λֹͣλ��1-1.5λֹͣλ��2-2λֹͣλ
	dcb.Parity = nParity;

	if(!SetCommState(m_hIDComDev,&dcb))
	{
		//AfxMessageBox("�򿪴���ʧ��!");
		return FAIL;
	}

	PurgeComm(m_hIDComDev,PURGE_TXCLEAR|PURGE_RXCLEAR);
	
	m_bOpened = TRUE;//�򿪴��ڱ�־
	return SUCC;

}

int ClosePort( void )//�رմ���
{

	if( !m_bOpened || m_hIDComDev == NULL ) return SUCC;//����û�д�

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

int WriteData(unsigned char *Data,int length )//дData��length�����ݵ�����
{
	DWORD dwBytesWritten;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	COMSTAT ComStat;
	if( !m_bOpened || m_hIDComDev == NULL ) return NoOpenCom ;
	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );//��������־

	bWriteStat=WriteFile(m_hIDComDev,Data,(DWORD)length,&dwBytesWritten,&m_OverlappedWrite);
	if(!bWriteStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			//WaitForSingleObject(m_OverlappedWrite.hEvent,1000);
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten,TRUE);
			//������һֱ�ȴ���ֱ��д������ɻ����ڴ��������
			return dwBytesWritten;
		}
		return FAIL;
	}
	return dwBytesWritten;

}

void ClearBuffer()//��ջ�����
{
	if( !m_bOpened || m_hIDComDev == NULL ) return;

	DWORD dwErrorFlags;
	COMSTAT ComStat;
	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	int nBuffer=(int) ComStat.cbInQue;//ȡ���������ݸ���
	if(nBuffer == 0) return;

	unsigned char* buffer = new BYTE[nBuffer];
	ReadData(buffer, nBuffer);
	delete[] buffer;

}
//��������ȡ����������
int GetReturn(unsigned char *buffer, int Least, int OutTime, char* Endflag, int nflagL)
	//Endflag��������������nflagL��������������û�н����������յ�Least���������ϻ�ʱ����
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
		
		if(EXIT)//�ж��Ƿ��˳�
		{
			EXIT=0;
			return EXITFlag;
          
		}

		if(Endflag != NULL && nflagL > 0)//�ж��Ƿ��н�����
		{
			for(int i = 0 ; i < nRead ; i++)
			{
				for(int j = 0 ; j < nflagL ; j++)
				{
					if(buffer[i + j] != Endflag[j])//�жϽ��յ��������Ƿ�����������
					break;
				}

				if(j == nflagL)
				{
					bFinish = TRUE;//��������ý�����־��Ȼ���˳�
					break;
				}
			}			
		}

		else//��û�н��������ⲿ�˳�������жϽ��յ������ݸ����Ƿ��Ѵﵽ��СҪ��ĸ���
		{
			if(nRead >= Least && n == 0)
			{	bFinish = TRUE;
		        break;
			}
		}		

		if(t > ot) bTimeover = TRUE;//��ʱ�˳�
		Sleep(20);
	
	}
	
	if(bTimeover) return TimeOutErr;//��ʱ
	return nRead;//���ؽ��յ������ݸ���
}
//ѡ����չ��
int SelectExtPort(char ExPort)
{
	int iRet;
	BYTE cmd[3]={0x1B, 0x25, 0x41};
    cmd[2]=ExPort;
    iRet=WriteData(cmd, 3);
	if(iRet!=3) return FAIL;	
	return SUCC;
}