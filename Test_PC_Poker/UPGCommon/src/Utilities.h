// Utilities.h: interface for the CUtilities class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILITIES_H__291BA4FC_7125_49FD_B102_50CDCFAA617B__INCLUDED_)
#define AFX_UTILITIES_H__291BA4FC_7125_49FD_B102_50CDCFAA617B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===================================================================

class CPrivateProfile
{
public:

	CPrivateProfile();
	CPrivateProfile(LPCTSTR strfname);
	virtual ~CPrivateProfile();

	BOOL  SetFileName(LPCTSTR strfname);
	void  SetCryptKey(LPCTSTR strkey);

	int   GetInt(LPCTSTR strSection, LPCTSTR strKeyname, int nDefault=0);
	INT64   GetInt64(LPCTSTR strSection, LPCTSTR strKeyname, INT64 nDefault=0);
	float GetFloat(LPCTSTR strSection, LPCTSTR strKeyname, float fDefault=0);
	char* GetString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault="");
	BOOL  GetString(char* buf, int bufsize, LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault="");
	char* GetMultilineString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault="");
	BOOL  GetMultilineString(char* buf, int bufsize, LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault="");
// 	char* GetDecryptString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault="");
// 	BOOL  GetDecryptString(char* buf, int bufsize, LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault="");
	BOOL  GetStruct(void* pStruct, int structsize, LPCTSTR strSection, LPCTSTR strKeyname);

	BOOL  WriteInt(LPCTSTR strSection, LPCTSTR strKeyname, int nValue);
	BOOL  WriteInt64(LPCTSTR strSection, LPCTSTR strKeyname, INT64 nValue);
	BOOL  WriteFloat(LPCTSTR strSection, LPCTSTR strKeyname, float fValue);
	BOOL  WriteFloatEx(LPCTSTR strSection, LPCTSTR strKeyname, float fValue);
	BOOL  WriteString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue);
	BOOL  WriteMultilineString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue);
//	BOOL  WriteEncryptString(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue);
	BOOL  WriteStruct(LPCTSTR strSection, LPCTSTR strKeyname, void* pStruct, int structsize);

protected:

	char	m_FileName[MAX_PATH];
	CString	m_CryptKey;

	BOOL GetDefPath(char* strPath, int nBufLen);

};

//===================================================================

#endif // !defined(AFX_UTILITIES_H__291BA4FC_7125_49FD_B102_50CDCFAA617B__INCLUDED_)
