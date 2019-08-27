// Utilities.cpp: implementation of the CUtilities class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utilities.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CPrivateProfile::CPrivateProfile()
{
	memset(m_FileName, 0, sizeof(m_FileName));
}

CPrivateProfile::CPrivateProfile(LPCTSTR strfname)
{
	memset(m_FileName, 0, sizeof(m_FileName));

	SetFileName(strfname);
}

CPrivateProfile::~CPrivateProfile()
{

}

BOOL  CPrivateProfile::SetFileName(LPCTSTR strfname)
{
	// 실행 파일명을 알아낸다
	char path[MAX_PATH];
	if(!GetDefPath(path, MAX_PATH-1))
		return FALSE;

	wsprintf(m_FileName, "%s\\%s", path, strfname);

	return TRUE;
}

void  CPrivateProfile::SetCryptKey(LPCTSTR strkey)
{
	m_CryptKey = strkey;
}

BOOL CPrivateProfile::GetDefPath(char* strPath, int nBufLen)
{
	// 실행 파일명을 알아낸다
	char origin[MAX_PATH];

	// GetModuleHandle(NULL) == AfxGetInstanceHandle()
	if(GetModuleFileName(GetModuleHandle(NULL), origin, MAX_PATH-1) == 0) {	
		return FALSE;
	}

	// 실행 파일명에서 폴더명을 분리한다
	strPath[0] = '\0';
	int slen = strlen(origin);
	for(int i=slen-1; i>0; i--) {
		if(origin[i] == '\\') {
			if(nBufLen < i+1) return 0;
			strncpy(strPath, origin, i);
			strPath[i] = '\0';
			return TRUE;
		}
	}

	return FALSE;
}

int   CPrivateProfile::GetInt(LPCTSTR strSection, LPCTSTR strKeyname, int nDefault/*=0*/)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	char strdef[buflen], strrtn[buflen];
	wsprintf(strdef, "%d", nDefault);

	if(GetPrivateProfileString(strSection, strKeyname, strdef, strrtn, buflen-1, m_FileName) == 0) {
		return nDefault;
	}

	return atoi(strrtn);
}

INT64   CPrivateProfile::GetInt64(LPCTSTR strSection, LPCTSTR strKeyname, INT64 nDefault/*=0*/)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	char strdef[buflen], strrtn[buflen];
	wsprintf(strdef, "%I64d", nDefault);

	if(GetPrivateProfileString(strSection, strKeyname, strdef, strrtn, buflen-1, m_FileName) == 0) {
		return nDefault;
	}

	return _atoi64(strrtn);
}

float CPrivateProfile::GetFloat(LPCTSTR strSection, LPCTSTR strKeyname, float fDefault/*=0*/)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	char strdef[buflen], strrtn[buflen];
	wsprintf(strdef, "%f", fDefault);

	if(GetPrivateProfileString(strSection, strKeyname, strdef, strrtn, buflen-1, m_FileName) == 0) {
		return fDefault;
	}

	return (float)atof(strrtn);
}

char* CPrivateProfile::GetString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault/*=""*/)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	static char strrtn[buflen];
	strrtn[0] = 0;

	GetString(strrtn, buflen, strSection, strKeyname, strDefault);

	return strrtn;
}

BOOL CPrivateProfile::GetString(char* buf, int bufsize, LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault/*=""*/)
{
	ASSERT(m_FileName[0] != 0);

	if(!buf || bufsize<1) return FALSE;

	if(GetPrivateProfileString(strSection, strKeyname, strDefault, buf, bufsize-1, m_FileName) == 0) {
		int copylen = min((int)strlen(strDefault), bufsize-1);
		strncpy(buf, strDefault, copylen);
		buf[copylen] = 0;
		return FALSE;
	}

	return TRUE;
}

char* CPrivateProfile::GetMultilineString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault/*=""*/)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	static char strrtn[buflen];
	strrtn[0] = 0;

	GetMultilineString(strrtn, buflen, strSection, strKeyname, strDefault);

	return strrtn;
}

