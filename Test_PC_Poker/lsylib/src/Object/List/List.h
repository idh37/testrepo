#pragma once

#include "../Button/CheckButton.h"
#include <queue>
#include "ListItem.h"

namespace LSY
{
class CListItem;
class CListItemData;
class CScrollBar;
class CDumyImage;

class UPG_LSYLIB_EXPORT CList : public CObjectBase
{
public:
	//아이템 비교 함수 포인터
	typedef bool (*LISTCOMPAREFUNCTION)(CListItemData *pItem1, CListItemData *pItem2, bool bIncrement, LPARAM lParam);
	//아이템이 일치하는지 검사하는 함수 포인터
	typedef bool (*LISTITEMFINDFUNCTION)(CListItemData *pItem, LPARAM lUserData, LPARAM lParam);
	//아이템 상태 셋팅 함수 포인터
	typedef bool (*LISTITEMUPDATAFUNCTION)(CListItem *pItem, LPARAM lParam);
	//아이템이 생성되었을 때 호출되는 함수 포인터
	typedef bool (*LISTITEMCLONEDFUNCTION)(CListItemData *pItem, LPARAM lParam);
protected:
	struct CCompareFuntion
	{
	public:
		LISTCOMPAREFUNCTION		m_pFunction;
		LPARAM					m_lParam;		//Compare시 사용할 변수
		bool					m_bIncrement;

		CCompareFuntion() : m_pFunction(NULL), m_lParam(NULL), m_bIncrement(true) {}
		bool operator() (CListItemData *pItem1, CListItemData *pItem2);
	};

	struct CFindFuntion
	{
	public:
		CFindFuntion():m_pFindFunction(NULL), m_lParam(NULL){}
		LISTITEMFINDFUNCTION	m_pFindFunction;
		LPARAM					m_lParam;		//Find시 사용할 변수
	};

	CCompareFuntion						m_DefaultCompare;
	CFindFuntion						m_DefaultFind;

	std::map<int, CCompareFuntion>		m_listCompare;	//아이템 비교 함수 포인트 리스트
	std::map<int, CFindFuntion>			m_listFind;		//아이템 찾기 함수 포인트 리스트

	LISTITEMUPDATAFUNCTION				m_pUpdateFunction;	//아이템 상태를 변경하는 함수 포인터
	LPARAM								m_lUpdateParam;
	LISTITEMCLONEDFUNCTION				m_pClonedFunction;	//복사된후 호출되는 함수 포인터
	LPARAM								m_lClonedParam;

	LISTITEM							m_listItems;
	LISTITEM							m_listNotUseItems;

	CListItem							*m_pSourceItem;
	LISTITEMDATA						m_listData;

	bool								m_bIncrement;
	int									m_nSortType;
	bool								m_bAutoSort;
	int									m_nFindType;

	int									m_nXNum;		//X축 분할
	int									m_nYNum;		//Y축 분할
	CPoint								m_ptItemGab;	//아이템 사이의 거리 차이

	bool								m_bSingleSelect;
	bool								m_bVirtical;	//세로

	CScrollBar							*m_pScroll;
	int									m_nItemWidth;
	int									m_nItemHeight;
	int									m_nClipWidth;
	int									m_nClipHeight;
	int									m_nScrollPos;

	CRect								m_rtListRect;
	LISTITEMDATAITER					m_itFind;
	bool								m_bLineScroll;
	CObjectBase							*m_pItemMain;
	bool								m_bReposition;

	CDumyImage							*m_pRectImage;
public:
	CList(void);
	virtual ~CList();

	virtual bool Create(int nXNum, int nYNum);

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_LIST;}

	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	virtual bool OnMouseWheel(const int &nKey, const short &zDelta);
	virtual LRESULT OnLocalChildNotify(CMessage *msg);

