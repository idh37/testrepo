#pragma once

#include "../Base/ObjectBase.h"
#include "EditCursor.h"

namespace LSY
{
#define PASSWORD_CHAR			"*"

class CScrollBar;
class UPG_LSYLIB_EXPORT CEditItem
{
public:
	char	m_str[3];
	CRect	m_rtRect;
public:
	CEditItem();
	virtual ~CEditItem(){}

	bool IsIn(const CPoint &ptLocal, const CPoint &ptStartPos, const float &fSpaceBetweenLetters);
	bool IsPrevIn(const CPoint &ptLocal, const CPoint &ptStartPos);

	const size_t GetLength(void);
	const float GetWidth(void){return m_rtRect.GetWidth();}
	const float GetHeight(void){return m_rtRect.GetHeight();}
	const CRect &GetRect(void){return m_rtRect;}
	const float &left(void){return m_rtRect.left;}
	const float &right(void){return m_rtRect.right;}
	const float &top(void){return m_rtRect.top;}
	const float &bottom(void){return m_rtRect.bottom;}
	void SetPos(const CPoint &ptPos){m_rtRect.SetPos(ptPos);}
};

typedef std::list<CEditItem *>		EDITITEMLIST;
typedef EDITITEMLIST::iterator		EDITITEMLISTITER;

class UPG_LSYLIB_EXPORT CEditLine
{
protected:
	EDITITEMLIST	m_listItems;
	CPoint			m_ptPos;
	float			m_fTotalWidth;
public:
	CEditLine();
	virtual ~CEditLine();

	const CEditLine &operator=(const CEditLine &w);
	const CEditLine &operator=(const CPoint &w);
	const CEditLine &operator=(const float &w);
	const CEditLine &operator=(const EDITITEMLIST &w);
	const CEditLine &operator+=(const float &w);
	const CEditLine &operator-=(const float &w);

	EDITITEMLISTITER begin();
	EDITITEMLISTITER end();
	const size_t size(void);

	bool IsLineIn(const CPoint &ptLocal, const CPoint &ptStartPos, const float &fLineSpace, const float &fLineHeight);
	bool IsPrevLineIn(const CPoint &ptLocal, const CPoint &ptStartPos);

	const float &GetTotalWidth(void){return m_fTotalWidth;}
	const CPoint &GetPoint(void){return m_ptPos;}
	const float &GetPosX(void){return m_ptPos.x;}
	const float &GetPosY(void){return m_ptPos.y;}
	void SetPosX(const float fPosX){m_ptPos.x = fPosX;}
	void SetPosY(const float fPosY){m_ptPos.y = fPosY;}

	void push_back(CEditItem *pItem);
	void push_front(CEditItem *pItem);
	EDITITEMLISTITER erase(const EDITITEMLISTITER &start, const EDITITEMLISTITER &end);
	EDITITEMLISTITER erase(const EDITITEMLISTITER &start);
	EDITITEMLISTITER insert(const EDITITEMLISTITER &start, CEditItem *pItem);
};

typedef std::list<CEditLine *>		EDITLINELIST;
typedef EDITLINELIST::iterator		EDITLINELISTITER;

class UPG_LSYLIB_EXPORT EDITPOS
{
protected:
	EDITLINELISTITER	m_itLine;
	EDITITEMLISTITER	m_itItem;

public:
	const EDITPOS &operator=(const EDITPOS &w);
	const EDITPOS &operator=(const EDITLINELISTITER &w);
	const EDITPOS &operator=(const EDITITEMLISTITER &w);

	operator CEditLine*();
	operator CEditItem*();

	operator CEditLine*() const;
	operator CEditItem*() const;

	operator EDITLINELISTITER();
	operator EDITITEMLISTITER();

