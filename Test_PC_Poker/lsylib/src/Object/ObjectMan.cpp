#include "stdafx.h"
#include "ObjectMan.h"
#include "./Base/Image.h"
#include "../ThreadLock/ThreadLock.h"
#include <algorithm>
#include "../Utility/ScanCode.h"
#include "../Functor/Message.h"
#include "./Base/Window.h"
#include "../../include/LSYLib.h"
#include "./Base/DumyImage.h"
#include "./Sprite/SpriteScene.h"
#include "../tinyxml/tinyxml.h"
#include "../Utility/MyMime64.h"
#include <MMSystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
map<HWND, CObjectMan *> CObjectMan::m_sMapProc;
CObjectMan	*CObjectMan::m_spMainObjectMan = NULL;

LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	map<HWND, CObjectMan *>::iterator findIt = CObjectMan::m_sMapProc.find(hWnd);
	if(findIt != CObjectMan::m_sMapProc.end())
	{
		if(findIt->second->WindowsProc(nMessage, wParam, lParam)) return TRUE;

		return CallWindowProc((WNDPROC)findIt->second->GetOldWindowProc(), hWnd, nMessage, wParam, lParam);
	}
	return FALSE;
}

CObjectProject::CObjectProject()
:	m_nID(_ID_PROJECT_NOTUSE),
	m_bActive(false),
	m_strName(""),
	m_pMainObject(NULL),
	m_pPopupObject(NULL),
	m_pPopupParent(NULL),
	m_szSize(800,600),
	m_pParent(NULL),
	m_bLogicProcess(false),
	m_bFirst(true)
{
	InitializeCriticalSection(&m_cLockLogic);
	InitializeCriticalSection(&m_cLockMouse);
	InitializeCriticalSection(&m_cLockObject);

	m_pMainObject = new CWindows;
	m_pMainObject->SetID(_ID_OBJECT_ROOT);
	m_pMainObject->SetObjectProject(this);

	m_itNextLogic = m_listLogicObject.end();
}

CObjectProject::~CObjectProject()
{
	if(m_pMainObject)
	{
		delete m_pMainObject;
		m_pMainObject = NULL;
	}

	ASSERT((int)m_listLogicObject.size() == 0);
	ASSERT((int)m_listMouseMessageObject.size() == 0);

	ASSERT(m_listRemove.size() == 0);

	DeleteCriticalSection(&m_cLockLogic);
	DeleteCriticalSection(&m_cLockMouse);
	DeleteCriticalSection(&m_cLockObject);

	m_cCreateObjectFucntor.Release();
	m_cCreatedObjectFucntor.Release();
}

bool CObjectProject::SaveXml(std::string strFileName)
{
	{
		const char* demoStart =
			"<?xml version=\"1.0\"  standalone='no' >\n"
			"<LSYToolScene version=\"2\"></LSYToolScene>\n"
			"<Scene>\n"
			"</Scene>";

		TiXmlDocument doc(strFileName.c_str()); 
		doc.Parse(demoStart);
		doc.SaveFile();
	}

	TiXmlNode* node = 0;

	TiXmlDocument doc(strFileName.c_str());
	if(doc.LoadFile() == false) return false;

	node = doc.FirstChild( "Scene" );
	if(node == NULL) return false;

	CWindows *pWindows = (CWindows *)m_pMainObject;
	m_strBackFilePath = pWindows->GetFileName();
	m_nBackIndex = pWindows->GetIndex();

	TiXmlElement *pXmlScene = (TiXmlElement *)node;
	pXmlScene->SetAttribute("path", m_strBackFilePath.c_str());
	pXmlScene->SetAttribute("index", m_nBackIndex);

	TiXmlElement xmlObjectList("ObjectList");

	LISTOBJECT::const_iterator start = m_pMainObject->GetChildList().begin();
	LISTOBJECT::const_iterator end = m_pMainObject->GetChildList().end();

	for(;start != end;++start)
	{
		(*start)->SaveXml(&xmlObjectList);
	}

	node->InsertEndChild(xmlObjectList);

	doc.SaveFile();
	doc.Clear();

	m_strFileName = strFileName;
	return true;
}

bool CObjectProject::LoadXml(std::string strFileName)
{
	if(strFileName == "")
	{
		strFileName = m_strFileName;
		if(strFileName == "") return false;
	}

	TiXmlNode* node = 0;

	TiXmlDocument doc(strFileName.c_str());
	if(doc.LoadFile() == false) return false;

	node = doc.FirstChild( "LSYToolScene" );
	if(node == NULL) return false;

	int nVersion = 0;
	((TiXmlElement *)node)->Attribute("version", &nVersion);
	ASSERT(nVersion > 0);

	node = doc.FirstChild( "Scene" );
	if(node == NULL) return false;

	TiXmlElement *pXmlScene = (TiXmlElement *)node;
	CWindows *pWindows = (CWindows *)m_pMainObject;
	GetAttribute(pXmlScene, "path", m_strBackFilePath);
	GetAttribute(pXmlScene, "index", m_nBackIndex);

	if(m_strBackFilePath != "")
	{
		pWindows->Create(m_strBackFilePath.c_str());
		pWindows->SetIndex(m_nBackIndex);
	}

	m_listLoadOverlapObject.clear();

	TiXmlElement *pObjectList = ((TiXmlElement *)node)->FirstChildElement("ObjectList");

	TiXmlElement *pXmlObject = pObjectList->FirstChildElement("Object");
	CObjectBase *pObject = NULL;
	m_pMainObject->OnCreateChildStart();
	while(pXmlObject)
	{
		pObject = LoadObject(nVersion, pXmlObject);
		m_pMainObject->AddChild(pObject);
		m_pMainObject->OnCreateChild(pObject);
		pObject->OnCreated();
		pXmlObject = pXmlObject->NextSiblingElement("Object");
	}
	m_pMainObject->OnCreateChildEnd();
	m_pMainObject->OnCreated();

	doc.Clear();

	m_strFileName = strFileName;
	return true;
}

int CObjectProject::GetLoadOverlapObjectCount(int &nID)
{
	if(m_mapObject.size() == 0)
	{
		nID = 0;
	}
	else
	{
		nID = m_mapObject.rbegin()->first + 1;
	}
	return (int)m_listLoadOverlapObject.size();
}

void CObjectProject::SetLoadOverlapObjectCompulsionID(int nID)
{
	LISTOBJECTITER start = m_listLoadOverlapObject.begin();
	LISTOBJECTITER end = m_listLoadOverlapObject.end();
	CObjectBase *pObject = NULL;

	for(;start != end;++start, ++nID)
	{
		pObject = *start;
		pObject->SetID(nID);
		AddObject(pObject);
	}
}

bool CObjectProject::LoadSceneXml(TiXmlElement *pElementScene, CObjectMan *pParent)
{
	std::string strName;
	int nID = 0;

	GetAttribute(pElementScene, "Name", strName);
	GetAttribute(pElementScene, "ID", nID);

	GetAttribute(pElementScene, "Path", m_strFileName);
	m_pParent = pParent;

	int nWidth = 0;
	int nHeight = 0;
	GetAttribute(pElementScene, "CX", nWidth);
	GetAttribute(pElementScene, "CY", nHeight);

	m_szSize = CSize(nWidth, nHeight);
	SetName(strName, true);
	SetID(nID);

	return true;
}

void CObjectProject::AddRemove(CObjectBase *pRemove)
{
	m_listRemove.push_back(pRemove);
}

bool CObjectProject::SetCallCreateObject(IFunctor* pFunctor)
{
	return m_cCreateObjectFucntor.Default(pFunctor);
}

bool CObjectProject::SetCallCreatedObject(IFunctor* pFunctor)
{
	return m_cCreatedObjectFucntor.Default(pFunctor);
}

CObjectBase *CObjectProject::LoadObject(const int &nVersion, TiXmlElement *pXmlObject)
{
	std::string strType = pXmlObject->Attribute("Type");
	std::string strName = pXmlObject->Attribute("Name");
	int nID = _ID_OBJECT_NOTUSE;
	pXmlObject->Attribute("ID", &nID);

	CMO_CreateObject msg(&nID, &strName);
	CObjectBase *pObject = (CObjectBase *)m_cCreateObjectFucntor.Fire(&msg);

	if(pObject == NULL || pObject == (CObjectBase *)-1)
	{
		pObject = CreateObject(strType, strName, nID);
	}
	else
	{
		ASSERT(pObject->GetType() == GetStringToObjectType(strType));

		pObject->SetName(strName);
		pObject->SetID(nID);
	}

	if(IsExistID(nID)) m_listLoadOverlapObject.push_back(pObject);
	else AddObject(pObject);

	pObject->LoadXml(nVersion, pXmlObject, this);

	CMO_CreatedObject msg2(pObject);
	m_cCreatedObjectFucntor.Fire(&msg2);

	return pObject;
}

CObjectBase *CObjectProject::CreateObject(const std::string &strType, const std::string &strName, const int &nID)
{
	CObjectBase *pObject = NULL;
	ENUM_OBJECTTYPE nType = GetStringToObjectType(strType);
	pObject = CreateNewObject(nType);
	pObject->SetName(strName);
	pObject->SetID(nID);
	return pObject;
}

void CObjectProject::SetSize(CSize szSize)
{
	if(m_szSize == szSize) return;
	m_szSize = szSize;
}


void CObjectProject::SetPopup(CObjectBase *pPopup, CObjectBase *pFocusObject)
{
	ASSERT(pPopup == NULL || pPopup->IsLocalChild());

	if(m_pPopupObject)
	{
		if(m_pPopupParent->OnClosePopup(pFocusObject))
		{
			m_pPopupObject = NULL;
			m_pPopupParent = NULL;
		}
		else return;
	}

	m_pPopupObject = pPopup;
	if(m_pPopupObject)
	{
		CObjectBase *pWindows = m_pPopupObject->GetWindowsParent();
		if(pWindows == NULL)
		{
			m_pPopupObject = NULL;
			return;
		}

		if(m_pParent) m_pParent->CloseAllPopup(this);
		m_pPopupParent = m_pPopupObject->GetParent();

		switch(pWindows->GetType())
		{
		case EOT_WINDOWS:
			((CWindows *)pWindows)->SetPopup(m_pPopupObject);
			break;
		case EOT_SPRITEWINDOWS:
			((CSpriteWindows *)pWindows)->SetPopup(m_pPopupObject);
			break;
		}

		if(m_pParent) m_pParent->SetFocus(m_pPopupObject);
	}
}

void CObjectProject::Create(int nWidth, int nHeight, CObjectMan *pParent)
{
	m_szSize	= CSize(nWidth, nHeight);
	m_pParent	= pParent;
}

bool CObjectProject::SetID(const int &nID)
{
	ASSERT(m_pParent != NULL && nID >= _ID_PROJECT_ROOT);
	if(m_nID == nID) return true;
	//루트 프로젝트의 아이디는 변경할 수 없음
	if(m_nID == _ID_PROJECT_ROOT) return true;
	int nOldID = m_nID;
	m_nID = nID;
	if(nOldID == _ID_PROJECT_NOTUSE)
	{
		if(m_pParent->AddProject(this) == false)
		{
			m_nID = nOldID;
			return false;
		}
	}
	else if(m_nID == _ID_PROJECT_NOTUSE)
	{
		m_pParent->RemoveProject(nOldID);
	}
	else
	{
		if(m_pParent->ChangeProjectID(nOldID, m_nID) == false)
		{
			m_nID = nOldID;
			return false;
		}
	}
	return true;
}

int CObjectProject::MakeID(void)
{
	if(m_mapObject.size() == 0) return 0;
	return m_mapObject.rbegin()->first + 1;
}

std::string CObjectProject::MakeName(std::string strName)
{
	if(m_mapNameToID.find(strName) == m_mapNameToID.end()) return strName;
	CString strFormat;
	int i=0;
	while(1)
	{
		strFormat.Format("%s%03d",strName.c_str(),i);
		if(m_mapNameToID.find(strFormat.GetString()) == m_mapNameToID.end()) break;
		++i;
	}
	return strFormat.GetString();
}

