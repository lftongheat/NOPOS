// CSDBPackagePayDlg.cpp : Implementation of CCSDBPackagePayDlgApp and DLL registration.

#include "stdafx.h"
#include "CSDBPackagePayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
char StartVou[500];
char m_inPayNO[500];
char prtWaterNo[30];

CString cm_AdminNO,cm_UnitNO,cm_VouDate,cm_VouKind,cm_VouNO,cm_PayerName,cm_VouFee,cm_MakeUserGuId,cm_TermNO,cm_SubTermNO;
CString cm_PayKind,cm_BankNO,cm_BankUnit,cm_BankName,cm_BankOper,cm_BankInDate,cm_CheckDate;

CString p_CardNO,p_TransDate,p_TransTime,p_TraceNO,p_BatchNO,p_AuthNO,p_RefNO,p_ValidDate,p_TransMoney;

CString CPayAmount,m_inNodeNO,m_inNoCount,CLateAmt,cm_intzdType,cm_inBatP,cm_OutBatP,cm_OutFSCode;
CString cm_inPayID,cm_inPayName,cm_inFeeItemNo1,cm_inFeeLevel1,cm_inFeeItemNo2,cm_inFeeLevel2,cm_inFeeItemNo3,cm_inFeeLevel3;
CString cm_inFeeItemNo4,cm_inFeeLevel4,cm_inFeeItemNo5,cm_inFeeLevel5,cm_inFeeItemNo6,cm_inFeeLevel6;
CString cm_inFeeItemName1,cm_inFeeItemName2,cm_inFeeItemName3,cm_inFeeItemName4,cm_inFeeItemName5,cm_inFeeItemName6;
int m_Comport,m_TimeOutV;
HANDLE hCommDev;// 串口设备句柄
int BpKeyType;
char  Track2Data[256];
char  Track3Data[256];
char  m_OutPacket[6000];
char  m_IP1[50],m_Port1[10],m_IP2[50],m_Port2[10];
char  m_MerchantNO[16];
char  cardno[30];
char  pReturnV[50];
long  FReturnVal;
long  prtFlag;
int hasReadPasswd,hasReadCard,CancelFlag,RefundFlag;
int Exit=0;
CString sendICDomain;
OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
BOOL m_bOpened;
char m_FeeItemNo[10][20+1];
char m_FeeLevel[10][16+1];
char m_FeeItemKind[10][20+1];
char m_FeeItemCount[10][16+1];
char m_FeeItemAmt[10][16+1];

CCSDBPackagePayDlgApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x30dfdcc9, 0xcf47, 0x43c8, { 0x95, 0x62, 0xfc, 0x6f, 0x21, 0x96, 0x83, 0xb4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 2;


////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgApp::InitInstance - DLL initialization

BOOL CCSDBPackagePayDlgApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		memset(prtWaterNo,0x00,sizeof(prtWaterNo));
		memset(pReturnV,0x00,sizeof(pReturnV));
		prtFlag =0;  //默认通过传入卡号、流水打印票据，5：联机获得卡号、流水号
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgApp::ExitInstance - DLL termination

int CCSDBPackagePayDlgApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

/*程序保护密钥，按照自己要求改动*/
char masKeyBuf[20] = "0101010101010101";
/////////////////////////////////////////////////////////////////////////////
// ChangeStringToByte used to convert a 2 char long string to one byte
BYTE ChangeStringToByte(const char * szParam)
{
	BYTE temp;

	if (szParam[0] != NULL)
	{

		if ((szParam[0] <= '9') && (szParam[0] >= '0'))
		{
			temp = ((szParam[0] - '0'))<<4;
		}
		else
			temp = ((szParam[0] - 'A') + 10)<<4;

		temp = temp & 0xF0;

		if ((szParam[1] <= '9') && (szParam[1] >= '0'))
		{
			temp = temp | (szParam[1] - '0');
		}
		else
			temp = temp | ((szParam[1] - 'A') + 10);

	}

	return temp;
}


/////////////////////////////////////////////////////////////////////////////
// ChangeUndefinedSizeStringToByte used to convert a string to an array of bytes
int ChangeUndefinedSizeStringToByte(char *szParam,BYTE *arrayOfByte)
{
	int i=0;

	while (szParam[0] != NULL)
	{
		arrayOfByte[i] = ChangeStringToByte(szParam);
		szParam++;szParam++;i++;
	}

	return i;
}




void str_xor(BYTE *src, BYTE *dst, int len)
{
	int	i;

	for (i=0; i<len; i++)
		dst[i] = dst[i] ^src[i];
}

void str_cpy(BYTE *src, BYTE *dst, int len)
{
	int	i;

	for (i=0; i<len; i++)
		dst[i] = src[i];
}

void circle(BYTE *str, int leng)	// to complete left circel shift
{                                           // 1 bit per time
	BYTE	tmp;
	int		i;

	tmp = str[0];
	for (i=0; i<leng-1; i++)
		str[i] = str[i+1];
	str[leng-1] = tmp;
}

void transfer(BYTE *src, BYTE *dst, BYTE *table)
{
	int	table_leng, i;

	table_leng = 0;
	while (table[table_leng] != 255)
		table_leng++;

	for (i=0; i<table_leng; i++) {
		dst[i] = src[table[i]];
	}
}

void key_generate(BYTE *key_in, BYTE *key_sub, int cnt)
{
	BYTE	buffer[56];
	BYTE	C0[28], D0[28];
	int				i;
	BYTE	shift[] = {
	 1,  2,  4,  6,  8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28,};
	BYTE	PC_1[] = {
	56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
	 9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
	62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3,
	255};
	BYTE	PC_2[] = {
	13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9, 22, 18, 11,  3,
	25, 7,	15,  6, 26, 19, 12,  1, 40, 51, 30, 36, 46, 54, 29, 39,
	50, 44,	32, 47,	43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31,
	255};


	transfer(key_in, buffer, PC_1);

	for (i=0; i<28; i++) {
		C0[i] = buffer[i];
		D0[i] = buffer[i+28];
	}
	for (i=0; i<shift[cnt]; i++) {
		circle(C0, 28);
		circle(D0, 28);
	}
	for (i=0; i<28; i++) {
		buffer[i] = C0[i];
		buffer[i+28] = D0[i];
	}

	transfer(buffer, key_sub, PC_2);
}


void S_change(BYTE *str)
{
	BYTE	src[8][6], dst[8][4];
	int		i, j, adr;
	BYTE	S[8][64] = {
	{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
	 },

	{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	  3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	  0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
	 },

	{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	 13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	 13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	  1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
	 },

	{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	 13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	 10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	  3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
	 },

	{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	 14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	  4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	 11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
	 },

	{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	  9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	  4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
	 },

	{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	  6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
	 },

	{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	  1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	  7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
	}};

	for (i=0; i<8; i++)
		for (j=0; j<6; j++)
			src[i][j] = str[i*6+j];				// seperate to 8 parts

	for (i=0; i<8; i++) {
		j = src[i][1]*8+src[i][2]*4+src[i][3]*2+src[i][4];
		adr = (src[i][0]*2+src[i][5])*16 + j;
		j = S[i][adr];
		dst[i][0] = j / 8;
		j %= 8;
		dst[i][1] = j / 4;
		j %= 4;
		dst[i][2] = j / 2;
		dst[i][3] = j % 2;                      // tansfer 6 bits to 4 bits per part
	}

	for (i=0; i<8; i++)
		for (j=0; j<4; j++)
			str[i*4+j] = dst[i][j];             // link them to 32 bits
}

void des_algo(BYTE *src, BYTE *dst, BYTE *key, int flag)
{
	BYTE	sub_key[48], tmp[32], buffer[48];//, cal_tmp[32];
	BYTE	left[32], right[32];
	int		i;
	BYTE	IP[] = {
	57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7,
	56, 48, 40, 32, 24, 16,  8,  0, 58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4, 62, 54, 46, 38, 30, 22, 14,  6,
	255};
	BYTE	IP_1[] = {
	39,  7, 47, 15, 55, 23, 63, 31, 38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29, 36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27, 34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25, 32,  0, 40,  8, 48, 16, 56, 24,
	255};
	BYTE	E[] = {
	31,  0,  1,  2,  3,  4,  3,  4,  5,  6,  7,  8,  7,  8,  9, 10,
	11, 12,	11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20,
	21, 22, 23, 24,	23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31,  0,
	255};
	BYTE	P[] = {
	15,  6, 19, 20, 28, 11, 27, 16,  0, 14, 22, 25,  4, 17, 30,  9,
	 1,  7, 23, 13, 31, 26,  2,  8, 18, 12, 29,  5, 21, 10,  3, 24,
	255};



	transfer(src, dst, IP);

	for (i=0; i<32; i++) {
		left[i] = dst[i];
		right[i] = dst[i+32];				// generate L0, R0
	}

	for (i=0; i<16; i++) {					// the main circle
		if (flag == 0)
			key_generate(key, sub_key, i); 	// generate the sub key
		else                                // flag = 0 is encypher; 1 is decypher
			key_generate(key, sub_key, 15-i);
		str_cpy(right, tmp, 32);			// save RIGHT_half to tmp

		transfer(right, buffer, E);			// R(i-1) from 32 bit to 48 bit
		str_xor(sub_key, buffer, 48);		// then XOR with k(i)
		S_change(buffer);            		// change result with S-table
		transfer(buffer, right, P);			// transfer using P-table ==> f

		str_xor(left, right, 32);			// generate R(i)
		str_cpy(tmp, left, 32);				// generate L(i)
	}

	for (i=0; i<32; i++) {
		src[i] = right[i];
		src[32+i] = left[i];
	}                       				// generate source(R[15]L[15])

	transfer(src, dst, IP_1);				// get result in dst
}

void	tostring(BYTE *dd, char *hh,int len)
{
	int		i;
	char	ddl,ddh;

	hh[len*2] = 0;
	for (i=0;i<len;i++){
		ddh = 48 + dd[i] / 16;
		ddl = 48 + dd[i] % 16;
		if (ddh > 57)  ddh = ddh + 7;
		if (ddl > 57)  ddl = ddl + 7;
		hh[i*2] = ddh;
		hh[i*2+1] = ddl;
	}
}

void dtob(BYTE dd,BYTE *hh)
{
	int i;

	for (i=0;i<8;i++){
		hh[i] = 0;
		if (dd > 0x7F) hh[i] = 1;
		dd = dd << 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
/* 外部DLL调用 argc 0是加密，1是解密，src为要加解密内容，key为密钥			 */
/* 所有参数为二进制方式传入													 */
///////////////////////////////////////////////////////////////////////////////
void  des0(int argc, BYTE *src, BYTE *key, BYTE *result)
{
	BYTE	source[64], dest[64], key_main[64];
	int		flag, i, j;

	if (argc == 0) flag = 0;
	else flag = 1 ;

	for (i=0; i<8; i++){
		dtob(src[i],source+i*8);
		dtob(key[i],key_main+i*8);
	}
	des_algo(source, dest, key_main, flag);
	for (i=0; i<8; i++){
		result[i] = 0;
		for (j=0;j<8;j++) result[i] |= (1<<(7-j))*dest[8*i+j];
	}
}

///////////////////////////////////////////////////////////////////////////////
/* 外部DLL调用 argc 0是加密，1是解密，src为要加解密内容，key为密钥			 */
/* 所有参数为字符方式传入													 */
///////////////////////////////////////////////////////////////////////////////
void  tohex(char *hh, BYTE *dd,int len)
{
	char h1,h2;
	BYTE i,s1,s2;

	for (i=0;i<len;i++){
		h1 = hh[2*i];
		h2 = hh[2*i+1];
		s1 = toupper(h1) - 0x30;
		if (s1 > 9) s1 -= 7;
		s2 = toupper(h2) - 0x30;
		if (s2 > 9) s2 -= 7;
		dd[i] = s1*16 + s2;
    }
}

void  DES(int mode, char *data, char *key, char *result)
{
	BYTE	Sou[8],Key[8],Res[8];

	tohex(data,Sou,8);
	tohex(key,Key,8);
	des0(mode,Sou,Key,Res);
	tostring(Res,result,8);
}


int GetDateTime(char *data)
{
	struct	tm	*newtime;
	time_t	aclock;

	time( &aclock );
	newtime = localtime( &aclock );
	newtime->tm_year += 1900;
	newtime->tm_mon += 1;

	data[0] = 0;
	itoa(newtime->tm_year,data,10);

	itoa(newtime->tm_mon/10,&data[26],10);
	strcat(data,&data[26]);
	itoa(newtime->tm_mon%10,&data[26],10);
	strcat(data,&data[26]);

	itoa(newtime->tm_mday/10,&data[26],10);
	strcat(data,&data[26]);
	itoa(newtime->tm_mday%10,&data[26],10);
	strcat(data,&data[26]);

	itoa(newtime->tm_hour/10,&data[26],10);
	strcat(data,&data[26]);
	itoa(newtime->tm_hour%10,&data[26],10);
	strcat(data,&data[26]);

	itoa(newtime->tm_min/10,&data[26],10);
	strcat(data,&data[26]);
	itoa(newtime->tm_min%10,&data[26],10);
	strcat(data,&data[26]);

	itoa(newtime->tm_sec/10,&data[26],10);
	strcat(data,&data[26]);
	itoa(newtime->tm_sec%10,&data[26],10);
	strcat(data,&data[26]);

	return 0;
}

void  hex2Asc(unsigned char * hexBuf,char * ascBuf,int hexLen)
{
    int i = 0;

    for( i = 0 ; i < hexLen ; i++ )
        sprintf(ascBuf+i*2,"%02X",hexBuf[i]);

}

///////////////////////////////////////////////////////////////////////////////
/* 生成用户秘码，cardNo用户卡号，orgPass用户密码明文，encPass密码密文		 */
///////////////////////////////////////////////////////////////////////////////
int	 genUsrPin(char * cardNo,char * orgPass,char *encPass)
{
	int	 i = 0;

	char panBuf[20];
	char passBuf[20];
	char keyBuf[40];

	unsigned char hexPanBuf[10];
	unsigned char hexPassBuf[10];
	unsigned char hexKeyBuf[10];
	unsigned char hexSecKey[10];
	unsigned char hexEncPass[10];
	unsigned char hexMasKeyBuf[10];
	unsigned char hexSecPass[10];

	FILE * fd = NULL;
    memcpy(cardNo,"00000000000000000000",20);
	memset(panBuf , 0x00 , sizeof(panBuf));
	memset(passBuf , 0x00 , sizeof(passBuf));
	memset(keyBuf , 0x00 , sizeof(keyBuf));

	memset(hexPanBuf , 0x00 , sizeof(hexPanBuf));
	memset(hexPassBuf , 0x00 , sizeof(hexPassBuf));
	memset(hexKeyBuf , 0x00 , sizeof(hexKeyBuf));
	memset(hexMasKeyBuf , 0x00 , sizeof(hexMasKeyBuf));
	memset(hexSecKey , 0x00 , sizeof(hexSecKey));
	memset(hexEncPass , 0x00 , sizeof(hexEncPass));
	memset(hexSecPass , 0x00 , sizeof(hexSecPass));
	memset(encPass, 0x00, sizeof(encPass));

	strcpy(panBuf , "0000");
	memcpy(panBuf+4 , cardNo+strlen(cardNo)-13 , 12);
	tohex(panBuf , hexPanBuf , 8);

	sprintf(passBuf,"%02d",strlen(orgPass));
	strcat(passBuf,orgPass);
	memcpy(passBuf+strlen(passBuf),"FFFFFFFFFFFFFFFF",16-strlen(passBuf));
	tohex(passBuf , hexPassBuf , 8);

/*	if((fd = fopen("SDBkey.dat","r")) == NULL) 
		return -1;
	fgets(keyBuf,sizeof(keyBuf),fd);
	fclose(fd);
*/
//	if( strlen(keyBuf) != 16 )
//		return -2;
//	if( memcmp(keyBuf,"0000000000000000",16) == 0 )
//		return -3;
	memcpy(keyBuf,"994D4DC157B96C52",16);
	tohex(keyBuf , hexKeyBuf , 8);
	tohex(masKeyBuf,hexMasKeyBuf,8);
	des0(1,hexKeyBuf,hexMasKeyBuf,hexSecKey);

	for(i = 0 ; i < 8 ; i++) 
		hexSecPass[i] = hexPanBuf[i]^hexPassBuf[i];

	des0(0,hexSecPass,hexSecKey,hexEncPass);

	hex2Asc(hexEncPass,encPass,8);

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
/* 生成数据MAC，data校验数据，length校验数据长度，macValue生成mac(16位)      */
///////////////////////////////////////////////////////////////////////////////
int  genMAC (char *data, int length, char *macValue)
{
	int i,j;
	char keyBuf[40];

	unsigned char macBuf[20];
    unsigned char tmpBuf[20];
    unsigned char datBuf[20];
	unsigned char hexKeyBuf[10];
	unsigned char hexMacKey[10];
	unsigned char hexMasKeyBuf[10];

	FILE * fd = NULL;

	/*取得MAC工作密钥*/
	memset(keyBuf , 0x00 , sizeof(keyBuf));
	memset(hexKeyBuf , 0x00 , sizeof(hexKeyBuf));
	memset(hexMasKeyBuf , 0x00 , sizeof(hexMasKeyBuf));
	memset(hexMacKey , 0x00 , sizeof(hexMacKey));

	if((fd = fopen("SDBkey.dat","r")) == NULL) 
		return -1;
	fgets(keyBuf,sizeof(keyBuf),fd);
	fclose(fd);
	if( strlen(keyBuf) != 32 )
		return -2;
	if( memcmp(keyBuf+16,"0000000000000000",16) == 0 )
		return -3;
	tohex(keyBuf+16 , hexKeyBuf , 8);
	tohex(masKeyBuf,hexMasKeyBuf,8);
	des0(1,hexKeyBuf,hexMasKeyBuf,hexMacKey);

	memset(macBuf , 0x00 , sizeof(macBuf));
    i=0;
    while( (i+8) <= length ) {
        memset(tmpBuf , 0x00 , sizeof(tmpBuf));
        for( j = 0 ; j < 8 ; j++ ) {
            tmpBuf[j]=macBuf[j]^data[i+j];
        }
        des0(0,tmpBuf,hexMacKey ,macBuf);
        i+=8;
    }

    if( i != length ) {
        memset(datBuf, 0x00 , sizeof(datBuf));
        memcpy(datBuf, data+i, length-i     );
        memset(tmpBuf , 0x00 , sizeof(tmpBuf));

        for( j = 0; j < 8; j++ ) {
            tmpBuf[j]=macBuf[j]^datBuf[j];
        }
        des0(0,tmpBuf,hexMacKey ,macBuf);
    }

	hex2Asc(macBuf,macValue,8);

	return(0);
}
////补足包长度////
void FullPacketLen(char *inStrPacket)
{
    const  char *temp   =  "0123456789!~#$%@^&*()_+abcdefghijklmnopqrstuvwxyz-=\";'<>?,./`ABCDEFGHIJKLMNOPQRSTUVWXYZ";   
	char FreePacket[2500];
    char CPackRand[7];
	long PackRand;
	unsigned int j;

	memset(FreePacket,0x00,sizeof(FreePacket));
	memset(CPackRand,0x00,sizeof(CPackRand));
    srand( (unsigned)time( NULL ) ); 
	for(unsigned int i =0;i<1300;i++)
	{
		        
		PackRand = 1000000 + rand();
		sprintf(CPackRand,"%d",PackRand);
		CPackRand[0] ='0';
		j = atoi(CPackRand) % strlen(temp);
        FreePacket[i] = temp[j];
	}
	memcpy(&inStrPacket[strlen(inStrPacket)],&FreePacket[strlen(inStrPacket)],1600 -strlen(inStrPacket));
}
/////
//异或加密
void   XOR(char *szSouse,   char *szDest)   
{   
   
    
   for(UINT i = 0,n = 0; i<strlen(szSouse); i++)
   {   
      if(n  == strlen(xorStr))  n = 0;   
      char   cn   =   (char)(xorStr[i]^xorStr[n]);   
      szDest[i]   =   cn;   
      n++;   
   }   
          
}   


//记录日志
void WriteDiary(CString strInfo,CString filename)
{
	CFile     cDiaryFile;//日志操作文件对象
	CString   cDiaryLogDir,cFileDate,cDeleteFile,cDeleteDay;
	CTime     cMyTime= CTime::GetCurrentTime();
	CFileFind DirExist;
  	CString   cWeekDay = cMyTime.Format("%A");
	if (cWeekDay == "星期五") 
	{
		cDeleteDay = "星期六";
	}
	if (cWeekDay == "星期六") 
	{
		cDeleteDay = "星期日";
	}
	if (cWeekDay == "星期六") 
	{
		cDeleteDay = "星期一";
	}
	if (cWeekDay == "星期一") 
	{
		cDeleteDay = "星期二";
	}
	if (cWeekDay == "星期二") 
	{
		cDeleteDay = "星期三";
	}
	if (cWeekDay == "星期三") 
	{
		cDeleteDay = "星期四";
	}

	if (cWeekDay == "星期四") 
	{
		cDeleteDay = "星期五";
	}

	cDeleteFile = "c:\\SDBLOG\\SDB"+ cDeleteDay + filename;
	remove(cDeleteFile);
	if (! DirExist.FindFile("c:\\SDBLOG"))  
	      CreateDirectory("c:\\SDBLOG",NULL);
    filename    ="c:\\SDBLOG\\SDB"+ cWeekDay + filename;
	//打开日志文件
	
	if(cDiaryFile.Open(filename,CFile::modeWrite))
	{
		cDiaryFile.SeekToEnd();
		CString strTemp;
		CTime cTime=CTime::GetCurrentTime();
		strInfo=( cTime.Format("%Y-%m-%d %H:%M:%S  :[") + CString(StartVou) +"]:" + strInfo);
		strInfo+="\r\n";
		cDiaryFile.Write(strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());
		strInfo.ReleaseBuffer();

		cDiaryFile.Close();
	}
	else
		if(cDiaryFile.Open(filename,CFile::modeCreate))
		{
		}

}

BOOL OpenComm(int CommPort, int nBaudRate)
{
	DCB dcb;
	switch(CommPort)
	 {
	 case 1:
		 hCommDev=CreateFile("COM1",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;
	 case 2:
		 hCommDev=CreateFile("COM2",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;
	 case 3:
		 hCommDev=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;	 
	 case 4:
		 hCommDev=CreateFile("COM4",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;	 
     case 5:
		 hCommDev=CreateFile("COM5",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;     	
	case 6:
		 hCommDev=CreateFile("COM6",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;	
	case 7:
		 hCommDev=CreateFile("COM7",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;	
	case 8:
		 hCommDev=CreateFile("COM8",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;	
	case 9:
		 hCommDev=CreateFile("COM9",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;	
	 default:
		 hCommDev=CreateFile("COM1",GENERIC_READ|GENERIC_WRITE,0,
		                      NULL,OPEN_EXISTING,0,NULL);
		 break;
	}
     if(hCommDev==(HANDLE)-1) return(FALSE);
	 GetCommState(hCommDev,&dcb);

	 switch(nBaudRate)
	 {
	 	case 9600:
			dcb.BaudRate=CBR_9600;
			break;
		case 4800:
			dcb.BaudRate=CBR_4800;
			break;
		case 2400:
			dcb.BaudRate=CBR_2400;
			break;
		case 1200:
			dcb.BaudRate=CBR_1200;
			break;
		default:
			dcb.BaudRate=CBR_9600;
	 }
     
	 dcb.ByteSize=8;
	 dcb.Parity=0;
	 dcb.StopBits=0;     
	 
	 SetCommState(hCommDev,&dcb);
	 COMMTIMEOUTS CommTimeOuts;
     CommTimeOuts.ReadIntervalTimeout=MAXDWORD;
	 CommTimeOuts.ReadTotalTimeoutMultiplier=0;
	 CommTimeOuts.ReadTotalTimeoutConstant=0;
	 CommTimeOuts.WriteTotalTimeoutMultiplier=0;
	 CommTimeOuts.WriteTotalTimeoutConstant=1000;
	 SetCommTimeouts(hCommDev,&CommTimeOuts);
	 PurgeComm(hCommDev,PURGE_TXCLEAR);
	 PurgeComm(hCommDev,PURGE_RXCLEAR);
  	 return TRUE;
}

BOOL CloseComm()
{
    PurgeComm(hCommDev,PURGE_TXCLEAR);
	PurgeComm(hCommDev,PURGE_RXCLEAR);
    return(CloseHandle(hCommDev));
}


void des(unsigned char *source, unsigned char *dest, unsigned char *inkey, int flg)
{
	unsigned char   TABLE1[64],
	TABLE2[56], TABLE3[48], TABLE4[64], TABLE5[48], key[64],
	nbrofshift, temp1, temp2;
	int     valindex;
	register i, j, k, iter;

	/* INITIALIZE THE TABLES */
	/* Table - s1 */
	static unsigned char    s1[4][16] = {
		14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
		0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
		4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
		15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13                    };

	/* Table - s2 */
	static unsigned char    s2[4][16] = {
		15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
		3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
		0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
		13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9                    };

	/* Table - s3 */
	static unsigned char    s3[4][16] = {
		10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
		13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
		13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
		1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12                    };

	/* Table - s4 */
	static unsigned char    s4[4][16] = {
		7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
		13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
		10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
		3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14                    };

	/* Table - s5 */
	static unsigned char    s5[4][16] = {
		2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
		14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
		4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
		11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3                    };

	/* Table - s6 */
	static unsigned char    s6[4][16] = {
		12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
		10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
		9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
		4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13                    };

	/* Table - s7 */
	static unsigned char    s7[4][16] = {
		4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
		13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
		6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12                    };

	/* Table - s8 */
	static unsigned char    s8[4][16] = {
		13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
		1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
		7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
		2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11                    };


	/* Table - Shift */
	static unsigned char    shift[16] = {
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1                  };


	/* Table - Binary */
	static unsigned char    binary[64] = {
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1,
		0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
		1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1                  };

	/* MAIN PROCESS */
	/* Convert from 64-bit key into 64-byte key */
	for (i = 0; i < 8; i++) {
		key[8*i] = ((j = *(inkey + i)) / 128) % 2;
		key[8*i+1] = (j / 64) % 2;
		key[8*i+2] = (j / 32) % 2;
		key[8*i+3] = (j / 16) % 2;
		key[8*i+4] = (j / 8) % 2;
		key[8*i+5] = (j / 4) % 2;
		key[8*i+6] = (j / 2) % 2;
		key[8*i+7] = j % 2;
	}

	/* Convert from 64-bit data into 64-byte data */
	for (i = 0; i < 8; i++) {
		TABLE4[8*i] = ((j = *(source + i)) / 128) % 2;
		TABLE4[8*i+1] = (j / 64) % 2;
		TABLE4[8*i+2] = (j / 32) % 2;
		TABLE4[8*i+3] = (j / 16) % 2;
		TABLE4[8*i+4] = (j / 8) % 2;
		TABLE4[8*i+5] = (j / 4) % 2;
		TABLE4[8*i+6] = (j / 2) % 2;
		TABLE4[8*i+7] = j % 2;
	}

	/* Initial Permutation of Data */
	TABLE1[ 0] = TABLE4[57];
	TABLE1[ 1] = TABLE4[49];
	TABLE1[ 2] = TABLE4[41];
	TABLE1[ 3] = TABLE4[33];
	TABLE1[ 4] = TABLE4[25];
	TABLE1[ 5] = TABLE4[17];
	TABLE1[ 6] = TABLE4[ 9];
	TABLE1[ 7] = TABLE4[ 1];
	TABLE1[ 8] = TABLE4[59];
	TABLE1[ 9] = TABLE4[51];
	TABLE1[10] = TABLE4[43];
	TABLE1[11] = TABLE4[35];
	TABLE1[12] = TABLE4[27];
	TABLE1[13] = TABLE4[19];
	TABLE1[14] = TABLE4[11];
	TABLE1[15] = TABLE4[ 3];
	TABLE1[16] = TABLE4[61];
	TABLE1[17] = TABLE4[53];
	TABLE1[18] = TABLE4[45];
	TABLE1[19] = TABLE4[37];
	TABLE1[20] = TABLE4[29];
	TABLE1[21] = TABLE4[21];
	TABLE1[22] = TABLE4[13];
	TABLE1[23] = TABLE4[ 5];
	TABLE1[24] = TABLE4[63];
	TABLE1[25] = TABLE4[55];
	TABLE1[26] = TABLE4[47];
	TABLE1[27] = TABLE4[39];
	TABLE1[28] = TABLE4[31];
	TABLE1[29] = TABLE4[23];
	TABLE1[30] = TABLE4[15];
	TABLE1[31] = TABLE4[ 7];
	TABLE1[32] = TABLE4[56];
	TABLE1[33] = TABLE4[48];
	TABLE1[34] = TABLE4[40];
	TABLE1[35] = TABLE4[32];
	TABLE1[36] = TABLE4[24];
	TABLE1[37] = TABLE4[16];
	TABLE1[38] = TABLE4[ 8];
	TABLE1[39] = TABLE4[ 0];
	TABLE1[40] = TABLE4[58];
	TABLE1[41] = TABLE4[50];
	TABLE1[42] = TABLE4[42];
	TABLE1[43] = TABLE4[34];
	TABLE1[44] = TABLE4[26];
	TABLE1[45] = TABLE4[18];
	TABLE1[46] = TABLE4[10];
	TABLE1[47] = TABLE4[ 2];
	TABLE1[48] = TABLE4[60];
	TABLE1[49] = TABLE4[52];
	TABLE1[50] = TABLE4[44];
	TABLE1[51] = TABLE4[36];
	TABLE1[52] = TABLE4[28];
	TABLE1[53] = TABLE4[20];
	TABLE1[54] = TABLE4[12];
	TABLE1[55] = TABLE4[ 4];
	TABLE1[56] = TABLE4[62];
	TABLE1[57] = TABLE4[54];
	TABLE1[58] = TABLE4[46];
	TABLE1[59] = TABLE4[38];
	TABLE1[60] = TABLE4[30];
	TABLE1[61] = TABLE4[22];
	TABLE1[62] = TABLE4[14];
	TABLE1[63] = TABLE4[ 6];

	/* Initial Permutation of Key */
	TABLE2[ 0] = key[56];
	TABLE2[ 1] = key[48];
	TABLE2[ 2] = key[40];
	TABLE2[ 3] = key[32];
	TABLE2[ 4] = key[24];
	TABLE2[ 5] = key[16];
	TABLE2[ 6] = key[ 8];
	TABLE2[ 7] = key[ 0];
	TABLE2[ 8] = key[57];
	TABLE2[ 9] = key[49];
	TABLE2[10] = key[41];
	TABLE2[11] = key[33];
	TABLE2[12] = key[25];
	TABLE2[13] = key[17];
	TABLE2[14] = key[ 9];
	TABLE2[15] = key[ 1];
	TABLE2[16] = key[58];
	TABLE2[17] = key[50];
	TABLE2[18] = key[42];
	TABLE2[19] = key[34];
	TABLE2[20] = key[26];
	TABLE2[21] = key[18];
	TABLE2[22] = key[10];
	TABLE2[23] = key[ 2];
	TABLE2[24] = key[59];
	TABLE2[25] = key[51];
	TABLE2[26] = key[43];
	TABLE2[27] = key[35];
	TABLE2[28] = key[62];
	TABLE2[29] = key[54];
	TABLE2[30] = key[46];
	TABLE2[31] = key[38];
	TABLE2[32] = key[30];
	TABLE2[33] = key[22];
	TABLE2[34] = key[14];
	TABLE2[35] = key[ 6];
	TABLE2[36] = key[61];
	TABLE2[37] = key[53];
	TABLE2[38] = key[45];
	TABLE2[39] = key[37];
	TABLE2[40] = key[29];
	TABLE2[41] = key[21];
	TABLE2[42] = key[13];
	TABLE2[43] = key[ 5];
	TABLE2[44] = key[60];
	TABLE2[45] = key[52];
	TABLE2[46] = key[44];
	TABLE2[47] = key[36];
	TABLE2[48] = key[28];
	TABLE2[49] = key[20];
	TABLE2[50] = key[12];
	TABLE2[51] = key[ 4];
	TABLE2[52] = key[27];
	TABLE2[53] = key[19];
	TABLE2[54] = key[11];
	TABLE2[55] = key[ 3];

	/* 16 Iterations */
	for (iter = 1; iter < 17; iter++) {
		for (i = 0; i < 32; i++)
			TABLE4[i] = TABLE1[32+i];

		/* Calculation of F(R, K) */
		/* Permute - E */
		TABLE3[ 0] = TABLE4[31];
		TABLE3[ 1] = TABLE4[ 0];
		TABLE3[ 2] = TABLE4[ 1];
		TABLE3[ 3] = TABLE4[ 2];
		TABLE3[ 4] = TABLE4[ 3];
		TABLE3[ 5] = TABLE4[ 4];
		TABLE3[ 6] = TABLE4[ 3];
		TABLE3[ 7] = TABLE4[ 4];
		TABLE3[ 8] = TABLE4[ 5];
		TABLE3[ 9] = TABLE4[ 6];
		TABLE3[10] = TABLE4[ 7];
		TABLE3[11] = TABLE4[ 8];
		TABLE3[12] = TABLE4[ 7];
		TABLE3[13] = TABLE4[ 8];
		TABLE3[14] = TABLE4[ 9];
		TABLE3[15] = TABLE4[10];
		TABLE3[16] = TABLE4[11];
		TABLE3[17] = TABLE4[12];
		TABLE3[18] = TABLE4[11];
		TABLE3[19] = TABLE4[12];
		TABLE3[20] = TABLE4[13];
		TABLE3[21] = TABLE4[14];
		TABLE3[22] = TABLE4[15];
		TABLE3[23] = TABLE4[16];
		TABLE3[24] = TABLE4[15];
		TABLE3[25] = TABLE4[16];
		TABLE3[26] = TABLE4[17];
		TABLE3[27] = TABLE4[18];
		TABLE3[28] = TABLE4[19];
		TABLE3[29] = TABLE4[20];
		TABLE3[30] = TABLE4[19];
		TABLE3[31] = TABLE4[20];
		TABLE3[32] = TABLE4[21];
		TABLE3[33] = TABLE4[22];
		TABLE3[34] = TABLE4[23];
		TABLE3[35] = TABLE4[24];
		TABLE3[36] = TABLE4[23];
		TABLE3[37] = TABLE4[24];
		TABLE3[38] = TABLE4[25];
		TABLE3[39] = TABLE4[26];
		TABLE3[40] = TABLE4[27];
		TABLE3[41] = TABLE4[28];
		TABLE3[42] = TABLE4[27];
		TABLE3[43] = TABLE4[28];
		TABLE3[44] = TABLE4[29];
		TABLE3[45] = TABLE4[30];
		TABLE3[46] = TABLE4[31];
		TABLE3[47] = TABLE4[ 0];

		/* KS Function Begin */
		if (flg) {
			nbrofshift = shift[iter-1];
			for (i = 0; i < (int) nbrofshift; i++) {
				temp1 = TABLE2[0];
				temp2 = TABLE2[28];
				for (j = 0; j < 27; j++) {
					TABLE2[j] = TABLE2[j+1];
					TABLE2[j+28] = TABLE2[j+29];
				}
				TABLE2[27] = temp1;
				TABLE2[55] = temp2;
			}
		} else if (iter > 1) {
			nbrofshift = shift[17-iter];
			for (i = 0; i < (int) nbrofshift; i++) {
				temp1 = TABLE2[27];
				temp2 = TABLE2[55];
				for (j = 27; j > 0; j--) {
					TABLE2[j] = TABLE2[j-1];
					TABLE2[j+28] = TABLE2[j+27];
				}
				TABLE2[0] = temp1;
				TABLE2[28] = temp2;
			}
		}

		/* Permute TABLE2 - PC2 */
		TABLE5[ 0] = TABLE2[13];
		TABLE5[ 1] = TABLE2[16];
		TABLE5[ 2] = TABLE2[10];
		TABLE5[ 3] = TABLE2[23];
		TABLE5[ 4] = TABLE2[ 0];
		TABLE5[ 5] = TABLE2[ 4];
		TABLE5[ 6] = TABLE2[ 2];
		TABLE5[ 7] = TABLE2[27];
		TABLE5[ 8] = TABLE2[14];
		TABLE5[ 9] = TABLE2[ 5];
		TABLE5[10] = TABLE2[20];
		TABLE5[11] = TABLE2[ 9];
		TABLE5[12] = TABLE2[22];
		TABLE5[13] = TABLE2[18];
		TABLE5[14] = TABLE2[11];
		TABLE5[15] = TABLE2[ 3];
		TABLE5[16] = TABLE2[25];
		TABLE5[17] = TABLE2[ 7];
		TABLE5[18] = TABLE2[15];
		TABLE5[19] = TABLE2[ 6];
		TABLE5[20] = TABLE2[26];
		TABLE5[21] = TABLE2[19];
		TABLE5[22] = TABLE2[12];
		TABLE5[23] = TABLE2[ 1];
		TABLE5[24] = TABLE2[40];
		TABLE5[25] = TABLE2[51];
		TABLE5[26] = TABLE2[30];
		TABLE5[27] = TABLE2[36];
		TABLE5[28] = TABLE2[46];
		TABLE5[29] = TABLE2[54];
		TABLE5[30] = TABLE2[29];
		TABLE5[31] = TABLE2[39];
		TABLE5[32] = TABLE2[50];
		TABLE5[33] = TABLE2[44];
		TABLE5[34] = TABLE2[32];
		TABLE5[35] = TABLE2[47];
		TABLE5[36] = TABLE2[43];
		TABLE5[37] = TABLE2[48];
		TABLE5[38] = TABLE2[38];
		TABLE5[39] = TABLE2[55];
		TABLE5[40] = TABLE2[33];
		TABLE5[41] = TABLE2[52];
		TABLE5[42] = TABLE2[45];
		TABLE5[43] = TABLE2[41];
		TABLE5[44] = TABLE2[49];
		TABLE5[45] = TABLE2[35];
		TABLE5[46] = TABLE2[28];
		TABLE5[47] = TABLE2[31];
		/* KS Function End */

		/* TABLE3 XOR TABLE5 */
		for (i = 0; i < 48; i++)
			TABLE3[i] = TABLE3[i] ^ TABLE5[i];

		/* 8 s-functions */
		valindex = s1[2*TABLE3[ 0]+TABLE3[ 5]]
		    [2*(2*(2*TABLE3[ 1]+TABLE3[ 2])+
			       TABLE3[ 3])+TABLE3[ 4]];
		valindex = valindex * 4;
		TABLE5[ 0] = binary[0+valindex];
		TABLE5[ 1] = binary[1+valindex];
		TABLE5[ 2] = binary[2+valindex];
		TABLE5[ 3] = binary[3+valindex];
		valindex = s2[2*TABLE3[ 6]+TABLE3[11]]
		    [2*(2*(2*TABLE3[ 7]+TABLE3[ 8])+
			       TABLE3[ 9])+TABLE3[10]];
		valindex = valindex * 4;
		TABLE5[ 4] = binary[0+valindex];
		TABLE5[ 5] = binary[1+valindex];
		TABLE5[ 6] = binary[2+valindex];
		TABLE5[ 7] = binary[3+valindex];
		valindex = s3[2*TABLE3[12]+TABLE3[17]]
		    [2*(2*(2*TABLE3[13]+TABLE3[14])+
			       TABLE3[15])+TABLE3[16]];
		valindex = valindex * 4;
		TABLE5[ 8] = binary[0+valindex];
		TABLE5[ 9] = binary[1+valindex];
		TABLE5[10] = binary[2+valindex];
		TABLE5[11] = binary[3+valindex];
		valindex = s4[2*TABLE3[18]+TABLE3[23]]
		    [2*(2*(2*TABLE3[19]+TABLE3[20])+
			       TABLE3[21])+TABLE3[22]];
		valindex = valindex * 4;
		TABLE5[12] = binary[0+valindex];
		TABLE5[13] = binary[1+valindex];
		TABLE5[14] = binary[2+valindex];
		TABLE5[15] = binary[3+valindex];
		valindex = s5[2*TABLE3[24]+TABLE3[29]]
		    [2*(2*(2*TABLE3[25]+TABLE3[26])+
			       TABLE3[27])+TABLE3[28]];
		valindex = valindex * 4;
		TABLE5[16] = binary[0+valindex];
		TABLE5[17] = binary[1+valindex];
		TABLE5[18] = binary[2+valindex];
		TABLE5[19] = binary[3+valindex];
		valindex = s6[2*TABLE3[30]+TABLE3[35]]
		    [2*(2*(2*TABLE3[31]+TABLE3[32])+
			       TABLE3[33])+TABLE3[34]];
		valindex = valindex * 4;
		TABLE5[20] = binary[0+valindex];
		TABLE5[21] = binary[1+valindex];
		TABLE5[22] = binary[2+valindex];
		TABLE5[23] = binary[3+valindex];
		valindex = s7[2*TABLE3[36]+TABLE3[41]]
		    [2*(2*(2*TABLE3[37]+TABLE3[38])+
			       TABLE3[39])+TABLE3[40]];
		valindex = valindex * 4;
		TABLE5[24] = binary[0+valindex];
		TABLE5[25] = binary[1+valindex];
		TABLE5[26] = binary[2+valindex];
		TABLE5[27] = binary[3+valindex];
		valindex = s8[2*TABLE3[42]+TABLE3[47]]
		    [2*(2*(2*TABLE3[43]+TABLE3[44])+
			       TABLE3[45])+TABLE3[46]];
		valindex = valindex * 4;
		TABLE5[28] = binary[0+valindex];
		TABLE5[29] = binary[1+valindex];
		TABLE5[30] = binary[2+valindex];
		TABLE5[31] = binary[3+valindex];

		/* Permute - P */
		TABLE3[ 0] = TABLE5[15];
		TABLE3[ 1] = TABLE5[ 6];
		TABLE3[ 2] = TABLE5[19];
		TABLE3[ 3] = TABLE5[20];
		TABLE3[ 4] = TABLE5[28];
		TABLE3[ 5] = TABLE5[11];
		TABLE3[ 6] = TABLE5[27];
		TABLE3[ 7] = TABLE5[16];
		TABLE3[ 8] = TABLE5[ 0];
		TABLE3[ 9] = TABLE5[14];
		TABLE3[10] = TABLE5[22];
		TABLE3[11] = TABLE5[25];
		TABLE3[12] = TABLE5[ 4];
		TABLE3[13] = TABLE5[17];
		TABLE3[14] = TABLE5[30];
		TABLE3[15] = TABLE5[ 9];
		TABLE3[16] = TABLE5[ 1];
		TABLE3[17] = TABLE5[ 7];
		TABLE3[18] = TABLE5[23];
		TABLE3[19] = TABLE5[13];
		TABLE3[20] = TABLE5[31];
		TABLE3[21] = TABLE5[26];
		TABLE3[22] = TABLE5[ 2];
		TABLE3[23] = TABLE5[ 8];
		TABLE3[24] = TABLE5[18];
		TABLE3[25] = TABLE5[12];
		TABLE3[26] = TABLE5[29];
		TABLE3[27] = TABLE5[ 5];
		TABLE3[28] = TABLE5[21];
		TABLE3[29] = TABLE5[10];
		TABLE3[30] = TABLE5[ 3];
		TABLE3[31] = TABLE5[24];

		/* TABLE1 XOR TABLE3 */
		for (i = 0; i < 32; i++) {
			TABLE1[i+32] = TABLE1[i] ^ TABLE3[i];
			TABLE1[i] = TABLE4[i];
		}
	} /* End of Iter */

	/* Prepare Output */
	for (i = 0; i < 32; i++) {
		j = TABLE1[i];
		TABLE1[i] = TABLE1[32+i];
		TABLE1[32+i] = j;
	}

	/* Inverse Initial Permutation */
	TABLE4[ 0] = TABLE1[39];
	TABLE4[ 1] = TABLE1[ 7];
	TABLE4[ 2] = TABLE1[47];
	TABLE4[ 3] = TABLE1[15];
	TABLE4[ 4] = TABLE1[55];
	TABLE4[ 5] = TABLE1[23];
	TABLE4[ 6] = TABLE1[63];
	TABLE4[ 7] = TABLE1[31];
	TABLE4[ 8] = TABLE1[38];
	TABLE4[ 9] = TABLE1[ 6];
	TABLE4[10] = TABLE1[46];
	TABLE4[11] = TABLE1[14];
	TABLE4[12] = TABLE1[54];
	TABLE4[13] = TABLE1[22];
	TABLE4[14] = TABLE1[62];
	TABLE4[15] = TABLE1[30];
	TABLE4[16] = TABLE1[37];
	TABLE4[17] = TABLE1[ 5];
	TABLE4[18] = TABLE1[45];
	TABLE4[19] = TABLE1[13];
	TABLE4[20] = TABLE1[53];
	TABLE4[21] = TABLE1[21];
	TABLE4[22] = TABLE1[61];
	TABLE4[23] = TABLE1[29];
	TABLE4[24] = TABLE1[36];
	TABLE4[25] = TABLE1[ 4];
	TABLE4[26] = TABLE1[44];
	TABLE4[27] = TABLE1[12];
	TABLE4[28] = TABLE1[52];
	TABLE4[29] = TABLE1[20];
	TABLE4[30] = TABLE1[60];
	TABLE4[31] = TABLE1[28];
	TABLE4[32] = TABLE1[35];
	TABLE4[33] = TABLE1[ 3];
	TABLE4[34] = TABLE1[43];
	TABLE4[35] = TABLE1[11];
	TABLE4[36] = TABLE1[51];
	TABLE4[37] = TABLE1[19];
	TABLE4[38] = TABLE1[59];
	TABLE4[39] = TABLE1[27];
	TABLE4[40] = TABLE1[34];
	TABLE4[41] = TABLE1[ 2];
	TABLE4[42] = TABLE1[42];
	TABLE4[43] = TABLE1[10];
	TABLE4[44] = TABLE1[50];
	TABLE4[45] = TABLE1[18];
	TABLE4[46] = TABLE1[58];
	TABLE4[47] = TABLE1[26];
	TABLE4[48] = TABLE1[33];
	TABLE4[49] = TABLE1[ 1];
	TABLE4[50] = TABLE1[41];
	TABLE4[51] = TABLE1[ 9];
	TABLE4[52] = TABLE1[49];
	TABLE4[53] = TABLE1[17];
	TABLE4[54] = TABLE1[57];
	TABLE4[55] = TABLE1[25];
	TABLE4[56] = TABLE1[32];
	TABLE4[57] = TABLE1[ 0];
	TABLE4[58] = TABLE1[40];
	TABLE4[59] = TABLE1[ 8];
	TABLE4[60] = TABLE1[48];
	TABLE4[61] = TABLE1[16];
	TABLE4[62] = TABLE1[56];
	TABLE4[63] = TABLE1[24];

	j = 0;
	for (i = 0; i < 8; i++) {
		*(dest + i) = 0x00;
		for (k = 0; k < 7; k++)
			*(dest + i) = ((*(dest + i)) + TABLE4[j+k]) * 2;
		*(dest + i) = *(dest + i) + TABLE4[j+7];
		j += 8;
	}

}

int ExtAscii2Ascii(unsigned char *Src, unsigned char *Dest)
{
   unsigned char Ch;
   int i,Len;
   Len=strlen((char *)Src);
   if((Len%2)) return(-1);
   Len/=2;
   for(i=0;i<Len;++i) {
		Ch=Src[i*2]&0x0f;
		Ch<<=4;
	    Ch&=0xf0;
	    Dest[i]=Src[i*2+1]&0x0f;
	    Dest[i]|=Ch;
	}
	Dest[i]=0x00;
	return(i);
}

int Ascii2ExtAscii(unsigned char *Src, int Len, unsigned char *Dest)
{
   unsigned char Ch;
   int i;
   for(i=0;i<Len;++i) {
		Ch=Src[i]&0xf0;
		Ch>>=4;
		Dest[i*2]=Ch|0x30;
		Ch=Src[i]&0x0f;
		Dest[i*2+1]=Ch|0x30;
   }
   Dest[i*2]=0x00;
   return(i*2);
}

int DesData(char *Src, int Len, char *Dest, char *KEY)
/* 单DES加密需要下载的管理/加密密钥为密文数据 
	char *Src   输入管理/加密密钥明文
	char *Dest  输出加密后管理/加密密钥密文
	char *KEY   加密使用的密钥
*/
{
   unsigned char Key[64],s1[128],s2[128],s3[128];
   int i,k;
	for(i=0;i<8;++i) Key[i]=KEY[i];
	Key[8]=0x00;
	strcpy(Dest,"");
	k=0;
	while(Len>0) {
		if((Len-8)>=0) {
			for(i=0;i<8;++i) s1[i]=Src[k*8+i];
		}
		else {
			for(i=0;i<Len;++i) s1[i]=Src[k*8+i];
			for(;i<8;++i) s1[i]=0x00; }
		des(s1,s2,Key, 1);
		i=Ascii2ExtAscii(s2,8,s3);
		strcat(Dest,(char *)s3);
		Len-=8;
		++k;
	}
	return(i);
}

int UnDesData(char *Src, char *Dest, char *KEY)
/* 单DES加密需要下载的管理/加密密钥为密文数据 
	char *Src   输入管理/加密密钥明文
	char *Dest  输出加密后管理/加密密钥密文
	char *KEY   加密使用的密钥
*/
{
   unsigned char Key[64],s1[128],s2[128],s3[128];
   int i,k,Len;
	Len=ExtAscii2Ascii((unsigned char *)Src,s3);
	for(i=0;i<8;++i) Key[i]=KEY[i];
	Key[8]=0x00;
	k=0;
	strcpy(Dest,"");
	while(Len>0) {
		for(i=0;i<8;++i) s1[i]=s3[k*8+i];
		des(s1,s2,Key, 0);
		s2[8]=0x00;
		strcat(Dest,(char *)s2);
		Len-=8;
		++k;
	}
	return(i);
}

int GetCommData(char *Data,long TimeOut)
{
	int ircvlen;
	char ctemp[100];
	int T1,T2;
	CTime T;
    DWORD dwBytesWritten=0,dwBytesRead=0;
		T = CTime::GetCurrentTime();
		T1=T.GetMinute()*60+T.GetSecond();
		while(1) {
			T = CTime::GetCurrentTime();
			T2=T.GetMinute()*60+T.GetSecond();
			if(abs(T2-T1)>TimeOut || CancelFlag ==1 ) {
				ircvlen=-1; 
				goto Exit; }
			ReadFile(hCommDev,ctemp,99,&dwBytesRead,NULL);
			if(dwBytesRead<1) continue;
			ircvlen=0;
			while(dwBytesRead>0) {
				for(int loop=0;loop<(int)dwBytesRead;loop++)
					Data[ircvlen+loop]=ctemp[loop];
				ircvlen=ircvlen+dwBytesRead;
				dwBytesRead=0;
				Sleep(200);
				ReadFile(hCommDev,ctemp,99,&dwBytesRead,NULL);
			}
			break;
		}
		Data[ircvlen]=0x00;
Exit:
		return(ircvlen);
}



int GetPinPadType()
{
		DWORD dwBytesWritten=0;
		char cbuf[200];
		int iloop,ircvlen;
		WriteFile(hCommDev,"\x1b\x25K",3,&dwBytesWritten, NULL);
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1bW\x0D\x0A",4,&dwBytesWritten, NULL);
		ircvlen=GetCommData(cbuf,3);
		if(ircvlen<0) return(0);
		for(iloop=0;(iloop<ircvlen)&&(cbuf[iloop]!=0x02);iloop++);
		iloop++;
		if((cbuf[iloop]=='L' && cbuf[iloop+1]=='K')||cbuf[iloop]==0x06||cbuf[iloop]==0xFFFFFFFF)
		{
			m_bOpened = TRUE;
			BYTE cmd[3]={0x1B, 0x25, 0x42};
			WriteData(cmd, 3);
	
			int ret=0;
			ret=ICC_CardPresentEx();
			if(ret!=0&&ret!=1&&ret!=2){
				return(1);
			}else if(cbuf[iloop]=='L' && cbuf[iloop+1]=='K'){
				return(3);
			}else
				return(2);
		}
		return(0);

}

int InitKeyPad(int iComm, int iBuad)
{
	int i;
	if(OpenComm(iComm,iBuad)) {
		i=GetPinPadType();
		if(i) {
			m_Comport=iComm;
			BpKeyType=i;
		}
		CloseComm(); 
		return(i); 
	}
	return(0); 
}

int GetComport()
{
	CUIntArray ports;
	int i,j,Len,CommID;
	int	ret=-1;
	CString Temp;

	EnumerateSerialPorts(ports);
	Len=ports.GetSize()-1;
	Len=ports.ElementAt(Len);
	j=0;
	for (i=0; i<Len; i++) {
		if((i+1)==( int )ports.ElementAt(j)) {
			CommID=ports.ElementAt(j++);
//			Temp.Format("通信口=%d",CommID);
//			::AfxMessageBox(Temp,NULL,MB_OK);
			if(InitKeyPad(CommID,9600)) { ret=1; goto Exit; }
		}
	}
Exit:
//	Temp.Format("串口=%d 键盘类型=%d, 长度=%d",m_Comport,BpKeyType,Len);
//	::AfxMessageBox(Temp,NULL,MB_OK);
	return(ret); 
}

int ReadKeyData(HANDLE hCommDev, char *Data,long TimeOut)
{
	DWORD dwBytesRead=0;
	char buf[128];
	int ilen=0,i=0;
	int flag=0;
	ilen=GetCommData(buf,TimeOut);
	if(ilen>0) {
		for(i=0;i<ilen;++i) 
			if(buf[i]==0x02) break;
		if(i<ilen) {
			  flag=( int ) buf[i+1];
			  strcpy(Data,buf+i+2);
			  ilen=strlen(Data);
			  if(flag!=(int)strlen(Data)) ilen=-1;
		  }
		  else ilen=-1;
	}
	return(ilen);
}


int ModifyUserKeyM(HANDLE hCommDev, int M, int N, char *NewKey,long TimeOut)
/* 加密修改加密密钥
int M		管理密钥编号
int N		加密密钥编号
char *New	加密新加密密钥的密文
long outtime	超时时间
返回:
	<0		操作失败
*/
{
	DWORD dwBytesWritten=0;
	int i;
	char Buff[128];
    dwBytesWritten=0;
	sprintf(Buff,"\033u%c%c%16s",M,N,NewKey);
	WriteFile(hCommDev,Buff,0x14,&dwBytesWritten,NULL);
	i=ReadKeyData(hCommDev,Buff,TimeOut);
	if(strncmp(Buff,"OK",2)) i=-1;
	else i=1;
    return(i);
}

int CryptDataKey(HANDLE hCommDev, char M, char N, char Len, char *SrcData, char *Data,long TimeOut)
/* 使用加密键盘加密效验串
int M		管理密钥编号
int N		加密密钥编号
char Len	需要加密的数据长度
char *SrcData	需要加密的数据
char *Data	加密键盘返回的密文
long outtime	超时时间
返回:
	<0		操作失败
*/
{
	DWORD dwBytesWritten=0;
	int i;
	char Buff[128];
	sprintf(Buff,"\033A%c%c\015\012",M,N);
	WriteFile(hCommDev,Buff,6,&dwBytesWritten,NULL);
	Sleep(100);
	dwBytesWritten=0;
	sprintf(Buff,"\033H0%c%s\x0d\x0a",Len,SrcData);
	WriteFile(hCommDev,Buff,strlen(Buff),&dwBytesWritten,NULL);
	i=ReadKeyData(hCommDev,Data,TimeOut);
	if(i<0) return(-1);
    	return(1);
}



int DesInput(HANDLE hCommDev, int Msg, char *Data,long TimeOut)
/* 使用加密键盘加密输入密码
int Msg		0:提示请输入密码
			1：提示请再输入一次
char *Data	加密键盘返回的密文
long outtime	超时时间
返回:
	<0		操作失败
*/
{
	DWORD dwBytesWritten=0;
	int i;
	char Buff[128];
	if(Msg) sprintf(Buff,"\033F7");
	else sprintf(Buff,"\033F6");
	WriteFile(hCommDev,Buff,strlen(Buff),&dwBytesWritten,NULL);
	i=ReadKeyData(hCommDev,Data,TimeOut);
	if(i<0) {
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x30",2,&dwBytesWritten, NULL);
		return(-1);
	}
    return(1);
}


long SendToSDB(char *sendString,char *recvString,int iTimeOut,char *SendIP,char *SendPort,unsigned int sendBufferLen)
{
	SOCKET hClientSocket;
	struct sockaddr_in addrClient;
	char szHostIP[32], szHostPort[8];
	char szRecvBuf[6000];
	int nStatus, nActLen, nBufferLen;
    
	memset(szHostIP,0x00,sizeof(szHostIP));
	memset(szHostPort,0x00,sizeof(szHostPort));

    strcpy(szHostIP  ,SendIP);
	strcpy(szHostPort,SendPort);  

    
	//下面发送数据到HOST
	WORD wVersion;
	WSADATA wsaData;
	wVersion = MAKEWORD(2,0); // 使用Socket 2.0版本
	nStatus = WSAStartup(wVersion,&wsaData); // 启动Windows Sockets DLL,协商版本支持
	if(nStatus != 0) 
	{
	   WriteDiary("通讯错误！","Diary.log");
	   return -90;
	}

	hClientSocket = socket(AF_INET, SOCK_STREAM, 0); // 建立socket套接字
	if(hClientSocket == SOCKET_ERROR) 
	{
	   WSACleanup();
	   WriteDiary("套接字建立失败，通讯错误！","Diary.log");
	   return -90;
	}

	addrClient.sin_family = AF_INET;
	addrClient.sin_addr.s_addr = inet_addr((char *)&szHostIP);
	addrClient.sin_port = htons( short(atoi(szHostPort)) );
	nStatus = connect(hClientSocket, (struct sockaddr *) &addrClient,sizeof(addrClient)); // 连接服务器
	if(nStatus != 0) 
	{
	   closesocket(hClientSocket);
	   WSACleanup();
	   WriteDiary(szHostIP,"Diary.log");
	   WriteDiary(szHostPort,"Diary.log");
	   WriteDiary("连接服务器失败，通讯错误！","Diary.log");
	   return -90;
	}
    WriteDiary("开始发送交易数据......","Diary.log");

	nBufferLen=sendBufferLen;
	nActLen = send(hClientSocket, sendString, nBufferLen, 0);

	fd_set fdReadSet;
	timeval tvTimeOut;
	FD_ZERO(&fdReadSet); // 设置接收超时时间
	FD_SET(hClientSocket, &fdReadSet);
	tvTimeOut.tv_sec = iTimeOut; // 超时秒数
	tvTimeOut.tv_usec = 0;
	nStatus = select(hClientSocket, &fdReadSet, NULL, NULL, &tvTimeOut);
	if(nStatus==-1) 
	{
	   closesocket(hClientSocket);
	   WSACleanup();
	   WriteDiary("数据发送失败！通讯错误！","Diary.log");
	   return -90;

	}
	if(nStatus==0) 
	{
	   WriteDiary("通讯超时错误！","Diary.log");
	   WriteDiary(szHostIP,"Diary.log");
	   WriteDiary(szHostPort,"Diary.log");
       closesocket(hClientSocket);
	   WSACleanup();
	   return -90;
	}
	memset(szRecvBuf, 0x00, sizeof(szRecvBuf)); // 初始化接收缓冲区
	nBufferLen=1024;
	if(FD_ISSET(hClientSocket, &fdReadSet))     // 如果是读的话
	{
	    nActLen = recv(hClientSocket, szRecvBuf, nBufferLen, 0); // 接收返回数据
		
		if(nActLen < 0)
		{
		   // 接收失败
		   closesocket(hClientSocket);
		   WSACleanup();
		   WriteDiary("接收数据失败！" + CString(szRecvBuf) ,"Diary.log");
		   return -90;
		} 
	
    }

	nStatus = closesocket(hClientSocket); // 传送完毕，关闭socket
	memcpy(recvString,szRecvBuf,strlen(szRecvBuf)); 
	if(nStatus != 0)  
	{
	   WriteDiary("销毁套接字失败！","Diary.log");
	}
	WSACleanup(); 
    if(sendString[0]==szRecvBuf[0] && sendString[1]==szRecvBuf[1] && sendString[2]==szRecvBuf[2] && sendString[3]==szRecvBuf[3] &&
	   sendString[4]==szRecvBuf[4] && sendString[5]==szRecvBuf[5] && sendString[6]==szRecvBuf[6] && sendString[7]==szRecvBuf[7] &&
	   sendString[8]==szRecvBuf[8] && sendString[9]==szRecvBuf[9] )
	{
	   WriteDiary("交易失败！原包返回！","Diary.log");
       return -90;	  
	}
	return (0);
}

long  AnalyzeP(char * IPacket,char * OPacket,char * IpayName,int IPassLen ,int Irandnum)
{
	char ReturnValue[3],recvString[6000],serialNo[15],Account[30];
	char cBankReturnVouNo1[500],cBankReturnVouNo2[500];
    char ReturnCode[10];
	char CheckCard6[7];
	char termNO[16],tranDate[9],tranTime[7],vouNO[11],unitNO[21];
	int  i,j;
	memset(ReturnValue,0x00,sizeof(ReturnValue));
	memset(recvString,0x00,sizeof(recvString));
    memset(serialNo,0x00,sizeof(serialNo));
	memset(Account,0x00,sizeof(Account));
    memset(cBankReturnVouNo2,0x00,sizeof(cBankReturnVouNo2));
	memset(cBankReturnVouNo1,0x00,sizeof(cBankReturnVouNo1));
	memset(ReturnCode,0x00,sizeof(ReturnCode));
    memset(CheckCard6,0x00,sizeof(CheckCard6));
	memset(termNO,0x00,sizeof(termNO));
	memset(tranDate,0x00,sizeof(tranDate));
	memset(tranTime,0x00,sizeof(tranTime));
	memset(vouNO,0x00,sizeof(vouNO));
	memset(unitNO,0x00,sizeof(unitNO));

	memcpy(recvString,IPacket,strlen(IPacket));

	//新的返回包解析
	char cardNO[20],transDate[9],transTime[7],traceNO[7],batchNO[7],authNO[7],refNO[9],validDate[5],transMoney[13];
	memset(cardNO,0x00,sizeof(cardNO));
	memset(transDate,0x00,sizeof(transDate));
	memset(transTime,0x00,sizeof(transTime));
	memset(traceNO,0x00,sizeof(traceNO));
	memset(batchNO,0x00,sizeof(batchNO));
	memset(authNO,0x00,sizeof(authNO));
	memset(refNO,0x00,sizeof(refNO));
	memset(validDate,0x00,sizeof(validDate));
	memset(transMoney,0x00,sizeof(transMoney));

	memcpy(recvString,IPacket,strlen(IPacket));
	
	//返回码
    for(i=0;i<2;i++)
	{
	   ReturnValue[i] = recvString[i];
	}
	
	//账号
	j=0;
    for(i=2;i<21;i++)
	{
	   if ( recvString[i] != ' ')
	   {
	      Account[j] = recvString[i];
		  j++;
	   }
	}
	
	//交易日期
	j=0;
    for(i=21;i<29;i++)
	{
	   if(recvString[i] != ' ')
	   {			
		   transDate[j] = recvString[i];
		   j++;
	   }
	}
	char printDate[11];
	memset(printDate, 0x00, sizeof(printDate));
	ToPrintDate(transDate,printDate);
	p_TransDate.Format("%s",printDate);
	WriteDiary("交易日期:" + p_TransDate,"Diary.log");
	//交易时间
	j=0;
	for(i=29;i<35;i++)
	{
	   if(recvString[i] != ' ')
	   {

		   transTime[j] = recvString[i];
		   j++;
	   }
	}
	char printTime[9];
	memset(printTime, 0x00, sizeof(printTime));
	ToPrintTime(transTime,printTime);
	p_TransTime.Format("%s",printTime);
	WriteDiary("交易时间：" + p_TransTime,"Diary.log");
	//终端流水号
	j=0;
    for(i=35;i<41;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   traceNO[j] = recvString[i];
		   j++;
	   }
	}
	p_TraceNO.Format("%s",traceNO);
    WriteDiary("终端流水号：" + p_TraceNO,"Diary.log");

	strcpy(cardNO,Account);

	unsigned int iCNo;
	for(iCNo=6;iCNo<12;iCNo++)
	{
	   cardNO[iCNo] ='*';
	}
	p_CardNO.Format("%s",cardNO);
	WriteDiary("获得扣款交易卡号：" + p_CardNO,"Diary.log");
	
   	//批次号
	j=0;
    for(i=41;i<47;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   batchNO[j] = recvString[i];
		   j++;
	   }
	}
	p_BatchNO.Format("%s",batchNO);
	WriteDiary("批次号：" + p_BatchNO,"Diary.log");
	//授权号 
	j=0;
    for(i=47;i<53;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   authNO[j] = recvString[i];
		   j++;
	   }
	}
	p_AuthNO.Format("%s",authNO);
	WriteDiary("授权号：" + p_AuthNO,"Diary.log");
	//检索参考号 
	j=0;
    for(i=53;i<61;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   refNO[j] = recvString[i];
		   j++;
	   }
	}
	p_RefNO.Format("%s",refNO);
	WriteDiary("检索参考号：" + p_RefNO,"Diary.log");
	//卡片有效期 
	j=0;
    for(i=61;i<65;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   validDate[j] = recvString[i];
		   j++;
	   }
	}
	p_ValidDate.Format("%s",validDate);
	WriteDiary("卡片有效期：" + p_ValidDate,"Diary.log");
	//交易金额 
	j=0;
    for(i=65;i<77;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   transMoney[j] = recvString[i];
		   j++;
	   }
	}
	char printMoney[13];
	memset(printMoney, 0x00, sizeof(printMoney));
	ToPrintMoney(transMoney,printMoney);
	p_TransMoney.Format("%s",printMoney);
	WriteDiary("交易金额：" + p_TransMoney,"Diary.log");

	if (CString(ReturnValue) == "00")
	{
		WriteDiary("交易数据返回成功！！","Diary.log");
		strcpy(m_OutPacket,recvString);
		FReturnVal = 0;
		prtFlag = 0;
		return (0);
	} 
	else if (ReturnValue[0] == '-')
	{
	    WriteDiary("银行返回交易失败代码：" + CString (ReturnCode),"Diary.log");
		if (CString(ReturnValue) == "-1") 
			strcpy(m_OutPacket,"-1余额不足！");  
		else if (CString(ReturnValue) == "-3") 
			strcpy(m_OutPacket,"-3无效卡号！");   
		else if (CString(ReturnValue) == "-8")
			strcpy(m_OutPacket,"-8交易密码错误或输入密码错误次数过多！"); 
		else if (CString(ReturnValue) == "-9")
			strcpy(m_OutPacket,"-9其他错误！"); 
		else
			strcpy(m_OutPacket,"-0未知错误！"); 
	}
	else if (CString(ReturnValue)=="05")
	{
		strcpy(m_OutPacket,"05扣款超时或扣款失败！"); 
	}
	else if (CString(ReturnValue)=="06")
	{
		strcpy(m_OutPacket,"06扣款超时/扣款失败！"); 
	}
	else if (CString(ReturnValue)=="10")
	{
		strcpy(m_OutPacket,"10该票据已缴费！"); 
	}
	else if (CString(ReturnValue)=="98")
	{
		strcpy(m_OutPacket,"98密码错误！"); 
	}
	else if (CString(ReturnValue)=="99")
	{
		strcpy(m_OutPacket,"99付款账户为空！"); 
	}
	else
	{
		strcpy(m_OutPacket,"-0未知错误！"); 
	}

// 	WriteDiary("返回数据包：" + CString (m_OutPacket),"Diary.log");
	return (-1);
}

long  AnalyzeQP(char * IPacket,char * OPacket,char * IpayName)
{
	char ReturnValue[6000],recvString[6000],ReturnMXValue[6000],serialNo[20],BankLogNo[20];
	char cBankReturnVouNo1[500];
    char ReturnCode[10];
	char CheckCard6[7];
	int  i=0,j=0,k=0,l=0,flag =0,m=0,n=0,o=0,p=0;
	memset(ReturnValue,0x00,sizeof(ReturnValue));
	memset(ReturnMXValue,0x00,sizeof(ReturnMXValue));
	memset(recvString,0x00,sizeof(recvString));
    memset(serialNo,0x00,sizeof(serialNo));
	memset(BankLogNo,0x00,sizeof(BankLogNo));
	memset(cBankReturnVouNo1,0x00,sizeof(cBankReturnVouNo1));
	memset(ReturnCode,0x00,sizeof(ReturnCode));
    memset(CheckCard6,0x00,sizeof(CheckCard6));
	memcpy(recvString,IPacket,strlen(IPacket));
    int nLenR = strlen(IPacket);

    //返回包包头84位,第6－9 (xx0000表示成功应答)  20100608 By LFT
    for(i=0;i<=nLenR;i++)
	{
	   ReturnValue[j] = recvString[i];
	   if(i >= 6 && i <= 9 )
	   {  
	      serialNo[k] = recvString[i];
	      k++;

	   }
	   else
	   {
	   	   serialNo[k] =0x00;
	   }
	}
   //WriteDiary("返回包:" + CString(recvString),"Diary.log");
   //WriteDiary("长度" + CString(serialNo),"Diary.log");


   l = 0;
   for(i=0;i<=nLenR;i++)
	{
	   if(i >= 84 && i <= 86 )    // 非税返回交易码 100成功，107 未找到缴款信息
	   {  
	      BankLogNo[l] = recvString[i];
	      l++;
	   }
	   else
	   {
	   	   BankLogNo[l] =0x00;
	   }
	}

    j=0;
    for(i=4;i<=9;i++)
	{
	   ReturnValue[j] = recvString[i];
	   j++;
	}

   WriteDiary("返回结果代码:" + CString(ReturnValue) + "非税返回代码:" + CString(BankLogNo),"Diary.log");
   cm_OutFSCode.Format("%s",ReturnValue);
   
   if ( CString(serialNo) == "0000" && CString(BankLogNo) == "107" )   //对比通知单号码
   {
	  WriteDiary("没有查找到该缴款通知书!!" + CString(IpayName),"Diary.log");

	  //FReturnVal = 290;
	  return 290;
	}
   else
   {
	  if ( CString(recvString[3]) != "N" )      // 包头第3位定义N 正常返回，否则其它错误。
	  {
	 	  WriteDiary("其他错误!!" + cm_OutFSCode,"Diary.log");
		  return 291;
	  }
	}
    

	//////通知单号码
	m = 0;
    for(i=0;i<=nLenR;i++)
	{
	   if(i >=89 && i <= 118 ) 
	   {  
		  if (recvString[i] != ' ')
		  {
	         cBankReturnVouNo1[m] = recvString[i];
	         m++;
		  }
	   }
	   else
	   {
	   	   cBankReturnVouNo1[m] =0x00;
	   }
	}
    WriteDiary("通知单号码：" + CString(cBankReturnVouNo1),"Diary.log");

  
    int cVoucmpVal  = strcmp(IpayName,cBankReturnVouNo1);

	if ( cVoucmpVal !=0  || cBankReturnVouNo1[0]==0x00 )   //对比通知单号码
	{
	  WriteDiary("交易数据返回错误！数据包不匹配！交易失败！" + CString(IpayName),"Diary.log");
	  FReturnVal =-90;
	  return -90;
	}
	else
	{
		  WriteDiary("交易数据包匹配成功！","Diary.log");
	}

//////////创建返回包///////////////////////////
	p=0;
    for(i=84;i<=nLenR;i++)
	{
	   if(recvString[i] != '@')
	   {
		   ReturnMXValue[p] = recvString[i];
	   }
	   else
	   {
	      ReturnMXValue[p] =0x00;
	   }
	   p++;
	}

   char ReturnStr[5000];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,ReturnMXValue,strlen(ReturnMXValue));
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
///////////////////////////////////////////////
   return (0);
}

long  AnalyzeVP(char * IPacket,char * OPacket,char * IpayName)
{
	char recvString[6000],serialNo[20];
	char cTzdNo[100],cFrontLogNo[20];
	int  i=0,k=0,flag =0,m=0;
		
	memset(recvString,0x00,sizeof(recvString));
    memset(serialNo,0x00,sizeof(serialNo));
	memset(cTzdNo,0x00,sizeof(cTzdNo));
	memset(cFrontLogNo,0x00,sizeof(cFrontLogNo));
	memcpy(recvString,IPacket,strlen(IPacket));
    int nLenR = strlen(IPacket);
	
    //返回包包头84位,第6－9 (xx0000表示成功应答)  20100608 By LFT
    for(i=4;i<10;i++)
	{
      serialNo[k] = recvString[i];
      k++;
	}
   
   WriteDiary("非税返回结果代码:" + CString(serialNo),"Diary.log");

   if ( CString(&serialNo[2]) == "0000" )   //对比通知单号码
   {
      
	    WriteDiary("缴费确认应答成功!" + CString(IpayName),"Diary.log");
	    flag = 0;
  		//////返回包通知单号码
		m = 0;
		for(i=116; i<146;i++)
		{
		  if (recvString[i] != ' ')
		  {
			 cTzdNo[m] = recvString[i];
			 m++;
		  }
		}
		WriteDiary("缴费确认通知单号码：" + CString(cTzdNo),"Diary.log");


   		//////返回前置流水号
		m = 0;
		for(i=84;i<98;i++)
		{
		  if (recvString[i] != ' ')
		  {
			 cFrontLogNo[m] = recvString[i];
			 m++;
		  }
		}
		WriteDiary("前置流水号码：" + CString(cFrontLogNo),"Diary.log");

  
		int cVoucmpVal  = strcmp(IpayName,cTzdNo);

		if ( cVoucmpVal !=0  || cTzdNo[0]==0x00 )   //对比通知单号码
		{
		  WriteDiary("交易数据返回错误！数据包不匹配！交易失败！" + CString(IpayName),"Diary.log");
		  if ( flag == 0)
		  {
			 strcpy(serialNo,"ERRCHK");
			 return -90;
		  }
		}
		else
		{
			  WriteDiary("交易数据包匹配成功！","Diary.log");
		}
    }
    else
		flag =-1;
//////////创建返回包///////////////////////////
   char ReturnStr[100];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,serialNo,strlen(serialNo));
   memcpy(ReturnStr + strlen(serialNo),cFrontLogNo,strlen(cFrontLogNo));
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
///////////////////////////////////////////////

   return (flag);
}

long  AnalyzeCZR(char * IPacket,char * OPacket,char * IpayName)
{
	char recvString[6000],serialNo[20];
	char cTzdNo[100],cFrontLogNo[20];
	int  i=0,k=0,flag =0,m=0;
		
	memset(recvString,0x00,sizeof(recvString));
    memset(serialNo,0x00,sizeof(serialNo));
	memset(cTzdNo,0x00,sizeof(cTzdNo));
	memset(cFrontLogNo,0x00,sizeof(cFrontLogNo));
	memcpy(recvString,IPacket,strlen(IPacket));
    int nLenR = strlen(IPacket);
	
    //返回包包头84位,第6－9 (xx0000表示成功应答)  20100608 By LFT
    for(i=4;i<10;i++)
	{
      serialNo[k] = recvString[i];
      k++;
	}
   
	//WriteDiary("返回包:" + CString(recvString),"Diary.log");
 
   WriteDiary("返回冲正请求结果代码:" + CString(serialNo),"Diary.log");

   if ( CString(&serialNo[2]) == "0000" )   //对比通知单号码
   {
        flag =0;
	    WriteDiary("冲正确认应答成功!" + CString(IpayName),"Diary.log");
   		//////返回包通知单号码
		m = 0;
		for(i=84; i<114;i++)
		{
		  if (recvString[i] != ' ')
		  {
			 cTzdNo[m] = recvString[i];
			 m++;
		  }
		}

		WriteDiary("缴费确认通知单号码：" + CString(cTzdNo),"Diary.log");


   		//////返回前置流水号
		m = 0;
		for(i=134;i<148;i++)
		{
		  if (recvString[i] != ' ')
		  {
			 cFrontLogNo[m] = recvString[i];
			 m++;
		  }
		}
		WriteDiary("原前置流水号码：" + CString(cFrontLogNo),"Diary.log");

  
		int cVoucmpVal  = strcmp(IpayName,cTzdNo);

		if ( cVoucmpVal !=0  || cTzdNo[0]==0x00 )   //对比通知单号码
		{
		  WriteDiary("交易数据返回错误！数据包不匹配！交易失败！" + CString(IpayName),"Diary.log");
		  if ( flag == 0)
		  {
			 strcpy(serialNo,"ERRCHK");
			 return -90;
		  }
		}
		else
		{
			  WriteDiary("交易数据包匹配成功！","Diary.log");
		}
 
   }
   else
	   flag = -1;
//////////创建返回包///////////////////////////
   char ReturnStr[100];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,serialNo,strlen(serialNo));
   memcpy(ReturnStr + strlen(serialNo),cFrontLogNo,strlen(cFrontLogNo));
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
///////////////////////////////////////////////

   return (flag);
}



long  AnalyzeCZP(char * IPacket,char * OPacket,char * IpayName)
{
	char ReturnValue[3],recvString[1024],serialNo[20],BankLogNo[20],Account[20];
	char cBankReturnVouNo1[500];
    char ReturnCode[20];
	char CheckCard6[7];
	char termNO[16],tranDate[9],tranTime[7],vouNO[11],unitNO[21];
	int  i=0,j=0,k=0;
	memset(ReturnValue,0x00,sizeof(ReturnValue));
	memset(recvString,0x00,sizeof(recvString));
    memset(serialNo,0x00,sizeof(serialNo));
	memset(BankLogNo,0x00,sizeof(BankLogNo));
	memset(cBankReturnVouNo1,0x00,sizeof(cBankReturnVouNo1));
	memset(ReturnCode,0x00,sizeof(ReturnCode));
    memset(CheckCard6,0x00,sizeof(CheckCard6));
	memset(termNO,0x00,sizeof(termNO));
	memset(tranDate,0x00,sizeof(tranDate));
	memset(tranTime,0x00,sizeof(tranTime));
	memset(vouNO,0x00,sizeof(vouNO));
	memset(unitNO,0x00,sizeof(unitNO));
	memset(Account,0x00,sizeof(Account));

//    WriteDiary("返回数据包：" + CString (IPacket),"Diary.log");
	memcpy(recvString,IPacket,strlen(IPacket));
    int nLenR = strlen(IPacket);

	//新的返回包解析
	char cardNO[20],transDate[9],transTime[7],traceNO[7],batchNO[7],authNO[7],refNO[9],validDate[5],transMoney[13];
	memset(cardNO,0x00,sizeof(cardNO));
	memset(transDate,0x00,sizeof(transDate));
	memset(transTime,0x00,sizeof(transTime));
	memset(traceNO,0x00,sizeof(traceNO));
	memset(batchNO,0x00,sizeof(batchNO));
	memset(authNO,0x00,sizeof(authNO));
	memset(refNO,0x00,sizeof(refNO));
	memset(validDate,0x00,sizeof(validDate));
	memset(transMoney,0x00,sizeof(transMoney));

	memcpy(recvString,IPacket,strlen(IPacket));
	
	//返回码
    for(i=0;i<2;i++)
	{
	   ReturnValue[i] = recvString[i];
	}
	
	//账号
	j=0;
    for(i=2;i<21;i++)
	{
	   if ( recvString[i] != ' ')
	   {
	      Account[j] = recvString[i];
		  j++;
	   }
	}
	
	//交易日期
	j=0;
    for(i=21;i<29;i++)
	{
	   if(recvString[i] != ' ')
	   {			
		   transDate[j] = recvString[i];
		   j++;
	   }
	}
	char printDate[11];
	memset(printDate, 0x00, sizeof(printDate));
	ToPrintDate(transDate,printDate);
	p_TransDate.Format("%s",printDate);
	WriteDiary("交易日期:" + p_TransDate,"Diary.log");
	//交易时间
	j=0;
	for(i=29;i<35;i++)
	{
	   if(recvString[i] != ' ')
	   {

		   transTime[j] = recvString[i];
		   j++;
	   }
	}
	char printTime[9];
	memset(printTime, 0x00, sizeof(printTime));
	ToPrintTime(transTime,printTime);
	p_TransTime.Format("%s",printTime);
	WriteDiary("交易时间：" + p_TransTime,"Diary.log");
	//终端流水号
	j=0;
    for(i=35;i<41;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   traceNO[j] = recvString[i];
		   j++;
	   }
	}
	p_TraceNO.Format("%s",traceNO);
    WriteDiary("终端流水号：" + p_TraceNO,"Diary.log");

	strcpy(cardNO,Account);

	unsigned int iCNo;
	for(iCNo=6;iCNo<12;iCNo++)
	{
	   cardNO[iCNo] ='*';
	}
	p_CardNO.Format("%s",cardNO);
	WriteDiary("获得扣款交易卡号：" + p_CardNO,"Diary.log");
	
   	//批次号
	j=0;
    for(i=41;i<47;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   batchNO[j] = recvString[i];
		   j++;
	   }
	}
	p_BatchNO.Format("%s",batchNO);
	WriteDiary("批次号：" + p_BatchNO,"Diary.log");
	//授权号 
	j=0;
    for(i=47;i<53;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   authNO[j] = recvString[i];
		   j++;
	   }
	}
	p_AuthNO.Format("%s",authNO);
	WriteDiary("授权号：" + p_AuthNO,"Diary.log");
	//检索参考号 
	j=0;
    for(i=53;i<61;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   refNO[j] = recvString[i];
		   j++;
	   }
	}
	p_RefNO.Format("%s",refNO);
	WriteDiary("检索参考号：" + p_RefNO,"Diary.log");
	//卡片有效期 
	j=0;
    for(i=61;i<65;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   validDate[j] = recvString[i];
		   j++;
	   }
	}
	p_ValidDate.Format("%s",validDate);
	WriteDiary("卡片有效期：" + p_ValidDate,"Diary.log");
	//交易金额 
	j=0;
    for(i=65;i<77;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   transMoney[j] = recvString[i];
		   j++;
	   }
	}
	char printMoney[13];
	memset(printMoney, 0x00, sizeof(printMoney));
	ToPrintMoney(transMoney,printMoney);
	p_TransMoney.Format("%s",printMoney);
	WriteDiary("交易金额：" + p_TransMoney,"Diary.log");
	
	if (CString(ReturnValue)=="00")
	{
		WriteDiary("交易数据返回成功！！","Diary.log");
		//////////创建返回包///////////////////////////
// 		char ReturnStr[100];
// 		memset(ReturnStr,0x00,sizeof(ReturnStr));
// 		memcpy(ReturnStr,recvString,strlen(recvString));
// 		memcpy(ReturnStr + strlen(recvString),m_MerchantNO,strlen(m_MerchantNO));
// 		memcpy(OPacket,ReturnStr,strlen(ReturnStr));
		
	   	strcpy(OPacket,recvString);
// 		WriteDiary("返回数据包：" + CString (OPacket),"Diary.log");
		return (0);
	}
	else
	{
		if ( CString(ReturnValue)=="02" )
		{
			strcpy(OPacket,"02冲正异常或失败！");
		}
		else if ( CString(ReturnValue)=="14" )
		{
			strcpy(OPacket,"14原始交易不存在！");
		}
		else
		{
			strcpy(OPacket,"-0未知错误！");
		}		

		return(-1);
	}