	friend UPG_LSYLIB_EXPORT bool operator==(const EDITPOS &z, const EDITPOS &w);
	friend UPG_LSYLIB_EXPORT bool operator!=(const EDITPOS &z, const EDITPOS &w);
	friend UPG_LSYLIB_EXPORT bool operator==(const EDITPOS &z, const EDITLINELISTITER &w);
	friend UPG_LSYLIB_EXPORT bool operator!=(const EDITPOS &z, const EDITLINELISTITER &w);
	friend UPG_LSYLIB_EXPORT bool operator==(const EDITPOS &z, const EDITITEMLISTITER &w);
	friend UPG_LSYLIB_EXPORT bool operator!=(const EDITPOS &z, const EDITITEMLISTITER &w);

	EDITLINELISTITER &NextLine(void);
	EDITITEMLISTITER &NextItem(void);
	EDITLINELISTITER &PrevLine(void);
	EDITITEMLISTITER &PrevItem(void);
};

class CDumyImage;

class UPG_LSYLIB_EXPORT CEdit : public CObjectBase
{
protected:
	EDITLINELIST		m_listLines;
	EDITLINELISTITER	m_itLineEnd;
	EDITPOS				m_cCurrentPos;

	CDumyImage		*m_pBack;

	CPoint			m_ptStartPos;

	FONTINFO		m_cFontInfo;

	int				m_nLimitText;
	bool			m_bPassword;

	CEditCursor		*m_pCursor;

	//임시 변수들
	bool			m_bKoreanMode;
	bool			m_bStat;				// WM_IME_STARTCOMPOSITION과 [WM_IME_ENDCOMPOSITION 또는 WM_IME_COMPOSITION - GCS_RESULTSTR] 사이에 true
	bool			m_bIgnoreWMChar;		// m_bStat을 false로 만들 때 켠다. 
	bool			m_bIgnoreWMKeyDown;		//

	bool			m_bRedraw;
	bool			m_bCaculateCursorPos;
	bool			m_bReCaculateItemWidth;
	bool			m_bLineScroll;			//라인 스크롤 여부

	float			m_fLineSpace;
	float			m_fTotalHeight;
	float			m_fLineHeight;

	int				m_nScrollPos;
	bool			m_bAddScroll;

	EDITPOS			m_cViewFirstItem;
	EDITPOS			m_cViewEndItem;
	EDITPOS			m_cViewFind;

	bool			m_bNumber;

	//클립보드에서 사용하는 변수들
	bool			m_bUseClipboard;
	COLORREF		m_clrSelectBack;
	COLORREF		m_clrSelectFont;

	EDITPOS			m_cSelectStart;
	EDITPOS			m_cSelectEnd;

	//드래그에서 사용하는 임시 변수들
	EDITPOS			m_cDragFirst;
	EDITPOS			m_cDragEnd;
	CPoint			m_ptDragPos;
	DWORD			m_dwDragStartTime;
	DWORD			m_dwDragMoveTime;

	//스크롤 바
	CScrollBar		*m_pScrollBar;
public:
	CEdit(void);
	virtual ~CEdit();

	virtual bool Create(int nWidth, int nHeight);

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_EDIT;}
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	virtual bool IsPossibilityReSize(void){return true;}

	virtual CObjectBase *OnFocusIn(CObjectBase *pObject);
	virtual void OnFocusOut(CObjectBase *pObject);
	virtual BOOL OnKeyProc(const int &nMessage, const WPARAM &wParam, const LPARAM &lParam);

	virtual bool OnDragBegin(const CPoint &ptPos);
	virtual bool OnDraging(const CPoint &ptPos);
	virtual bool OnDragEnd(const CPoint &ptPos);
	virtual void Logic(const DWORD &dwTime);
	virtual void OnReturn(void);

	virtual LRESULT OnLocalChildNotify(CMessage *msg);

	LRESULT OnClickUp(CMessage *pMsg);
	LRESULT OnClickDown(CMessage *pMsg);
	LRESULT OnClickPageUp(CMessage *pMsg);
	LRESULT OnClickPageDown(CMessage *pMsg);
	LRESULT OnClickValueChange(CMessage *pMsg);
	LRESULT OnFocusInScroll(CMessage *pMsg);

