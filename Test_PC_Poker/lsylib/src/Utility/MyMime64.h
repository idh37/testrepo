#pragma once

namespace LSY
{
class UPG_LSYLIB_EXPORT CMyMime64  
{
	static char BASE64CODE[64];

	BOOL	m_bEncoded;
	string	m_strString;
public:
	CMyMime64();
	CMyMime64(const char *str, BOOL bEncoded, BOOL bAutoChange);
	CMyMime64(const string &str, BOOL bEncoded, BOOL bAutoChange);
	virtual ~CMyMime64();
	void Clear(void);

	LPCTSTR SetString(const char *str, BOOL bEncoded, BOOL bAutoChange);
	LPCTSTR SetString(const string &str, BOOL bEncoded, BOOL bAutoChange);

	BOOL Encoding(void);
	BOOL Encoding(const char *src);
	BOOL Decoding(void);
	BOOL Decoding(const char *src);

	LPCTSTR GetString(void){return m_strString.c_str();}
	BOOL IsEncoded(void){return m_bEncoded;}
};
} //namespace LSY