//////////创建返回包///////////////////////////
/*   char ReturnStr[100];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,sReturnCode,strlen(sReturnCode));
   memcpy(ReturnStr + strlen(sReturnCode),BankLogNo,strlen(BankLogNo));
   
   memcpy(ReturnStr + strlen(sReturnCode) + strlen(BankLogNo),cBankReturnVouNo1,strlen(cBankReturnVouNo1));
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
   
 */
}


long  AnalyzeVou(char * IPacket,char * OPacket,char * IpayName)
{
	char ReturnValue[60],recvString[6000],bankReturnCode[10];
	char tzdNO[50];
	int  i=0,j=0;

	memset(ReturnValue,0x00,sizeof(ReturnValue));
	memset(recvString,0x00,sizeof(recvString));
    memset(tzdNO,0x00,sizeof(tzdNO));
	memset(bankReturnCode,0x00,sizeof(bankReturnCode));
	    
	memcpy(recvString,IPacket,strlen(IPacket));
    int nLenR = strlen(IPacket);

    //返回包包头84位,第4－9 (xx0000表示成功应答)  20100608 By LFT
    j=0;
    for(i=4;i<=9;i++)
	{
	   ReturnValue[j] = recvString[i];
	   j++;
	}

    
    cm_OutFSCode.Format("%s",ReturnValue);
	strcpy(bankReturnCode,&ReturnValue[2]);
    WriteDiary("返回结果代码:" + CString(bankReturnCode),"Diary.log");
    if ( CString(bankReturnCode) == "0000" ) 
	{
		//////通知单号码
		j = 0;
		for(i=116;i<146;i++)
		{
		   if (recvString[i] != ' ')
		   {
			  tzdNO[j] = recvString[i];
			  j++;
		   }
		}
		WriteDiary("银行返回通知单号码：" + CString(tzdNO),"Diary.log");

    
		int cVoucmpVal  = strcmp(IpayName,tzdNO);

		if ( cVoucmpVal !=0  || tzdNO[0]==0x00 )   //对比通知单号码
		{
		  WriteDiary("交易数据返回错误！数据包不匹配！交易失败！" + CString(IpayName),"Diary.log");
		  FReturnVal =-90;
		  return -90;
		}
		else
		{
			  WriteDiary("交易数据包匹配成功！","Diary.log");
		}
	}
//////////创建返回包///////////////////////////
   char ReturnStr[20];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,ReturnValue,strlen(ReturnValue));
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
///////////////////////////////////////////////
   return (0);
}