	virtual void AttachScroll(CScrollBar *pScroll);
	virtual void DettachScroll(void);
	virtual CScrollBar *GetScroll(void){return m_pScrollBar;}

	virtual bool OnMouseWheel(const int &nKey, const short &zDelta);

	void	OnAddChar(const std::string &strChar, bool bForced = false);
public:
	void		SetText( const std::string& str );
	const std::string GetText(void);

	//선택된 영역의 텍스트를 구해오는 함수
	const std::string GetSelectText(void);				
	//영역을 선택하는 함수, 클립보드를 사용할 때만 유효하다.
	//nSelectStart == -1이면 선택이 취소된다.
	//nSelectEnd가 == -1이면 nSelectStart부터 끝까지 선택된다.
	void SetSelect(int nSelectStart, int nSelectEnd);
	//커서 위치를 변경해주는 함수
	void SetCursorPos(int nCursorPos);

	virtual	void SetCursorColor(const COLORREF &clrColor);
	virtual const COLORREF GetCursorColor(void);

	virtual	void SetCursorFontColor(const COLORREF &clrColor);
	virtual const COLORREF GetCursorFontColor(void);

	virtual void SetBlinkTime(const DWORD &dwBlinkTime);
	virtual const DWORD GetBlinkTime(void);

	virtual void SetFormat(const UINT &uFormat);
	virtual const UINT &GetFormat(void){return m_cFontInfo.m_uFormat;}

	virtual void SetPassword(const bool &bPassword);
	virtual const bool &IsPassword(void){return m_bPassword;}

	virtual void SetFont(const std::string &strFontName, const int &nFontSize, const bool &bBold);
	virtual const std::string &GetFontName(void){return m_cFontInfo.GetName();}
	virtual const int &GetFontSize(void){return m_cFontInfo.GetSize();}
	virtual const bool &IsBold(void){return m_cFontInfo.IsBold();}

	virtual void SetTextColor(const COLORREF &clrColor);
	virtual const COLORREF &GetTextColor(void){return m_cFontInfo.m_clrColor;}

	virtual void SetTextDrawType(const ENUM_TEXTDRAW_TYPE &nType);
	virtual const ENUM_TEXTDRAW_TYPE &GetTextDrawType(void){return m_cFontInfo.m_nDrawType;}

	virtual void SetShadowColor(const COLORREF &clrColor);
	virtual const COLORREF &GetShadowColor(void){return m_cFontInfo.m_clrOutline;}

	virtual void SetLimitText(const int &nLimit);
	virtual const int &GetLimitText(void){return m_nLimitText;}

	virtual void SetNumber(const bool &bNumber);
	virtual const bool &IsNumber(void){return m_bNumber;}

	virtual void SetUseClipboard(const bool &bUseClipboard);
	virtual const bool &IsUseClipboard(void){return m_bUseClipboard;}

	virtual void SetSelectBackColor(const COLORREF &clrColor);
	virtual const COLORREF &GetSelectBackColor(void){return m_clrSelectBack;}

	virtual void SetSelectFontColor(const COLORREF &clrColor);
	virtual const COLORREF &GetSelectFontColor(void){return m_clrSelectFont;}

	virtual void SetLineSpace(const int &nLineSpace);
	virtual const int GetLineSpace(void){return (int)m_fLineSpace;}

	virtual void SetLineScroll(const bool &bLineScroll);
	virtual const bool &IsLineScroll(void){return m_bLineScroll;}

	virtual const bool &IsRedraw(void){return m_bRedraw;}
	virtual CEditCursor *GetCursor(void){return m_pCursor;}
	virtual CDumyImage *GetBack(void){return m_pBack;}

	//자간 관련 함수들
	void SetSpaceBetweenLetters(const int &nSpaceBetweenLetters);
	const int GetSpaceBetweenLetters(void){return m_cFontInfo.m_nSpaceBetweenLetters;}