	//아이템의 상태를 셋팅하는 함수를 지정한다.
	void SetUpdateFunction(LISTITEMUPDATAFUNCTION pUpdateFunction);
	void SetUpdateFunctionParam(LPARAM lParam){m_lUpdateParam = lParam;}
	//아이템이 생성되었을 때 호출해주는 함수를 지정한다.
	void SetClonedFunction(LISTITEMCLONEDFUNCTION pClonedFunction);
	void SetClonedFunctionParam(LPARAM lParam){m_lClonedParam = lParam;}

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
	virtual const int Sort(CListItemData *pData = NULL);
	//접속중인 유저 리스트 셔플기능 추가:2012.10.22 YS.Jeong
	virtual const int SortShuffle(CListItemData *pData = NULL);

	//찾기 관련 함수들
	virtual void SetFindFunction(const int &nType, const LISTITEMFINDFUNCTION &pFindFunction);
	virtual void SetFindFunctionParam(const int &nType, const LPARAM &lParam);
	virtual CFindFuntion *GetFindFunction(const int &nType);

	//아이템을 선택 상태를 변경하여주는 함수
	bool SetItemSelectFromFind(int nType, LPARAM lUserData, bool bSelect);
	bool SetItemSelectFromIndex(int nIndex, bool bSelect);
	bool SetItemSelectFromParam(LPARAM lParam, bool bSelect);
	bool SetItemSelectFromString(std::string strText, bool bSelect);

	//아이템의 총 갯수를 구하는 함수
	int GetItemCount(void){return (int)m_listData.size();}

	//아이템을 상태를 설정하는 함수들
	void UpdateItemFromFind(int nType, LPARAM lUserData);	//아이템을 검색하여 해당 아이템이 발견되면 상태를 셋팅하는 함수
	void UpdateItemFromIndex(int nIndex);
	void UpdateItemFromParam(LPARAM lParam);
	void UpdateItemFromString(std::string strText);

	//아이템의 활성화 상태를 변경하는 함수
	bool SetEnableItemFromFind(int nType, LPARAM lUserData, bool bEnable);	//아이템을 검색하여 검색된 아이템의 활성화 상태를 변경하는 함수
	bool SetEnableItemFromIndex(int nIndex, bool bEnable);					//nIndex번째의 아이템의 활성화 상태를 변경하는 함수
	bool SetEnableItemFromParam(LPARAM lParam, bool bEnable);
	bool SetEnableItemFromString(std::string strText, bool bEnable);

	//아이템의 활성화 상태를 구해오는 함수
	bool IsEnableItemFromFind(int nType, LPARAM lUserData);			//아이템을 검색하여 검색된 아이템의 활성화 상태를 구해오는 함수
	bool IsEnableItemFromIndex(int nIndex);							//nIndex번째의 아이템의 활성화 상태를 구해오는 함수
	bool IsEnableItemFromParam(LPARAM lParam);
	bool IsEnableItemFromString(std::string strText);

	//아이템의 텍스트를 변경한다.
	bool SetStringFromIndex(int nIndex, std::string strText);
	bool SetStringFromParam(LPARAM lParam, std::string strText);
	bool SetStringFromFind(int nType, LPARAM lUserData, std::string strText);
	bool SetStringFromString(std::string strOldText, std::string strText);

	//아이템을 지워주는 함수
	virtual bool DeleteItem(int nIndex);							//nIndex 번째의 아이템을 삭제하는 함수
	virtual bool DeleteItem(LPARAM lParam);							//lParam과 같은 UserData를 가지는 첫번째 아이템을 삭제하는 함수
	virtual bool DeleteItem(int nType, LPARAM lUserData);			//Find함수를 사용하여 아이템을 검색한 후 검색된 아이템을 삭제하는 함수
	virtual bool DeleteItem(std::string strText);						//아이템의 텍스트가 strText인 첫번째 아이템을 찾아서 지워주는 함수
	virtual void DeleteAllItem(void);