long  AnalyzeinVaildVou(char * IPacket,char * OPacket,char * IpayName)
{
	char ReturnValue[60],recvString[6000],bankReturnCode[10];
	char tzdNO[50];
	int  i=0,j=0;

	memset(ReturnValue,0x00,sizeof(ReturnValue));
	memset(recvString,0x00,sizeof(recvString));
    memset(tzdNO,0x00,sizeof(tzdNO));
	memset(bankReturnCode,0x00,sizeof(bankReturnCode));
	    
	memcpy(recvString,IPacket,strlen(IPacket));
    int nLenR = strlen(IPacket);

    //返回包包头84位,第4－9 (xx0000表示成功应答)  20100608 By LFT
    j=0;
    for(i=4;i<=9;i++)
	{
	   ReturnValue[j] = recvString[i];
	   j++;
	}
    
    cm_OutFSCode.Format("%s",ReturnValue);
	strcpy(bankReturnCode,&ReturnValue[2]);
    WriteDiary("返回结果代码:" + CString(bankReturnCode),"Diary.log");
  ////////创建返回包///////////////////////////
   char ReturnStr[20];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,ReturnValue,strlen(ReturnValue));
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
///////////////////////////////////////////////
   return (0);
}


long  AnalyzeQPayed(char * IPacket,char * OPacket,char * IpayName)
{
	char recvString[6000],serialNo[20];
	char cTzdNo[100],cFrontLogNo[20];
	int  i=0,k=0,flag =0,m=0;
		
	memset(recvString,0x00,sizeof(recvString));
    memset(serialNo,0x00,sizeof(serialNo));
	memset(cTzdNo,0x00,sizeof(cTzdNo));
	memset(cFrontLogNo,0x00,sizeof(cFrontLogNo));
	memcpy(recvString,IPacket,strlen(IPacket));
    int nLenR = strlen(IPacket);
	//WriteDiary("返回包:" + CString(IPacket),"Diary.log");
	
    //返回包包头84位,第6－9 (xx0000表示成功应答)  20100608 By LFT
    for(i=4;i<10;i++)
	{
      serialNo[k] = recvString[i];
      k++;
	}
   
    WriteDiary("返回结果代码:" + CString(serialNo),"Diary.log");
    
    if (CString(&serialNo[2]) == "0000")
	{
   		//////返回包通知单号码
		flag =0;
		m = 0;
		for(i=426; i<456;i++)
		{
		  if (recvString[i] != ' ')
		  {
			 cTzdNo[m] = recvString[i];
			 m++;
		  }
		}
		WriteDiary("缴费确认通知单号码：" + CString(cTzdNo),"Diary.log");


   		//////返回前置流水号
		m = 0;
		for(i=478;i<492;i++)
		{
		  if (recvString[i] != ' ')
		  {
			 cFrontLogNo[m] = recvString[i];
			 m++;
		  }
		}
		WriteDiary("前置流水号码：" + CString(cFrontLogNo),"Diary.log");


		int cVoucmpVal  = strcmp(IpayName,cTzdNo);

		if ( cVoucmpVal !=0  || cTzdNo[0]==0x00 )   //对比通知单号码
		{
		  WriteDiary("交易数据返回错误！数据包不匹配！交易失败！" + CString(IpayName),"Diary.log");
		}
		else
		{
		  WriteDiary("交易数据包匹配成功！","Diary.log");
		}
 
	}
	else 
		flag =-1;
//////////创建返回包///////////////////////////
//   char ReturnStr[100];
//   memset(ReturnStr,0x00,sizeof(ReturnStr));
//   memcpy(ReturnStr,serialNo,strlen(serialNo));
//   memcpy(ReturnStr + strlen(serialNo),cFrontLogNo,strlen(cFrontLogNo));
//   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
///////////////////////////////////////////////
//////////创建返回包///////////////////////////
	int p=0;
	char ReturnMXValue[5000];
	memset(ReturnMXValue,0x00,sizeof(ReturnMXValue));
    for(i=423;i<=nLenR;i++)
	{
	   if(recvString[i] != '@')
	   {
		   ReturnMXValue[p] = recvString[i];
	   }
	   else
	   {
	      ReturnMXValue[p] =0x00;
	   }
	   p++;
	}

   char ReturnStr[6000];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,serialNo,strlen(serialNo));
   memcpy(ReturnStr + strlen(serialNo),ReturnMXValue,strlen(ReturnMXValue));
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));
///////////////////////////////////////////////
   return (flag);
}

