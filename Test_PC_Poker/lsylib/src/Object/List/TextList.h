#pragma once

#include "../Base/ObjectBase.h"
#include <string>
#include <list>
#include <vector>

namespace LSY
{
class CScrollBar;
class CDumyImage;
class CImage;

class UPG_LSYLIB_EXPORT CTextList : public CObjectBase
{
public:
	class TEXTITEM
	{
	public:
		std::string m_strOutput;
		CSize		m_szSize;
		COLORREF	m_clrColor;
		float		m_fStartY;
		LPARAM		m_lParam;			//아이템의 User Data
	public:
		TEXTITEM();
		virtual ~TEXTITEM();

		TEXTITEM *Clone(void);

		virtual size_t GetInheritanceClipSize(int &nVersion);
		virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
		virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);
	};

	typedef bool (*LISTCOMPAREFUNCTION)(TEXTITEM *pItem1, TEXTITEM *pItem2, bool bIncrement, LPARAM lParam);
	typedef bool (*LISTITEMFINDFUNCTION)(TEXTITEM *pItem, LPARAM lUserData, LPARAM lParam);
protected:
	struct CCompareFuntion
	{
	public:
		LISTCOMPAREFUNCTION		m_pFunction;
		LPARAM					m_lParam;		//Compare시 사용할 변수
		bool					m_bIncrement;

		CCompareFuntion() : m_pFunction(NULL), m_lParam(NULL), m_bIncrement(true) {}
		bool operator() (TEXTITEM *pItem1, TEXTITEM *pItem2);
	};

	struct CFindFuntion
	{
	public:
		CFindFuntion():m_pFindFunction(NULL), m_lParam(NULL){}
		LISTITEMFINDFUNCTION	m_pFindFunction;
		LPARAM					m_lParam;		//Find시 사용할 변수
	};

	CCompareFuntion	m_DefaultCompare;
	CFindFuntion	m_DefaultFind;

	typedef std::list<TEXTITEM *>			TEXTITEMLIST;
	typedef TEXTITEMLIST::iterator			TEXTITEMLISTITER;

	NMBASE::GRAPHICGDI::xSprite			*m_pBar;
	int				m_nBarIndex;

	TEXTITEMLIST					m_listItems;
	std::map<int, CCompareFuntion>	m_listCompare;	//아이템 비교 함수 포인트 리스트
	std::map<int, CFindFuntion>		m_listFind;		//아이템 찾기 함수 포인트 리스트

	bool			m_bRedraw;

	FONTINFO		m_cFontInfo;

	COLORREF		m_clrFocusText;
	COLORREF		m_clrSelectText;
	
	COLORREF		m_clrSelectBack;
	COLORREF		m_clrFocusBack;
	bool			m_bUseFocus;

	int				m_nSortType;
	int				m_nFindType;
	int				m_nIndent;		//들여쓰기
	bool			m_bIncrement;
	bool			m_bAutoSort;

	bool			m_bUseSprite;
	bool			m_bLineScroll; //라인 단위로 스크롤바의 Thumb를 마우스로 드래그 할때 Line단위로 스크롤 할지 여부를 설정한다.
	int				m_nLineHeight;

	int				m_nFindItem;
	TEXTITEMLISTITER m_itFind;

	float			m_fStartY;
	float			m_fTotalHeight;

	CScrollBar		*m_pScrollBar;
	CImage			*m_pImage;
	CDumyImage		*m_pBack;
protected:
	TEXTITEM		*m_pFirstVisibleItem;
	TEXTITEMLISTITER m_itFirstVisible;
	int				 m_nFirstVisible;

	TEXTITEM		*m_pLastVisibleItem;
	TEXTITEMLISTITER m_itLastVisible;
	int				 m_nLastVisible;
	
	TEXTITEM		*m_pSelectItem;
	int				 m_nSelectItem;

	TEXTITEM		*m_pFocusItem;
	int				 m_nFocusItem;
public:
	CTextList(void);
	virtual ~CTextList();