bool CObjectProject::AddObject(CObjectBase *pObject)
{
	CThreadLock lock(&m_cLockObject);
	if(pObject == NULL) return false;
	const int &nID = pObject->GetID();
	const std::string &strName = pObject->GetName();
	if(nID < 0 || strName == "") return false;
	ASSERT(m_mapObject.end() == m_mapObject.find(nID));
	ASSERT(m_mapNameToID.end() == m_mapNameToID.find(strName));
	m_mapObject[nID] = pObject;
	m_mapNameToID[strName] = nID;
	return true;
}

bool CObjectProject::SubObject(CObjectBase *pObject)
{
	CThreadLock lock(&m_cLockObject);
	if(pObject == NULL) return false;
	const int &nID = pObject->GetID();
	const std::string &strName = pObject->GetName();
	MAPIDTOOBJECTITER findIt = m_mapObject.find(nID);
	if(findIt == m_mapObject.end()) return false;
	m_mapObject.erase(nID);
	m_mapNameToID.erase(strName);
	return true;
}

bool CObjectProject::ChangeObjectID(const int &nOldID, const int &nNewID)
{
	CThreadLock lock(&m_cLockObject);
	MAPIDTOOBJECTITER findIt = m_mapObject.find(nOldID);
	if(findIt == m_mapObject.end()) return false;
	if(m_mapObject.find(nNewID) != m_mapObject.end()) return false;
	CObjectBase *pObject = findIt->second;
	MAPNAMETOIDITER findIt2 = m_mapNameToID.find(pObject->GetName());
	if(findIt2 == m_mapNameToID.end()) return false;
	m_mapObject.erase(nOldID);
	m_mapObject[nNewID] = pObject;
	findIt2->second = nNewID;
	return true;
}

bool CObjectProject::ChangeObjectName(const std::string &strOldName, const std::string &strNewName)
{
	CThreadLock lock(&m_cLockObject);
	MAPNAMETOIDITER findIt = m_mapNameToID.find(strOldName);
	if(findIt == m_mapNameToID.end()) return false;
	if(m_mapNameToID.find(strNewName) != m_mapNameToID.end()) return false;
	int nID = findIt->second;
	m_mapNameToID.erase(findIt);
	m_mapNameToID[strNewName] = nID;
	return true;
}

bool CObjectProject::IsExistID(const int &nID)
{
	CThreadLock lock(&m_cLockObject);
	if(m_mapObject.end() == m_mapObject.find(nID)) return false;
	return true;
}

bool CObjectProject::IsExistName(const std::string &strName)
{
	CThreadLock lock(&m_cLockObject);
	if(m_mapNameToID.end() == m_mapNameToID.find(strName)) return false;
	return true;
}

bool CObjectProject::IsExistObject(CObjectBase *pObject)
{
	if(pObject == NULL) return false;
	CObjectBase *pSorce = GetObject(pObject->GetID());
	if(pSorce == pObject) return true;
	return false;
}

CObjectBase *CObjectProject::GetObject(const int &nID)
{
	CThreadLock lock(&m_cLockObject);

	MAPIDTOOBJECTITER findIt = m_mapObject.find(nID);
	if(m_mapObject.end() == findIt) return NULL;
	return findIt->second;
}

CObjectBase *CObjectProject::GetObject(const std::string &strName)
{
	CThreadLock lock(&m_cLockObject);

	MAPNAMETOIDITER findIt = m_mapNameToID.find(strName);
	if(m_mapNameToID.end() == findIt) return NULL;
	return GetObject(findIt->second);
}

CObjectBase *CObjectProject::GetFirstObject(const ENUM_OBJECTTYPE &nType)
{
	m_itFind = m_mapObject.begin();
	MAPIDTOOBJECTITER end = m_mapObject.end();
	for(;m_itFind != end;++m_itFind)
	{
		if(nType == EOT_UNKNOWN || nType == m_itFind->second->GetType())
			return m_itFind->second;
	}
	return NULL;
}

CObjectBase *CObjectProject::GetNextObject(const ENUM_OBJECTTYPE &nType)
{
	MAPIDTOOBJECTITER end = m_mapObject.end();
	if(m_itFind == end) return NULL;
	++m_itFind;
	if(m_itFind == end) return NULL;
	for(;m_itFind != end;++m_itFind)
	{
		if(nType == EOT_UNKNOWN || nType == m_itFind->second->GetType())
			return m_itFind->second;
	}
	return NULL;
}

bool CObjectProject::SetName(const string &strName, bool bForce)
{
	if(m_strName == strName) return true;
	string strOldName = m_strName;
	m_strName = strName;
	if(m_strName == "")
	{
		if(m_pParent)
		{
			m_strName = strOldName;
			return false;
		}
	}
	else
	{
		if(m_pParent && m_pParent->SetProjectName(strOldName, m_strName) == false && !bForce)
		{
			m_strName = strOldName;
			return false;
		}
	}

	return true;
}

void CObjectProject::SetActive(const bool &bActive)
{
	if(m_bActive == bActive) return;
	m_bActive = bActive;
	m_bFirst = true;
	m_pMainObject->SetChange(true);
	m_pMainObject->SetChangeAlpha(true);
	m_pMainObject->OnProjectActiveChange(bActive);
}

bool CObjectProject::AddLogicObject(IObject *pObject)
{
	CThreadLock lock(&m_cLockLogic);
	LISTLOGICITER end =  m_listLogicObject.end();
	LISTLOGICITER findIt =  m_listLogicObject.find(pObject);
	ASSERT(findIt == end);
	if(findIt != end) return false;
	m_listLogicObject[pObject] = m_pParent->m_dwTickCount;
	pObject->SetLogicStartTime(m_pParent->m_dwTickCount);
	return true;
}

bool CObjectProject::SubLogicObject(IObject *pObject)
{
	CThreadLock lock(&m_cLockLogic);
	LISTLOGICITER end =  m_listLogicObject.end();
	LISTLOGICITER findIt =  m_listLogicObject.find(pObject);

	ASSERT(findIt != end);
	if(findIt == end) return false;
	if(m_bLogicProcess && m_itNextLogic != end)
	{
		if(m_itNextLogic->first == pObject) ++m_itNextLogic;
	}
	m_listLogicObject.erase(findIt);
	return true;
}

bool CObjectProject::AddMouseMessageObject(CObjectBase *pObject)
{
	if(pObject->GetID() == _ID_OBJECT_ROOT) return false;
	CThreadLock lock(&m_cLockMouse);

	LISTOBJECT::iterator start = m_listMouseMessageObject.begin();
	LISTOBJECT::iterator end = m_listMouseMessageObject.end();

	LISTOBJECT::iterator findIt = find(start, end, pObject);
	ASSERT(findIt == end);
	if(findIt != end) return false;

	bool bResult = false;
	AddMouseMessageObjectTemp(m_pMainObject, pObject, start, end, bResult);
	return bResult;
}

bool CObjectProject::ChangeMouseMessageObject(CObjectBase *pObject)
{
	if(pObject->GetID() == _ID_OBJECT_ROOT) return false;
	CThreadLock lock(&m_cLockMouse);

	LISTOBJECT listTemp;
	GetMouseMessageObject(pObject, listTemp);

	if(listTemp.size() == 0) return false;

	LISTOBJECTITER start_temp = listTemp.begin();
	LISTOBJECTITER end_temp = listTemp.end();

	LISTOBJECT::iterator start = m_listMouseMessageObject.begin();
	LISTOBJECT::iterator end = m_listMouseMessageObject.end();

	LISTOBJECT::iterator findIt = find(start, end, *start_temp);
	ASSERT(findIt != end);
	if(findIt == end) return false;

	for(;start_temp != end_temp;++start_temp)
	{
		ASSERT(findIt != end);
		findIt = m_listMouseMessageObject.erase(findIt);
	}

	start = m_listMouseMessageObject.begin();

	bool bResult = false;
	start_temp = listTemp.begin();
	AddMouseMessageObjectTemp(m_pMainObject, *start_temp, start, end, bResult);

	if(bResult == true)
	{
		end_temp = listTemp.end();

		++start_temp;
		++start;

		if(start_temp != end_temp) m_listMouseMessageObject.insert(start, start_temp, end_temp);
	}

	return bResult;
}

void CObjectProject::GetMouseMessageObject(CObjectBase *pObject, LISTOBJECT &listTemp)
{
	if(pObject->IsMessage() && pObject->IsShow() && pObject->IsShowRef())
		listTemp.push_back(pObject);

	LISTOBJECT::const_iterator start = pObject->GetLocalChildList().begin();
	LISTOBJECT::const_iterator end = pObject->GetLocalChildList().end();

	CObjectBase *pTempObject = NULL;
	for(;start != end;++start)
	{
		pTempObject = *start;
		GetMouseMessageObject(pTempObject, listTemp);
	}

	start = pObject->GetChildList().begin();
	end = pObject->GetChildList().end();

	for(;start != end;++start)
	{
		pTempObject = *start;
		GetMouseMessageObject(pTempObject, listTemp);
	}
}

bool CObjectProject::AddMouseMessageObjectTemp(CObjectBase *pParent, CObjectBase *pAddObject, LISTOBJECTITER &it, LISTOBJECTITER &end_it, OUT bool &bResult)
{
	if(it == end_it)
	{
		it = m_listMouseMessageObject.insert(end_it, pAddObject);
		bResult = true;
		return true;
	}

	ASSERT(*it != pAddObject);
	if(*it == pAddObject)
	{
		return true;
	}

	const LISTOBJECT &listLocalChild = pParent->GetLocalChildList();
	LISTOBJECT::const_iterator start = listLocalChild.begin();
	LISTOBJECT::const_iterator end = listLocalChild.end();

	for(;start != end;++start)
	{
		if(*start == pAddObject)
		{
			it = m_listMouseMessageObject.insert(it, pAddObject);
			bResult = true;
			return true;
		}

		if(*it == *start) ++it;
		if(AddMouseMessageObjectTemp(*start, pAddObject, it, end_it, bResult)) return true;
	}

	const LISTOBJECT &listChild = pParent->GetChildList();
	start = listChild.begin();
	end = listChild.end();

	for(;start != end;++start)
	{
		if(*start == pAddObject)
		{
			it = m_listMouseMessageObject.insert(it, pAddObject);
			bResult = true;
			return true;
		}

		if(*it == *start) ++it;
		if(AddMouseMessageObjectTemp(*start, pAddObject, it, end_it, bResult)) return true;
	}

	return false;
}

bool CObjectProject::SubMouseMessageObject(CObjectBase *pObject)
{
	if(pObject->GetID() == _ID_OBJECT_ROOT) return false;
	CThreadLock lock(&m_cLockMouse);

	LISTOBJECT::iterator start = m_listMouseMessageObject.begin();
	LISTOBJECT::iterator end = m_listMouseMessageObject.end();

	LISTOBJECT::iterator findIt = find(start, end, pObject);
	ASSERT(findIt != end);
	if(findIt == end) return false;
	m_listMouseMessageObject.erase(findIt);

	if(m_pParent) m_pParent->OnRemoveChildObject(pObject);

	return true;
}

void CObjectProject::CallLogic(const DWORD &dwTime)
{
	CThreadLock lock(&m_cLockLogic);
	m_bLogicProcess = true;

	if(m_bFirst)
	{
		LISTLOGICITER start = m_listLogicObject.begin();
		LISTLOGICITER end = m_listLogicObject.end();

		m_itNextLogic = start;
		DWORD dwTickCount = m_pParent->GetTickCount();

		while(start != end)
		{
			++m_itNextLogic;
			start->second = dwTickCount;
			start->first->SetLogicStartTime(dwTickCount);
			start = m_itNextLogic;
		}

		m_bFirst = false;
	}

	LISTLOGICITER start = m_listLogicObject.begin();
	LISTLOGICITER end = m_listLogicObject.end();

	m_itNextLogic = start;

	while(start != end)
	{
		++m_itNextLogic;
		start->first->Logic(dwTime - start->second);
		start = m_itNextLogic;
	}

	m_bLogicProcess = false;

	LISTOBJECTITER start_remove = m_listRemove.begin();
	LISTOBJECTITER end_remove = m_listRemove.end();
	for(;start_remove != end_remove;++start_remove)
	{
		delete *start_remove;
	}
	m_listRemove.clear();
}