	const int	GetLength(void);
protected:
	void	CheckKoreanMode(bool bFocus);
	void	Clear(void);
	void	SetRedraw(void);
	void	SetCaculateCursorPos(void);
	void	SetReCaculateItemWidth(void);

	CDC		*ReCaculateItemWidth(void);

	void	SetCursorText( const std::string& str );
	const std::string &GetCursorText(void);

	CEditItem *GetItem(EDITPOS &cPos);
	CEditItem *GetPrevItem(IN EDITPOS &cPos, OUT EDITPOS &cPrevPos);
	CEditItem *GetNextItem(IN EDITPOS &cPos, OUT EDITPOS &cNextPos);
	bool	  IsFirstItem(EDITPOS &cPos);
	bool	  IsLastItem(EDITPOS &cPos);
	EDITPOS	  GetFirstPos(void);
	EDITPOS	  GetLastPos(void);
	bool	  IsFirstLine(EDITPOS &cPos);
	bool	  IsLastLine(EDITPOS &cPos);
	//w가 z보다 크거나 같은지를 비교하는 함수
	bool	  MoreThanItem(EDITPOS &w, EDITPOS &z);
	//w가 z보다 큰지를 비교하는 함수
	bool	  BigItem(EDITPOS &w, EDITPOS &z);

	CEditItem *GetViewFirstItem(EDITPOS &cPos);
	CEditItem *GetViewNextItem(void);

	CEditLine *GetViewFirstLine(EDITPOS &cStart, EDITPOS &cEnd);
	CEditLine *GetViewNextLine(EDITPOS &cStart, EDITPOS &cEnd);

	//ptLocal 위치의 아이템의 위치를 구해온다.
	EDITPOS	HitTest(EDITPOS &cStart, EDITPOS &cEnd, const CPoint &ptLocal);

	void		InsertEditItem( CEditItem *pItem );
	void		DeleteEditItem( bool bFront );
	bool		DeleteSelectItem( void );
	void		CheckItemTotalWidth(void);
	bool		IsByteToMerge(char const* pcchInput) const;

	bool	IsLineFeedChar(const char *pChar);
	void	CheckInsertMultiLine(EDITLINELISTITER &itLine, const float &fWidth);
	void	CheckDeleteMultiLine(EDITLINELISTITER &itLine, const float &fWidth);

	bool	OnSpecialKeyDown(const WPARAM &wParam, const LPARAM &lParam);
	void	OnCursorLeft(const bool &bPushCtrl, const bool &bPushShift);
	void	OnCursorHome(const bool &bPushCtrl, const bool &bPushShift);
	void	OnCursorRight(const bool &bPushCtrl, const bool &bPushShift);
	void	OnCursorEnd(const bool &bPushCtrl, const bool &bPushShift);
	void	OnCursorUp(const bool &bPushCtrl, const bool &bPushShift);
	void	OnCursorDown(const bool &bPushCtrl, const bool &bPushShift);
	void	OnCursorPageDown(const bool &bPushCtrl, const bool &bPushShift);
	void	OnCursorPageUp(const bool &bPushCtrl, const bool &bPushShift);
	void	OnBackSpace(void);
	void	OnDelete(void);
	CDC		*CalculateCursorPos(CDC *pDC);
	int		GetStartingOffset(size_t iFrom) const;

	void	CheckRedraw(CDC *pDC);
	void	OnMakeAddChars(char const * ach);

	bool	OnPasteClipboard(void);
	bool	OnCopyClipboard(void);
	bool	OnCutClipboard(void);

	virtual void CheckScrollData(void);
	virtual void CheckUp(void);
	virtual void CheckDown(void);
	virtual void CheckPageUp(void);
	virtual void CheckPageDown(void);

	virtual void SetObjectProject(CObjectProject *pProject);

	friend class CEditCursor;
};
} //namespace LSY