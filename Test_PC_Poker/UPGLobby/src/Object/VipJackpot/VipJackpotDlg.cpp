#include "StdAfx.h"
#include "VipJackpotDlg.h"



CVipJackpotListItem::CVipJackpotListItem()
:	
m_pTxtNum(NULL),
m_pTxtMoney(NULL),
m_pTxtId(NULL),
m_pTxtGameName(NULL),
m_pTxtTime(NULL)
{
	// 	m_listUserCount.resize(DEF_MAX_USERS);
	// 	for(int i=0;i<DEF_MAX_USERS;++i)
	// 	{
	// 		m_listUserCount[i] = NULL;
	// 	}
}

CVipJackpotListItem::~CVipJackpotListItem()
{
}

void CVipJackpotListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_TXT_VIP_MONTH_NUM:
	case ID_LOBBY_TXT_VIP_TODAY_NUM: m_pTxtNum = (LSY::CText *)pObject;		break;		
	case ID_LOBBY_TXT_VIP_MONTH_MONEY:
	case ID_LOBBY_TXT_VIP_TODAY_MONEY: m_pTxtMoney = (LSY::CText *)pObject;		break;		
	case ID_LOBBY_TXT_VIP_MONTH_ID:
	case ID_LOBBY_TXT_VIP_TODAY_ID: m_pTxtId = (LSY::CText *)pObject;		break;		
	case ID_LOBBY_TXT_VIP_MONTH_GAMENAME:
	case ID_LOBBY_TXT_VIP_TODAY_GAMENAME: m_pTxtGameName = (LSY::CText *)pObject;		break;		
	case ID_LOBBY_TXT_VIP_MONTH_TIME:
	case ID_LOBBY_TXT_VIP_TODAY_TIME: m_pTxtTime = (LSY::CText *)pObject;		break;

		// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER0:
		// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER1:
		// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER2:
		// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER3:
		// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER4:
		// 		m_listUserCount[pObject->GetID() - ID_LOBBY_IMG_ROOMLISTITEM_USER0] = (LSY::CImage *)pObject;
		// 		break;
	}

	__super::OnCreateChild(pObject);
}

void CVipJackpotListItem::OnCreated(void)
{
	__super::OnCreated();
}

LSY::CObjectBase *CVipJackpotListItem::NewObject(void)
{
	return new CVipJackpotListItem;
}


void CVipJackpotListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL){
		return;
	}
	tVipJackpotUser* ptVipJackpotUser= (tVipJackpotUser*)pListItemData->GetItemData();
	if(NULL!=ptVipJackpotUser){
		CString strNum;
		strNum.Format("%d", pListItemData->GetItemIndex()+1);
		CString strMoney = g_NumberToOrientalString(ptVipJackpotUser->llMoney, 4);
		CString strId;
		//string sID=ChangeID("DLFEKSFPDLTM");
		string sID=ChangeID((char*)ptVipJackpotUser->sId.c_str());
		strId.Format("%s", sID.c_str());
		CString strGameName = GM().GetIDXToGameName((IDX_GAME)ptVipJackpotUser->nGameType);
		CString strTime;
		if(0!=ptVipJackpotUser->sTime.wYear){
			strTime.Format("%d.%d.%d", ptVipJackpotUser->sTime.wYear, ptVipJackpotUser->sTime.wMonth, ptVipJackpotUser->sTime.wDay);
		}
		else{
			strTime.Format("%d", ptVipJackpotUser->nTime);
		}
		if(m_pTxtNum) m_pTxtNum->SetText(strNum.GetString());
		if(m_pTxtMoney) m_pTxtMoney->SetText(strMoney.GetString());
		if(m_pTxtId) m_pTxtId->SetText(strId.GetString());
		if(m_pTxtGameName) m_pTxtGameName->SetText(strGameName.GetString());
		if(m_pTxtTime) m_pTxtTime->SetText(strTime.GetString());
	}
}

string CVipJackpotListItem::ChangeID(char* pID)
{
	if(NULL==pID) return "";
	int nSize=strlen(pID);
	if(3>=nSize) return "***";
	char* pTempID=new char[nSize+1];
	memcpy(pTempID, pID, (sizeof(char)*nSize));
	pTempID[nSize]=NULL;
	for(int i=nSize-1; i>=0; i--){
		if(' '==pTempID[i]){
			pTempID[i]=NULL;
		}
	}
	nSize=strlen(pTempID);
	if(3>=nSize) return "***";
	int nFind=0;
	for(int i=0; i<3; i++){
		if(IsDBCSLeadByte(pTempID[nSize-1-nFind])){
			nFind+=2;
		}
		else{
			nFind+=1;
		}
	}
	if(nFind>=nSize) return "***";
	string sChange;
	sChange.insert(0, pTempID, nSize-nFind);
	sChange+="***";
	return sChange;
}