const DWORD &CObjectProject::GetTickCount(void)
{
	return m_pParent->GetTickCount();
}

void CObjectProject::CallDraw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_pMainObject->m_bShow && m_pMainObject->m_bShowRef)
		m_pMainObject->Draw(pPage);
}

CObjectBase *CObjectProject::IsInObject(CPoint &ptPos, const bool &bClick, OUT CObjectBase **ppDisableInObject)
{
	CThreadLock lock(&m_cLockLogic);
	LISTOBJECT::reverse_iterator rstart = m_listMouseMessageObject.rbegin();
	LISTOBJECT::reverse_iterator rend = m_listMouseMessageObject.rend();

	CObjectBase *pObjectRet = NULL;
	CObjectBase *pObject = NULL;
	bool bIn = false;
	for(;rstart != rend;++rstart)
	{
		pObject = *rstart;
		if(pObject->IsIn(ptPos, bIn))
		{
			pObjectRet = *rstart;
		}
		else if(bIn && ppDisableInObject)
		{
			*ppDisableInObject = *rstart;
		}
		if(pObjectRet || bIn) break;
	}

	if(pObjectRet)
	{
		CObjectBase *pIntercept = pObjectRet->GetInterceptSelect();
		if(pIntercept) pObjectRet = pIntercept;
	}

	CObjectBase *pRet = pObjectRet;
	if(bIn && pObjectRet == NULL)
	{
		pObjectRet = pObject;
		CObjectBase *pIntercept = pObjectRet->GetInterceptSelect();
		if(pIntercept) pObjectRet = pIntercept;
	}

	//팝업 Object가 있을 경우
	if(m_pPopupObject && bClick)
	{
		//선택된 오브젝트가 없거나, 선택된 Object가 팝업 Object의 Child가 아닐경우 닫을지 여부를 조사한다.
		if(pObjectRet == NULL || pObjectRet->IsParent(m_pPopupObject) == false)
		{
			SetPopup(NULL, pObjectRet);
		}
	}

	return pRet;
}

void CObjectProject::OnRemoveObject(CObjectBase *pObject)
{
	if(pObject == NULL) return;
	if(m_pPopupObject == pObject)
	{
		m_pPopupObject = NULL;
		m_pPopupParent = NULL;
	}
}

void CObjectProject::OnRemoveChildObject(CObjectBase *pObject)
{
	if(pObject == NULL) return;
	if(m_pParent) m_pParent->OnRemoveChildObject(pObject);
}

LSY::CObjectBase* CObjectProject::AddSprObj( const std::string& strPath_,UINT nID_ , UINT nPlayIndex_, bool bAfterHide_, CPoint cpPos)
{
	LSY::CSprite* pSpr = new LSY::CSprite;
	if(NULL==pSpr) return NULL;
	pSpr->Create( strPath_.c_str() );
	pSpr->SetID( nID_ );
	stringstream str;
	str << nID_;
	pSpr->SetName( str.str() );
	pSpr->SetUseMultiScene(true);
	pSpr->SetPlayIndex(nPlayIndex_);
	pSpr->SetLocalPos(cpPos);
	pSpr->SetRepeatCount(1);
	pSpr->SetAfterHide(bAfterHide_);
	pSpr->SetMessage(true);
	AddObject(pSpr);
	GetMainObject()->AddChild(pSpr);
	return (LSY::CObjectBase*)pSpr;
}

LSY::CImage* CObjectProject::AddImgObj( const std::string& strPath_,UINT nID_, UINT nPlayIndex_, CPoint cpPos)
{
	LSY::CImage* pImg = new LSY::CImage;
	if(NULL==pImg) return NULL;
	pImg->Create( strPath_.c_str() );
	stringstream str;
	str << nID_;
	pImg->SetName( str.str());
	pImg->SetID( nID_ );
	pImg->SetIndex(nPlayIndex_);
	pImg->SetShow(false);
	pImg->SetLocalPos(cpPos);
	AddObject(pImg);
	GetMainObject()->AddChild(pImg);

	return pImg;
}

const LONG_PTR &CObjectMan::GetOldWindowProc(void)
{
	return m_pOldProc;
}

CObjectMan::CObjectMan(CWnd *pWnd, CWnd *pMainWnd/* = NULL*/, bool bMessageHooking/* = true*/, bool bEventDraw/* = false*/)
:	m_pWnd(pWnd),
	m_bEventDraw(bEventDraw),
	m_pOverObject(NULL),
	m_pSelectObject(NULL),
	m_dwTickCount(0),
	m_pFocusObject(NULL),
	m_pWheelFocusObject(NULL),
	m_bFirst(true),
	m_bUseCustomCursor(false),
	m_bMouseObserver(false),
	m_bDrag(false),
	m_szSize(0, 0),
	m_bTracking(false),
	m_dwMouseStatus(0),
	m_nCaptionHeight(0),
	m_ptDragGab(CPointZero),
	m_pMemDC(NULL),
	m_fMultiple(1.0f),
	m_pMainWnd(pWnd),
	m_pBackPage(NULL),
	m_bSendEventDraw(true),
	m_pBackDC(NULL),
	m_bMouseCapture(false),
	m_bFirstPaint(true),
	m_bCallTimer(false),
	m_bFirstDrawPage(false)
{
	m_pPage = new NMBASE::GRAPHICGDI::CPage;
	if(pMainWnd) m_pMainWnd = pMainWnd;

	if(bEventDraw) m_pBackPage = new NMBASE::GRAPHICGDI::CPage;

	m_hCursorNormal = LoadCursor(NULL, IDC_ARROW);
	m_hCursorOver	= LoadCursor(NULL, IDC_HAND);
	m_hCursorDown	= LoadCursor(NULL, IDC_ARROW);
	m_hCursorEdit	= LoadCursor(NULL, IDC_IBEAM);

	::CRect rt;
	pWnd->GetWindowRect(&rt);
	m_pPage->Init(rt.Width(), rt.Height(), 16);
	if(m_pBackPage) m_pBackPage->Init(rt.Width(), rt.Height(), 16);

	InitializeCriticalSection(&m_cLock);
	InitializeCriticalSection(&m_cHotkeyLock);
	InitializeCriticalSection(&m_cTimerLock);

	if(bMessageHooking)
	{
		m_pOldProc = SetWindowLongPtr(m_pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)StaticWindowProc);
		ASSERT(m_sMapProc.find(m_pWnd->GetSafeHwnd()) == m_sMapProc.end());
		m_sMapProc[m_pWnd->GetSafeHwnd()] = this;
	}
	else
	{
		m_pOldProc = NULL;
	}
}

CObjectMan::~CObjectMan()
{
	m_bMouseCapture = false;

	if(GetCapture() == m_pWnd->GetSafeHwnd())
		ReleaseCapture();

	if(m_pOldProc)
	{
		SetWindowLongPtr(m_pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)m_pOldProc);
		m_pOldProc = NULL;
		m_sMapProc.erase(m_pWnd->GetSafeHwnd());
	}

	m_pWnd = NULL;

	if(m_pPage) delete m_pPage;
	m_pPage = NULL;

	if(m_pBackPage) delete m_pBackPage;
	m_pBackPage = NULL;

	MAPPROJECT::iterator start = m_mapProject.begin();
	MAPPROJECT::iterator end = m_mapProject.end();

	for(;start != end;++start)
	{
		if(start->second->IsActive()) start->second->SetActive(false);
		delete start->second;
	}

	m_mapProject.clear();

	ClearHotKey();
	ClearTimer();

	DeleteCriticalSection(&m_cLock);
	DeleteCriticalSection(&m_cHotkeyLock);
	DeleteCriticalSection(&m_cTimerLock);

	if(m_pMemDC)
	{
		m_pMemDC->DeleteDC();
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if(m_pBackDC)
	{
		m_pBackDC->DeleteDC();
		delete m_pBackDC;
		m_pBackDC = NULL;
	}

	if(m_spMainObjectMan == this) m_spMainObjectMan = NULL;
	//UPGUnInitSSOWebBrowser();
}

void CObjectMan::AddChildObjectMan(CObjectMan *pObjectMan)
{
	m_listChildObjectMan.push_back(pObjectMan);
}

void CObjectMan::SubChildObjectMan(CObjectMan *pObjectMan)
{
	m_listChildObjectMan.remove(pObjectMan);
}

void CObjectMan::CloseAllPopup(CObjectProject *pIgnoreProject)
{
	MAPLAYER::iterator start = m_mapLayer.begin();
	MAPLAYER::iterator end = m_mapLayer.end();

	for(;start != end;++start)
	{
		if(start->second != pIgnoreProject) start->second->SetPopup(NULL, NULL);
	}
}

const bool CObjectMan::IsMouseMessage(UINT &nMessage)
{
	bool bCapture = false;
	bool bRelease = false;
	bool bFirstClick = (m_dwMouseStatus == 0)?true:false;
	switch(nMessage)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		bCapture = true;
		m_dwMouseStatus |= GMKS_LBUTTON;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		bCapture = true;
		m_dwMouseStatus |= GMKS_RBUTTON;
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		bCapture = true;
		m_dwMouseStatus |= GMKS_MBUTTON;
		break;
	case WM_LBUTTONUP:
		bRelease = true;
		m_dwMouseStatus &= (~GMKS_LBUTTON);
		break;
	case WM_RBUTTONUP:
		bRelease = true;
		m_dwMouseStatus &= (~GMKS_RBUTTON);
		break;
	case WM_MBUTTONUP:
		bRelease = true;
		m_dwMouseStatus &= (~GMKS_MBUTTON);
		break;
	case WM_MOUSEWHEEL:
		return true;
	case WM_MOUSEMOVE:
		if(!m_bTracking)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.hwndTrack = m_pWnd->GetSafeHwnd();
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 1;
			_TrackMouseEvent(&tme);

			m_bTracking = true;

			CMO_WindowMouseOver msg;
			m_cMessageManager.Call(&msg);
		}
		return true;
	case WM_MOUSELEAVE:
		{
			m_bTracking = false;

			CMO_WindowMouseLeave msg;
			m_cMessageManager.Call(&msg);
		}
		return true;
	}

	if(bCapture)
	{
		m_pWnd->SetFocus();
		if(!m_bMouseCapture) m_pWnd->SetCapture();
		if(bFirstClick) m_dwFirstClickMouseKey = m_dwMouseStatus;
		return true;
	}

	if(bRelease)
	{
		//m_pWnd->SetFocus();
		if(m_dwMouseStatus == 0 && (GetCapture() == m_pWnd->GetSafeHwnd()) && !m_bMouseCapture)
			ReleaseCapture();
		return true;
	}

	return false;
}

const bool CObjectMan::IsKeyboardMessage(UINT &nMessage)
{
	switch(nMessage)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_SYSKEYDOWN:
	case WM_IME_CHAR:
	case WM_IME_COMPOSITION:
	case WM_IME_COMPOSITIONFULL:
	case WM_IME_CONTROL:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_KEYDOWN:
	case WM_IME_KEYUP:
	case WM_IME_NOTIFY:
	case WM_IME_REQUEST:
	case WM_IME_SELECT:
	case WM_IME_SETCONTEXT:
	case WM_IME_STARTCOMPOSITION:
		return true;
	}

	return false;
}

void CObjectMan::SetMouseOverObject(CObjectBase *pObject)
{
	if(m_pOverObject != pObject)
	{
		if(m_pOverObject) m_pOverObject->OnMouseLeave();
		m_pOverObject = pObject;
		if(pObject) m_pOverObject->OnMouseOver();
	}
	if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);
}