long  AnalyzeRPP(char * IPacket,char * OPacket,char * IpayName)
{
	char ReturnValue[3],recvString[1024],serialNo[20],BankLogNo[20],Account[20];
	char cBankReturnVouNo1[500];
    char ReturnCode[20];
	char CheckCard6[7];
	char termNO[16],tranDate[9],tranTime[7],vouNO[11],unitNO[21];
	int  i=0,j=0;
	memset(ReturnValue,0x00,sizeof(ReturnValue));
	memset(recvString,0x00,sizeof(recvString));
    memset(serialNo,0x00,sizeof(serialNo));
	memset(BankLogNo,0x00,sizeof(BankLogNo));
	memset(cBankReturnVouNo1,0x00,sizeof(cBankReturnVouNo1));
	memset(ReturnCode,0x00,sizeof(ReturnCode));
    memset(CheckCard6,0x00,sizeof(CheckCard6));
	memset(termNO,0x00,sizeof(termNO));
	memset(tranDate,0x00,sizeof(tranDate));
	memset(tranTime,0x00,sizeof(tranTime));
	memset(vouNO,0x00,sizeof(vouNO));
	memset(unitNO,0x00,sizeof(unitNO));
	memset(Account,0x00,sizeof(Account));

	//新的返回包解析
	char cardNO[20],transDate[9],transTime[7],traceNO[7],batchNO[7],authNO[7],refNO[9],validDate[5],transMoney[13];
	memset(cardNO,0x00,sizeof(cardNO));
	memset(transDate,0x00,sizeof(transDate));
	memset(transTime,0x00,sizeof(transTime));
	memset(traceNO,0x00,sizeof(traceNO));
	memset(batchNO,0x00,sizeof(batchNO));
	memset(authNO,0x00,sizeof(authNO));
	memset(refNO,0x00,sizeof(refNO));
	memset(validDate,0x00,sizeof(validDate));
	memset(transMoney,0x00,sizeof(transMoney));

	memcpy(recvString,IPacket,strlen(IPacket));
	
	//返回码
    for(i=0;i<2;i++)
	{
	   ReturnValue[i] = recvString[i];
	}
	
	//账号
	j=0;
    for(i=2;i<21;i++)
	{
	   if ( recvString[i] != ' ')
	   {
	      Account[j] = recvString[i];
		  j++;
	   }
	}
	
	//交易日期
	j=0;
    for(i=21;i<29;i++)
	{
	   if(recvString[i] != ' ')
	   {			
		   transDate[j] = recvString[i];
		   j++;
	   }
	}
	char printDate[11];
	memset(printDate, 0x00, sizeof(printDate));
	ToPrintDate(transDate,printDate);
	p_TransDate.Format("%s",printDate);
	WriteDiary("交易日期:" + p_TransDate,"Diary.log");
	//交易时间
	j=0;
	for(i=29;i<35;i++)
	{
	   if(recvString[i] != ' ')
	   {

		   transTime[j] = recvString[i];
		   j++;
	   }
	}
	char printTime[9];
	memset(printTime, 0x00, sizeof(printTime));
	ToPrintTime(transTime,printTime);
	p_TransTime.Format("%s",printTime);
	WriteDiary("交易时间：" + p_TransTime,"Diary.log");
	//终端流水号
	j=0;
    for(i=35;i<41;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   traceNO[j] = recvString[i];
		   j++;
	   }
	}
	p_TraceNO.Format("%s",traceNO);
    WriteDiary("终端流水号：" + p_TraceNO,"Diary.log");

	strcpy(cardNO,Account);

	unsigned int iCNo;
	for(iCNo=6;iCNo<12;iCNo++)
	{
	   cardNO[iCNo] ='*';
	}
	p_CardNO.Format("%s",cardNO);
	WriteDiary("获得扣款交易卡号：" + p_CardNO,"Diary.log");
	
   	//批次号
	j=0;
    for(i=41;i<47;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   batchNO[j] = recvString[i];
		   j++;
	   }
	}
	p_BatchNO.Format("%s",batchNO);
	WriteDiary("批次号：" + p_BatchNO,"Diary.log");
	//授权号 
	j=0;
    for(i=47;i<53;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   authNO[j] = recvString[i];
		   j++;
	   }
	}
	p_AuthNO.Format("%s",authNO);
	WriteDiary("授权号：" + p_AuthNO,"Diary.log");
	//检索参考号 
	j=0;
    for(i=53;i<61;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   refNO[j] = recvString[i];
		   j++;
	   }
	}
	p_RefNO.Format("%s",refNO);
	WriteDiary("检索参考号：" + p_RefNO,"Diary.log");
	//卡片有效期 
	j=0;
    for(i=61;i<65;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   validDate[j] = recvString[i];
		   j++;
	   }
	}
	p_ValidDate.Format("%s",validDate);
	WriteDiary("卡片有效期：" + p_ValidDate,"Diary.log");
	//交易金额 
	j=0;
    for(i=65;i<77;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   transMoney[j] = recvString[i];
		   j++;
	   }
	}
	char printMoney[13];
	memset(printMoney, 0x00, sizeof(printMoney));
	ToPrintMoney(transMoney,printMoney);
	p_TransMoney.Format("%s",printMoney);
	WriteDiary("交易金额：" + p_TransMoney,"Diary.log");

	//票据号 
	j=0;
    for(i=77;i<87;i++)
	{
	   if(recvString[i] != ' ')
	   {
		   vouNO[j] = recvString[i];
		   j++;
	   }
	}
	cm_VouNO.Format("%s",vouNO);
	WriteDiary("卡片有效期：" + cm_VouNO,"Diary.log");


	if (CString(ReturnValue)=="00")
	{
	   WriteDiary("交易数据返回成功！！","Diary.log");
	   	strcpy(OPacket,recvString);
		WriteDiary("返回数据包：" + CString (OPacket),"Diary.log");
	   return (0);
	}
	else
	{
	   return (-1);
	}

