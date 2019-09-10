/**    
@file    StringBase.h
@date    2011/2/15
@author  PJH(gamking@cj.com)
@brief   StringManager�� BaseŬ����	
*/

#if !defined(AFX_NMSTR_H__8AF03876_798F_4326_8D19_99B258F11809__INCLUDED_)
#define AFX_NMSTR_H__8AF03876_798F_4326_8D19_99B258F11809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NMStr.h : header file
//

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		/////////////////////////////////////////////////////////////////////////////
		// CNMStr command target

		class NMUTIL_CLASS CNMStr : public CObject
		{
		protected:
			DECLARE_SERIAL(CNMStr)

		public:
			CString GetID();
			CString Get();
			int      m_nIDSize;
			int      m_nDataSize;

			CString  m_strID;
			CString  m_strData;

			void Clear();

			BOOL Compare(CString strData);
			BOOL CompareID(CString strID);

			void SetData(CString strData);
			void SetID(CString strID);

			void SetStr(CString strID,CString strData);

			CNMStr();
			virtual ~CNMStr();
			CNMStr& operator=( CNMStr &assign );
			virtual void Serialize(CArchive &ar);

#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

		};

	}//namespace UTIL

}//namespace NMBASE


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMSTR_H__8AF03876_798F_4326_8D19_99B258F11809__INCLUDED_)