	virtual bool Create(int nWidth, int nHeight);
	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_TEXTLIST;}

	virtual bool IsPossibilityReSize(void){return true;}

	//추가 삭제 관련 함수들
	virtual int AddText(const std::string &strText);
	virtual int InsertText(const int &nIndex, const std::string &strText);
	
	virtual bool RemoveText(const std::string &strText);
	virtual bool RemoveText(const int &nIndex);
	virtual bool RemoveText(const LPARAM &lParam);
	virtual void RemoveAll(void);

	void MakeSample(void);

	//아이템 데이터 관련 함수들
	virtual void SetItemData(const int &nIndex, const LPARAM &lParam);
	virtual const LPARAM GetItemData(const int nIndex);

	//아이템 선택 관련 함수들
	virtual const int &GetCurSelectItem(void);
	virtual void SetCurSelectItem(const int &nSel);

	//라인 높이 관련 함수들
	virtual void SetLineHeight(const int &nLineHeight);
	virtual const int &GetLineHeight(void);

	//nIndex 아이템이 화면에 보이는지 알아보는 함수
	virtual const bool IsVisualItem(const int &nIndex);
	//화면에 보이는 아이템중 ptPos에 있는 아이템이 어떤것인지 알아보는 함수
	virtual const int  HitTest(const CPoint &ptPos);

	//화면에 처음으로 보이는 아이템, 마지막으로 보이는 아이템을 알아내는 함수
	virtual const int &GetFirstVisibleItem(void){return m_nFirstVisible;}
	virtual const int &GetLastVisibleItem(void){return m_nLastVisible;}

	//아이템의 총 갯수를 구한다.
	virtual const int GetItemCount(void);
	//nIndex의 아이템 텍스트를 구한다.
	virtual const std::string GetItemString(const int &nIndex);
	virtual void SetItemString(const int &nIndex, const std::string &strText);
	virtual void SetItemColor(const int &nIndex, const COLORREF &clrColor);

	virtual void SetLineScroll(const bool &bLineScroll){m_bLineScroll = bLineScroll;}
	virtual const bool &IsLineScroll(void){return m_bLineScroll;}

	//폰트 관련 함수들
	virtual void SetFont(const std::string &strName, const int &nSize, const bool &bBold);
	virtual const CFont *GetFont(void);
	virtual const std::string &GetFontName(void);
	virtual const int &GetFontSize(void);
	virtual const bool &IsBold(void);

	virtual void SetDrawType(const ENUM_TEXTDRAW_TYPE &nType);
	virtual void SetFormat(const UINT &uFormat);
	virtual void SetSpaceBetweenLetters(const int &nSpaceBetweenLetters);

	virtual void SetIndent(const int &nIndent);
	virtual const int &GetIndent(void){return m_nIndent;}

	virtual const ENUM_TEXTDRAW_TYPE &GetDrawType(void);
	virtual const UINT &GetFormat(void);
	virtual const int &GetSpaceBetweenLetters(void);

	//nIndex 아이템의 화면의 uFormat 위치에 보이도록 셋팅한다.
	//uFormat값은 DT_VCENTER, DT_TOP, DT_BOTTOM 이다.
	virtual void SetVisibleItem(int nIndex, UINT uFormat);
	//nIndex 아이템을 화면에 보이게 한다. 이미 보이면 그래도 두고
	//보이는쪽 보다 윗쪽에 있으면 TOP으로 아래에 있으면 Bottom으로 보이도록 한다.
	virtual void SetVisibleItem(int nIndex);

	//텍스트 및 바색관련 함수들
	virtual void SetTextNormalColor(const COLORREF &clrNormal);
	virtual void SetTextFocusColor(const COLORREF &clrFocus);
	virtual void SetTextSelectColor(const COLORREF &clrSelect);
	virtual void SetTextOutlineColor(const COLORREF &clrOutline);

	virtual const COLORREF &GetTextNormalColor(void);
	virtual const COLORREF &GetTextFocusColor(void);
	virtual const COLORREF &GetTextSelectColor(void);
	virtual const COLORREF &GetTextOutlineColor(void);

	virtual void SetBackFocusColor(const COLORREF &clrFocus);
	virtual void SetBackSelectColor(const COLORREF &clrSelect);

	virtual const COLORREF &GetBackFocusColor(void);
	virtual const COLORREF &GetBackSelectColor(void);

	//이미지 바 관련 함수들
	virtual bool SetBarSprite(const std::string &strPath);
	virtual const std::string GetBarSprite(void);

	virtual void SetBarIndex(const int &nIndex);
	virtual const int &GetBarIndex(void){return m_nBarIndex;}
	virtual const int GetTotalBarNum(void);

	virtual void SetUseBarSprite(const bool &bUse);
	virtual const bool &IsUseBarSprite(void);

	virtual void SetUseFocus(const bool &bUseFocus);
	virtual const bool &IsUseFocus(void){return m_bUseFocus;}

	//스크롤 컨트롤 관련 함수들
	virtual void AttachScroll(CScrollBar *pScall);
	virtual const CScrollBar *GetScrollBar(void){return m_pScrollBar;}
	virtual void DettachScroll(void);

	virtual void AttachBackImage(CImage *pImage);
	virtual const CImage *GetBackImage(void){return m_pImage;}
	virtual void DettachBackImage(void);

	//소트 관련 함수들
	virtual CCompareFuntion *GetCompareFunction(const int &nType);
	virtual void SetCompareFunction(const int &nType, const LISTCOMPAREFUNCTION pCompare);
	virtual void SetCompareFunctionParam(const int &nType, const LPARAM &lParam);
	virtual void SetIncrement(const bool &bIncrement);
	virtual const bool &IsIncrement(void){return m_bIncrement;}
	virtual void SetSortType(const int &nType);
	virtual const int &GetSortType(void){return m_nSortType;}
	virtual void SetAutoSort(const bool &bAuto);
	virtual const bool &IsAutoSort(void){return m_bAutoSort;}
	virtual const int Sort(TEXTITEM *pItem = NULL);

	//찾기 관련 함수들
	virtual void SetFindFunction(const int &nType, const LISTITEMFINDFUNCTION &pFindFunction);
	virtual void SetFindFunctionParam(const int &nType, const LPARAM &lParam);
	virtual CFindFuntion *GetFindFunction(const int &nType);

	virtual void SetDefaultFindType(bool bUserData);

	//기본 찾기는 스트링으로 검색한다.
	virtual int FirstFind(const LPARAM &lUserData);
	virtual int NextFind(const LPARAM &lUserData);

	//라인높이를 이용하여 리스트의 크기를 결정하는 함수
	//nTotalLine * m_nLineHeight를 높이로 설정한다.
	virtual void SetTotalHeightUseLineHeight(int nTotalLine);
