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
	//������ �� �Լ� ������
	typedef bool (*LISTCOMPAREFUNCTION)(CListItemData *pItem1, CListItemData *pItem2, bool bIncrement, LPARAM lParam);
	//�������� ��ġ�ϴ��� �˻��ϴ� �Լ� ������
	typedef bool (*LISTITEMFINDFUNCTION)(CListItemData *pItem, LPARAM lUserData, LPARAM lParam);
	//������ ���� ���� �Լ� ������
	typedef bool (*LISTITEMUPDATAFUNCTION)(CListItem *pItem, LPARAM lParam);
	//�������� �����Ǿ��� �� ȣ��Ǵ� �Լ� ������
	typedef bool (*LISTITEMCLONEDFUNCTION)(CListItemData *pItem, LPARAM lParam);
protected:
	struct CCompareFuntion
	{
	public:
		LISTCOMPAREFUNCTION		m_pFunction;
		LPARAM					m_lParam;		//Compare�� ����� ����
		bool					m_bIncrement;

		CCompareFuntion() : m_pFunction(NULL), m_lParam(NULL), m_bIncrement(true) {}
		bool operator() (CListItemData *pItem1, CListItemData *pItem2);
	};

	struct CFindFuntion
	{
	public:
		CFindFuntion():m_pFindFunction(NULL), m_lParam(NULL){}
		LISTITEMFINDFUNCTION	m_pFindFunction;
		LPARAM					m_lParam;		//Find�� ����� ����
	};

	CCompareFuntion						m_DefaultCompare;
	CFindFuntion						m_DefaultFind;

	std::map<int, CCompareFuntion>		m_listCompare;	//������ �� �Լ� ����Ʈ ����Ʈ
	std::map<int, CFindFuntion>			m_listFind;		//������ ã�� �Լ� ����Ʈ ����Ʈ

	LISTITEMUPDATAFUNCTION				m_pUpdateFunction;	//������ ���¸� �����ϴ� �Լ� ������
	LPARAM								m_lUpdateParam;
	LISTITEMCLONEDFUNCTION				m_pClonedFunction;	//������� ȣ��Ǵ� �Լ� ������
	LPARAM								m_lClonedParam;

	LISTITEM							m_listItems;
	LISTITEM							m_listNotUseItems;

	CListItem							*m_pSourceItem;
	LISTITEMDATA						m_listData;

	bool								m_bIncrement;
	int									m_nSortType;
	bool								m_bAutoSort;
	int									m_nFindType;

	int									m_nXNum;		//X�� ����
	int									m_nYNum;		//Y�� ����
	CPoint								m_ptItemGab;	//������ ������ �Ÿ� ����

	bool								m_bSingleSelect;
	bool								m_bVirtical;	//����

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

	//�������� ���¸� �����ϴ� �Լ��� �����Ѵ�.
	void SetUpdateFunction(LISTITEMUPDATAFUNCTION pUpdateFunction);
	void SetUpdateFunctionParam(LPARAM lParam){m_lUpdateParam = lParam;}
	//�������� �����Ǿ��� �� ȣ�����ִ� �Լ��� �����Ѵ�.
	void SetClonedFunction(LISTITEMCLONEDFUNCTION pClonedFunction);
	void SetClonedFunctionParam(LPARAM lParam){m_lClonedParam = lParam;}

	//��Ʈ ���� �Լ���
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
	//�������� ���� ����Ʈ ���ñ�� �߰�:2012.10.22 YS.Jeong
	virtual const int SortShuffle(CListItemData *pData = NULL);

	//ã�� ���� �Լ���
	virtual void SetFindFunction(const int &nType, const LISTITEMFINDFUNCTION &pFindFunction);
	virtual void SetFindFunctionParam(const int &nType, const LPARAM &lParam);
	virtual CFindFuntion *GetFindFunction(const int &nType);

	//�������� ���� ���¸� �����Ͽ��ִ� �Լ�
	bool SetItemSelectFromFind(int nType, LPARAM lUserData, bool bSelect);
	bool SetItemSelectFromIndex(int nIndex, bool bSelect);
	bool SetItemSelectFromParam(LPARAM lParam, bool bSelect);
	bool SetItemSelectFromString(std::string strText, bool bSelect);

	//�������� �� ������ ���ϴ� �Լ�
	int GetItemCount(void){return (int)m_listData.size();}

	//�������� ���¸� �����ϴ� �Լ���
	void UpdateItemFromFind(int nType, LPARAM lUserData);	//�������� �˻��Ͽ� �ش� �������� �߰ߵǸ� ���¸� �����ϴ� �Լ�
	void UpdateItemFromIndex(int nIndex);
	void UpdateItemFromParam(LPARAM lParam);
	void UpdateItemFromString(std::string strText);

	//�������� Ȱ��ȭ ���¸� �����ϴ� �Լ�
	bool SetEnableItemFromFind(int nType, LPARAM lUserData, bool bEnable);	//�������� �˻��Ͽ� �˻��� �������� Ȱ��ȭ ���¸� �����ϴ� �Լ�
	bool SetEnableItemFromIndex(int nIndex, bool bEnable);					//nIndex��°�� �������� Ȱ��ȭ ���¸� �����ϴ� �Լ�
	bool SetEnableItemFromParam(LPARAM lParam, bool bEnable);
	bool SetEnableItemFromString(std::string strText, bool bEnable);

	//�������� Ȱ��ȭ ���¸� ���ؿ��� �Լ�
	bool IsEnableItemFromFind(int nType, LPARAM lUserData);			//�������� �˻��Ͽ� �˻��� �������� Ȱ��ȭ ���¸� ���ؿ��� �Լ�
	bool IsEnableItemFromIndex(int nIndex);							//nIndex��°�� �������� Ȱ��ȭ ���¸� ���ؿ��� �Լ�
	bool IsEnableItemFromParam(LPARAM lParam);
	bool IsEnableItemFromString(std::string strText);

	//�������� �ؽ�Ʈ�� �����Ѵ�.
	bool SetStringFromIndex(int nIndex, std::string strText);
	bool SetStringFromParam(LPARAM lParam, std::string strText);
	bool SetStringFromFind(int nType, LPARAM lUserData, std::string strText);
	bool SetStringFromString(std::string strOldText, std::string strText);

	//�������� �����ִ� �Լ�
	virtual bool DeleteItem(int nIndex);							//nIndex ��°�� �������� �����ϴ� �Լ�
	virtual bool DeleteItem(LPARAM lParam);							//lParam�� ���� UserData�� ������ ù��° �������� �����ϴ� �Լ�
	virtual bool DeleteItem(int nType, LPARAM lUserData);			//Find�Լ��� ����Ͽ� �������� �˻��� �� �˻��� �������� �����ϴ� �Լ�
	virtual bool DeleteItem(std::string strText);						//�������� �ؽ�Ʈ�� strText�� ù��° �������� ã�Ƽ� �����ִ� �Լ�
	virtual void DeleteAllItem(void);

	//������ �������� ���̵���(��ũ��) �ϴ� �Լ���
	//������ �������� ����Ʈ�� ù ���ο� ���̴°� ��Ģ���� �Ѵ�.
	//����, ������ �����ۿ��� ������ �����۱��� ���̰� Page���� ������ ���� ���������� Scroll�� �Ѵ�.
	void SetVisibleItemFromIndex(int nIndex);
	void SetVisibleItemFromParam(LPARAM lParam);
	void SetVisibleItemFromFind(int nType, LPARAM lUserData);
	void SetVisibleItemFromString(std::string strText);

	virtual void SetSingleSelect(bool bSingleSelect);				//�Ѱ��� �����۸� ���� ���θ� �����Ѵ�.
	void ClearAllSelect(void);										//��� �������� ������ ����Ѵ�
	bool IsSingleSelect(void){return m_bSingleSelect;}				//�Ѱ��� �����۸� ���� ���� ���ؿ´�.

	const bool &IsVirtical(void){return m_bVirtical;}
	void SetVirtical(const bool &bVirtical);

	void SetClipSize(int nWidth, int nHeight);
	int GetClipSizeWidth(void){return m_nClipWidth;}
	int GetClipSizeHehght(void){return m_nClipHeight;}

	//���� ���õ� �������� ���ؿ��� �Լ���
	//GetFirstSelectItem�� ���� ȣ���Ͽ� ù��° ���õ� �������� ���ؿ���, �� ���� ���ʹ� GetNextSelectItem �Լ��� 
	//���ʷ� ���õ� �������� ���ؿ��� �ȴ�.
	CListItemData *GetFirstSelectItem(void);
	CListItemData *GetNextSelectItem(void);

	//�������� ó������ �˻��ϴ� �Լ�
	//GetFirstItem�� ���� ȣ���Ͽ� ù��° �������� ���ؿ���, �� ���� ���ʹ� GetNextItem �Լ��� 
	//���ʷ� �������� ���ؿ��� �ȴ�.
	CListItemData *GetFirstItem(void);
	CListItemData *GetNextItem(void);

	CListItemData *GetItemFromIndex(int nIndex);			//nIndex��° �������� �������� �Լ�
	CListItemData *GetItemFromParam(LPARAM lParam);			//lParam�� ������ User Data�� ������ �ִ� �������� ���ؿ��� �Լ�
	CListItemData *GetItemFromString(std::string strText);
	CListItemData *GetFirstFindItem(int nType, LPARAM lUserData);	//������ �����ϴ� �������� ���ؿ��� �Լ�, SetFindFunction���� ����� �Լ��� �˻��Ѵ�.
	CListItemData *GetNextFindItem(int nType, LPARAM lUserData);	//������ �����ϴ� �������� ���ؿ��� �Լ�, SetFindFunction���� ����� �Լ��� �˻��Ѵ�.

	//������ ������ Gap�� �����Ѵ�
	void SetItemGab(float fGapX, float fGapY);
	void SetItemGab(CPoint ptGap);
	float GetItemGapX(void){return m_ptItemGab.x;}
	float GetItemGapY(void){return m_ptItemGab.y;}

	//�������� �����ؼ� ������ �⺻ �������� ����Ѵ�.
	void AttachItem(CListItem *pItem);
	//�������� �����ؼ� ������ �⺻ �������� ���ؿ��� �Լ�
	CListItem *GetItem(void){return m_pSourceItem;}
	void DettachItem(void);
	//��ũ�ѹٸ� ����ϴ� �Լ�
	void AttachScroll(CScrollBar *pScroll);
	//��ϵ� ��ũ�ѹ߸� ���ؿ��� �Լ�
	CScrollBar *GetScrollBar(void){return m_pScroll;}
	void DettachScroll(void);
	//����Ʈ�� ������ �����ϴ� �Լ�
	void SetSplit(int nXNum, int nYNum);
	const int &GetXSplit(void){return m_nXNum;}
	const int &GetYSplit(void){return m_nYNum;}

	virtual int AddItem(LPARAM lParam);
	virtual int AddString(std::string strText);

	void SetLineScroll(const bool &bLineScroll){m_bLineScroll = bLineScroll;}
	const bool &IsLineScroll(void){return m_bLineScroll;}

	int GetFirstVisibleItem(void);	//ȭ�鿡 ���� ó���� ���̴� ������
	int GetLastVisibleItem(void);	//ȭ�鿡 ���� �������� ���̴� ������
	CListItemData *HitTest(const CPoint &ptPos);

	LRESULT OnListItemNotify(CMessage *pMsg);

	void MakeSample(void);
