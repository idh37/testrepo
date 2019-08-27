/**    
@file    GameDataManager.h
@date    2011/2/15
@author  PJH(gamking@cj.com)
@brief   Data 관리자 ( 게임에서 사용하는 데이터 값을 Command/value형식으로 저장한다. )
@brief   이전에는 CStringManager라는 클래스로 사용됨.
*/

#if !defined(AFX_NMSTRMAN_H__591F95D9_101C_4CE5_AC62_4ECF78EA62DF__INCLUDED_)
#define AFX_NMSTRMAN_H__591F95D9_101C_4CE5_AC62_4ECF78EA62DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NMStrMan.h : header file
//

#include "UtilDefine.h"
#include <afxcoll.h>
#include "StringBase.h"
#include <string>
#include <vector>
#include <algorithm>

#define  STRDATA_HEADER ("NETMARBLE PROGRAM (www.netmarble.net)")

namespace NMBASE
{
	namespace UTIL
	{

		/////////////////////////////////////////////////////////////////////////////
		// CStringManager command target

		class NMUTIL_CLASS CStringManager : public CObject
		{
			//private:
		public:
			CStringManager();
			virtual ~CStringManager();

			//public:
			//	static	CStringManager& Get();

		protected:
			DECLARE_SERIAL(CStringManager)

		public:
			BOOL Switch(CString strID1,CString strID2);
			BOOL Change(CString strID, CString strData);
			BOOL ChangeID(CString strID, CString strNew);
			BOOL Add(CString strID, CString strData);
			BOOL Add(CNMStr *pData);

			CNMStr * GetObj(CString strID);
			CString  Get(CString strID);
			char *  _Get(CString strID);

			void Clear(CString strID);
			void Clear();

			BOOL Compare(CString strData);
			BOOL CompareID(CString strData);

			BOOL Save(CString strFileName);
			BOOL Open(CString strFileName);

			CObList  m_LIST;
			CString  m_strHeader;
			int  	 m_nTotalNum;

			virtual void Serialize(CArchive &ar);
		};
		//yoo
		NMUTIL_API std::string format(const char* fmt, ...); 
		NMUTIL_API std::string mbstring(const std::wstring &text);
		NMUTIL_API std::wstring wcstring(const std::string &text);
		NMUTIL_API std::vector<std::string> strtovstr(const std::string &text);


		//NMUTIL_API CStringManager& GSTRMAN();

	}//namespace UTIL

}//namespace NMBASE

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMSTRMAN_H__591F95D9_101C_4CE5_AC62_4ECF78EA62DF__INCLUDED_)