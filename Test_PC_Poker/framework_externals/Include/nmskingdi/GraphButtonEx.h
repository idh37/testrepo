// GraphButtonEx.h: interface for the CGraphButtonEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHBUTTONEX_H__F99D1E88_1E32_43A3_91F5_0F25D8468190__INCLUDED_)
#define AFX_GRAPHBUTTONEX_H__F99D1E88_1E32_43A3_91F5_0F25D8468190__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GraphButton.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CGraphButtonEx  : public CGraphButton
		{
		public:
			CGraphButtonEx();
			virtual ~CGraphButtonEx();

			void SetSprite(NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber, int iButtoncnt = 4 );
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRAPHBUTTONEX_H__F99D1E88_1E32_43A3_91F5_0F25D8468190__INCLUDED_)
