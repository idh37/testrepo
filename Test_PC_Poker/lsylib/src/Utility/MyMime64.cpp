#include "stdafx.h"
#include "MyMime64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
char CMyMime64::BASE64CODE[64] =
{
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'0','1','2','3','4','5','6','7','8','9','+','/'
};

CMyMime64::CMyMime64()
	:	m_bEncoded(FALSE)
{
}

CMyMime64::CMyMime64(const char *str, BOOL bEncoded, BOOL bAutoChange)
	:	m_bEncoded(FALSE)
{
	SetString(str, bEncoded, bAutoChange);
}

CMyMime64::CMyMime64(const string &str, BOOL bEncoded, BOOL bAutoChange)
	:	m_bEncoded(FALSE)
{
	SetString(str, bEncoded, bAutoChange);
}

CMyMime64::~CMyMime64()
{
	Clear();
}

void CMyMime64::Clear(void)
{
	m_strString.clear();
	m_bEncoded = FALSE;
}

LPCTSTR CMyMime64::SetString(const string &str, BOOL bEncoded, BOOL bAutoChange)
{
	return SetString(str.c_str(), bEncoded, bAutoChange);
}

LPCTSTR CMyMime64::SetString(const char *str, BOOL bEncoded, BOOL bAutoChange)
{
	Clear();

	m_strString = str;
	m_bEncoded = bEncoded;

	if(bAutoChange)
	{
		if(bEncoded)
		{
			Decoding();
		}
		else
		{
			Encoding();
		}
	}

	return m_strString.c_str();
}

BOOL CMyMime64::Encoding(void)
{
	if(m_strString.length() == 0) return FALSE;
	if(m_bEncoded) return FALSE;

	int len = (int)m_strString.length();

	string str;

	int iCount = 0;

	char cData[5];
	size_t nDataSize = sizeof(cData);
	memset(cData,0,nDataSize);

	unsigned char ch;
	int i=0;

	for(i=0;i<len;++i)
	{
		ch = m_strString[i];

		switch(iCount)
		{
		case 0:
			cData[0] = ch >> 2;
			cData[1] = (ch&0x03)<<4;
			break;
		case 1:
			cData[1] |= ch >> 4;
			cData[2] = (ch&0x0f)<<2;
			break;
		case 2:
			cData[2] |= ch >> 6;
			cData[3] = ch&0x3f;
			break;
		}
		++iCount;
		if(iCount%3==0)
		{
			cData[0] = BASE64CODE[cData[0]];
			cData[1] = BASE64CODE[cData[1]];
			cData[2] = BASE64CODE[cData[2]];
			cData[3] = BASE64CODE[cData[3]];
			cData[4] = 0;

			str += cData;

			memset(cData, 0, nDataSize);
			iCount = 0;
		}
	}

	if(iCount%3!=0)
	{
		for(i=0;i<4;++i)
		{
			if(cData[i] == 0)
			{
				cData[i] = '=';
			}
			else
			{
				cData[i] = BASE64CODE[cData[i]];
			}
		}

		str += cData;
	}

	m_strString = str;
	m_bEncoded = TRUE;	

	return TRUE;
}

BOOL CMyMime64::Encoding(const char *src)
{
	if(src == NULL) return FALSE;

	int len = (int)strlen(src);

	string str;

	int iCount = 0;
	char cData[5];

	size_t nDataSize = sizeof(cData);

	memset(cData,0,nDataSize);

	unsigned char ch;
	int i=0;
	for(i=0;i<len;++i)
	{
		ch = src[i];

		switch(iCount)
		{
		case 0:
			cData[0] = ch >> 2;
			cData[1] = (ch&0x03)<<4;
			break;
		case 1:
			cData[1] |= ch >> 4;
			cData[2] = (ch&0x0f)<<2;
			break;
		case 2:
			cData[2] |= ch >> 6;
			cData[3] = ch&0x3f;
			break;
		}
		++iCount;
		if(iCount%3==0)
		{
			cData[0] = BASE64CODE[cData[0]];
			cData[1] = BASE64CODE[cData[1]];
			cData[2] = BASE64CODE[cData[2]];
			cData[3] = BASE64CODE[cData[3]];

			str += cData;

			memset(cData, 0, nDataSize);
			iCount = 0;
		}
	}

	if(iCount%3!=0)
	{
		for(i=0;i<4;++i)
		{
			if(cData[i] == 0)
			{
				cData[i] = '=';
			}
			else
			{
				cData[i] = BASE64CODE[cData[i]];
			}
		}

		str += cData;
	}

	m_strString = str;
	m_bEncoded = TRUE;	

	return TRUE;
}

BOOL CMyMime64::Decoding(void)
{
	if(m_strString.length() == 0) return FALSE;
	if(!m_bEncoded) return FALSE;

	int len = (int)m_strString.length();

	string str;

	int iCount = 0;
	char cData[4];

	size_t nDataSize = sizeof(cData);
	memset(cData,0,nDataSize);

	char ch;

	for(int i=0;i<len;++i)
	{
		ch = m_strString[i];

		if(ch == '=')
		{
			str += cData;
			break;
		}

		if(ch >= 'A' && ch <= 'Z')
			ch -= 'A';
		else if(ch >= 'a' && ch <= 'z')
			ch = ch - 'a' + 26;
		else if(ch >= '0' && ch <= '9')
			ch = ch - '0' + 52;
		else if(ch=='+')
			ch = 62;
		else if(ch=='/')
			ch = 63;

		switch(iCount)
		{
		case 0:
			cData[0] = ch<<2;
			break;
		case 1:
			cData[0] = (cData[0]|((ch&0x30)>>4));
			cData[1] = (ch & 0x0f) << 4;
			break;
		case 2:
			cData[1] = cData[1]|((ch&0x3c)>>2);
			cData[2] = (ch&0x03)<<6;
			break;
		case 3:
			cData[2] = cData[2]|(ch);
			break;
		}

		++iCount;
		if((iCount%4)==0)
		{
			str += cData;
			memset(cData,0,nDataSize);
			iCount = 0;
		}
	}

	m_strString = str;
	m_bEncoded = FALSE;

	return TRUE;
}

BOOL CMyMime64::Decoding(const char *src)
{
	if(src == NULL) return FALSE;

	int len = (int)strlen(src);

	string str;

	int iCount = 0;
	char cData[4];
	size_t nDataSize = sizeof(cData);
	memset(cData,0,nDataSize);

	char ch;

	for(int i=0;i<len;++i)
	{
		ch = src[i];

		if(ch == '=')
		{
			str += cData;
			break;
		}

		if(ch >= 'A' && ch <= 'Z')
			ch -= 'A';
		else if(ch >= 'a' && ch <= 'z')
			ch = ch - 'a' + 26;
		else if(ch >= '0' && ch <= '9')
			ch = ch - '0' + 52;
		else if(ch=='+')
			ch = 62;
		else if(ch=='/')
			ch = 63;

		switch(iCount)
		{
		case 0:
			cData[0] = ch<<2;
			break;
		case 1:
			cData[0] = (cData[0]|((ch&0x30)>>4));
			cData[1] = (ch & 0x0f) << 4;
			break;
		case 2:
			cData[1] = cData[1]|((ch&0x3c)>>2);
			cData[2] = (ch&0x03)<<6;
			break;
		case 3:
			cData[2] = cData[2]|(ch);
			break;
		}

		++iCount;
		if((iCount%4)==0)
		{
			str += cData;
			memset(cData, 0, nDataSize);
			iCount = 0;
		}
	}

	m_strString = str;
	m_bEncoded = FALSE;

	return TRUE;
}
} //namespace LSY