void CVipJackpotListItem::OnMouseOver(void)
{
	__super::OnMouseOver();
	UpdateData(0);
}

void CVipJackpotListItem::OnMouseLeave(void)
{
	__super::OnMouseLeave();
	UpdateData(0);
}


CVipJackpotDlg::CVipJackpotDlg(void)
{
	m_pBtnClose = NULL;
	m_pTabControl = NULL;
	m_pGrpInfo = NULL;
	m_pGrpRanking = NULL;
}

CVipJackpotDlg::~CVipJackpotDlg(void)
{
	for(int i=0; i<(int)m_cvtTodayUser.size(); i++){
		tVipJackpotUser** ppTemp=(&m_cvtTodayUser[i]);
		if(*ppTemp){
			delete (*ppTemp);
			(*ppTemp)=NULL;
		}
	}
	for(int i=0; i<(int)m_cvtMonthUser.size(); i++){
		tVipJackpotUser** ppTemp=(&m_cvtMonthUser[i]);
		if(*ppTemp){
			delete (*ppTemp);
			(*ppTemp)=NULL;
		}
	}
	m_cvtTodayUser.clear();
	m_cvtMonthUser.clear();
}



void CVipJackpotDlg::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_VIP_CLOSE: m_pBtnClose = (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_TAB_VIP_INFORANKING: m_pTabControl = (LSY::CTabControl *)pObject; break;
	case ID_LOBBY_GRP_VIP_INFO: m_pGrpInfo	= (LSY::CGroup *)pObject; break;
	case ID_LOBBY_GRP_VIP_RANKING: m_pGrpRanking	= (LSY::CGroup *)pObject; break;
	}
	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CVipJackpotDlg::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

bool VipJacpotListCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	tVipJackpotUser* ptVipJackpotUser1 = (tVipJackpotUser*)pItem1->GetItemData();
	tVipJackpotUser* ptVipJackpotUser2 = (tVipJackpotUser*)pItem2->GetItemData();

	//return (ptVipJackpotUser1->llMoney > ptVipJackpotUser2->llMoney);
	if(ptVipJackpotUser1->llMoney > ptVipJackpotUser2->llMoney){
		return true;
	}
	if(ptVipJackpotUser1->llMoney < ptVipJackpotUser2->llMoney){
		return false;
	}
	if(ptVipJackpotUser1->nGameType > ptVipJackpotUser2->nGameType){
		return true;
	}
	if(ptVipJackpotUser1->nGameType < ptVipJackpotUser2->nGameType){
		return false;
	}
	if(0!=ptVipJackpotUser1->sTime.wYear){
		COleDateTime TimeUser1(ptVipJackpotUser1->sTime);
		COleDateTime TimeUser2(ptVipJackpotUser2->sTime);
		return TimeUser1.m_dt > TimeUser2.m_dt;
	}
	return ptVipJackpotUser1->nTime < ptVipJackpotUser2->nTime;
	//로우, 포커 
	//빠른순 회차 or 날짜 느린순
    
}


void CVipJackpotDlg::OnCreated(void)
{
	if(m_pTabControl){
		m_pTabControl->SetCurSelFromID(ID_LOBBY_RAD_VIP_INFO);
		m_pTabControl->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CVipJackpotDlg::OnTabSelChange));
	}
	if(m_pGrpInfo){
		m_pGrpInfo->SetShow(true);
		m_pGrpInfo->SetEnable(true);
		SetInfoState(g_cVipJackpot.m_nStage, g_cVipJackpot.m_nState);
	}
	if(m_pGrpRanking){
		m_pGrpRanking->SetShow(false);
		m_pGrpRanking->SetEnable(false);
		LSY::CList* pList_Today=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_TODAY);
		LSY::CList* pList_Month=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_MONTH);
		if(pList_Today){
			pList_Today->SetCompareFunction(0, (LSY::CList::LISTCOMPAREFUNCTION)&VipJacpotListCompareFunction);
			pList_Today->SetSortType(0);
			pList_Today->SetAutoSort(true);
		}
		if(pList_Month){
			pList_Month->SetCompareFunction(0, (LSY::CList::LISTCOMPAREFUNCTION)&VipJacpotListCompareFunction);
			pList_Month->SetSortType(0);
			pList_Month->SetAutoSort(true);
		}
