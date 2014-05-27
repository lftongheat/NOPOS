//=========================================================
/// 每个字节的偏移量
#define BIT_OFFSET      8
#define HALF_BIT_OFFSET 4
//---------------------------------------------------------
class ByteConvert
{
private:
	ByteConvert() {}
public:
	static void ByteToAscii(unsigned char bit, char* dst)
    {
        if(dst == NULL)
            return;
		
        unsigned char tmp = 0;
		
        for (int j = 0; j < 2; ++j)
        {
            if(j == 0)
                tmp = (bit & 0xf0) >> HALF_BIT_OFFSET;
            else
                tmp = bit & 0x0f;
			
            if (tmp <= 0x09) 
                dst[j] = tmp + '0';
            else 
            {
                tmp -= 0x0A;
                dst[j] = tmp + 'A';
            }
        }
    }
	static size_t BcdToAscii(const unsigned char* src, unsigned int srclen, char* dst)
    {
        if(src == NULL || dst == NULL)
            return 0;
		
        for(unsigned int i = 0;i < srclen; ++i)
        {
            ByteToAscii(src[i], dst + 2*i);
        }
        int dstlen = 2 * srclen;
        dst[dstlen] = '\0';
		
        return dstlen;
    }
	static unsigned int AsciiToBcd(const char* src, unsigned int srclen, unsigned char* dst)
    {
        if(src == NULL || dst == NULL)
            return 0;
		
        unsigned char tmp;
        int dstlen = srclen / 2;
        int index = 0;
		
        for(int i = 0;i < dstlen; ++i)
        {
            for(int j = 0;j < 2; ++j)
            {
                index  = 2*i + j;
                if(src[index] >= '0' && src[index] <= '9')
                    tmp = src[index] - '0';
                else if(src[index] >= 'A' && src[index] <= 'F')
                    tmp = src[index] - 'A' + 0x0A;
                else if(src[index] >= 'a' && src[index] <= 'f')
                    tmp = src[index] - 'a' + 0x0A;
                else 
                    tmp = 0x00;
				
                if(j == 0)
                    dst[i] = tmp << HALF_BIT_OFFSET;
                else 
                    dst[i] |= tmp;
            }
        }
		
        return dstlen;
    }
};
//=========================================================