protected:
	void ReIndexItem(void);
	void SetReposition(const bool &bReposition){m_bReposition = bReposition;}
	const bool &IsReposition(void){return m_bReposition;}

	int	GetItemIndex(CListItem *pItem);						//�������� �ε����� ���ؿ��� �Լ�
	CListItem *GetNewItem(void);

	//�������� ���¸� �����ϴ� �Լ���
	void UpdateItem(CListItem *pItem);						//pItem�� ���¸� �����ϵ��� �ϴ� �Լ�
	void UpdateItem(CListItemData *pData);					//pItem�� ���¸� �����ϵ��� �ϴ� �Լ�

	//�������� ���� ���¸� �����Ͽ��ִ� �Լ�
	void SetItemSelect(CListItem *pItem);
	bool SetItemSelect(CListItemData *pData, bool bSelect);

	//�������� Ȱ��ȭ ���¸� �����ϴ� �Լ�
	void SetEnableItem(CListItem *pItem);					//pItem�� Ȱ��ȭ ���¸� �����ϴ� �Լ�
	bool SetEnableItem(CListItemData *pData, bool bEnable);	//pItem�� Ȱ��ȭ ���¸� �����ϴ� �Լ�

	//�������� Ȱ��ȭ ���¸� ���ؿ��� �Լ�
	bool IsEnableItem(CListItem *pItem);					//pItem�� Ȱ��ȭ ���¸� ���ؿ��� �Լ�
	bool IsEnableItem(CListItemData *pData);				//pItem�� Ȱ��ȭ ���¸� ���ؿ��� �Լ�

	//�������� �ؽ�Ʈ�� �����Ѵ�.
	void SetString(CListItem *pItem);
	bool SetString(CListItemData *pData, std::string strText);

	//�������� �����ִ� �Լ�
	bool DeleteItem(CListItemData *pData);			//pItem�� ������ ����Ʈ���� �����ϴ� �Լ�

	//������ �������� ���̵���(��ũ��) �ϴ� �Լ���
	//������ �������� ����Ʈ�� ù ���ο� ���̴°� ��Ģ���� �Ѵ�.
	//����, ������ �����ۿ��� ������ �����۱��� ���̰� Page���� ������ ���� ���������� Scroll�� �Ѵ�.
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

//Clone�� ListItem�� Child Object���� �߻��� �޽����� Original Object�� Functor�� ���ؼ� ���� �޾�����
//�ش� �޽����� ������ ������ ������ ������ �Լ���
UPG_LSYLIB_EXPORT extern CListItem *GetListItem(CMessage *pMsg);
UPG_LSYLIB_EXPORT extern CListItem *GetListItem(CObjectBase *pObject);
UPG_LSYLIB_EXPORT extern CListItemData *GetListItemData(CMessage *pMsg);
UPG_LSYLIB_EXPORT extern CListItemData *GetListItemData(CObjectBase *pObject);
UPG_LSYLIB_EXPORT extern int GetListItemIndex(CMessage *pMsg);
} //namespace LSY