#if defined(_DEBUG)
		tVipJackpotUser tVJ1;
		tVJ1.llMoney=1ll;
		AddTodayItem(&tVJ1);
		tVipJackpotUser tVJ2;
		tVJ2.llMoney=20ll;
		AddTodayItem(&tVJ2);
		tVJ2.llMoney=10ll;
		AddTodayItem(&tVJ2);
		tVJ2.llMoney=3ll;
		AddTodayItem(&tVJ2);
		tVJ2.llMoney=4ll;
		AddTodayItem(&tVJ2);
		tVJ2.llMoney=50ll;
		AddTodayItem(&tVJ2);
#endif
	}
	__super::OnCreated();
}

void CVipJackpotDlg::SetInfoState(void)
{
	SetInfoState(g_cVipJackpot.m_nStage, g_cVipJackpot.m_nState);
}

void CVipJackpotDlg::SetInfoState(int nStage, int nState)
{
	ErrorLogConsole("SetInfoState  = %d = %d", nStage, nState);
	if(m_pGrpInfo){
		LSY::CImage* pImg1st = (LSY::CImage*)m_pGrpInfo->GetFirstChildFromID(ID_LOBBY_IMG_VIP_1ST);
		LSY::CImage* pImg2nd = (LSY::CImage*)m_pGrpInfo->GetFirstChildFromID(ID_LOBBY_IMG_VIP_2ND);
		LSY::CImage* pImg3rd = (LSY::CImage*)m_pGrpInfo->GetFirstChildFromID(ID_LOBBY_IMG_VIP_3RD);
		LSY::CImage* pImg4th = (LSY::CImage*)m_pGrpInfo->GetFirstChildFromID(ID_LOBBY_IMG_VIP_4TH);
		if(NULL==pImg1st) return;
		if(NULL==pImg2nd) return;
		if(NULL==pImg3rd) return;
		if(NULL==pImg4th) return;
		int nIndex1=0, nIndex2=0, nIndex3=0, nIndex4=0;
		switch(nStage){
		case 0: nIndex1=11; nIndex2=14; nIndex3=17; nIndex4=20; break; 
		case 1: nIndex1=11+nState; nIndex2=14; nIndex3=17; nIndex4=20; break;
		case 2: nIndex1=11+2; nIndex2=14+nState; nIndex3=17; nIndex4=20; break;
		case 3: nIndex1=11+2; nIndex2=14+2; nIndex3=17+nState; nIndex4=20; break;
		case 4: nIndex1=11+2; nIndex2=14+2; nIndex3=17+2; nIndex4=20+nState; break;
		default: nIndex1=11+2; nIndex2=14+2; nIndex3=17+2; nIndex4=20+2; break;
		}
		pImg1st->SetIndex(nIndex1);
		pImg2nd->SetIndex(nIndex2);
		pImg3rd->SetIndex(nIndex3);
		pImg4th->SetIndex(nIndex4);
	}
}

void CVipJackpotDlg::AddTodayItem(tVipJackpotUser* ptVipJackpotUser)
{
	if(NULL==m_pGrpRanking) return;
	int nArray=m_cvtTodayUser.size();
	tVipJackpotUser* ptVJ = new tVipJackpotUser();
	(*ptVJ) = (*ptVipJackpotUser);
	m_cvtTodayUser.push_back(ptVJ);
	LSY::CList* pList_Today=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_TODAY);
	if(pList_Today) pList_Today->AddItem((LPARAM)m_cvtTodayUser[nArray]);

	LSY::CText* pTxt_TodayEmpty= (LSY::CText*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_TEXT_VIP_TODAY_EMPTY);
	if(pTxt_TodayEmpty) pTxt_TodayEmpty->SetShow(false);
}

void CVipJackpotDlg::DelTodayItem(void)
{
	if(NULL==m_pGrpRanking) return;
	LSY::CList* pList_Today=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_TODAY);
	if(pList_Today) pList_Today->DeleteAllItem();
	LSY::CText* pTxt_TodayEmpty= (LSY::CText*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_TEXT_VIP_TODAY_EMPTY);
	if(pTxt_TodayEmpty) pTxt_TodayEmpty->SetShow(true);
	int nSize=(int)m_cvtTodayUser.size();
	for(int i=0; i<nSize; i++){
		tVipJackpotUser** ppTemp=(&m_cvtTodayUser[i]);
		if(*ppTemp){
			delete (*ppTemp);
			(*ppTemp)=NULL;
		}
	}
	m_cvtTodayUser.clear();
}

