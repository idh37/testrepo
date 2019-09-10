#pragma once

#include "../Base/ObjectBase.h"
#include <string>
#include <list>
#include <vector>

namespace LSY
{
class CScrollBar;
class CDumyImage;

class CRichText;

class CRichItem
{
public:
	std::string m_strOutput;
	CSize		m_szSize;
	FONTINFO	*m_pFontInfo;
	LPARAM		m_lParam;

	NMBASE::GRAPHICGDI::xSprite		*m_pSprite;
	int			m_nIndex;
public:
	CRichItem();
	virtual ~CRichItem();

	CRichItem *Clone(CRichText *pSrcRichText, CRichText *pDestRichText);
};

typedef std::list<CRichItem *>			RICHITEMLIST;
typedef RICHITEMLIST::iterator			RICHITEMLISTITER;

class CRichLine
{
public:
	RICHITEMLIST		m_listItems;
	CSize				m_szSize;
	UINT				m_uFormat;
	bool				m_bEnd;
	float				m_fStartY;
public:
	CRichLine();
	virtual ~CRichLine();

	const int GetLineHeight(const int &nMinimumHeight);

	void Clear(void);
	CRichLine *Clone(CRichText *pSrcRichText, CRichText *pDestRichText);
};

typedef std::list<CRichLine *>		RICHLINELIST;
typedef RICHLINELIST::iterator		RICHLINELISTITER;

typedef std::map<std::string, int>	EMOTICONMAP;
typedef EMOTICONMAP::iterator		EMOTICONMAPITER;

class UPG_LSYLIB_EXPORT CRichText : public CObjectBase
{
protected:
	class CTempRichText
	{
	public:
		std::string m_strOutput;
		int			m_nIndex;

		CTempRichText(std::string strOutput, int nIndex):m_strOutput(strOutput),m_nIndex(nIndex){}
		~CTempRichText(){}
	};

	FONTINFOLIST		m_listFonts;
	RICHLINELIST		m_listLines;

	int					m_nLineSpace;

	CDumyImage			*m_pBack;

	FONTINFO			*m_pFontInfo;
	int					m_nFontIndex;

	CRichLine			*m_pLastLine;
	CRichItem			*m_pLastItem;

	RICHLINELISTITER	m_itFirstVisibleLine;

	CScrollBar			*m_pScrollBar;
	float				m_fTotalHeight;

	float				m_fStartY;
	bool				m_bRedraw;
	unsigned int		m_nLimitLine;
	bool				m_bLineScroll;	//���� ������ ��ũ�ѹ��� Thumb�� ���콺�� �巡�� �Ҷ� Line������ ��ũ�� ���� ���θ� �����Ѵ�.

	NMBASE::GRAPHICGDI::xSprite				*m_pEmoticonSpr;
	EMOTICONMAP			m_mapEmoticon;

	int					m_nMinimumLineHeight;
	std::string			m_strTotalString;	//GetString�� ����ϴ� �ӽ� ����, Clone�� �ʿ� ����
public:
	CRichText(void);
	virtual ~CRichText();

	virtual bool Create(int nWidth, int nHeight);
	virtual bool CreateEmoticon(std::string strPath);

	virtual bool SetEmoticonText(std::string strText, int nIndex);

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_RICHTEXT;}
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	virtual bool IsPossibilityReSize(void){return true;}

	virtual void Clear(void);

	//�־��ִ� ��Ʈ ����Ʈ���� ��Ʈ�� �����Ǿ� ������ �ȵȴ�.
	virtual void SetFontList(const FONTINFOLIST &listFont);
	virtual const FONTINFOLIST &GetFontList(void){return m_listFonts;}
	virtual void AddFont(const FONTINFO &fontInfo);
	
	virtual void SetLineSpace(const int &nLineSpace);
	virtual const int &GetLineSpace(void){return m_nLineSpace;}

	virtual void SetCurrentFont(int nIndex);
	virtual void AttachScroll(CScrollBar *pScall);
	virtual const CScrollBar *GetScrollBar(void){return m_pScrollBar;}
	virtual void DettachScroll(void);

	virtual void AddText(std::string strText, LPARAM lParam = NULL);

	//�ִ� ���μ��� �����Ѵ�. 0�̸� �ִ� ���μ� ���� ���� ����ؼ� �޾Ƶ��δ�.
	virtual void SetLimitLine(const unsigned int &nLimitLine);
	virtual const unsigned int &GetLimitLine(void){return m_nLimitLine;}

	virtual const int GetFontInfoIndex(const FONTINFO *pFontInfo);
	virtual const FONTINFO *GetFontInfo(const int &nIndex);

	virtual const int GetLineIndex(CRichLine *pLine);
	virtual const CRichLine *GetLineFromIndex(const int &nIndex);

	//�� ������ �ּ� ���̸� �����Ѵ�. ���ο� ���Ե� �������� �� ���̺��� ū���� �ִٸ� �װ��� �ش� ������ ���̷� ����Ѵ�.
	//-1�̸� �ּ� ���̰� �������� ���������� �� ������ �������� ���� ���̰� ū���� �ش� ������ ���̷� ����Ѵ�.
	virtual void SetMinimumHight(int nMinimumHight);
	virtual const int &GetMiminumHight(void);

	//nMaxLength�� �������� ������ �ִ� ���ڰ��� �ǹ��Ѵ�.
	//-1�̸� ��� ���ڿ��� �����´�.
	virtual const std::string &GetString(int nMaxLength = -1);

	//Object ���� ���� �Լ���
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual void MakeSample(void);

	//���� ������ ��ũ�ѹ��� Thumb�� ���콺�� �巡�� �Ҷ� Line������ ��ũ�� ���� ���θ� �����Ѵ�.
	virtual void SetLineScroll(const bool &bLine);
	virtual const bool &IsLineScroll(void){return m_bLineScroll;}

	virtual bool OnLButtonDown(const CPoint &ptPos);
	virtual bool OnRButtonDown(const CPoint &ptPos);

	virtual LRESULT OnLocalChildNotify(CMessage *msg);

	LRESULT OnClickUp(CMessage *pMsg);
	LRESULT OnClickDown(CMessage *pMsg);
	LRESULT OnClickPageUp(CMessage *pMsg);
	LRESULT OnClickPageDown(CMessage *pMsg);
	LRESULT OnClickValueChange(CMessage *pMsg);
	LRESULT OnFocusInScroll(CMessage *pMsg);

	virtual bool OnMouseWheel(const int &nKey, const short &zDelta);
	virtual void SetObjectProject(CObjectProject *pProject);
protected:
	virtual void SplitEmoticon(std::string strOutput, std::list<CTempRichText> &listText);
	virtual int GetEmoticonIndex(std::string &strTemp);

	virtual void AddForceText(std::string strText, LPARAM lParam);
	virtual	CRichItem *AddEmoticon(const std::string &strOutput, int nIndex, LPARAM lParam);
	virtual	CRichItem *AddItem(const std::string &strOutput, int nEmoticonIndex, const CSize &szSize, bool bEnd, LPARAM lParam);

	virtual void SetRedraw(void);
	virtual	CSize GetTextSize(HDC &dc, LPCTSTR lpText, const int &nLength);
	virtual	bool CheckRedraw(void);
	virtual void CheckScrollData(void);
	virtual void CheckUp(void);
	virtual void CheckDown(void);
	virtual void CheckPageUp(void);
	virtual void CheckPageDown(void);
	virtual void RemoveLine(int nLineCount);
	virtual CRichItem *HitTest(const CPoint &ptPos, OUT CRect &rtItem);

	virtual void RepositionLine(void);
};
} //namespace LSY