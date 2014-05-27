// Cryptography.h: interface for the CCryptography class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYPTOGRAPHY_H__E471AF82_251E_11D5_9CEE_5254AB2D88C3__INCLUDED_)
#define AFX_CRYPTOGRAPHY_H__E471AF82_251E_11D5_9CEE_5254AB2D88C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCryptography
{
public:
	void UnDESDB(CString& ID);
	void DESDB(CString& ID);
	CCryptography();
	virtual ~CCryptography();

	void Decode3DES(BYTE InData[] , BYTE Key[] , BYTE OutData[]);
	void Encode3DES(BYTE InData[] , BYTE Key[] , BYTE OutData[]);
	void DecodeDES(BYTE InData[] , BYTE Key[] , BYTE OutData[]);
	void EncodeDES(BYTE InData[] , BYTE Key[] , BYTE OutData[]);

protected:	
	void DES(BYTE m_bit[], BYTE k_bit[], BYTE e_bit[]);
	void UnDES(BYTE m_bit[], BYTE k_bit[], BYTE e_bit[]);
	void keychange(BYTE oldkey[], BYTE newkey[][8]);
	void s_replace(BYTE s_bit[]);
	void bit2byte(BYTE bit[], BYTE byte[]);
	void byte2bit(BYTE byte[], BYTE bit[]);
	void TriDES(BYTE In[],BYTE K[],BYTE Out[]);
	void UnTriDES(BYTE In[],BYTE K[],BYTE Out[]);

};

#endif // !defined(AFX_CRYPTOGRAPHY_H__E471AF82_251E_11D5_9CEE_5254AB2D88C3__INCLUDED_)