void CVipJackpotDlg::AddMonthItem(tVipJackpotUser* ptVipJackpotUser)
{
	if(NULL==m_pGrpRanking) return;
	int nArray=m_cvtMonthUser.size();
	tVipJackpotUser* ptVJ = new tVipJackpotUser();
	(*ptVJ) = (*ptVipJackpotUser);
	m_cvtMonthUser.push_back(ptVJ);
	LSY::CList* pList_Month=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_MONTH);
	if(pList_Month) pList_Month->AddItem((LPARAM)m_cvtMonthUser[nArray]);

	LSY::CText* pTxt_MonthEmpty= (LSY::CText*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_TEXT_VIP_MONTH_EMPTY);
	if(pTxt_MonthEmpty) pTxt_MonthEmpty->SetShow(false);
}

void CVipJackpotDlg::DelMonthItem(void)
{
	if(NULL==m_pGrpRanking) return;
	LSY::CList* pList_Month=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_MONTH);
	if(pList_Month) pList_Month->DeleteAllItem();

	LSY::CText* pTxt_MonthEmpty= (LSY::CText*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_TEXT_VIP_MONTH_EMPTY);
	if(pTxt_MonthEmpty) pTxt_MonthEmpty->SetShow(true);
	int nSize=(int)m_cvtMonthUser.size();
	for(int i=0; i<nSize; i++){
		tVipJackpotUser** ppTemp=(&m_cvtMonthUser[i]);
		if(*ppTemp){
			delete (*ppTemp);
			(*ppTemp)=NULL;
		}
	}
	m_cvtMonthUser.clear();
}

LRESULT CVipJackpotDlg::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_VIP_CLOSE:
		SetShow(false);
		break;
	}
	return TRUE;
}


LRESULT CVipJackpotDlg::OnTabSelChange(LSY::CMessage *msg)
{
	if(NULL==m_pTabControl) return TRUE;
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	LSY::CRadio *pRadio = m_pTabControl->GetRadioFromIndex(nIndex);

	if(pRadio->GetID() == ID_LOBBY_RAD_VIP_INFO){
		if(m_pGrpInfo){
			m_pGrpInfo->SetShow(true);
			m_pGrpInfo->SetEnable(true);
		}
		if(m_pGrpRanking){
			m_pGrpRanking->SetShow(false);
			m_pGrpRanking->SetEnable(false);
		}
	}
	else{
		if(m_pGrpInfo){
			m_pGrpInfo->SetShow(false);
			m_pGrpInfo->SetEnable(false);
		}
		if(m_pGrpRanking){
			m_pGrpRanking->SetShow(true);
			m_pGrpRanking->SetEnable(true);
			LSY::CList* pList_Month=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_MONTH);
			LSY::CList* pList_Today=(LSY::CList*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_LIST_VIP_TODAY);
			if(pList_Month){
				bool bShowMonth=((0==pList_Month->GetItemCount())?(true):(false));
				LSY::CText* pTxt_MonthEmpty= (LSY::CText*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_TEXT_VIP_MONTH_EMPTY);
				if(pTxt_MonthEmpty) pTxt_MonthEmpty->SetShow(bShowMonth);
			}
			if(pList_Today){
				bool bShowToday=((0==pList_Today->GetItemCount())?(true):(false));
				LSY::CText* pTxt_TodayEmpty= (LSY::CText*)m_pGrpRanking->GetFirstChildFromID(ID_LOBBY_TEXT_VIP_TODAY_EMPTY);
				if(pTxt_TodayEmpty) pTxt_TodayEmpty->SetShow(bShowToday);
			}
		}
	}

	NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_C" );

	return TRUE;
}


void CVipJackpotDlg::SetShowDlg()
{
	SetInfoState(g_cVipJackpot.m_nStage, g_cVipJackpot.m_nState);

	
	SetShow(true);
	//SetTracking(true);

}


bool CVipJackpotDlg::OnClickOutside(CObjectBase *pFocus)
{
	__super::OnClickOutside(pFocus);

	
	if(pFocus)
	{

		if (IsShow())
		{
			SetShow(false);
		}

// 		LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
// 		LSY::CList *pList1 = (LSY::CList *)pProject->GetObject(ID_LOBBY_LIST_ROOM);
// 		LSY::CList *pList2 = (LSY::CList *)pProject->GetObject(ID_LOBBY_LIST_ROOM_LOWBD);
// 
// 		if(pFocus->IsParent(pList1) || pFocus->IsParent(pList2))
// 		{
// 			return false;
// 		}
	}

	
	return true;
}

void CVipJackpotDlg::SetShow(const bool &bShow)
{
	__super::SetShow(bShow);

	//if (bShow)
	//{
	//	SetTracking(true);
	//}
	//else
	//{
	//	RemoveTracking();
	//}
}

