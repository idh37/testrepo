// SysUtil.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSUTIL_H__363BCC4C_6969_4BB5_B3E4_F4A65D94718B__INCLUDED_)
#define AFX_SYSUTIL_H__363BCC4C_6969_4BB5_B3E4_F4A65D94718B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilDefine.h"
#include <winsvc.h>

namespace NMBASE
{
	namespace UTIL
	{

		NMUTIL_API BOOL  GetProfile_DefPath(char* strPath, int nBufLen);

		NMUTIL_API int   GetProfile_int(LPCTSTR strSection, LPCTSTR strKeyname, int nDefault);
		NMUTIL_API INT64 GetProfile_int64(LPCTSTR strSection, LPCTSTR strKeyname, INT64 nDefault);
		NMUTIL_API float GetProfile_float(LPCTSTR strSection, LPCTSTR strKeyname, float fDefault);
		NMUTIL_API char* GetProfile_string(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault);

		NMUTIL_API BOOL  WriteProfile_int(LPCTSTR strSection, LPCTSTR strKeyname, int nValue);
		NMUTIL_API BOOL  WriteProfile_int64(LPCTSTR strSection, LPCTSTR strKeyname, INT64 nValue);
		NMUTIL_API BOOL  WriteProfile_float(LPCTSTR strSection, LPCTSTR strKeyname, float fValue);
		NMUTIL_API BOOL  WriteProfile_string(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue);

		//===================================================================

		NMUTIL_API BOOL  OpenNehompy(LPCTSTR szMyUniqNo, LPCTSTR szTargetID);
		NMUTIL_API BOOL  OpenNetmarbleWebpage(LPCTSTR szMyUniqNo, LPCTSTR szTargetURL);

		//===================================================================

		// nmgraphicgdi로 이전됨
		//NMUTIL_API BOOL  ScreenCapture(HWND hwnd, LPCTSTR fname);

		//===================================================================



		class NMUTIL_CLASS CMsgServiceControl  
		{
		public:
			CMsgServiceControl();
			virtual ~CMsgServiceControl();

			BOOL InitService(LPCTSTR lpServiceName);
			BOOL StopService(BOOL bStop);

		public:
			BOOL			m_Inited;

			BOOL			m_bIsStart;

			SC_HANDLE		m_SCMHandle;		// COM/DCOM용 핸들
			CString			m_Servicename;
			SERVICE_STATUS	m_serviceStatus;
		};

		//===================================================================
	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_SYSUTIL_H__363BCC4C_6969_4BB5_B3E4_F4A65D94718B__INCLUDED_)
