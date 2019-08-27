#pragma once

#include "../Button/CheckButton.h"

namespace LSY
{
class CList;
class CListItem;
class CListItemData;

typedef std::list<CListItemData *>	LISTITEMDATA;
typedef LISTITEMDATA::iterator		LISTITEMDATAITER;

typedef std::list<CListItem *>		LISTITEM;
typedef LISTITEM::iterator			LISTITEMITER;

class UPG_LSYLIB_EXPORT CListItemData
{
protected:
	LPARAM					m_lParam;
	std::string				m_strText;
	int						m_nIndex;
	bool					m_bSelect;
	bool					m_bEnable;
	CListItem				*m_pItem;
	CList					*m_pList;

	std::map<int, LPARAM>	m_mapUserData;
public:
	CListItemData();
	virtual ~CListItemData();

	LPARAM GetItemData(void){return m_lParam;}			//설정된 Item Data를 구해오는 함수
	void SetItemData(LPARAM lParam){m_lParam = lParam;}	//Item Data를 설정하는 함수

	void SetItemIndex(const int &nIndex){m_nIndex = nIndex;}
	const int &GetItemIndex(void){return m_nIndex;}

	void SetSelect(const bool &bSelect){m_bSelect = bSelect;}
	const bool &IsSelect(void){return m_bSelect;}

	void SetEnable(const bool &bEnable){m_bEnable = bEnable;}
	const bool &IsEnable(void){return m_bEnable;}

	void SetList(CList *pList){m_pList = pList;}

	void SetItem(CListItem *pItem){m_pItem = pItem;}
	CListItem *GetItem(void);

	void SetText(const std::string &strText);
	const std::string &GetText(void){return m_strText;}

	void SetUserData(int nIndex, LPARAM lParam);
	LPARAM GetUserData(int nIndex);
};

class UPG_LSYLIB_EXPORT CListItem : public CCheckButton
{
protected:
	CListItemData	*m_pData;
	CList			*m_pList;
	bool			m_bAttach;
public:
	CListItem();
	virtual ~CListItem();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_LISTITEM;}

	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual void SetAttach(bool bAttach){m_bAttach = bAttach;}

	virtual LRESULT OnObserverNotify(CMessage *msg);

	void SetData(CListItemData *pData){m_pData = pData;}
	CListItemData *GetData(void);

	void SetList(CList *pList){m_pList = pList;}
	CList *GetList(void){return m_pList;}

	virtual void OnMouseLClick(const CPoint &ptPos);
	virtual void UpdateData(const LPARAM &lParam){}
};
} //namespace LSY