	//지정된 아이템을 보이도록(스크롤) 하는 함수들
	//지정된 아이템이 리스트의 첫 라인에 보이는걸 원칙으로 한다.
	//만약, 지정된 아이템에서 마지막 아이템까지 높이가 Page보다 작으면 가장 마지막으로 Scroll을 한다.
	void SetVisibleItemFromIndex(int nIndex);
	void SetVisibleItemFromParam(LPARAM lParam);
	void SetVisibleItemFromFind(int nType, LPARAM lUserData);
	void SetVisibleItemFromString(std::string strText);

	virtual void SetSingleSelect(bool bSingleSelect);				//한개의 아이템만 선택 여부를 지정한다.
	void ClearAllSelect(void);										//모든 아이템의 선택을 취소한다
	bool IsSingleSelect(void){return m_bSingleSelect;}				//한개의 아이템만 선택 여부 구해온다.

	const bool &IsVirtical(void){return m_bVirtical;}
	void SetVirtical(const bool &bVirtical);

	void SetClipSize(int nWidth, int nHeight);
	int GetClipSizeWidth(void){return m_nClipWidth;}
	int GetClipSizeHehght(void){return m_nClipHeight;}

	//현재 선택된 아이템을 구해오는 함수들
	//GetFirstSelectItem을 먼저 호출하여 첫번째 선택된 아이템을 구해오고, 그 다음 부터는 GetNextSelectItem 함수로 
	//차례로 선택된 아이템을 구해오면 된다.
	CListItemData *GetFirstSelectItem(void);
	CListItemData *GetNextSelectItem(void);

	//아이템을 처음부터 검색하는 함수
	//GetFirstItem을 먼저 호출하여 첫번째 아이템을 구해오고, 그 다음 부터는 GetNextItem 함수로 
	//차례로 아이템을 구해오면 된다.
	CListItemData *GetFirstItem(void);
	CListItemData *GetNextItem(void);

	CListItemData *GetItemFromIndex(int nIndex);			//nIndex번째 아이템을 가져오는 함수
	CListItemData *GetItemFromParam(LPARAM lParam);			//lParam과 동일한 User Data를 가지고 있는 아이템을 구해오는 함수
	CListItemData *GetItemFromString(std::string strText);
	CListItemData *GetFirstFindItem(int nType, LPARAM lUserData);	//조건을 만족하는 아이템을 구해오는 함수, SetFindFunction에서 등록한 함수로 검색한다.
	CListItemData *GetNextFindItem(int nType, LPARAM lUserData);	//조건을 만족하는 아이템을 구해오는 함수, SetFindFunction에서 등록한 함수로 검색한다.

	//아이템 사이의 Gap을 설정한다
	void SetItemGab(float fGapX, float fGapY);
	void SetItemGab(CPoint ptGap);
	float GetItemGapX(void){return m_ptItemGab.x;}
	float GetItemGapY(void){return m_ptItemGab.y;}

	//아이템을 복사해서 생성할 기본 아이템을 등록한다.
	void AttachItem(CListItem *pItem);
	//아이템을 복사해서 생성할 기본 아이템을 구해오는 함수
	CListItem *GetItem(void){return m_pSourceItem;}
	void DettachItem(void);
	//스크롤바를 등록하는 함수
	void AttachScroll(CScrollBar *pScroll);
	//등록된 스크롤발를 구해오는 함수
	CScrollBar *GetScrollBar(void){return m_pScroll;}
	void DettachScroll(void);
	//리스트의 분할을 설정하는 함수
	void SetSplit(int nXNum, int nYNum);
	const int &GetXSplit(void){return m_nXNum;}
	const int &GetYSplit(void){return m_nYNum;}

	virtual int AddItem(LPARAM lParam);
	virtual int AddString(std::string strText);

	void SetLineScroll(const bool &bLineScroll){m_bLineScroll = bLineScroll;}
	const bool &IsLineScroll(void){return m_bLineScroll;}