void CObjectMan::OnRemoveChildObject(CObjectBase *pObject)
{
	if(pObject == NULL) return;

	bool bOver = (pObject == m_pOverObject)?true:pObject->IsChild(m_pOverObject);
	bool bSelect = bOver;
	bool bFocus = bOver;
	bool bWheel = bOver;

	if(m_pOverObject != m_pFocusObject) bFocus = (pObject == m_pFocusObject)?true:pObject->IsChild(m_pFocusObject);
	if(m_pOverObject != m_pSelectObject) bSelect = (pObject == m_pSelectObject)?true:pObject->IsChild(m_pSelectObject);
	if(m_pOverObject != m_pWheelFocusObject) bWheel = (pObject == m_pWheelFocusObject)?true:pObject->IsChild(m_pWheelFocusObject);

	LISTOBJECTITER start = m_listTrackingOutsideClick.begin();
	LISTOBJECTITER end = m_listTrackingOutsideClick.end();
	LISTOBJECTITER findIt = find(start, end, pObject);
	if(findIt != end)
	{
		(*start)->SetTracking(false);
		m_listTrackingOutsideClick.erase(start);
	}

	if(bSelect) 
	{
		if(m_pSelectObject)
		{
			m_pSelectObject = NULL;
			if(GetCapture() == m_pWnd->GetSafeHwnd() && !m_bMouseCapture) ReleaseCapture();
		}
		if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);
	}
	if(bFocus) SetFocus(NULL, false);
	if(bWheel) m_pWheelFocusObject = NULL;
	if(bOver)
	{
		CheckMouseOverObject();
	}
}

void CObjectMan::SetUseObserverMouseEvent(bool bUse)
{
	if(m_bMouseObserver == bUse) return;
	m_bMouseObserver = bUse;
	if(bUse == false && m_bDrag)
	{
		m_bDrag = false;
	}
}

void CObjectMan::ClearObserverMouseEvent(void)
{
	if(!m_bMouseObserver) return;
	if(m_bDrag)
	{
		m_bDrag = false;
	}
}

