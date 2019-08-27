// FontBag.cpp: implementation of the CFontBag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FontBag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CFontBag::CFontBag()
		{
			Init();
		}

		CFontBag::~CFontBag()
		{

		}

		void CFontBag::Init()
		{
			m_Font[FONT_10_BOLD].CreateFont(10,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");


			m_Font[FONT_10_NORMAL].CreateFont(10,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");

			m_Font[FONT_11_BOLD].CreateFont(11,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");


			m_Font[FONT_11_NORMAL].CreateFont(11,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");


			m_Font[FONT_12_NORMAL].CreateFont(12,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");


			m_Font[FONT_12_BOLD].CreateFont(12,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");

			m_Font[FONT_13_BOLD].CreateFont(13,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");

			m_Font[FONT_13_NORMAL].CreateFont(13,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");


			m_Font[FONT_14_BOLD].CreateFont(14,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");

			m_Font[FONT_14_NORMAL].CreateFont(14,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");

			m_Font[FONT_15_BOLD].CreateFont(15,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");

			//< CYS 100604
			m_Font[FONT_17_BOLD].CreateFont(17,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");

			m_Font[FONT_18_BOLD].CreateFont(18,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"±¼¸²");
			//> CYS 100604

			m_Font[FONT_11D_BOLD].CreateFont(11,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_11D_NORMAL].CreateFont(11,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");

			m_Font[FONT_12D_BOLD].CreateFont(12,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_12D_NORMAL].CreateFont(12,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");

			m_Font[FONT_13D_BOLD].CreateFont(13,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_13D_NORMAL].CreateFont(13,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");

			m_Font[FONT_14D_BOLD].CreateFont(14,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_14D_NORMAL].CreateFont(14,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_16D_NORMAL].CreateFont(16,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_16D_BOLD].CreateFont(16,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_26D_NORMAL].CreateFont(26,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_26D_BOLD].CreateFont(26,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");

			m_Font[FONT_35D_NORMAL].CreateFont(35,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_35D_BOLD].CreateFont(35,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_40D_NORMAL].CreateFont(40,0,0,0,
				FW_NORMAL,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");


			m_Font[FONT_40D_BOLD].CreateFont(40,0,0,0,
				FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"µ¸¿ò");
		}

		CFont& CFontBag::GetFont( STD_FONT font_index )
		{
			if( font_index < 0 || font_index >= MAX_STD_FONT ) 
				return m_Font[0];

			return m_Font[font_index];
		}

		CFontBag& CFontBag::GetInstance()
		{
			static CFontBag bag;
			return bag;
		}


		CFontBag& GetFontBag()
		{
			return CFontBag::GetInstance();
		}

		CFont& GetMyFont( STD_FONT font_index )
		{
			return GetFontBag().GetFont( font_index );
		}

	}//namespace UTIL

}//namespace NMBASE