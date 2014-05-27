#include "WRITELog.h"
#include"stdafx.h"

FILE *fp;

void GetTime(char *Date,char *Time)
{
	SYSTEMTIME st;
	char sTimes[20]="\0";
	GetLocalTime(&st);
	if(Date!=NULL)
	{   memset(sTimes,0,sizeof(sTimes));
		sprintf(sTimes,"%02d-%02d-%02d",st.wYear,st.wMonth,st.wDay);
		strcpy(Date,sTimes); 
	}
	if(Time!=NULL)
	{
		memset(sTimes,0,sizeof(sTimes));
		sprintf(sTimes,"%02d:%02d:%02d [%04d]",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		strcpy(Time,sTimes); 
	}
}

int OpenLog(char *File)
{
    fp = fopen(File, "a+");
    if(fp!=NULL) return 0;
    else return -1;
}

int CloseLog()
{
    fclose(fp);
    fp =NULL;
    return 0;
}
/*
int WriteLog(char *fmt)
{
    if(fp==NULL)  return -1;
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "%s",fmt);  
	return 0;
}*/
int WriteLog(const char *Format, ...)
{
	if(fp==NULL)  return -1;
    fseek(fp, 0, SEEK_END);
	
	va_list arg_ptr;
	va_start(arg_ptr, Format);
	vfprintf(fp, Format, arg_ptr);
	va_end(arg_ptr);
	return 0;
}

int WriteLog_uchar(BYTE *fmt,int FLen)
{
    int i;
    if(fp==NULL) return -1;
    fseek(fp, 0, SEEK_END);
    for(i=0;i<FLen;i++)
    {
        if((i%20)==0)
        {
            if(i==0) fprintf(fp, "[%04d]",(i+1)); 
            else fprintf(fp, "\n     [%04d]",(i+1)); 
        }
        fprintf(fp, "%02X ",fmt[i]); 
    }
    fprintf(fp, "\n");
	return 0;
}

int WriteLog_APDU(BYTE *fmt,int FLen)
{
    if(fp==NULL) return -1;
    int i;
    BYTE Data[1024];
    int DataLen=0;
    memset(Data,0,sizeof(Data));
    memcpy(Data,fmt,FLen);
    Data[FLen]=0x03;
    DataLen=FLen+1;
    fseek(fp, 0, SEEK_END);
    for(i=0;i<DataLen;i++)
    {
        if((i%20)==0)
        {
            if(i==0) fprintf(fp, "[%04d] ",(i+1)); 
            else fprintf(fp, "\n     [%04d] ",(i+1)); 
        }
        fprintf(fp, "%02X ",Data[i]); 
    }
    
    fprintf(fp, "\n");
	return 0;
}