LRESULT CObjectMan::WindowsProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == WM_TIMER) return FALSE;
	if(nMessage == WM_KILLFOCUS)
	{
		char strClassName[MAX_PATH];
		HWND hWnd = (HWND)wParam;
		GetClassName((HWND)wParam, strClassName, MAX_PATH);
		if(strcmp(strClassName, "Shell Embedding") == 0)
		{
			HWND hParent = GetParent(hWnd);
			//while(hParent)
			if(hParent)
			{
				if(m_pWnd->GetSafeHwnd() == hParent)
				{
					hParent = GetParent(hWnd);
					m_pWnd->SetFocus();
					if(m_pSelectObject)
					{
						if(GetCapture() != m_pWnd->GetSafeHwnd())
							if(!m_bMouseCapture) m_pWnd->SetCapture();
					}
					return TRUE;
				}
				//return FALSE;
			}
		}

		if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);

		CloseAllPopup(NULL);
		m_bDrag = false;

		if(m_pSelectObject)
		{
			m_pSelectObject = NULL;
			if(GetCapture() == m_pWnd->GetSafeHwnd() && !m_bMouseCapture) ReleaseCapture();
		}
		if(m_pFocusObject) SetFocus(NULL);
		if(m_pWheelFocusObject) m_pWheelFocusObject = NULL;
		if(m_pOverObject) SetMouseOverObject(NULL);

		m_dwMouseStatus = 0;
		return TRUE;
	}

	if(IsMouseMessage(nMessage))
	{
		CObjectBase *pObject = NULL;
		
		m_ptMousePos.x = (float)GET_X_LPARAM(lParam);
		m_ptMousePos.y = (float)GET_Y_LPARAM(lParam);

		if(m_fMultiple != 1.0f)
		{
			m_ptMousePos.x /= m_fMultiple;
			m_ptMousePos.y /= m_fMultiple;
		}

		if(m_bMouseObserver)
		{
			switch(nMessage)
			{
			case WM_LBUTTONDOWN:
				{
					CMO_Observer_DragBegin msg(m_ptMousePos);
					if(m_cMessageManager.Call(&msg) == RESULT_OK)
					{
						m_bDrag = true;
						return TRUE;
					}

					if(msg.IsRet()) return TRUE;
				}
				break;
			case WM_MOUSEMOVE:
				if(m_bDrag)
				{
					CMO_Observer_Draging msg(m_ptMousePos);
					m_cMessageManager.Call(&msg);
					return TRUE;
				}
				break;
			case WM_LBUTTONUP:
				if(m_bDrag)
				{
					CMO_Observer_DragEnd msg(m_ptMousePos);
					m_cMessageManager.Call(&msg);
					m_bDrag = false;
					return TRUE;
				}
				break;
			}
		}
		else if(m_bDrag)
		{
			switch(nMessage)
			{
			case WM_MOUSEMOVE:
				if(m_bDrag)
				{			
					POINT ptPos;
					ptPos.x = (int)m_ptMousePos.x;
					ptPos.y = (int)m_ptMousePos.y;
					m_pMainWnd->ClientToScreen(&ptPos);
					m_pMainWnd->SetWindowPos(NULL, ptPos.x + (int)m_ptDragGab.x, ptPos.y + (int)m_ptDragGab.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
					return TRUE;
				}
				break;
			case WM_LBUTTONUP:
				if(m_bDrag)
				{
					CWnd *pRoot = m_pWnd;
					CWnd *pParent = m_pWnd->GetParent();
					while(pParent)
					{
						pRoot = pParent;
						pParent = pParent->GetParent();
					}

					POINT ptPos;
					ptPos.x = (int)m_ptMousePos.x;
					ptPos.y = (int)m_ptMousePos.y;
					m_pMainWnd->ClientToScreen(&ptPos);
					m_pMainWnd->SetWindowPos(NULL, ptPos.x + (int)m_ptDragGab.x, ptPos.y + (int)m_ptDragGab.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
					m_bDrag = false;
					return TRUE;
				}
				break;
			}
		}

		if(nMessage == WM_MOUSEWHEEL)
		{
			POINT ptMouse = m_ptMousePos.GetPoint();
			m_pWnd->ScreenToClient(&ptMouse);
			m_ptMousePos = ptMouse;

			if(m_pWheelFocusObject)
			{
				int nKey = LOWORD(wParam) ; 
				short zDelta = HIWORD(wParam) ; 

				m_pWheelFocusObject->OnMouseWheel(nKey, zDelta);
				return TRUE;
			}
			return FALSE;
		}

		if(m_pSelectObject)
		{
			switch(nMessage)
			{
			case WM_LBUTTONUP:
				m_pSelectObject->OnLButtonUP(m_ptMousePos);
				break;
			case WM_RBUTTONUP:
				m_pSelectObject->OnRButtonUp(m_ptMousePos);
				break;
			case WM_MBUTTONUP:
				m_pSelectObject->OnMButtonUp(m_ptMousePos);
				break;
			case WM_MOUSEMOVE:
				if(IsTool())
				{
					m_pWnd->PostMessage(UM_OBJECTMOVE, wParam, lParam);
				}
				m_pSelectObject->OnMouseMove(m_ptMousePos);
				break;
			}

			//팝업이 떠서 KillFocus가 되었을때 m_pSelectObject이 NULL로 변경 되기 때무에
			//NULL인지를 검토하여야 한다.
			if(m_pSelectObject == NULL)
			{
				return TRUE;
			}

			switch(nMessage)
			{
			case WM_LBUTTONUP:
				pObject = IsInObject(m_ptMousePos, true);
				if(m_dwFirstClickMouseKey == GMKS_LBUTTON)
				{
					if(pObject && pObject == m_pSelectObject) pObject->OnMouseLClick(m_ptMousePos);

					m_pSelectObject = NULL;
					SetMouseOverObject(pObject);
					m_dwFirstClickMouseKey = 0;
				}
				if(IsTool())
				{
					m_pWnd->PostMessage(UM_OBJECTMOVEEND, wParam, lParam);
				}
				break;
			case WM_RBUTTONUP:
				pObject = IsInObject(m_ptMousePos, true);
				if(m_dwFirstClickMouseKey == GMKS_RBUTTON)
				{
					if(pObject && pObject == m_pSelectObject) pObject->OnMouseRClick(m_ptMousePos);

					m_pSelectObject = NULL;
					SetMouseOverObject(pObject);
					m_dwFirstClickMouseKey = 0;
				}
				break;
			case WM_MBUTTONUP:
				pObject = IsInObject(m_ptMousePos, true);
				if(m_dwFirstClickMouseKey == GMKS_MBUTTON)
				{
					if(pObject && pObject == m_pSelectObject) pObject->OnMouseMClick(m_ptMousePos);

					m_pSelectObject = NULL;
					SetMouseOverObject(pObject);
					m_dwFirstClickMouseKey = 0;
				}
				break;
			}
			return TRUE;
		}
		else
		{
			bool bClick = true;
			switch(nMessage)
			{
			case WM_MOUSEMOVE:
			case WM_MOUSEWHEEL:
			case WM_MOUSELEAVE:
				bClick = false;
				break;
			}

			CObjectBase *pDisableInObject = NULL;

			if(nMessage == WM_MOUSELEAVE)
			{
				pObject = NULL;
				m_ptMousePos = CPoint(-10000, -10000);
			}
			else pObject = IsInObject(m_ptMousePos, bClick, &pDisableInObject);

			if(pObject && pObject->GetType() == EOT_WEB)
			{
				if(!IsTool() || pObject->IsFixed()) 
					pObject = NULL;
			}

			//마우스 트래킹 중일때 Diable된 Object가 트랙킹중 Object의 하위인지 판단하기 위해서
			//pDisableInObject 마우스 클릭이 일어난 Diable된 Object를 구해온다.
			if(bClick) CallTracking(pObject?pObject:pDisableInObject);

			switch(nMessage)
			{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
				if(IsTool())
				{
					WPARAM wParam = ((GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00))?GKS_SHIFT:0;
					wParam |= (GetKeyState(VK_LMENU) & 0xff00)?GKS_ALT:0;
					wParam |= (GetKeyState(VK_LCONTROL) & 0xff00)?GKS_CONTROL:0;
					m_pWnd->PostMessage(UM_OBJECTSELECT, wParam, (LPARAM)pObject);
				}
				break;
			}

			switch(nMessage)
			{
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
				if(pObject)
				{
					m_pSelectObject = pObject;
					SetMouseOverObject(pObject);
				}
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
				SetFocus(pObject);
				break;
			}

			if(m_nCaptionHeight > 0 && nMessage == WM_LBUTTONDOWN && m_ptMousePos.y <= m_nCaptionHeight && pObject == NULL)
			{
				m_bDrag = true;
				RECT rtWindow;
				m_pMainWnd->GetWindowRect(&rtWindow);
				POINT ptPos;
				ptPos.x = (int)m_ptMousePos.x;
				ptPos.y = (int)m_ptMousePos.y;
				m_pMainWnd->ClientToScreen(&ptPos);
				m_ptDragGab.SetPoint((float)rtWindow.left - ptPos.x, (float)rtWindow.top - ptPos.y);
				return TRUE;
			}

			//Object가 선택되어 마우스가 눌렸을 때
			if(m_pSelectObject)
			{
				switch(nMessage)
				{
				case WM_LBUTTONDOWN:
					m_pSelectObject->OnLButtonDown(m_ptMousePos);
					break;
				case WM_RBUTTONDOWN:
					m_pSelectObject->OnRButtonDown(m_ptMousePos);
					break;
				case WM_MBUTTONDOWN:
					m_pSelectObject->OnMButtonDown(m_ptMousePos);
					break;
				}

				//팝업이 떠서 KillFocus가 되었을때 m_pSelectObject이 NULL로 변경 되기 때무에
				//NULL인지를 검토하여야 한다.
				if(m_pSelectObject == NULL)
				{
					return TRUE;
				}

				m_pSelectObject->OnMouseMove(m_ptMousePos);
				return TRUE;
			}
			else if(pObject)
			{
				switch(nMessage)
				{
				case WM_LBUTTONDBLCLK:
					pObject->OnLButtonDbClick(m_ptMousePos);
					pObject->OnMouseLClick(m_ptMousePos);
					break;
				case WM_RBUTTONDBLCLK:
					pObject->OnRButtonDbClick(m_ptMousePos);
					pObject->OnMouseRClick(m_ptMousePos);
					break;
				case WM_MBUTTONDBLCLK:
					pObject->OnMButtonDbClick(m_ptMousePos);
					pObject->OnMouseMClick(m_ptMousePos);
					break;
				case WM_LBUTTONUP:
					pObject->OnLButtonUP(m_ptMousePos);
					break;
				case WM_RBUTTONUP:
					pObject->OnRButtonUp(m_ptMousePos);
					break;
				case WM_MBUTTONUP:
					pObject->OnMButtonUp(m_ptMousePos);
					break;
				}

				SetMouseOverObject(pObject);
				m_pOverObject->OnMouseMove(m_ptMousePos);
				return TRUE;
			}
			else if(m_pOverObject)
			{	
				SetMouseOverObject(NULL);
				return TRUE;
			}
		}
	}
	else if(IsKeyboardMessage(nMessage))
	{
		if(CheckHotkey(nMessage, wParam, lParam)) return TRUE;

		if(m_pFocusObject)
		{
			switch(nMessage)
			{
			case WM_KEYDOWN:
				m_pFocusObject->OnKeyDown(wParam);
				switch(wParam)
				{
				case VK_TAB:
					m_pFocusObject->OnTabDown();
					break;
				case VK_RETURN:
					m_pFocusObject->OnReturn();
					break;
				}
				break;
			case WM_KEYUP:
				m_pFocusObject->OnKeyUp(wParam);
				break;
			}

			if(m_pFocusObject && m_pFocusObject->OnKeyProc(nMessage, wParam, lParam)) return TRUE;
		}
	}

	switch(nMessage)
	{
	case WM_INPUTLANGCHANGE:
	case WM_IME_STARTCOMPOSITION:
		return TRUE;
	case WM_SETCURSOR:
		if(m_bUseCustomCursor)
		{
			CMO_SetCursor msg(m_pOverObject, m_pSelectObject);
			if(m_cMessageManager.Call(&msg) != RESULT_OK)
			{
				if(m_pSelectObject)
				{
					SetCursor(m_hCursorDown);
				}
				else if(m_pOverObject)
				{
					if(m_pOverObject->GetType() == EOT_EDIT)
					{
						SetCursor(m_hCursorEdit);
					}
					else if( m_pOverObject->GetType() > EOT_DUMYIMAGE &&
							 m_pOverObject->GetType() < EOT_TEXT || 
							 m_pOverObject->GetType() == EOT_TEXTBUTTON )
					{
						SetCursor(m_hCursorOver);
					}
					else
					{
						SetCursor(m_hCursorNormal);
					}
				}
				else
				{
					SetCursor(m_hCursorNormal);
				}
			}

			return TRUE;
		}
		break;
	case LSY_UM_EVENT_DRAW:
		if(OnEventObjectDraw((CObjectBase *)lParam)) return TRUE;
		break;
	default:
		break;
	}

	return FALSE;
}

bool CObjectMan::OnEventObjectDraw(CObjectBase *pObject)
{
	if(pObject == NULL) return false;

	ClearEventDrawList();
	switch(pObject->GetType())
	{
	case EOT_DUMYIMAGE:
		{
			CObjectBase *pParent = pObject->GetParent();
			switch(pParent->GetType())
			{
			case EOT_EDIT:
				{
					CEdit *pEdit = (CEdit *)pParent;
					if(pEdit->IsRedraw())
					{
						CEditCursor *pCursor = pEdit->GetCursor();
						CRect rtRect = pEdit->GetRect();

						--rtRect.left;
						--rtRect.top;
						++rtRect.right;
						++rtRect.bottom;

						EventBackToFrontBitBlt(&rtRect);

						pEdit->Draw(m_pPage);
						if(pObject->IsShow() && pObject->IsShowRef()) pObject->CObjectBase::DrawLocal(m_pPage);
						else pObject->ClearSendEventDraw();

						if(pCursor->IsShow() && pCursor->IsShowRef()) pCursor->CObjectBase::DrawLocal(m_pPage);
						else pCursor->ClearSendEventDraw();

						EventBitBlt(&rtRect);
					}
				}
				break;
			case EOT_AVATAR:
				{
					if(pObject->IsSendEventDraw())
					{
						CRect &rtRect = pObject->GetRect();
						EventBackToFrontBitBlt(&rtRect);

						if(pObject->IsShow() && pObject->IsShowRef()) pObject->CObjectBase::DrawLocal(m_pPage);
						else pObject->ClearSendEventDraw();

						EventBitBlt(&rtRect);
					}
				}
				break;
			case EOT_RICHTEXT:
			case EOT_TEXTLIST:
				{
					if(pObject->IsSendEventDraw())
					{
						CRect &rtRect = pObject->GetRect();
						EventBackToFrontBitBlt(&rtRect);

						if(pObject->IsShow() && pObject->IsShowRef())
						{
							pParent->Draw(m_pPage);
							pObject->CObjectBase::DrawLocal(m_pPage);
						}
						else pObject->ClearSendEventDraw();

						EventBitBlt(&rtRect);
					}
				}
				break;
			}
		}
		break;
	case EOT_EDITCURSOR:
		{
			CEditCursor *pCursor = (CEditCursor *)pObject;
			if(pCursor->IsTempEventDraw())
			{
				CEdit *pEdit = (CEdit *)pCursor->GetParent();
				CRect rtRect = pEdit->GetRect();
				CDumyImage *pBack = pEdit->GetBack();

				--rtRect.left;
				--rtRect.top;
				++rtRect.right;
				++rtRect.bottom;

				EventBackToFrontBitBlt(&rtRect);

				pEdit->Draw(m_pPage);
				if(pBack->IsShow() && pBack->IsShowRef()) pBack->CObjectBase::DrawLocal(m_pPage);
				else pBack->ClearSendEventDraw();

				if(pCursor->IsShow() && pCursor->IsShowRef()) pCursor->CObjectBase::DrawLocal(m_pPage);
				else pCursor->ClearSendEventDraw();

				EventBitBlt(&rtRect);
			}
		}
		break;
	case EOT_IMAGE:
		{
			CImage *pImage = (CImage *)pObject;
			CObjectBase *pParent = pObject->GetParent();
			ENUM_OBJECTTYPE nParentType = pParent->GetType();
			if(nParentType == EOT_BUTTON)
			{
				CObjectBase *pParentParent = pParent->GetParent();
				if(pParentParent && (pParentParent->GetType() == EOT_SLIDEBAR))
				{
					pParent = pParentParent;
				}
			}

			switch(pParent->GetType())
			{
			case EOT_SLIDEBAR:
				{
					CSlideBar *pSlideBar = (CSlideBar *)pParent;
					pSlideBar->EventDraw(this, m_pPage);
				}
				break;
			default:
				if(pImage->IsSendEventDraw())
				{
					CRect rtOldDraw = pImage->GetOldDrawRect();

					pImage->Draw(m_pPage);

					CRect &rtDraw = pImage->GetRect();

					if(rtOldDraw != rtDraw)
					{
						EventBackToFrontBitBlt(&rtOldDraw);
						EventBackToFrontBitBlt(&rtDraw);

						if(pImage->IsShow() && pImage->IsShowRef())	pImage->CObjectBase::DrawLocal(m_pPage);
						else pImage->ClearSendEventDraw();

						EventBitBlt(&rtOldDraw);
						EventBitBlt(&rtDraw);
					}
					else
					{
						EventBackToFrontBitBlt(&rtDraw);

						if(pImage->IsShow() && pImage->IsShowRef())	pImage->CObjectBase::DrawLocal(m_pPage);
						else pImage->ClearSendEventDraw();

						EventBitBlt(&rtDraw);
					}
				}
				break;
			}
		}
		break;
	}

	return false;
}

//OnClickOutside내에서 SetShow, SetShowRef, SubChild등 화면에 사라지게 되면 Tracking 중단된다.
//사라지지 않으면서 트래킹을 중지하려면 RemoveTracking 호출해 주어야 한다.
void CObjectMan::CallTracking(CObjectBase *pObject)
{
	LISTOBJECTITER start = m_listTrackingOutsideClick.begin();
	LISTOBJECTITER end = m_listTrackingOutsideClick.end();
	LISTOBJECTITER prev, findIt;

	LSY::CObjectBase *pObject2 = NULL;
	if(pObject == NULL)
	{
		while(start != end)
		{
			prev = start;
			++start;
			pObject2 = *prev;
			pObject2->OnClickOutside(NULL);
		}
	}
	else
	{
		while(start != end)
		{
			prev = start;
			++start;

			pObject2 = *prev;
			if(!pObject->IsParent(pObject2))
			{
				pObject2->OnClickOutside(pObject);
			}
		}
	}
}

int	CObjectMan::GetScanCode(const WPARAM &wParam, const LPARAM &lParam)
{
	int nScanCode = (lParam >> 16) & 0x000000FF;
	switch(nScanCode)
	{
	case VK_S_PAUSE:
		if(wParam == VK_NUMLOCK) nScanCode = VK_SP_NUM;
		break;
	case VK_S_DEL:
		if(wParam == VK_DECIMAL) nScanCode = VK_SP_PERIOD;
		break;
	}

	switch(wParam)
	{
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
		nScanCode = VK_SP_0 + (nScanCode - VK_NUMPAD0);
		break;
	}
	return nScanCode;
}

LRESULT	CObjectMan::CheckHotkey(UINT &nMessage, WPARAM &wParam, LPARAM &lParam)
{
	switch(nMessage)
	{
	case WM_KEYDOWN:
		{
			unsigned short nHotKey = 0;
			int		nScanCode = GetScanCode(wParam, lParam);
			bool	bPrevious = ((nScanCode >> 30) & 0x00000001)?true:false;
			if(GetAsyncKeyState(VK_LCONTROL) & 0x8000) nHotKey |= 0x0001;
			if(GetAsyncKeyState(VK_LMENU) & 0x8000) nHotKey |= 0x0002;
			if(CallHotKey(nHotKey, nScanCode, bPrevious)) return TRUE;
		}
		break;
	}
	return FALSE;
}

void CObjectMan::SetTimer(int nID, DWORD dwElapse, IFunctor* pFunctor, int nRepeat/*=0*/)
{
	ASSERT(nRepeat >= 0);
	CThreadLock lock(&m_cTimerLock);
	MAP_TIMER::iterator findIt = m_mapTimer.find(nID);
	ASSERT(findIt == m_mapTimer.end());
	TIMER_INFO info;
	info.dwElapse	= dwElapse;
	info.dwStart	= m_dwTickCount;
	info.dwNext		= m_dwTickCount + dwElapse;
	info.nRepeat	= nRepeat;
	info.nCurRepeat	= 0;
	info.cDelegate.Default(pFunctor);
	m_mapTimer[nID] = info;
}

void CObjectMan::KillTimer(int nID, bool bDel /* = false */ )
{
	CThreadLock lock(&m_cTimerLock);
	if(m_bCallTimer && bDel==false)
	{
		m_listDeleteTimer.push_back(nID);
	}
	else
	{
		MAP_TIMER::iterator findIt = m_mapTimer.find(nID);
		if(findIt == m_mapTimer.end()) return;
		findIt->second.cDelegate.Release();
		m_mapTimer.erase(findIt);
	}
}

bool CObjectMan::IsTimer(int nID)
{
	CThreadLock lock(&m_cTimerLock);

	if(m_mapTimer.find(nID) == m_mapTimer.end()) return false;
	return true;
}

void CObjectMan::ClearTimer(void)
{
	CThreadLock lock(&m_cTimerLock);

	MAP_TIMER::iterator start = m_mapTimer.begin();
	MAP_TIMER::iterator end = m_mapTimer.end();

	for(;start != end;++start)
	{
		start->second.cDelegate.Release();
	}

	m_mapTimer.clear();
}

void CObjectMan::InitFirstTime(void)
{
	CThreadLock lock(&m_cTimerLock);

	MAP_TIMER::iterator start = m_mapTimer.begin();
	MAP_TIMER::iterator end = m_mapTimer.end();

	for(;start != end;++start)
	{
		start->second.dwStart = m_dwTickCount;
		start->second.dwNext = m_dwTickCount + start->second.dwElapse;
	}
}

void CObjectMan::CheckTimer(void)
{
	CThreadLock lock(&m_cTimerLock);
	m_bCallTimer = true;

	MAP_TIMER::iterator start = m_mapTimer.begin();
	MAP_TIMER::iterator end = m_mapTimer.end();

	std::list<int>::iterator start_del, end_del, findIt;
	while(start != end)
	{
		while(1)
		{
			if(start->second.dwNext < m_dwTickCount)
			{
				++start->second.nCurRepeat;
				CMO_Timer msg(start->first, start->second.nCurRepeat, start->second.nRepeat);
				start->second.cDelegate.Fire(&msg);

				start_del = m_listDeleteTimer.begin();
				end_del = m_listDeleteTimer.end();
				findIt = find(start_del, end_del, start->first);

				if((findIt == end_del) && ((start->second.nRepeat == 0) || (start->second.nRepeat > start->second.nCurRepeat)))
				{
					start->second.dwNext += start->second.dwElapse;
				}
				else
				{
					if(findIt != end_del)
					{
						m_listDeleteTimer.remove(start->first);
					}

					start->second.cDelegate.Release();
					start = m_mapTimer.erase(start);
					break;
				}
			}
			else
			{
				++start;
				break;
			}
		}
	}

	m_listDeleteTimer.clear();
	m_bCallTimer = false;
}

LRESULT	CObjectMan::CallHotKey(const unsigned short &sHotkey, const int &nScanCode, const bool &bPrevious)
{
	CThreadLock lock(&m_cHotkeyLock);
	CDelegate *pDel = FindDelegate(sHotkey, nScanCode);
	if(pDel)
	{
		CMO_HotKey msg(m_pFocusObject, nScanCode, (sHotkey&0x0001)?true:false, (sHotkey&0x0002)?true:false, bPrevious);
		return pDel->Fire(&msg);
	}
	return FALSE;
}

LSY::CDelegate *CObjectMan::FindDelegate(const unsigned short &sHotkey, const int &nScanCode)
{
	CThreadLock lock(&m_cHotkeyLock);
	MAP_HOTKEY::iterator it = m_mapHotKey.find( sHotkey );
	if( m_mapHotKey.end() == it )
	{
		return NULL;
	}

	MAP_SCANCODE_DELEGATE::iterator itScan = it->second.find(nScanCode);
	if(it->second.end() == itScan)
	{
		return NULL;
	}

	return &itScan->second;
}

bool CObjectMan::AddHotKeyMsgH(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cHotkeyLock);
	unsigned short nHotKey = 0;
	if(bControl) nHotKey |= 0x0001;
	if(bAlt) nHotKey |= 0x0002;

	CDelegate* pDel = FindDelegate( nHotKey, nScanCode );

	if(!pDel)
	{
		// create a new delegate
		CDelegate del;
		del.Default( pFunctor );

		m_mapHotKey[nHotKey][nScanCode] = del;
		return true;
	}

	if( false == pDel->Default( pFunctor ) )
	{		
		return false;
	}

	return true;
}

