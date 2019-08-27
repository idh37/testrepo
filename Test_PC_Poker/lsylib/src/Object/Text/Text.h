#pragma once

#include "../Base/ObjectBase.h"
#include <utility>
#include <list>

//Slide가 나오는 방향
#define TEXT_STS_RIGHT	0x00000001
#define TEXT_STS_LEFT	0x00000002
#define TEXT_STS_BOTTOM	0x00000004
#define TEXT_STS_TOP	0x00000008

//Slide가 사라지는 방향
#define TEXT_STE_LEFT	0x00000010
#define TEXT_STE_RIGHT  0x00000020
#define TEXT_STE_TOP	0x00000040
#define TEXT_STE_BOTTOM	0x00000080

namespace LSY
{
class CDumyImage;
class UPG_LSYLIB_EXPORT CText : public CObjectBase
{
protected:
	//멀티라인을 찍기 위해서 사용하는 STL
	typedef		std::pair<std::string, int>		PAIRSTRINGANDWIDTH;
	typedef		std::list<PAIRSTRINGANDWIDTH>	LISTSTRINGINFO;

	bool				m_bDot;

//폰트 관련 변수들
	FONTINFO			m_cFontInfo;

	std::string			m_strText;
	int					m_nLineSpace;
//임시 변수들
	bool				m_bRedraw;
	int					m_nTotalHight;

	std::string			m_strOutput;
	//AddString은 Slide모드에서는 사용되지 않는다.
	std::string			m_strAddString;

	CDumyImage			*m_pBack;

	//m_bSlide가 true이면 MultiLine, m_uFormat는 동작하지 않는다.
	class SLIDE_ITEM
	{
	public:
		enum enumSLIDEStatus
		{
			SS_START = 0,
			SS_WAIT,
			SS_END
		};
	public:
		SLIDE_ITEM();
		virtual ~SLIDE_ITEM();

		UINT					m_uFormat;
		UINT					m_nDrawMode;
		DWORD					m_dwSlideType;
		DWORD					m_dwDelayTime;

		int						m_nRepeat;
		std::string				m_strOutput;
		CSize					m_szSize;

		DWORD					m_dwStartTime;
		enumSLIDEStatus			m_nCurStatus;
	};

	bool					m_bSlide;
	float					m_fSlideGap;

	std::list<SLIDE_ITEM>	m_listSlide;
	std::list<SLIDE_ITEM>	m_listSlideShow;
	DWORD					m_dwSlideDelayTime;
	DWORD					m_dwSlideType;
	int						m_nSlideRepeatCount;
	int						m_nLineCount;
public:
	CText(void);
	virtual ~CText();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_TEXT;}

	virtual bool Create(int nWidth, int nHeight);
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void Logic(const DWORD &dwTime);

	virtual bool IsPossibilityReSize(void){return true;}

	//출력 문자열 관련 함수들
	void SetText(const std::string &strText);
	std::string &GetText(void){return m_strText;}

	void SetAddText(const std::string &strAddText);
	std::string &GetAddText(void){return m_strAddString;}

	//그리기 정렬 방식 관련 함수들, DT_SINGLELINE, DT_CENTER 등
	const UINT SetFormat(const UINT &uFormat);
	const UINT &GetFormat(void){return m_cFontInfo.m_uFormat;}

	//출간격 관련 함수들
	const int SetLineSpace(int nLineSpace);
	const int GetLineSpace(void){return m_nLineSpace;}

	//그리기 방식 관련 함수들
	const ENUM_TEXTDRAW_TYPE SetTextDrawType(ENUM_TEXTDRAW_TYPE nType);
	const ENUM_TEXTDRAW_TYPE &GetTextDrawType(void){return m_cFontInfo.m_nDrawType;}

