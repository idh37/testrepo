#pragma once

#include "../Base/ObjectBase.h"
#include <utility>
#include <list>

//Slide�� ������ ����
#define TEXT_STS_RIGHT	0x00000001
#define TEXT_STS_LEFT	0x00000002
#define TEXT_STS_BOTTOM	0x00000004
#define TEXT_STS_TOP	0x00000008

//Slide�� ������� ����
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
	//��Ƽ������ ��� ���ؼ� ����ϴ� STL
	typedef		std::pair<std::string, int>		PAIRSTRINGANDWIDTH;
	typedef		std::list<PAIRSTRINGANDWIDTH>	LISTSTRINGINFO;

	bool				m_bDot;

//��Ʈ ���� ������
	FONTINFO			m_cFontInfo;

	std::string			m_strText;
	int					m_nLineSpace;
//�ӽ� ������
	bool				m_bRedraw;
	int					m_nTotalHight;

	std::string			m_strOutput;
	//AddString�� Slide��忡���� ������ �ʴ´�.
	std::string			m_strAddString;

	CDumyImage			*m_pBack;

	//m_bSlide�� true�̸� MultiLine, m_uFormat�� �������� �ʴ´�.
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

	//��� ���ڿ� ���� �Լ���
	void SetText(const std::string &strText);
	std::string &GetText(void){return m_strText;}

	void SetAddText(const std::string &strAddText);
	std::string &GetAddText(void){return m_strAddString;}

	//�׸��� ���� ��� ���� �Լ���, DT_SINGLELINE, DT_CENTER ��
	const UINT SetFormat(const UINT &uFormat);
	const UINT &GetFormat(void){return m_cFontInfo.m_uFormat;}

	//�Ⱓ�� ���� �Լ���
	const int SetLineSpace(int nLineSpace);
	const int GetLineSpace(void){return m_nLineSpace;}

	//�׸��� ��� ���� �Լ���
	const ENUM_TEXTDRAW_TYPE SetTextDrawType(ENUM_TEXTDRAW_TYPE nType);
	const ENUM_TEXTDRAW_TYPE &GetTextDrawType(void){return m_cFontInfo.m_nDrawType;}

	//���� �Ѵ� ���ڿ� ��½� ... �� ������� ���θ� ��Ÿ���� �Լ���
	//true�̸� Mutiline �� ���� ����Ҽ� ������ ������ Single Line �̴�.
	const bool SetUseDot(const bool &bUseDot);
	const bool &IsUseDot(void){return m_bDot;}

	//��� �ؽ�Ʈ�� ������ �Լ���
	const COLORREF SetTextColor(COLORREF clrColor);
	const COLORREF &GetTextColor(void){return m_cFontInfo.m_clrColor;}

	//��� �ؽ�Ʈ�� �׸��ڻ����� �Լ���
	const COLORREF SetShadowColor(COLORREF clrColor);
	const COLORREF &GetShadowColor(void){return m_cFontInfo.m_clrOutline;}

	//��Ʈ ���� �Լ���
	void SetFont(const std::string &strFontName, const int &nFontSize, const bool &bBold);
	const std::string &GetFontName(void){return m_cFontInfo.GetName();}
	const int &GetFontSize(void){return m_cFontInfo.GetSize();}
	const bool &IsBold(void){return m_cFontInfo.IsBold();}

	//�ڰ� ���� �Լ���
	void SetSpaceBetweenLetters(const int &nSpaceBetweenLetters);
	const int GetSpaceBetweenLetters(void){return m_cFontInfo.m_nSpaceBetweenLetters;}

	//�����̵� ���� �Լ���
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

	//Object ���� ���� �Լ���
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
	virtual bool OnSetLineCount(const int &nLine); //����� �������� ���θ� �����Ѥ�.

	void DrawText(const CDC *pDC, const LISTSTRINGINFO &listText, const CPoint &ptGab = CPointZero);
	
	int GetTextWidth(const CDC *pDC, LPCTSTR lpText, const int &nLength);
	int GetTextHeight(const CDC *pDC, LPCTSTR lpText, const int &nLength);
	CSize GetTextSize(const CDC *pDC, LPCTSTR lpText, const int &nLength);

	void MakeMultiLine(CDC *pDC, LISTSTRINGINFO &listText);
	void MakeDotText(CDC *pDC);
};
} //namespace LSY