bool CObjectMan::AppendHotKeyMsgH(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cHotkeyLock);
	unsigned short nHotKey = 0;
	if(bControl) nHotKey |= 0x0001;
	if(bAlt) nHotKey |= 0x0002;

	CDelegate* pDel = FindDelegate( nHotKey, nScanCode );

	if(!pDel )
	{
		// create a new delegate
		CDelegate del;
		del.Append( pFunctor );

		m_mapHotKey[nHotKey][nScanCode] = del;
		return true;
	}

	if( false == pDel->Append( pFunctor ) )
	{		
		return false;
	}

	return true;
}

bool CObjectMan::OverrideHotKeyMsgH(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cHotkeyLock);
	unsigned short nHotKey = 0;
	if(bControl) nHotKey |= 0x0001;
	if(bAlt) nHotKey |= 0x0002;

	CDelegate* pDel = FindDelegate( nHotKey, nScanCode );
	if(!pDel )
	{
		// create a new delegate
		CDelegate del;
		del.Override( pFunctor );

		m_mapHotKey[nHotKey][nScanCode] = del;
		return true;
	}

	// Override
	if(false ==  pDel->Override( pFunctor) )
	{		
		return false;
	}

	return true;
}

bool CObjectMan::RemoveHotKeyMsgH(bool bControl,  bool bAlt, int nScanCode)
{
	CThreadLock lock(&m_cHotkeyLock);
	unsigned short nHotKey = 0;
	if(bControl) nHotKey |= 0x0001;
	if(bAlt) nHotKey |= 0x0002;

	MAP_HOTKEY::iterator it = m_mapHotKey.find( nHotKey );
	if( m_mapHotKey.end() == it )
	{
		return false;
	}

	MAP_SCANCODE_DELEGATE::iterator itScan = it->second.find(nScanCode);
	if(it->second.end() == itScan)
	{
		return false;
	}

	itScan->second.Release();
	it->second.erase(itScan);
	if(it->second.size() == 0)
	{
		m_mapHotKey.erase(it);
	}
	return true;
}

bool CObjectMan::RemoveAppendHotkeyMsgH(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cHotkeyLock);
	// Find
	unsigned short nHotKey = 0;
	if(bControl) nHotKey |= 0x0001;
	if(bAlt) nHotKey |= 0x0002;

	MAP_HOTKEY::iterator it = m_mapHotKey.find( nHotKey );
	if( m_mapHotKey.end() == it )
	{
		return false;
	}

	MAP_SCANCODE_DELEGATE::iterator itScan = it->second.find(nScanCode);
	if(it->second.end() == itScan)
	{
		return false;
	}
	// AppendRemove
	itScan->second.ReleaseAppend(pFunctor);
	pFunctor->Release();

	return true;
}

bool CObjectMan::RemoveOverrideHotKeyMsgH(bool bControl,  bool bAlt, int nScanCode, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cHotkeyLock);
	// Find
	unsigned short nHotKey = 0;
	if(bControl) nHotKey |= 0x0001;
	if(bAlt) nHotKey |= 0x0002;

	MAP_HOTKEY::iterator it = m_mapHotKey.find( nHotKey );
	if( m_mapHotKey.end() == it )
	{
		return false;
	}

	MAP_SCANCODE_DELEGATE::iterator itScan = it->second.find(nScanCode);
	if(it->second.end() == itScan)
	{
		return false;
	}
	// Remove
	itScan->second.ReleaseOverride(pFunctor);
	pFunctor->Release();

	return true;
}

void CObjectMan::SetFocus(CObjectBase *pObject, bool bReleseWheel/* = true*/)
{
	if(m_pFocusObject == pObject)
	{
		if(bReleseWheel && m_pWheelFocusObject && pObject == NULL)
		{
			m_pWheelFocusObject = NULL;
		}
		return;
	}
	if(m_pFocusObject) 
	{
		m_pFocusObject->OnFocusOut(pObject);
	}

	CObjectBase *pOldFocusObject = m_pFocusObject;

	if(pObject && pObject->IsMessage())
	{
		m_pFocusObject = pObject;
		m_pWheelFocusObject = pObject->GetWheelFocusObject();
	}
	else
	{
		m_pFocusObject = NULL;
		if(bReleseWheel) m_pWheelFocusObject = NULL;
	}

	if(m_pFocusObject) 
	{
		m_pFocusObject = m_pFocusObject->OnFocusIn(pOldFocusObject);
	}

	//if(pObject && (CWnd::GetFocus() != m_pWnd)) m_pWnd->SetFocus();
}

void CObjectMan::ClearHotKey(void)
{
	CThreadLock lock(&m_cHotkeyLock);

	MAP_HOTKEY::iterator start_hotkey = m_mapHotKey.begin();
	MAP_HOTKEY::iterator end_hotkey = m_mapHotKey.end();

	MAP_SCANCODE_DELEGATE::iterator start_scan, end_scan;
	for(;start_hotkey != end_hotkey;++start_hotkey)
	{
		start_scan = start_hotkey->second.begin();
		end_scan = start_hotkey->second.end();
		for(;start_scan != end_scan;++start_scan)
		{
			start_scan->second.Release();
		}
	}

	m_mapHotKey.clear();
}

CObjectBase *CObjectMan::GetFocus(void)
{
	return m_pFocusObject;
}

CObjectBase *CObjectMan::IsInObject(CPoint &ptPos, const bool &bClick, OUT CObjectBase **ppDisableInObject/* = NULL*/)
{
	MAPLAYER::reverse_iterator rstart = m_mapLayer.rbegin();
	MAPLAYER::reverse_iterator rend = m_mapLayer.rend();

	CObjectBase *pSelect = NULL;

	for(;rstart != rend;++rstart)
	{
		pSelect = rstart->second->IsInObject(ptPos, bClick, ppDisableInObject);
		if(pSelect)	return pSelect;
	}

	return NULL;
}

void CObjectMan::SetLayer(int nLayer, CObjectProject *pProject)
{
	CThreadLock lock(&m_cLock);
	MAPLAYER::iterator findIt = m_mapLayer.find(nLayer);
	ASSERT(findIt == m_mapLayer.end());
	m_mapLayer[nLayer] = pProject;
}

void CObjectMan::SetFPS(float fps)
{
	m_FrameSkip.SetFramePerSec(fps);
	m_FrameSkip.stTimer(FrameSkip::TIMER_RESET);
	m_FrameSkip.stReset();	
}

void CObjectMan::AllDraw(void)
{
	if(m_pWnd->GetSafeHwnd() && m_pWnd->IsWindowVisible())
	{
		if(IsEventDraw())
		{	
			if(IsEventDraw())
			{
				CallEventLogic();

				if(IsSendEventDraw())
				{
					CDC *pDC = m_pWnd->GetDC();
					EventBackDraw(pDC);
					BitBlt(pDC);
					m_pWnd->ReleaseDC(pDC);
					SetFirstDraw(false);
				}
			}
			else
			{
				Draw();
			}
		}
		else
		{
			Draw();
			m_bFirstDrawPage = true;
		}
	}

	std::list<CObjectMan *>::iterator start = m_listChildObjectMan.begin();
	std::list<CObjectMan *>::iterator end = m_listChildObjectMan.end();

	CObjectMan *pObjectMan = NULL;
	for(;start != end;++start)
	{
		pObjectMan = *start;
		CWnd *pWnd = pObjectMan->GetWnd();

		if(pWnd->GetSafeHwnd() && pWnd->IsWindowVisible())
		{
			if(pObjectMan->IsEventDraw())
			{	
				if(pObjectMan->IsEventDraw())
				{
					pObjectMan->CallEventLogic();

					if(pObjectMan->IsSendEventDraw())
					{
						CDC *pDC = pWnd->GetDC();
						pObjectMan->EventBackDraw(pDC);
						pObjectMan->BitBlt(pDC);
						pWnd->ReleaseDC(pDC);
						pObjectMan->SetFirstDraw(false);
					}
				}
				else
				{
					pObjectMan->Draw();
				}
			}
			else
			{
				pObjectMan->Draw();
			}
		}
	}
}

bool CObjectMan::Draw(CDC *pDC /*= NULL */)
{
	m_dwTickCount = timeGetTime();
	if(m_bFirst)
	{
		InitFirstTime();
		m_bFirst = false;

		m_FrameSkip.stTimer(FrameSkip::TIMER_RESET);
		m_FrameSkip.stReset();
	}

	CheckTimer();

	if(!m_FrameSkip.Update(m_FrameSkip.stTimer(FrameSkip::TIMER_GETELAPSEDTIME))) return false;

	CheckMouseOverObject();

	MAPLAYER::iterator start = m_mapLayer.begin();
	MAPLAYER::iterator end = m_mapLayer.end();

	for(;start != end;++start)
	{
		start->second->CallLogic(m_dwTickCount);
	}

	bool bRelease = false;
	if(pDC == NULL)
	{
		pDC = m_pWnd->GetDC();
		bRelease = true;
	}

	if(m_pMemDC == NULL)
	{
		m_pMemDC = new CDC;
		m_pMemDC->CreateCompatibleDC(pDC);
		m_pMemDC->SelectObject(m_pPage->hBmp);
	}

	if(IsTool()) m_pMemDC->FillSolidRect(0,0,m_pPage->width,m_pPage->height,RGB(0,0,0));

	start = m_mapLayer.begin();
	end = m_mapLayer.end();

	for(;start != end;++start)
	{
		start->second->CallDraw(m_pPage);
	}

	BitBlt(pDC);

	if(bRelease) m_pWnd->ReleaseDC(pDC);

	return true;
}