	int GetFirstVisibleItem(void);	//화면에 가장 처음에 보이는 아이템
	int GetLastVisibleItem(void);	//화면에 가장 마지막에 보이는 아이템
	CListItemData *HitTest(const CPoint &ptPos);

	LRESULT OnListItemNotify(CMessage *pMsg);

	void MakeSample(void);
protected:
	void ReIndexItem(void);
	void SetReposition(const bool &bReposition){m_bReposition = bReposition;}
	const bool &IsReposition(void){return m_bReposition;}

	int	GetItemIndex(CListItem *pItem);						//아이템의 인덱스를 구해오는 함수
	CListItem *GetNewItem(void);

	//아이템을 상태를 설정하는 함수들
	void UpdateItem(CListItem *pItem);						//pItem의 상태를 셋팅하도록 하는 함수
	void UpdateItem(CListItemData *pData);					//pItem의 상태를 셋팅하도록 하는 함수

	//아이템을 선택 상태를 변경하여주는 함수
	void SetItemSelect(CListItem *pItem);
	bool SetItemSelect(CListItemData *pData, bool bSelect);

	//아이템의 활성화 상태를 변경하는 함수
	void SetEnableItem(CListItem *pItem);					//pItem의 활성화 생태를 변경하는 함수
	bool SetEnableItem(CListItemData *pData, bool bEnable);	//pItem의 활성화 생태를 변경하는 함수

	//아이템의 활성화 상태를 구해오는 함수
	bool IsEnableItem(CListItem *pItem);					//pItem의 활성화 생태를 구해오는 함수
	bool IsEnableItem(CListItemData *pData);				//pItem의 활성화 생태를 구해오는 함수

	//아이템의 텍스트를 변경한다.
	void SetString(CListItem *pItem);
	bool SetString(CListItemData *pData, std::string strText);

	//아이템을 지워주는 함수
	bool DeleteItem(CListItemData *pData);			//pItem을 아이템 리스트에서 삭제하는 함수

	//지정된 아이템을 보이도록(스크롤) 하는 함수들
	//지정된 아이템이 리스트의 첫 라인에 보이는걸 원칙으로 한다.
	//만약, 지정된 아이템에서 마지막 아이템까지 높이가 Page보다 작으면 가장 마지막으로 Scroll을 한다.
	void SetVisibleItem(CListItem *pItem);
	void SetVisibleItem(CListItemData *pData);

	virtual void CalculteClipRect(void);
	virtual void SetScrollData(void);
	virtual void RepositionItem(void);

	virtual void CheckUp(void);
	virtual void CheckDown(void);
	LRESULT OnClickUp(CMessage *pMsg);
	LRESULT OnClickDown(CMessage *pMsg);

	virtual void CheckPageUp(void);
	virtual void CheckPageDown(void);
	LRESULT OnClickPageUp(CMessage *pMsg);
	LRESULT OnClickPageDown(CMessage *pMsg);

	LRESULT OnClickValueChange(CMessage *pMsg);
	LRESULT OnFocusInLocalChild(CMessage *pMsg);

	void GetNextItemIter(LISTITEMITER &it, int nNext);
	void CheckReposition(void);

	friend CListItem;
	friend CListItemData;
};

//Clone된 ListItem의 Child Object에서 발생한 메시지를 Original Object에 Functor을 통해서 전달 받았을때
//해당 메시지를 가지고 아이템 정보를 얻어오는 함수들
UPG_LSYLIB_EXPORT extern CListItem *GetListItem(CMessage *pMsg);
UPG_LSYLIB_EXPORT extern CListItem *GetListItem(CObjectBase *pObject);
UPG_LSYLIB_EXPORT extern CListItemData *GetListItemData(CMessage *pMsg);
UPG_LSYLIB_EXPORT extern CListItemData *GetListItemData(CObjectBase *pObject);
UPG_LSYLIB_EXPORT extern int GetListItemIndex(CMessage *pMsg);
} //namespace LSY