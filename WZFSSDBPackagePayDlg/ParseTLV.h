#if !defined(_PARSE_TLV_H_)
#define _PARSE_TLV_H_

#include "stdafx.h"

typedef struct _tlv_buf_t {
    int  size;
    BYTE *data;
} tlv_buf_t;

typedef struct _tlv_data_t {
    tlv_buf_t tag;
    tlv_buf_t val;
} tlv_data_t;





#define ParseTlvError      -400   /*½âÎöTLVÊý¾Ý´íÎó*/

extern  int  Hex2Str(int InLen, BYTE *InData, char *OutBuffer);
extern  int  Str2Hex(int InLen, char *InBuffer, BYTE *OutArray);
extern	int  Parse_tlv_data_Ttag(BYTE *buf, char *tag, char *tmplt, BYTE *src, int size);
extern	int  Parse_tlv_data_tag(BYTE **buf, char *tag, BYTE *src, int size);
extern	int  Parse_tlv_length(tlv_buf_t *tlv, BYTE *src);
extern	void Parse_tlv_tag(tlv_buf_t *tag, BYTE *src);
extern	int  Parse_tlv(tlv_data_t *tlv, BYTE *src, int size);
extern	int  Parse_tlv_Data(BYTE*buf, char *tag, BYTE *src, int size);
extern	int  Pack_tlv_Data(BYTE *buf, char *tag, BYTE *src, int size);
extern  int  Find_dol_tag(BYTE *tag, BYTE *src, int *size);

#endif