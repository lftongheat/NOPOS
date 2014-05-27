#include "ParseTLV.h"
#include "stdafx.h"


int Hex2Str(int InLen, BYTE *InData, char *OutBuffer)
{
	int i,j;
	char ch1,ch2;
	j=0;
	for(i=0;i<InLen;i++)
	{
		ch1=(InData[i]&0xf0)>>4;
		ch2=(InData[i]&0x0f);
		if(ch1<10)
		{
			OutBuffer[j++]=ch1+0x30;
		}
		else
		{
			OutBuffer[j++]=ch1+0x37;
		}
	
		if(ch2<10)
		{
			OutBuffer[j++]=ch2+0x30;
		}
		else
		{
			OutBuffer[j++]=ch2+0x37;
		}
	}
	OutBuffer[j]=0;
	return j;
}

int Str2Hex(int InLen, char *InBuffer, BYTE *OutArray)
{
	int  i = 0,j = 0;
    char tmp;
    for( i=0,j=0;i < InLen;)
    {
        if( InBuffer[i] >= 0x30 && InBuffer[i] <= 0x39 )
        {
            tmp = (InBuffer[i] - 0x30)<<4;
        }
        else if( InBuffer[i] >= 0x41 && InBuffer[i] <= 0x46 )
        {
            tmp=(InBuffer[i] - 0x37)<<4;
        }
        else if( InBuffer[i] >= 0x61 && InBuffer[i] <= 0x66 )
        {
            tmp=(InBuffer[i] - 0x57)<<4; 	
        }
        i++;

        if( InBuffer[i] >= 0x30 && InBuffer[i] <= 0x39 )
        {
            tmp = tmp | (InBuffer[i] - 0x30);
        }
        else if( InBuffer[i] >= 0x41 && InBuffer[i] <= 0x46 )
        {
            tmp= tmp | (InBuffer[i] - 0x37);
        }
        else if( InBuffer[i] >= 0x61 && InBuffer[i] <= 0x66 )
        {
            tmp =tmp | (InBuffer[i] - 0x57);
        }

        OutArray[j++] = tmp;
        i++;
    }
   	return(j);
}

//解析TLV数据，Tag(Len+Data)+Val(Len+Data)，解析src中的一个TLV(原始数据可能有一个或以上)，标签未指定
int Parse_tlv(tlv_data_t *tlv, BYTE *src, int size)
{
	int  rv;
    int  offset;
    BYTE *ptr  = src;
    BYTE *head = src;  
    /* Parse Tlv Data Tag */
    Parse_tlv_tag(&(tlv->tag), ptr);//找出标签T
    /* ptr point to the head of the length data of tlv data */
    ptr = &(tlv->tag.data[tlv->tag.size]);//把标签后面开始的数据指针给ptr，即数据有LV
    /* Parse Tlv Data Length */
    rv  = Parse_tlv_length(&(tlv->val), ptr);//除去标签T后，把余下数据LV分解
    if (rv != 0)        return rv;

    /* ptr point to the head of the next tlv data */
    if (tlv->val.data==NULL && tlv->val.size==0)//如果标签T后L=00，V无数据，把L后一位的地位给ptr
        ptr = &(tlv->tag.data[tlv->tag.size+1]);
    else
        ptr = &(tlv->val.data[tlv->val.size]);//若有数据，则V的数据后一位给ptr
    /* check format */
    offset = (int)(ptr - head);//解析完了判断一下，若所解析的数据长度比原始数据长度大则出错
    return offset<=size ? SUCC : ParseTlvError;
}
//解析TLV标签T
void Parse_tlv_tag(tlv_buf_t *tag, BYTE *src)
{
	if (0x1F == (src[0]&0x1F))
        tag->size = 2;
    else
        tag->size = 1;
    tag->data = src;
}
//解析标签后面数据Val，长度+数据
int Parse_tlv_length(tlv_buf_t *tlv, BYTE *src)
{
	char *t;

    if (0 == src[0]) 
	{
        tlv->size = 0;
        tlv->data = NULL;
    } 
	else if (0x80 >src[0]) 
	{
        tlv->size = (int)src[0];
        tlv->data = &src[1];
    } 
	else if (0x81 == src[0]) 
	{
        tlv->size = (int)src[1];
        tlv->data = &src[2];
    } 
	else if (0x82 == src[0]) //长度为2字节
	{
        t = (char*)&(tlv->size);
        t[0] = src[2];
        t[1] = src[1];
        t[2] = 0x00;
        t[3] = 0x00;
        tlv->data = &src[3];
    } 
	else if (0x83 == src[0])//长度为3字节
	{
        t = (char*)&(tlv->size);
        t[0] = src[3];
        t[1] = src[2];
        t[2] = src[1];
        t[3] = 0x00;
        tlv->data = &src[4];
    } 
	else 
	{
        return ParseTlvError;
    }

    return SUCC;
}
//解析tag数据，在原始数据中解析出tag标签后的数据
int Parse_tlv_data_tag(BYTE **buf, char *tag, BYTE *src, int size)
{
	int  rv;
    int  len;
    int  offset = 0;
    BYTE *last  = src;
    BYTE *next  = src;
    tlv_data_t tlv;
    
    len = strlen(tag);//要输出数据的标签长度
    while(size > 0) 
	{
        rv = Parse_tlv(&tlv, next, size); //解析一个标签及数据
        if (rv != 0)  return rv;
		//判断所解析出来的标签是否是所要输出数据的标签，若是则把数据输出，否则则继续解析
        if ((len==tlv.tag.size) && (!memcmp(tag, tlv.tag.data, len))) 
		{
            *buf = tlv.val.data;
            return tlv.val.size;
        }

        last = next;
        if (tlv.val.data==NULL && tlv.val.size==0)
            next = &(tlv.tag.data[tlv.tag.size+1]);
        else
            next = &(tlv.val.data[tlv.val.size]);
		
        offset = (int)(next - last);//此次已解析的数据长度
        size  -= offset;//还未解析的数据长度
    }
    return ParseTlvError;//无此标签数据
}