	//폭을 넘는 문자열 출력시 ... 을 출력할지 여부를 나타내는 함수들
	//true이면 Mutiline 과 같이 사용할수 없으며 무조건 Single Line 이다.
	const bool SetUseDot(const bool &bUseDot);
	const bool &IsUseDot(void){return m_bDot;}

	//출력 텍스트의 색관련 함수들
	const COLORREF SetTextColor(COLORREF clrColor);
	const COLORREF &GetTextColor(void){return m_cFontInfo.m_clrColor;}

	//출력 텍스트의 그림자색관련 함수들
	const COLORREF SetShadowColor(COLORREF clrColor);
	const COLORREF &GetShadowColor(void){return m_cFontInfo.m_clrOutline;}

	//폰트 관련 함수들
	void SetFont(const std::string &strFontName, const int &nFontSize, const bool &bBold);
	const std::string &GetFontName(void){return m_cFontInfo.GetName();}
	const int &GetFontSize(void){return m_cFontInfo.GetSize();}
	const bool &IsBold(void){return m_cFontInfo.IsBold();}

	//자간 관련 함수들
	void SetSpaceBetweenLetters(const int &nSpaceBetweenLetters);
	const int GetSpaceBetweenLetters(void){return m_cFontInfo.m_nSpaceBetweenLetters;}

	//슬라이드 관련 함수들
	virtual void AddSlideString_Back(const std::string &strSlide);
	virtual void AddSlideString_Front(const std::string &strSlide);
	virtual void ClearSlideString(const bool &bForce);

	void SetSlide(const bool &bSlide);
	void SetSlideType(const DWORD &dwSlideType = TEXT_STS_RIGHT | TEXT_STE_LEFT){m_dwSlideType = dwSlideType;}
	void SetSlideDelayTime(const DWORD &dwSlideDelayTime){m_dwSlideDelayTime = dwSlideDelayTime;}
	void SetSlideRepeatCount(const int &nRepeat = -1){m_nSlideRepeatCount = nRepeat;}
	void SetSlideMovePerSec(const int &nPixcel = 50){m_fSlideGap = (float)nPixcel;}

	const bool &IsSlide(void){return m_bSlide;}
	const DWORD &GetSlideType(void){return m_dwSlideType;}
	const DWORD &GetSlideDelayTime(void){return m_dwSlideDelayTime;}
	const int &GetSlideRepeatCount(void){return m_nSlideRepeatCount;}
	const float &GetSlideMovePerSec(void){return m_fSlideGap;}

	DWORD _RedrawSlideStart(SLIDE_ITEM &item, DWORD &dwDeltaTime, CPoint &ptStart, CPoint &ptEnd, CPoint &ptCur, int &nWidth, int &nHeight, CSize &szSize);
	bool _RedrawSlideEnd(SLIDE_ITEM &item, DWORD &dwDeltaTime, CPoint &ptStart, CPoint &ptEnd, CPoint &ptCur, int &nWidth, int &nHeight, CSize &szSize);
	void _RedrawSlideWait(SLIDE_ITEM &item, DWORD &dwDeltaTime, CPoint &ptStart, CPoint &ptEnd, CPoint &ptCur, int &nWidth, int &nHeight, CSize &szSize);

	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
	
	void SetRedraw(bool bRedraw = true);
protected:
	virtual int CheckRedraw(void);
	virtual bool OnSetLineCount(const int &nLine); //사이즈가 변경됬는지 여부를 리턴한ㄷ.

	void DrawText(const CDC *pDC, const LISTSTRINGINFO &listText, const CPoint &ptGab = CPointZero);
	
	int GetTextWidth(const CDC *pDC, LPCTSTR lpText, const int &nLength);
	int GetTextHeight(const CDC *pDC, LPCTSTR lpText, const int &nLength);
	CSize GetTextSize(const CDC *pDC, LPCTSTR lpText, const int &nLength);

	void MakeMultiLine(CDC *pDC, LISTSTRINGINFO &listText);
	void MakeDotText(CDC *pDC);
};
} //namespace LSY