// 	char sReturnCode[50];
// 	memset(sReturnCode,0x00,sizeof(sReturnCode));
//     if(!strcmp(serialNo,"-7"))
// 	{
//        strcpy(sReturnCode,"287");
// 	}
//     if(!strcmp(serialNo,"000000"))
// 	{
//        strcpy(sReturnCode,"100");
// 	}
	


//////////创建返回包///////////////////////////未修改
	/*
   char ReturnStr[100];
   memset(ReturnStr,0x00,sizeof(ReturnStr));
   memcpy(ReturnStr,sReturnCode,strlen(sReturnCode));
   memcpy(ReturnStr + strlen(sReturnCode),BankLogNo,strlen(BankLogNo));
   
   memcpy(ReturnStr + strlen(sReturnCode) + strlen(BankLogNo),cBankReturnVouNo1,strlen(cBankReturnVouNo1));

   
   memcpy(OPacket,ReturnStr,strlen(ReturnStr));*/

	
// 	strcpy(OPacket,recvString);
// 	WriteDiary("返回数据包：" + CString (OPacket),"Diary.log");
// 	return (0);
}

/*
int vOne2Two(unsigned char *DataIn,unsigned char *DataOut,int iLen)
{
	int   			i;
	unsigned char	pChar0,pChar1;
	
	for(i=0;i<iLen;i++) 
	{
		pChar0 = pChar1 = DataIn[i];
		DataOut[2*i] = ((pChar0&0xf0)>0x90)?(((pChar0&0xf0)>>4)+0x41-10) : (((pChar0&0xf0)>>4)+0x30);
		DataOut[2*i+1] =((pChar1&0x0f)>0x09)?((pChar1&0x0f)+0x41-10) : ((pChar1&0x0f)+0x30);
	}
	DataOut[2*iLen]=0x00;
	return  0;
}
*/
// Two( 30~~3f 41~~46 ) --> One
 int vTwo2One( unsigned char *psFrom, unsigned char *psTo, int  uiFromLen )
{
	int  			i;
	unsigned char	ucTmp;

	for( i=0; i<uiFromLen; i+=2 )
	{
		ucTmp = psFrom[i];
		if( ucTmp == 0x00 ) ucTmp = 0x00;
		else {
			if( ucTmp < 'A' )
				ucTmp = ucTmp - '0';
			else
				ucTmp = toupper(ucTmp) - 'A' + 0x0A;
		}
		ucTmp <<= 4;
		psTo[i/2] = ucTmp;

		ucTmp = psFrom[i+1];
		if( ucTmp == 0x00 ) ucTmp = 0x00;
		else if( ucTmp < 'A' )
				ucTmp = ucTmp - '0';
			 else
				ucTmp = toupper(ucTmp) - 'A' + 0x0A;

		psTo[i/2] = psTo[i/2] + ucTmp;
	}
	
	return  0;
}