public:
	//상속 받은 함수들
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual LRESULT OnLocalChildNotify(CMessage *msg);

	virtual void OnMouseLeave(void);
	virtual bool OnMouseMove(const CPoint &ptPos);
	virtual void OnMouseLClick(const CPoint &ptPos);

	LRESULT OnClickUp(CMessage *pMsg);
	LRESULT OnClickDown(CMessage *pMsg);
	LRESULT OnClickPageUp(CMessage *pMsg);
	LRESULT OnClickPageDown(CMessage *pMsg);
	LRESULT OnClickValueChange(CMessage *pMsg);
	LRESULT OnFocusInLocalChild(CMessage *pMsg);

	virtual bool OnMouseWheel(const int &nKey, const short &zDelta);
protected:
	virtual void	SetRedraw(void);
	virtual CSize	GetTextSize(HDC &dc, LPCTSTR lpText, const int &nLength);
	virtual bool	CheckRedraw(void);
	virtual void	CheckVisibleItem(void);
	virtual void	CheckFocusItem(void);
	virtual const bool	IsVisualItem(TEXTITEM *pItem);
	virtual bool	IsInItem(TEXTITEM *pItem, const CPoint &ptPos);
	virtual int		GetItemIndex(TEXTITEM *pItem);
	virtual TEXTITEM *GetItem(const int &nIndex);
	virtual void	CheckItemResize(void);
	virtual void	CheckScrollData(void);
	virtual void	CheckUp(void);
	virtual void	CheckDown(void);
	virtual void	CheckPageUp(void);
	virtual void	CheckPageDown(void);
};
} //namespace LSY