BOOL CPrivateProfile::GetMultilineString(char* buf, int bufsize, LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault/*=""*/)
{
	ASSERT(m_FileName[0] != 0);

	if(!buf || bufsize<1) return FALSE;

	const int buflen = 2048;
	char* tmp = new char[buflen];
	memset(tmp, 0, buflen);

	if(GetPrivateProfileString(strSection, strKeyname, strDefault, tmp, buflen-1, m_FileName) == 0) {
		strncpy(tmp, strDefault, buflen-1);
	}

	int totlen = strlen(tmp);
	int n = 0;
	for(int i=0; i<totlen; i++, n++) {
		if(n >= bufsize-1)
			break;

		if(strncmp(&tmp[i], "\\r", 2) == 0) {
			buf[n] = '\r';
			i++;
		}
		else if(strncmp(&tmp[i], "\\n", 2) == 0) {
			buf[n] = '\n';
			i++;
		}
		else {
			buf[n] = tmp[i];
		}
	}

	buf[n] = 0;

	delete tmp;

	return TRUE;
}
// 
// char* CPrivateProfile::GetDecryptString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault/*=""*/)
// {
// 	ASSERT(m_CryptKey.GetLength() != 0);
// 
// 	const int buflen = 1024;
// 	static char strrtn[buflen];
// 	strrtn[0] = 0;
// 
// 	CString strVal = GetString(strSection, strKeyname, strDefault);
// 	if(strVal.GetLength() > 0) {
// 		if(CRYPT_SUCCESS == DecryptString(strVal, m_CryptKey, strrtn))
// 			return strrtn;
// 	}
// 
// 	return (char*)strDefault;
// }
// 
// BOOL CPrivateProfile::GetDecryptString(char* buf, int bufsize, LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault/*=""*/)
// {
// 	ASSERT(m_CryptKey.GetLength() != 0);
// 
// 	const int buflen = 1024;
// 	static char strrtn[buflen];
// 	strrtn[0] = 0;
// 
// 	CString strVal = GetString(strSection, strKeyname, strDefault);
// 	if(strVal.GetLength() > 0) {
// 		if(CRYPT_SUCCESS == DecryptString(strVal, m_CryptKey, strrtn)) {
// 			int slen = strlen(strrtn);
// 			if(slen <= 0 || slen > buflen-1)
// 				return FALSE;
// 			strncpy(buf, strrtn, slen);
// 			buf[slen] = 0;
// 			return TRUE;
// 		}
// 	}
// 
// 	return FALSE;
// }

BOOL  CPrivateProfile::GetStruct(void* pStruct, int structsize, LPCTSTR strSection, LPCTSTR strKeyname)
{
	ASSERT(m_FileName[0] != 0);

	if(!pStruct || structsize<1) return FALSE;

	if(GetPrivateProfileStruct(strSection, strKeyname, pStruct, structsize, m_FileName) == TRUE) {
		return TRUE;
	}

	return FALSE;
}

BOOL  CPrivateProfile::WriteInt(LPCTSTR strSection, LPCTSTR strKeyname, int nValue)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	char strval[buflen];
	wsprintf(strval, "%d", nValue);

	return WritePrivateProfileString(strSection, strKeyname, strval, m_FileName);
}

BOOL  CPrivateProfile::WriteInt64(LPCTSTR strSection, LPCTSTR strKeyname, INT64 nValue)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	char strval[buflen];
	CString strMoney;
	strMoney.Format("%I64d", nValue);
	strcpy(strval , strMoney);
	strval[strlen(strval)] = 0;

	return WritePrivateProfileString(strSection, strKeyname, strval, m_FileName);
}

BOOL  CPrivateProfile::WriteFloat(LPCTSTR strSection, LPCTSTR strKeyname, float fValue)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	char strval[buflen];
	wsprintf(strval, "%f", fValue);

	return WritePrivateProfileString(strSection, strKeyname, strval, m_FileName);
}

BOOL  CPrivateProfile::WriteFloatEx(LPCTSTR strSection, LPCTSTR strKeyname, float fValue)
{
	ASSERT(m_FileName[0] != 0);

	const int buflen = 1024;
	char strval[buflen];
	sprintf(strval, "%.5f", fValue);

	return WritePrivateProfileString(strSection, strKeyname, strval, m_FileName);
}


BOOL  CPrivateProfile::WriteString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue)
{
	ASSERT(m_FileName[0] != 0);

	return WritePrivateProfileString(strSection, strKeyname, strValue, m_FileName);
}

BOOL  CPrivateProfile::WriteMultilineString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue)
{
	ASSERT(m_FileName[0] != 0);

	int buflen = (strlen(strValue)*2) + 1;	// 개행문자의 길이가 2배로 늘어날 것을 대비
	char* buf = new char[buflen];
	memset(buf, 0, buflen);

	int totlen = strlen(strValue);
	int n = 0;
	for(int i=0; i<totlen; i++, n++) {
		if(n >= buflen-1)
			break;

		if(strValue[i] == '\r') {
			buf[n++] = '\\';
			if(n >= buflen-1)
				break;
			buf[n] = 'r';
		}
		else if(strValue[i] == '\n') {
			buf[n++] = '\\';
			if(n >= buflen-1)
				break;
			buf[n] = 'n';
		}
		else {
			buf[n] = strValue[i];
		}
	}

	buf[n] = 0;

	BOOL bRtn = WritePrivateProfileString(strSection, strKeyname, buf, m_FileName);

	delete buf;

	return bRtn;
}
// 
// BOOL  CPrivateProfile::WriteEncryptString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue)
// {
// 	ASSERT(m_CryptKey.GetLength() != 0);
// 
// 	const int buflen = 1024;
// 	static char strrtn[buflen];
// 	strrtn[0] = 0;
// 
// 	EncryptString(strValue, m_CryptKey, strrtn);
// 
// 	return WriteString(strSection, strKeyname, strrtn);
// }

BOOL  CPrivateProfile::WriteStruct(LPCTSTR strSection, LPCTSTR strKeyname, void* pStruct, int structsize)
{
	ASSERT(m_FileName[0] != 0);

	return WritePrivateProfileStruct(strSection, strKeyname, pStruct, structsize, m_FileName);
}