bool CObjectMan::BitBlt(CDC *pDC/* = NULL*/)
{
	bool bRelease = false;
	if(pDC == NULL)
	{
		pDC = m_pWnd->GetDC();
		bRelease = true;
	}

	if(m_pMemDC && m_pPage)
	{
		if(m_fMultiple == 1.0f) pDC->BitBlt(0, 0, m_pPage->width, m_pPage->height, m_pMemDC, 0, 0, SRCCOPY);
		else pDC->StretchBlt(0,0,(int)(m_pPage->width*m_fMultiple),(int)(m_pPage->height*m_fMultiple), m_pMemDC, 0, 0, m_pPage->width, m_pPage->height, SRCCOPY);
	}

	if(bRelease) m_pWnd->ReleaseDC(pDC);
	return true;
}

void CObjectMan::SetEventDraw(void)
{
	//이벤트 그리기가 아니면 무시
	if(!m_bEventDraw) return;
	//이미 그리기 예약 되어 있으면 무시
	if(m_bSendEventDraw) return;
	//이벤트 그리기 예약
	m_bSendEventDraw = true;
	//m_pWnd->Invalidate(FALSE);
}

void CObjectMan::AddEventObject(CObjectBase *pObject)
{
	m_listEventObject.push_back(pObject);
}

void CObjectMan::CallEventLogic(void)
{
	m_dwTickCount = timeGetTime();
	if(m_bFirst)
	{
		InitFirstTime();
		m_bFirst = false;

		m_FrameSkip.stTimer(FrameSkip::TIMER_RESET);
		m_FrameSkip.stReset();
	}

	CheckTimer();
	if(!m_FrameSkip.Update(m_FrameSkip.stTimer(FrameSkip::TIMER_GETELAPSEDTIME))) return;

	CheckMouseOverObject();

	MAPLAYER::iterator start = m_mapLayer.begin();
	MAPLAYER::iterator end = m_mapLayer.end();

	for(;start != end;++start)
	{
		start->second->CallLogic(m_dwTickCount);
	}
}

void CObjectMan::ClearEventDrawList(void)
{
	m_listEventObject.clear();
}

//다이알로그에서 이벤트에 의한 그리기 처리
//배경쪽만 그리는데 사용한다. 아바타, 에디트 처리는 따로 한다.
bool CObjectMan::EventBackDraw(CDC *pDC /*= NULL */)
{
	if(!m_bSendEventDraw) return false;
	m_bSendEventDraw = false;

	ClearEventDrawList();

	bool bRelease = false;
	if(pDC == NULL)
	{
		pDC = m_pWnd->GetDC();
		bRelease = true;
	}

	if(m_pBackDC == NULL)
	{
		m_pBackDC = new CDC;
		m_pBackDC->CreateCompatibleDC(pDC);
		m_pBackDC->SelectObject(m_pBackPage->hBmp);
	}

	if(m_pMemDC == NULL)
	{
		m_pMemDC = new CDC;
		m_pMemDC->CreateCompatibleDC(pDC);
		m_pMemDC->SelectObject(m_pPage->hBmp);
	}

	MAPLAYER::iterator start = m_mapLayer.begin();
	MAPLAYER::iterator end = m_mapLayer.end();

	for(;start != end;++start)
	{
		start->second->CallDraw(m_pBackPage);
	}

	if(bRelease) m_pWnd->ReleaseDC(pDC);

	EventBackToFrontBitBlt();

	LISTOBJECTITER start_object = m_listEventObject.begin();
	LISTOBJECTITER end_object = m_listEventObject.end();

	CObjectBase *pObject = NULL;

	for(;start_object != end_object;++start_object)
	{
		pObject = (*start_object);
		if(pObject->IsShow() && pObject->IsShowRef()) pObject->CObjectBase::DrawLocal(m_pPage);
		else pObject->ClearSendEventDraw();
	}

	return true;
}

bool CObjectMan::EventDraw(CObjectBase *pObject/* = NULL*/, CDC *pDC/* = NULL*/)
{
	bool bRelease = false;
	if(pDC == NULL)
	{
		pDC = m_pWnd->GetDC();
		bRelease = true;
	}

	if(m_pMemDC == NULL)
	{
		m_pMemDC = new CDC;
		m_pMemDC->CreateCompatibleDC(pDC);
		m_pMemDC->SelectObject(m_pPage->hBmp);
	}

	if(pObject)
	{
		CRect &rtRect = pObject->GetRect();
		m_pMemDC->BitBlt((int)rtRect.left, (int)rtRect.top, (int)rtRect.GetWidth(), (int)rtRect.GetHeight(), m_pBackDC, (int)rtRect.left, (int)rtRect.top, SRCCOPY);

		CObjectBase *pParent = pObject->GetParent();
		pParent->Draw(m_pPage);
		pObject->DrawLocal(m_pPage);
	}
	else
	{
		m_pMemDC->BitBlt(0, 0, m_pPage->width, m_pPage->height, m_pBackDC, 0, 0, SRCCOPY);
	}

	if(bRelease) m_pWnd->ReleaseDC(pDC);
	return true;
}

bool CObjectMan::EventBackToFrontBitBlt(CRect *pRect/* = NULL*/)
{
	if(m_pMemDC == NULL)
	{
		CDC *pDC = m_pWnd->GetDC();
		m_pMemDC = new CDC;
		m_pMemDC->CreateCompatibleDC(pDC);
		m_pMemDC->SelectObject(m_pPage->hBmp);

		m_pWnd->ReleaseDC(pDC);
	}

	if(pRect)
	{
		m_pMemDC->BitBlt((int)pRect->left, (int)pRect->top, (int)pRect->GetWidth(), (int)pRect->GetHeight(), m_pBackDC, (int)pRect->left, (int)pRect->top, SRCCOPY);
	}
	else
	{
		m_pMemDC->BitBlt(0, 0, m_pPage->width, m_pPage->height, m_pBackDC, 0, 0, SRCCOPY);
	}
	return true;
}

bool CObjectMan::EventFrontBitBlt(CRect *pRect, CDC *pDC, CPoint *pPtSorce)
{
	m_pMemDC->BitBlt((int)pRect->left, (int)pRect->top, (int)pRect->GetWidth(), (int)pRect->GetHeight(), pDC, (int)pPtSorce->x, (int)pPtSorce->y, SRCCOPY);
	return true;
}

bool CObjectMan::EventBitBlt(CRect *pRect/* = NULL*/, CDC *pDC/* = NULL*/)
{
	bool bRelease = false;
	if(pDC == NULL)
	{
		pDC = m_pWnd->GetDC();
		bRelease = true;
	}

	if(m_pMemDC && m_pPage)
	{
		if(pRect) pDC->BitBlt((int)pRect->left, (int)pRect->top, (int)pRect->GetWidth(), (int)pRect->GetHeight(), m_pMemDC, (int)pRect->left, (int)pRect->top, SRCCOPY);
		else pDC->BitBlt(0, 0, m_pPage->width, m_pPage->height, m_pMemDC, 0, 0, SRCCOPY);
	}

	if(bRelease) m_pWnd->ReleaseDC(pDC);
	return true;
}

void CObjectMan::CheckMouseOverObject(void)
{
	if(m_pSelectObject != NULL) return;
	CObjectBase *pObject = NULL;
	if(m_ptMousePos != CPoint(-10000, -10000))
	{
		pObject = IsInObject(m_ptMousePos, false);
	}
	if(pObject != m_pOverObject)
	{
		SetMouseOverObject(pObject);
	}
}

int CObjectMan::MakeProjectID(void)
{
	CThreadLock lock(&m_cLock);
	MAPPROJECT::reverse_iterator start = m_mapProject.rbegin();
	if(start == m_mapProject.rend()) return 0;
	return start->first + 1;
}

std::string	CObjectMan::MakeProjectName(void)
{
	CThreadLock lock(&m_cLock);
	MAPPROJECTNAMETOID::iterator findIt;
	CString strName;
	int i=1;
	while(1)
	{
		strName.Format("project%03d", i);
		findIt = m_mapProjectNameToID.find(strName.GetString());
		if(findIt == m_mapProjectNameToID.end()) break;
		++i;
	}

	return strName.GetString();
}

CObjectProject *CObjectMan::CreateProject(const int &nWidth, const int &nHeight)
{
	CObjectProject *pProject = new CObjectProject;
	pProject->Create(nWidth, nHeight, this);
	return pProject;
}

bool CObjectMan::AddProject(CObjectProject *pProject)
{
	CThreadLock lock(&m_cLock);
	if(pProject == NULL) return false;
	const int &nID = pProject->GetID();
	const string &strName = pProject->GetName();
	if((nID != _ID_PROJECT_OBJECTRECT && nID < 0) || strName == "") return false;
	if(m_mapProject.end() != m_mapProject.find(nID)) return false;
	if(m_mapProjectNameToID.end() != m_mapProjectNameToID.find(strName)) return false;
	m_mapProject[nID] = pProject;
	m_mapProjectNameToID[strName] = nID;
	return true;
}

bool CObjectMan::ChangeProjectID(const int &nOldID, const int &nNewID)
{
	CThreadLock lock(&m_cLock);
	MAPPROJECT::iterator findIt = m_mapProject.find(nOldID);
	if(findIt == m_mapProject.end()) return false;
	if(m_mapProject.find(nNewID) != m_mapProject.end()) return false;
	CObjectProject *pProject = findIt->second;
	MAPPROJECTNAMETOID::iterator findIt2 = m_mapProjectNameToID.find(pProject->GetName());
	if(findIt2 == m_mapProjectNameToID.end()) return false;
	m_mapProject.erase(nOldID);
	m_mapProject[nNewID] = pProject;
	findIt2->second = nNewID;
	return true;
}

bool CObjectMan::RemoveProject(const int &nID)
{
	CThreadLock lock(&m_cLock);
	MAPPROJECT::iterator findIt = m_mapProject.find(nID);
	if(findIt == m_mapProject.end()) return false;
	CObjectProject *pProject = findIt->second;
	m_mapProject.erase(nID);
	m_mapProjectNameToID.erase(pProject->GetName());
	return true;
}

bool CObjectMan::SetProjectName(const string &strOldName, const string &strNewName)
{
	CThreadLock lock(&m_cLock);
	MAPPROJECTNAMETOID::iterator findIt = m_mapProjectNameToID.find(strOldName);
	if(findIt == m_mapProjectNameToID.end()) return false;
	if(m_mapProjectNameToID.find(strNewName) != m_mapProjectNameToID.end()) return false;
	int nID = findIt->second;
	m_mapProjectNameToID.erase(strOldName);
	m_mapProjectNameToID[strNewName] = nID;
	return true;
}

bool CObjectMan::SetProject(const int &nLayer, const std::string &strName)
{
	int nID = GetProjectNameToID(strName);
	ASSERT(nID != -1);
	return SetProject(nLayer, nID);
}