//解析tmplt里的tag数据，此处tmplt数据里包含了tag，输出tmplt1.../tmplt2.../tag标签后面的数据
int Parse_tlv_data_Ttag(BYTE *buf, char *tag, char *tmplt, BYTE *src, int size)
{
	int  len;
	char buf_tag[2] = {0};//两个标签
	BYTE *last = src;
	BYTE *next = NULL;
	if (tmplt != NULL) 
	{
		while(tmplt[0] != '\0' && size > 0) 
		{
			buf_tag[0] = tmplt[0];
		
			len = Parse_tlv_data_tag(&next, buf_tag, last, size);//输出tmpltg标签后面的数据
			if (len < 0)  return len;
			size = len;
			last = next;
			++tmplt;
		}
	}

	if (size <= 0)  return ParseTlvError;

	len = Parse_tlv_data_tag(&next, tag, last, size);//输出tag标签后面的数据
	if (len < 0) return len;

	memcpy(buf, next, len);
	return len;
}
//找到标签Tag后面的数据
int Parse_tlv_Data(BYTE *buf, char *tag, BYTE *src, int size)
{
    int  len;
    unsigned char *ptr = NULL;
    len = Parse_tlv_data_tag(&ptr, tag, src, size);
    if (len < 0 ) return len;
    memcpy(buf, ptr, len);
    return len;
}




//找到各个标签
int Find_dol_tag(BYTE *tag, BYTE *src, int *size)
{
    if(size<=0)  return FAIL;
	if ((src[0] & '\x1F') == '\x1F' )
	{
        memcpy(tag,src,2);
        *size -= 3;
        return SUCC;
    }
    else
	{
        tag[0] = src[0];
        *size -= 2;
        return SUCC;
    }
}
//将数据组合成TLV包，即标签+长度+数据格式
int Pack_tlv_Data(BYTE *buf, char *tag, BYTE *src, int size)
{
	int  tag_len;
    int  length = 0;
    BYTE *ptr = buf;
    char *tmp = NULL;
    
    tag_len = strlen(tag);
    /* Check Tag Format */ 
    if (((tag[0]&0x1f)==0x1f && tag_len!=2)|| ((tag[0]&0x1f)!=0x1f && tag_len!=1))
        return ParseTlvError;
    
    memcpy(ptr, tag, tag_len);
    ptr += tag_len;
    
    if (size < 0x80) 
	{
        ptr[0] = size&0xff;
        memcpy(&ptr[1], src, size);
        length += tag_len + 1 + size;
    } 
	else if (size > 0x80 && size <= 0xff) 
	{
        ptr[0] = 0x81;
        ptr[1] = size&0xff;
        memcpy(&ptr[2], src, size);
        length += tag_len + 2 + size;
    } 
	else if (size > 0xff && size <= 0xffff) 
	{
        tmp = (char*)&size;
        ptr[0] = 0x82;
        ptr[1] = tmp[1];
        ptr[2] = tmp[0];
        memcpy(&ptr[3], src, size);
        length += tag_len + 3 + size;
    } 
	else if (size > 0xffff && size <= 0xffffff) 
	{
        tmp = (char*)&size;
        ptr[0] = 0x83;
        ptr[1] = tmp[2];
        ptr[2] = tmp[1];
        ptr[3] = tmp[0];
        memcpy(&ptr[4], src, size);
        length += tag_len + 4 + size;
    } 
	else 
	{
         return ParseTlvError;
    }
    return length;
}