bool OpenKeyComm(const char * CommPort, int iBaud)
{
	//Validate our parameters
//	ASSERT(nPort>0 && nPort<=255);	
	CloseKeyComm(); //In case we are already open
	//Call CreateFile to open up the comms port
//	CString sPort;
//	sPort.Format(_T("\\\\.\\COM%d"), nPort);

	hCommDev = CreateFile(CommPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	if (hCommDev == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	SetupComm(hCommDev,100,100); //输入缓冲区和输出缓冲区的大小都是1024

	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。

	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier=100;
	TimeOuts.WriteTotalTimeoutConstant=500;
	SetCommTimeouts(hCommDev, &TimeOuts); //设置超时

	//Get the current state prior to changing it
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);

	if (!GetCommState(hCommDev, &dcb))
	{
		TRACE("Failed in call to GetCommState\n");
	}
	
	dcb.BaudRate = iBaud;	//Setup the baud rate
	dcb.Parity = NOPARITY;	//Setup the Parity
	dcb.ByteSize = 8;	//Setup the data bits
	dcb.StopBits = ONESTOPBIT;	//Setup the stop bits
	
	dcb.fDsrSensitivity = FALSE;	//Setup the flow control 
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	
	//Now that we have all the settings in place, make the changes
	if (!SetCommState(hCommDev, &dcb))
	{
		TRACE("Failed in call to SetCommState\n");
	}
	PurgeComm( hCommDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
	return true;
}


bool CloseKeyComm()
{
	SetCommMask( hCommDev, 0 ) ;
	PurgeComm( hCommDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
	
	//Close down the comms port
	BOOL bSuccess = CloseHandle(hCommDev);
	if (!bSuccess)
	{
		return false;
	}
	else
	{
		hCommDev = INVALID_HANDLE_VALUE;
		return true;
	}
}

volatile bool bCancel = false;
volatile bool bOK = true;

//5.读取客户密码密文　　　－－－－－－－－－使用　TMKIndex＝1　；modex＝0x30 或 0x31 ；  idnum 设备编号，转出参数；pinLength　密码的长度，传出参数；pinCrypt 加密后的密码，TPKCheck　密码校验值，可有可无
 //ANSI X9.8格式输入密码由ZPK加密。
// int UnionReadCustomerPin(const char * CommPort, const char * TMKIndex,	const char * accNo, const char * modex, const char * timeout, char * pinLength, char * pinCrypt, char * TPKCheck)
 int SDBReadCustomerPin(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * accNo, const char * modex, const int timeout,char * idnum, char * pinLength, char * pinCrypt, char * TPKCheck)
{
	while(1)
	{
		if(bOK==false)
		{
			WriteDiary("上一调用未结束","Diary.log");
			Sleep(1000);
		}
		else
			break;
	}
	bOK = false;
	bCancel = false;
	WriteDiary("SDBReadCustomerPinUseSDB","Diary.log");

	if(!OpenKeyComm(CommPort))
		return -1;
// 	WriteDiary("打开串口","Diary.log");
	

		BYTE cmd4[] = {0x1B,0x25,0x4B};
		SDBOutput(cmd4,3);
// 		WriteDiary("转K口","Diary.log");

	
//	BYTE ccc[] = "\x1B\x30";
//	BYTE RetV[2];
//	COMSendCmd(2,ccc,RetV,0);
//	Sleep(200);
	int Length = 0;
	BYTE Cmd[32] = "\x1BJ";//x1BJ输入密码由ZPK加密 x1BK异或
	BYTE Data[32] = "\0";
	BYTE Response[32] = "\0";
	int TimeOut =timeout;
	
	int Index = atoi(TMKIndex)-1;
	Cmd[2] = (Index >> 4) +0x30;
	Cmd[3] = (Index & 0x0F)+0x30;
	Cmd[4] = 0x30;
	Cmd[5] = 0x31;
	Cmd[6] = modex[0];
	
	memcpy(Cmd+7, accNo, strlen(accNo));
	strcat((char *)Cmd, "\x0D\x0A");
	Length =  strlen( (char *)Cmd);
/*
	BYTE cmd[] = "\x1BH\x30\x30\x30\x31\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x0D\x0A";
	cmd[2] = Cmd[2];
	cmd[3] = Cmd[3];
	WriteDiary("取校验值","Diary.log");
	int ret1 = SDBCOMSendCmd(21, cmd, (unsigned char *)TPKCheck, TimeOut);
	WriteDiary("取校验值返回","Diary.log");
	if(ret1==-1)
	{
		bOK = true;
		return -1;
	}
	Sleep(500);

/*******************处理键盘信息长度问题***********************/
	if(bCancel)
	{
		bOK = true;
		return -1;
	}
    BYTE lcmd[32] = "\x1BT\x0D\x0A";
// 	WriteDiary("取键盘信息","Diary.log");
	int ret2 = SDBCOMSendCmd(4, lcmd,  (unsigned char *)Response, TimeOut);
// 	WriteDiary("取键盘信息返回","Diary.log");
	if(ret2==-1)
	{
		bOK = true;
		return -1;
	}
	Sleep(500);
	memcpy(idnum,&Response[1],strlen((char *)&Response[1])-1);
	int pinfolen = strlen(idnum);

/**************************************************************/
	if(bCancel)
	{
		bOK = true;
		return -1;
	}
// 	WriteDiary("读键盘","Diary.log");

	memset(Response,0x00,sizeof(Response));

	int ret = SDBCOMSendCmd(Length, Cmd, Response, TimeOut);
// 	WriteDiary("读键盘返回","Diary.log");
	
	//退出转A口
	BYTE cmd2A[] = "\x1B%A";
	SDBOutput(cmd2A,3);

	CloseKeyComm();
// 	WriteDiary("关闭串口","Diary.log");
//	Sleep(500);
	if(ret!=0)
	{
		bOK = true;
		return -1;
	}
	else
	{
		memcpy(pinLength, &Response[1]+pinfolen, 2);
		memcpy(pinCrypt, &Response[1]+pinfolen+2, 16);
		bOK = true;
		return 0;
	}
}
/*
//11.获取卡号
long SDBFormatAnsiX98Account(const char * AccNo, char * AnsiX98AccNo)
{
//	WriteLog("SDBFormatAnsiX98Account");
	int i,j;
	for(i=0;i<12;i++) AnsiX98AccNo[i]='0';

	int len = strlen(AccNo);
    if (len<1) return -1;
	if (len<13)
	{
		j=11;
        for(i=len-2;i>=0 ;i--)
		{
			AnsiX98AccNo[j]=AccNo[i];
			j--;
		}
		return 0;
	
	}
	else
	{
		int j=0;
		for(i=len-13;i<len-1 ;i++)
		{
			AnsiX98AccNo[j]=AccNo[i];
			j++;
		}
		return 0;
	}
}
*/
int SDBReadDeviceInfo(char *commPort,char*strInfo)
{
	char cmd[200];
	char ExtPort[]= {0x1B,0x25,0x4B};
	char cInfo[100];
	char cresult[100];

	
	memset(cInfo,0x00,sizeof(cInfo));
	memset(cmd,0x00,sizeof(cmd));	 
	memset(cresult,0x00,sizeof(cresult));	 
	if(!OpenKeyComm(commPort))
		return -1;
	SDBOutput(ExtPort,3);
	cmd[0] = 0x1b;
	cmd[1] = 0x54;
	cmd[2] = 0x0d;
	cmd[4] = 0x0a;
	short nret = SDBCOMSendCmd(4,(unsigned char*)cmd,(unsigned char*)cresult,10);

	if (nret !=0)
	{
		return -1;
	}
	
	memcpy(strInfo,&cresult[1],strlen(cresult)-2);
	CloseKeyComm();
// 	WriteDiary("~~~~~~~~~~~~strInfo~~~~~~~~~~~~~" + CString(strInfo),"Diary.log");
	return 0;
}


/************************************************************************/
/* 加载工作密钥
/* keyValue:工作密钥值
/* nMNo:主密钥号
/* nwno:工作密钥号
/************************************************************************/
/*
 int LoadWorkKey(char *Commport, char *keyValue,short nMNo ,short nWNo)
 {
	char cmd[200];
	char ExtPort[]= {0x1B,0x25,0x4B};
	char Response[200];
	char keyValueTmp[200];

	memset(Response,0x00,sizeof(Response));
	memset(cmd,0x00,sizeof(cmd));
	memset(keyValueTmp,0x00,sizeof(keyValueTmp));

	if(!OpenKeyComm(Commport))
	 return -1;
	SDBOutput(ExtPort,3);

	vOne2Two((unsigned char*)keyValue,(unsigned char *)keyValueTmp,strlen(keyValue));
	cmd[0]= 0x1B;  //	4B	M	N
	cmd[1]= 0x4b;
	cmd[2] = nMNo;
	cmd[3] = nWNo;
	memcpy(&cmd[4],keyValueTmp,strlen(keyValueTmp));
	short ret = SDBCOMSendCmd(strlen(cmd),(unsigned char*) cmd,(unsigned char*)Response,20);
	if (ret<0)
	{
		//WriteLog("加载工作密钥失败");
		return -1;
	}

	if (strlen(Response)!=4)
	{
		//WriteLog("返回数据包长度错误");
		return -1;
	}

	if(Response[2]== 0x4f && Response[3]== 0x4b) // =ok
	{
		return 0;
	}else
		return -1;

 }
 */

//6.TPK/TAK注入密码键盘-----------------使用　主密钥号　TMKIndex＝1，KEYIndex工作密钥号＝'01',KeyLen 值（1，2，3）取其中的一个，keyByTMK　下载的工作密钥，　KeyValue是校验值
// int UnionInputKEY(const char * CommPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, const char * KeyValue)
//int CPin::SDBInputKEY(const char * CommPort, const char *ExtPort,const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue)
short LoadWorkKye(char *Commport,char *keyValue,char *keyCheck)
{
	short nret=0;

	WriteDiary("变量KeyByTMK:" + CString(keyValue),"Diary.log");
	WriteDiary("变量KeyValue:" + CString(keyCheck),"Diary.log");
	nret=SDBInputKEY(Commport,"","001","01","2",keyValue,keyCheck);
	WriteDiary("变量KeyValue:" + CString(keyCheck),"Diary.log");
	if(nret!=0)
	{
		return -7;
	}
	return nret;
	
}

//6.TPK/TAK注入密码键盘-----------------使用　主密钥号　TMKIndex＝1，KEYIndex工作密钥号＝'01',KeyLen 值（1，2，3）取其中的一个，keyByTMK　下载的工作密钥，　KeyValue是校验值
// int UnionInputKEY(const char * CommPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, const char * KeyValue)
 int SDBInputKEY(const char * CommPort, const char *ExtPort,const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue)
{
	if(!OpenKeyComm(CommPort))
		return -1;
	BYTE cmd[] = {0x1B,0x25,0x4B};
	SDBOutput(cmd,3);
	int Length = 0;
	BYTE Cmd[64] = "\x1Bu";//下载工作密钥
	BYTE Data[32] = "\0";
	BYTE Response[32] = "\0";
	int TimeOut = 10;

	int Index1 = atoi(TMKIndex)-1;	
	Cmd[2] = (Index1 >> 4) +0x30;
	Cmd[3] = (Index1 & 0x0F)+0x30;

	Cmd[4] = KEYIndex[0];//此处0需要拆字成0x30？
	Cmd[5] = KEYIndex[1];//此处1需要拆字成0x31？
	
	int len = atoi(KeyLen)*8;
	Cmd[6] = (len >> 4) +0x30;
	Cmd[7] = (len & 0x0F)+0x30;

// 	CString TMKLEN;
// 	TMKLEN.Format("%d", strlen(keyByTMK));
// 	WriteDiary("TMKLEN----" + TMKLEN,"Diary.log");

	unsigned char psTo[64] = "\0";
	vTwo2One((unsigned char *)keyByTMK, psTo, strlen(keyByTMK));
	memcpy(Cmd+8, psTo, len);

	memcpy(Cmd+(strlen(keyByTMK)/2)+8, "0000000000000000\x0D\x0A", 18);
	Length =  strlen(keyByTMK)/2 + 26;

	char ret = SDBCOMSendCmd(Length, Cmd, Response, TimeOut);

	//退出转A口
	BYTE cmd2A[] = "\x1B%A";
	SDBOutput(cmd2A,3);

	CloseKeyComm();

	if(ret)
	{
		WriteDiary("返回-1","Diary.log");
		return -1;
	}
	else
	{
		memcpy(KeyValue, &Response[1], strlen((char *)Response)-2);
		return 0;
	}
}

void SDBClearReadBuffer()
{
	ASSERT(IsOpen());
	if (!PurgeComm(hCommDev, PURGE_RXCLEAR))
	{
		TRACE("Failed in call to PurgeComm\n");
	}
}

void SDBClearWriteBuffer()
{
	ASSERT(IsOpen());
	if (!PurgeComm(hCommDev, PURGE_TXCLEAR))
	{
		TRACE("Failed in call to PurgeComm\n");
	}
}

DWORD SDBGetInBufferCount()
{
   	COMSTAT stat;
	::ZeroMemory(&stat,sizeof(stat));
	DWORD dwErrors;
	if (!ClearCommError(hCommDev, &dwErrors, &stat))
	{
		TRACE("Failed in call to ClearCommError\n");
	}
	return stat.cbInQue;
}

DWORD SDBGetInput(void* pBuf,DWORD Count,DWORD dwMilliseconds)
{   
	ASSERT(IsOpen());
	DWORD Errors;
	COMSTAT comstat;
	BOOL ok;
	ok = ::ClearCommError(hCommDev,&Errors,&comstat);
	if(!ok)
	{
		return 0;
	}
	if(comstat.cbInQue==0)
	{
		return 0;
	}
	
	DWORD dwBytesRead = 0;
	if (!ReadFile(hCommDev, pBuf, comstat.cbInQue, &dwBytesRead, NULL))
	{
		TRACE("Failed in call to ReadFile\n");
	}
	return dwBytesRead; 
}

DWORD SDBOutput(void* pBuf,DWORD dwCount)
{   
    ASSERT(IsOpen());
	DWORD dwBytesWritten = 0;
	if (!WriteFile(hCommDev, pBuf, dwCount, &dwBytesWritten, NULL))
	{
		TRACE("Failed in call to WriteFile\n");
	}
	return dwBytesWritten;
}


void SDBProcessMessageLoop()
{
	MSG MessageFetch;
	if(::PeekMessage(&MessageFetch,NULL,0,0,PM_REMOVE))
	{
		::TranslateMessage(&MessageFetch);
		::DispatchMessage(&MessageFetch);
	}
}

//13823326021yu
//13826563864zeng
//向串口发送命令
int SDBCOMSendCmd(int Length, BYTE Cmd[], BYTE Response[], int TimeOut)
{
// 	CString length;
// 	length.Format("%d",Length);
// 	WriteDiary("length:" + CString(length),"Diary.log");


	BYTE CmdBuf[128] = "\0";
	BYTE RecvBuf[128] = "\0";
	DWORD InBufferCount = 0;
	time_t tStart,tEnd;

	SDBClearReadBuffer();
	SDBClearWriteBuffer();
	SDBOutput(Cmd, Length);
	
	if(TimeOut == 0)	//无返回数据情况
		return 0;
	
	tStart = tEnd = clock();
	while((tEnd-tStart)/CLK_TCK <TimeOut)
	{
		if(bCancel)
			return -1;
		SDBProcessMessageLoop();
		Sleep(100);
		InBufferCount= SDBGetInBufferCount();	//第一次读数据
		
		if(InBufferCount)
		{
			Sleep(100);
			if(InBufferCount == SDBGetInBufferCount())	//第二次读数据
			{
// 				WriteDiary("第二次读数据","Diary.log");
				SDBGetInput(RecvBuf, InBufferCount);

				break;
			}			
		}
		tEnd = clock();
	}
// 	for (unsigned int i=0;i<InBufferCount-1;i++)
// 	{
// 		CString buf;
// 		buf.Format("%d",RecvBuf[i]);
// 		WriteDiary("buf:" + buf,"Diary.log");
// 	}

	if(InBufferCount)
	{
		memcpy(Response,RecvBuf, InBufferCount);

// 		for (unsigned int j=0;i<InBufferCount-1;j++)
// 		{
// 			CString buf;
// 			buf.Format("%d",Response[j]);
// 			WriteDiary("buf:" + buf,"Diary.log");
// 		}

		for(unsigned int i=0;i<InBufferCount-1;i++)
		{
			if(RecvBuf[i]=='E' && RecvBuf[i+1]=='R')
			{
				WriteDiary("blablabla+++++++++++-2","Diary.log");
				return -2;
			}
		}

	}
	else
	{
		WriteDiary("blablabla+++++++++++-1","Diary.log");
		return -1;
	}
	return 0;
}

int ReadData( void *buffer, int limit )
{

//	if( !m_bOpened || hCommDev == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( hCommDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;
   	bReadStatus = ReadFile( hCommDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
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
//	if( !m_bOpened || hCommDev == NULL ) return NoOpenCom ;
	ClearCommError( hCommDev, &dwErrorFlags, &ComStat );//清除错误标志

	bWriteStat=WriteFile(hCommDev,Data,(DWORD)length,&dwBytesWritten,&m_OverlappedWrite);
	if(!bWriteStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			//WaitForSingleObject(m_OverlappedWrite.hEvent,1000);
			GetOverlappedResult( hCommDev, &m_OverlappedWrite, &dwBytesWritten,TRUE);
			//函数会一直等待，直到写操作完成或由于错误而返回
			return dwBytesWritten;
		}
		return FAIL;
	}
	return dwBytesWritten;

}

void ClearBuffer()//清空缓冲区
{
//	if( !m_bOpened || hCommDev == NULL ) return;

	DWORD dwErrorFlags;
	COMSTAT ComStat;
	ClearCommError( hCommDev, &dwErrorFlags, &ComStat );
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
    Exit=0;
	for(; !bTimeover && !bFinish ; t++)
	{
		n = ReadData(&buffer[nRead] ,1024);
		nRead += n;

		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))	
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		
		if(Exit)//判断是否退出
		{
			Exit=0;
			return -2;
          
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


//add by tong  20140414 to 2014/04/14 
extern  long ToPrintDate(char * InStr,char * OutStr)
{
	char printDate[11];
	memset(printDate,0x00,sizeof(printDate));

	int i=0,j=0;
	for (i=0;i<8;i++)
	{
		if (i == 4 || i == 6 )
		{
			printDate[j] = '/';
			j++;
		}
		printDate[j] = InStr[i];		
		j++;
	}

	strcpy(OutStr, printDate);
	return (0);
}

//add by tong  200414 to 20:04:14 
extern  long ToPrintTime(char * InStr,char * OutStr)
{
	char printDate[9];
	memset(printDate,0x00,sizeof(printDate));

	int i=0,j=0;
	for (i=0;i<6;i++)
	{
		if (i == 2 || i == 4 )
		{
			printDate[j] = ':';
			j++;
		}
		printDate[j] = InStr[i];		
		j++;
	}

	strcpy(OutStr, printDate);
	return (0);
}

//add by tong  
extern  long ToPrintMoney(char * InStr,char * OutStr)
{
	char printMoney[13];
	memset(printMoney,0x00,sizeof(printMoney));
// 	WriteDiary("instr：" + CString(InStr), "Diary.log");
	int j=0,k=0;
	//去掉左补的0
	for (int i=0; i<13; i++)
	{
		if (i == j && InStr[i] == '0')
		{
			j++;
		}
		else if(InStr[i] != ' ' && InStr[i] != '\0')
		{
			printMoney[k] = InStr[i];
// 			WriteDiary("Value：" + CString(printMoney[k]), "Diary.log");
			k++;
		}
	}
	int len = k;
// 	CString length;
// 	length.Format("%d",len);
// 	WriteDiary("长度：" + length, "Diary.log");
	printMoney[len] = printMoney[len-1];
	printMoney[len-1]	= printMoney[len-2];
	printMoney[len-2] = '.';
	
	strcpy(OutStr, printMoney); 
	return (0);
}