bool CObjectMan::SetProject(const int &nLayer, const int &nID)
{
	CThreadLock lock(&m_cLock);

	CObjectProject *pCurProject = GetProjectFromLayer(nLayer);
	if(pCurProject)
	{
		if(pCurProject->GetID() == nID) return true;

		LISTOBJECTITER start = m_listTrackingOutsideClick.begin();
		LISTOBJECTITER end = m_listTrackingOutsideClick.end();
		while(start != end)
		{
			if((*start)->GetObjectProject() == pCurProject)
			{
				(*start)->SetTracking(false);
				start = m_listTrackingOutsideClick.erase(start);
			}
			else ++start;
		}

		if(m_pOverObject && m_pOverObject->GetObjectProject() == pCurProject) SetMouseOverObject(NULL);
		if(m_pFocusObject && m_pFocusObject->GetObjectProject() == pCurProject) SetFocus(NULL);
		if(m_pSelectObject && m_pSelectObject->GetObjectProject() == pCurProject)
		{
			m_pSelectObject->OnLButtonUP(m_ptMousePos);
			m_pSelectObject = NULL;
		}

		pCurProject->SetPopup(NULL, NULL);
	}

	ASSERT(nLayer >= 0);

	if(nID == _ID_PROJECT_NOTUSE)
	{
		MAPLAYER::iterator findIt = m_mapLayer.find(nLayer);
		if(findIt == m_mapLayer.end()) return true;

		findIt->second->SetActive(false);
		m_mapLayer.erase(nLayer);

		if(nLayer == 0)
		{
			m_pWnd->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		MAPPROJECT::iterator findIt2 = m_mapProject.find(nID);
		if(findIt2 == m_mapProject.end() || findIt2->second->IsActive()) return false;

		MAPLAYER::iterator findIt = m_mapLayer.find(nLayer);
		if(findIt != m_mapLayer.end())
		{
			if(findIt->second->GetID() == nID) 
			{
				return true;
			}
			else
			{
				findIt->second->SetActive(false);
			}
		}

		m_mapLayer[nLayer] = findIt2->second;
		findIt2->second->SetActive(true);

		if(nLayer == 0)
		{
			ReSize(findIt2->second->GetSize());
			if(m_pWnd->IsWindowVisible() == FALSE) m_pWnd->ShowWindow(SW_SHOW);
			if(m_pMemDC)
			{
				m_pMemDC->DeleteDC();
				delete m_pMemDC;
				m_pMemDC = NULL;
			}

			if(m_pBackDC)
			{
				m_pBackDC->DeleteDC();
				delete m_pBackDC;
				m_pBackDC = NULL;
			}
		}
	}

	return true;
}

void CObjectMan::ReSize(CSize szSize)
{
	if(m_pMemDC)
	{
		m_pMemDC->DeleteDC();
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if(m_pBackDC)
	{
		m_pBackDC->DeleteDC();
		delete m_pBackDC;
		m_pBackDC = NULL;
	}

	CSize szTemp = szSize;
	szTemp.cx = (int)(szTemp.cx * m_fMultiple);
	szTemp.cy = (int)(szTemp.cy * m_fMultiple);

	m_pPage->Init(szTemp.cx, szTemp.cy, 16);
	if(m_pBackPage) m_pBackPage->Init(szTemp.cx, szTemp.cy, 16);
	m_pWnd->SetWindowPos(NULL, 0, 0, szTemp.cx, szTemp.cy, SWP_NOMOVE | SWP_NOZORDER);
	m_szSize = szSize;
}

void CObjectMan::SetMultiple(const float &fMultiple)
{
	if(m_fMultiple == fMultiple) return;
	m_fMultiple = fMultiple;
	ReSize(m_szSize);
	
	MAPLAYER::iterator start = m_mapLayer.begin();
	MAPLAYER::iterator end = m_mapLayer.end();

	for(;start != end;++start)
	{
		start->second->GetMainObject()->SetChange(true);
	}
}

void CObjectMan::SetTracking(CObjectBase *pObject)
{
	LISTOBJECTITER start = m_listTrackingOutsideClick.begin();
	LISTOBJECTITER end = m_listTrackingOutsideClick.end();
	LISTOBJECTITER findIt = find(start, end, pObject);
	if(findIt != end) return;
	m_listTrackingOutsideClick.push_back(pObject);
}

void CObjectMan::RemoveTracking(CObjectBase *pObject)
{
	LISTOBJECTITER start = m_listTrackingOutsideClick.begin();
	LISTOBJECTITER end = m_listTrackingOutsideClick.end();
	LISTOBJECTITER findIt = find(start, end, pObject);
	if(findIt == end) return;
	m_listTrackingOutsideClick.erase(start);
}

CObjectProject *CObjectMan::GetProjectFromLayer(const int &nLayer)
{
	CThreadLock lock(&m_cLock);

	ASSERT(nLayer >= 0);
	MAPLAYER::iterator findIt = m_mapLayer.find(nLayer);
	if(findIt == m_mapLayer.end()) return NULL;
	return findIt->second;
}

CObjectProject *CObjectMan::GetProjectFromID(const int &nID)
{
	CThreadLock lock(&m_cLock);
	MAPPROJECT::iterator findIt = m_mapProject.find(nID);
	if(findIt == m_mapProject.end()) return NULL;
	return findIt->second;
}

CObjectProject *CObjectMan::GetProjectFromName(const std::string &strName)
{
	int nID = GetProjectNameToID(strName);
	if(nID == -1) return NULL;
	return GetProjectFromID(nID);
}

int CObjectMan::GetProjectNameToID(const std::string &strName)
{
	CThreadLock lock(&m_cLock);
	MAPPROJECTNAMETOID::iterator findIt = m_mapProjectNameToID.find(strName);
	if(findIt == m_mapProjectNameToID.end()) return -1;
	return findIt->second;
}

void CObjectMan::SetUseCustomCursor(bool bUse)
{
	m_bUseCustomCursor = bUse;
	if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);
}

void CObjectMan::SetCursorNormal(HCURSOR hCursor)
{
	m_hCursorNormal = hCursor;
	if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);
}

void CObjectMan::SetCursorDown(HCURSOR hCursor)
{
	m_hCursorDown = hCursor;
	if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);
}

void CObjectMan::SetCursorOver(HCURSOR hCursor)
{
	m_hCursorOver = hCursor;
	if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);
}

void CObjectMan::SetCursorEdit(HCURSOR hCursor)
{
	m_hCursorEdit = hCursor;
	if(m_pWnd) m_pWnd->PostMessage(WM_SETCURSOR);
}

void CObjectMan::SetCursorFunctor(IFunctor* pFunctor)
{
	m_cMessageManager.RemoveMsgH(EM_SETCURSOR);
	m_cMessageManager.AddMsgH(EM_SETCURSOR, pFunctor);
}

void CObjectMan::SetObserverMouseDragBeginFuntor(IFunctor* pFunctor)
{
	m_cMessageManager.RemoveMsgH(EM_DRAG_BEGIN);
	m_cMessageManager.AddMsgH(EM_DRAG_BEGIN, pFunctor);
}

void CObjectMan::SetObserverMouseDragingFuntor(IFunctor* pFunctor)
{
	m_cMessageManager.RemoveMsgH(EM_DRAGING);
	m_cMessageManager.AddMsgH(EM_DRAGING, pFunctor);
}

void CObjectMan::SetObserverMouseDragEndFuntor(IFunctor* pFunctor)
{
	m_cMessageManager.RemoveMsgH(EM_DRAG_END);
	m_cMessageManager.AddMsgH(EM_DRAG_END, pFunctor);
}

void CObjectMan::SetMouseLeaveFunctor(IFunctor* pFunctor)
{
	m_cMessageManager.RemoveMsgH(EM_WINDOW_MOUSELEAVE);
	m_cMessageManager.AddMsgH(EM_WINDOW_MOUSELEAVE, pFunctor);
}

void CObjectMan::SetMouseOverFunctor(IFunctor* pFunctor)
{
	m_cMessageManager.RemoveMsgH(EM_WINDOW_MOUSEOVER);
	m_cMessageManager.AddMsgH(EM_WINDOW_MOUSEOVER, pFunctor);
}

bool CObjectMan::LoadXml(std::string strFileName)
{
	TiXmlNode* node = 0;

	TiXmlDocument doc(strFileName.c_str());
	if(doc.LoadFile() == false) return false;

	TiXmlElement *pElement = doc.FirstChildElement("LSYToolProject");
	if(pElement == NULL) return false;

	double dbVersion = double(0);
	pElement->Attribute("version", &dbVersion);

	node = doc.FirstChild( "Project" );
	if(node == NULL) return false;

	CObjectProject *pProject = NULL;

	pElement = node->FirstChildElement("UI_Scene");

	TiXmlElement *pElementScene = pElement->FirstChildElement("Scene");
	while(pElementScene)
	{
		pProject = new CObjectProject;
		pProject->LoadSceneXml(pElementScene, this);

		pElementScene = pElementScene->NextSiblingElement("Scene");
	}

	doc.Clear();
	m_strFileName = strFileName;
	return true;
}

void CObjectMan::SetMouseCapture(void)
{
	if(m_bMouseCapture) return;
	m_bMouseCapture = true;
	m_pWnd->SetCapture();
}

void CObjectMan::ReleaseMouseCapture(void)
{
	if(!m_bMouseCapture) return;
	m_bMouseCapture = false;
	if(GetCapture() == m_pWnd->GetSafeHwnd()) ReleaseCapture();
}

static bool st_bTool = false;

const bool &IsTool(void)
{
	return st_bTool;
}

void SetTool(const bool &bTool)
{
	st_bTool = bTool;
}

CObjectBase *CreateNewObject(ENUM_OBJECTTYPE nType)
{
	CObjectBase *pRet = NULL;
	switch(nType)
	{
	case EOT_WINDOWS:			pRet = new CWindows;		break;
	case EOT_GROUP:				pRet = new CGroup;			break;
	case EOT_IMAGE:				pRet = new CImage;			break;
	case EOT_DUMYIMAGE:			pRet = new CDumyImage;		break;
	case EOT_BUTTON:			pRet = new CButton;			break;
	case EOT_CHECKBUTTON:		pRet = new CCheckButton;	break;
	case EOT_CHECKBOX:			pRet = new CCheckBox;		break;
	case EOT_RADIO:				pRet = new CRadio;			break;
	case EOT_TEXT:				pRet = new CText;			break;
	case EOT_TAB:				pRet = new CTabControl;		break;
	case EOT_SLIDEBAR:			pRet = new CSlideBar;		break;
	case EOT_PROGRESSIVEBAR:	pRet = new CProgressiveBar;	break;
	case EOT_SCROLLBAR:			pRet = new CScrollBar;		break;
	case EOT_EDIT:				pRet = new CEdit;			break;
	case EOT_EDITCURSOR:		pRet = new CEditCursor;		break;
	case EOT_RICHTEXT:			pRet = new CRichText;		break;
	case EOT_TEXTLIST:			pRet = new CTextList;		break;
	case EOT_COMBO:				pRet = new CCombo;			break;
	case EOT_LISTITEM:			pRet = new CListItem;		break;
	case EOT_LIST:				pRet = new CList;			break;
	case EOT_SPRITE:			pRet = new CSprite;			break;
	case EOT_SPRITESCENE:		pRet = new CSpriteScene;	break;
	case EOT_WEB:				pRet = new CWeb;			break;
	case EOT_AVATAR:			pRet = new CAvatar;			break;
	case EOT_SPRITEBUTTON:		pRet = new CSpriteButton;	break;
	case EOT_SPRITECHECKBUTTON:	pRet = new CSpriteCheckButton;	break;
	case EOT_IMAGENUMBER:		pRet = new CImageNumber;	break;
	case EOT_SPRITEWINDOWS:		pRet = new CSpriteWindows;	break;
	case EOT_TEXTNUMBER:		pRet = new CTextNumber;		break;
	case EOT_TEXTBUTTON:		pRet = new CTextButton;		break;
	}

	if(pRet && IsTool())
	{
		pRet->SetUserData(LSY::OUDT_MESSAGE, pRet->IsMessage()?1:0);
		pRet->SetUserData(LSY::OUDT_ENABLE, pRet->IsEnable()?1:0);
		pRet->SetUserData(LSY::OUDT_USEMOUSEWHEEL, pRet->IsUseMouseWheel()?1:0);
		pRet->SetUserData(LSY::OUDT_USELOGIC, pRet->IsUseLogic()?1:0);
	}

	return pRet;
}

BOOL IsExistFile(LPCTSTR szFileName) 
{
	if(szFileName == NULL) return FALSE;
	DWORD dwAttributes = ::GetFileAttributes(szFileName);
	return (dwAttributes != ((DWORD)-1)) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

BOOL IsExistFile(std::string strFileName)
{
	return IsExistFile(strFileName.c_str());
}

void InitPlatform(CWnd *pWnd)
{
	NMBASE::UTIL::NMInitSSOWebBrowser(pWnd->GetSafeHwnd(),"","","");
	NMBASE::AVATAR::InitAvatarModule();
}

void ReleasePlatform(void)
{
	NMBASE::UTIL::NMUnInitSSOWebBrowser();
}
} //namespace LSY