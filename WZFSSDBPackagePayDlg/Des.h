#ifndef _DES_
#define _DES_

void  DES(char *key,char *s_text,char *d_text);
void  _DES(char *key,char *s_text,char *d_text);
void  AdapteDES(char *key,int klen,char *s_text,int slen,char *d_text);
void  _AdapteDES(char *key,int klen,char *s_text,int slen,char *d_text);

#endif