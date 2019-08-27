// FontBag.h: interface for the CFontBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTBAG_H__A6076F32_C3C2_4E9F_B524_6AEA9A5720D2__INCLUDED_)
#define AFX_FONTBAG_H__A6076F32_C3C2_4E9F_B524_6AEA9A5720D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		enum STD_FONT
		{
			FONT_10_BOLD,
			FONT_10_NORMAL,
			FONT_11_BOLD,
			FONT_11_NORMAL,
			FONT_12_NORMAL,
			FONT_12_BOLD,
			FONT_13_BOLD,
			FONT_13_NORMAL,
			FONT_14_BOLD,
			FONT_14_NORMAL,		//G¸¶ÄÏÀÌº¥Æ®
			FONT_15_BOLD,
			FONT_17_BOLD,		//< CYS 100604 >
			FONT_18_BOLD,		//< CYS 100604 >
			FONT_11D_BOLD,		//11p µ¸¿ò º¼µå
			FONT_11D_NORMAL,	//11p µ¸¿ò ±âº»
			FONT_12D_BOLD,		//12p µ¸¿ò º¼µå
			FONT_12D_NORMAL,	//12p µ¸¿ò ±âº»
			FONT_13D_BOLD,		//13p µ¸¿ò º¼µå
			FONT_13D_NORMAL,	//13p µ¸¿ò ±âº»	
			FONT_14D_BOLD,		//14p µ¸¿ò º¼µå
			FONT_14D_NORMAL,	//14p µ¸¿ò ±âº»
			FONT_16D_NORMAL,	//35p µ¸¿ò ±âº»
			FONT_16D_BOLD,		//35p µ¸¿ò º¼µå
			FONT_26D_NORMAL,	//35p µ¸¿ò ±âº»
			FONT_26D_BOLD,		//35p µ¸¿ò º¼µå
			FONT_35D_NORMAL,	//35p µ¸¿ò ±âº»
			FONT_35D_BOLD,		//35p µ¸¿ò º¼µå
			FONT_40D_NORMAL,	//40p µ¸¿ò ±âº»
			FONT_40D_BOLD,		//40p µ¸¿ò º¼µå
			MAX_STD_FONT,
		};

		class NMUTIL_CLASS CFontBag  
		{
		public:
			virtual ~CFontBag();

			void Init();
			CFont& GetFont( STD_FONT font_index );
			static CFontBag& GetInstance();

		private:
			CFontBag();


		protected:

			// ÆùÆ®
			CFont m_Font[MAX_STD_FONT];

		};

		NMUTIL_API CFont& GetMyFont( STD_FONT font_index );
		NMUTIL_API CFontBag& GetFontBag();

	}//namespace UTIL

}//namespace NMBASE



#endif // !defined(AFX_FONTBAG_H__A6076F32_C3C2_4E9F_B524_6AEA9A5720D2__INCLUDED_)
