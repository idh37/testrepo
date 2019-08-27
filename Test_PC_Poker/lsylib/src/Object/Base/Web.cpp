#include "stdafx.h"
#include "Web.h"
#include "../ObjectMan.h"
#include "./DumyImage.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_nID = 32664;

namespace LSY
{
CWeb::CWeb(void)
:	m_pWeb(NULL),
	m_pRectImage(NULL),
	m_pBeforeNavigateCallback(NULL),
	m_pNavigateCompleteCallback(NULL),
	m_pOnQuitCallback(NULL)
{
	if(IsTool())
	{
		m_pRectImage = new CDumyImage;
		m_pRectImage->SetLocalAlpha(0.5f);
		AddLocalChild(m_pRectImage);
	}
}

CWeb::~CWeb()
{
	if(m_pWeb)
	{
		m_pWeb->Destroy();
		delete m_pWeb;
		m_pWeb = NULL;
	}
}

void CWeb::Create(int nWidth, int nHeight)
{
	float fWidth = (float)nWidth;
	float fHeight = (float)nHeight;
	CRect rtRect(0,0,fWidth,fHeight);
	SetLocalRect(rtRect);
}

CObjectBase *CWeb::NewObject(void)
{
	return new CWeb;
}

BOOL CWeb::SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback)
{
	if(m_pBeforeNavigateCallback == pCallback) return FALSE;
	m_pBeforeNavigateCallback = pCallback;
	if(m_pWeb) return m_pWeb->SetCallback_OnBeforeNavigate2(pCallback);
	return TRUE;
}

BOOL CWeb::SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback)
{
	if(m_pNavigateCompleteCallback == pCallback) return FALSE;
	m_pNavigateCompleteCallback = pCallback;
	if(m_pWeb) return m_pWeb->SetCallback_OnNavigateComplete2(pCallback);
	return TRUE;
}

BOOL CWeb::SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback)
{
	if(m_pOnQuitCallback == pCallback) return FALSE;
	m_pOnQuitCallback = pCallback;
	if(m_pWeb) return m_pWeb->SetCallback_OnQuit(pCallback);
	return TRUE;
}

CObjectBase *CWeb::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CWeb *pImage = (CWeb *)pObject;
	return pObject;
}

bool CWeb::CheckChange(void)
{
	bool bRet = __super::CheckChange();

	if(GetLocalRect() == CRectZero)
	{
		if(m_pWeb) m_pWeb->ShowWindow(SW_HIDE);
	}
	else
	{
		if(m_pWeb == NULL)
		{
			CObjectMan *pManager = m_pProject?m_pProject->GetObjectMan():NULL;
			if(pManager)
			{
				CWnd *pWnd = pManager->GetWnd();
				if(pWnd)
				{
					pWnd->EnableAutomation();
					m_pWeb = new NMBASE::UTIL::CNMSSOWebBrowser();

					BOOL bRet = FALSE;
					::CRect rtTemp;

					if(m_pProject)
					{
						rtTemp.left		= (int)(m_ptPos.x * m_pProject->GetObjectMan()->GetMultiple());
						rtTemp.top		= (int)(m_ptPos.y * m_pProject->GetObjectMan()->GetMultiple());
						rtTemp.right	= rtTemp.left + (int)(GetWidth() * m_pProject->GetObjectMan()->GetMultiple());
						rtTemp.bottom	= rtTemp.top + (int)(GetHeight() * m_pProject->GetObjectMan()->GetMultiple());
					}
					else
					{
						rtTemp.left		= (int)m_ptPos.x;
						rtTemp.top		= (int)m_ptPos.y;
						rtTemp.right	= rtTemp.left + (int)GetWidth();
						rtTemp.bottom	= rtTemp.top + (int)GetHeight();
					}

					bRet = m_pWeb->Create( WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rtTemp,  pWnd->GetSafeHwnd(), g_nID, pWnd->GetIDispatch(FALSE));

					++g_nID;
					if(bRet)
					{
						if(m_pBeforeNavigateCallback) m_pWeb->SetCallback_OnBeforeNavigate2(m_pBeforeNavigateCallback);
						if(m_pNavigateCompleteCallback) m_pWeb->SetCallback_OnNavigateComplete2(m_pNavigateCompleteCallback);
						if(m_pOnQuitCallback) m_pWeb->SetCallback_OnQuit(m_pOnQuitCallback);

						if(m_strURL == "") m_pWeb->Navigate("about:blank");
						else m_pWeb->Navigate(m_strURL.c_str());
						m_pWeb->SetHideBorder( TRUE );
					}
					else
					{
						NMSSOWEB_RETCODE nError = NMBASE::UTIL::NMSSOWebLib_GetLastError();
						m_pWeb->Destroy();
						delete m_pWeb;
						m_pWeb = NULL;
					}
				}
			}
		}
		else
		{
			RECT rtTemp;

			if(m_pProject)
			{
				rtTemp.left		= (int)(m_ptPos.x * m_pProject->GetObjectMan()->GetMultiple());
				rtTemp.top		= (int)(m_ptPos.y * m_pProject->GetObjectMan()->GetMultiple());
				rtTemp.right	= rtTemp.left + (int)(GetWidth() * m_pProject->GetObjectMan()->GetMultiple());
				rtTemp.bottom	= rtTemp.top + (int)(GetHeight() * m_pProject->GetObjectMan()->GetMultiple());
			}
			else
			{
				rtTemp.left		= (int)m_ptPos.x;
				rtTemp.top		= (int)m_ptPos.y;
				rtTemp.right	= rtTemp.left + (int)GetWidth();
				rtTemp.bottom	= rtTemp.top + (int)GetHeight();
			}

			m_pWeb->MoveWindow(&rtTemp, FALSE);
		}

		CheckShowWebControl(m_pProject);
	}
	return bRet;
}

void CWeb::SetURL(std::string strURL, bool bRefresh/*=false*/)
{
	if(bRefresh == false && strURL == m_strURL) return;
	m_strURL = strURL;
	if(m_pWeb) m_pWeb->Navigate(m_strURL.c_str());
}

void CWeb::SetShow(const bool &bShow)
{
	CObjectBase::SetShow(bShow);
	CheckShowWebControl(m_pProject);
}

void CWeb::SetShowRef(const bool &bShowRef)
{
	CObjectBase::SetShowRef(bShowRef);
	CheckShowWebControl(m_pProject);
}

void CWeb::SetFixed(const bool &bFixed)
{
	CObjectBase::SetFixed(bFixed);
	CheckShowWebControl(m_pProject);

	if(m_pRectImage)
	{
		if(m_bFixed) m_pRectImage->SetShow(false);
		else m_pRectImage->SetShow(true);
	}
}

void CWeb::OnAddSubRootObject(bool bAdd)
{
	CObjectBase::OnAddSubRootObject(bAdd);
	CheckShowWebControl(bAdd?m_pProject:NULL);
}

void CWeb::OnProjectActiveChange(bool bActive)
{
	CObjectBase::OnProjectActiveChange(bActive);
	CheckShowWebControl(bActive?m_pProject:NULL);
}

BOOL CWeb::ExecJavascript(LPCSTR strScript)
{
	if(m_pWeb == NULL)
		return FALSE;

	return m_pWeb->ExecJavascript(strScript);
}

void CWeb::ResetUserNavigatedCount()
{
	if(m_pWeb == NULL)
		return;

	m_pWeb->ResetUserNavigatedCount();
}

int	CWeb::GetUserNavigatedCount()
{
	if(m_pWeb == NULL)
		return 0;

	return m_pWeb->GetUserNavigatedCount();
}

void CWeb::CheckShowWebControl(CObjectProject *pProject)
{
	bool bShow = true;
	//툴일경우
	if(m_pRectImage)
	{
		//다음 조건이면 웹컨트롤을 숨긴다.
		if(m_bShow					== false	//자신이 안보이도록 설정 되어 있다.
		|| m_bShowRef				== false	//부모가 안보이도록 설정 되어 있다.
		|| m_bFixed					== false	//고정되어 있지 않다.
		|| m_pProject				== NULL
		|| pProject					== NULL		//메인 트리에 붙어있지 않다.
		|| pProject->IsActive()		== false)	//프로젝트가 활성화 되어 있지 않다.
			bShow = false;

		if(m_bFixed) m_pRectImage->SetShow(false);
		else m_pRectImage->SetShow(true);
	}
	//툴이 아닐경우
	else
	{
		//다음 조건이면 웹컨트롤을 숨긴다.
		if(m_bShow					== false	//자신이 안보이도록 설정 되어 있다.
		|| m_bShowRef				== false	//부모가 안보이도록 설정 되어 있다.
		|| m_pProject				== NULL
		|| pProject					== NULL		//메인 트리에 붙어있지 않다.
		|| pProject->IsActive()		== false)	//프로젝트가 활성화 되어 있지 않다.
			bShow = false;
	}

	if(m_pWeb)
	{
		if(bShow)
		{
			m_pWeb->ShowWindow(TRUE);
		}
		else
		{
			m_pWeb->ShowWindow(FALSE);
		}
	}
	else if(bShow) SetChange(true);

}

void CWeb::SetLocalRect(const CRect &rtRect)
{
	if(rtRect == m_rtLocalRect) return;
	if(IsTool())
	{
		CRect rtTemp = rtRect;
		if(rtTemp.GetWidth() == 0.0f || rtTemp.GetHeight() == 0.0f)
		{
			m_pRectImage->SetShow(false);
		}
		else
		{
			m_pRectImage->Create((int)rtTemp.GetWidth(), (int)rtTemp.GetHeight());
			m_pRectImage->ClearColor(RGB(255, 168, 192));
			m_pRectImage->SetShow(true);
		}

		if(m_bFixed) m_pRectImage->SetShow(false);
		else m_pRectImage->SetShow(true);
	}
	CObjectBase::SetLocalRect(rtRect);
}

size_t CWeb::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);
	nRet += m_strURL.size();
	return nRet;
}

size_t CWeb::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = m_strURL.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);
	if(nSize)
	{
		memcpy(pDest+nRet, m_strURL.c_str(), nSize);
		nRet += nSize;
	}
	return nRet;
}

size_t CWeb::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, m_strURL);
		nRet += nSize;
	}

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	if(m_pWeb)
	{
		if(m_strURL == "") m_pWeb->Navigate("about:blank");
		else m_pWeb->Navigate(m_strURL.c_str());
	}

	if(IsTool())
	{
		CRect rtTemp = m_rtLocalRect;
		if(rtTemp.GetWidth() == 0.0f || rtTemp.GetHeight() == 0.0f)
		{
			m_pRectImage->SetShow(false);
		}
		else
		{
			m_pRectImage->Create((int)rtTemp.GetWidth(), (int)rtTemp.GetHeight());
			m_pRectImage->ClearColor(RGB(255, 168, 192));
			m_pRectImage->SetShow(true);
		}

		if(m_bFixed) m_pRectImage->SetShow(false);
		else m_pRectImage->SetShow(true);

		SetMessage(true);
	}

	return nRet;
}

bool CWeb::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("url", m_strURL.c_str());
	pXmlObject->InsertEndChild(xmlInheritance);
	return true;
}

bool CWeb::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *pXmlFile = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlFile, "url", m_strURL);

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	if(m_pWeb)
	{
		if(m_strURL == "") m_pWeb->Navigate("about:blank");
		else m_pWeb->Navigate(m_strURL.c_str());
	}

	if(IsTool())
	{
		CRect rtTemp = m_rtLocalRect;
		if(rtTemp.GetWidth() == 0.0f || rtTemp.GetHeight() == 0.0f)
		{
			m_pRectImage->SetShow(false);
		}
		else
		{
			m_pRectImage->Create((int)rtTemp.GetWidth(), (int)rtTemp.GetHeight());
			m_pRectImage->ClearColor(RGB(255, 168, 192));
			m_pRectImage->SetShow(true);
		}

		if(m_bFixed) m_pRectImage->SetShow(false);
		else m_pRectImage->SetShow(true);

		SetMessage(true);
	}

	return true;
}

} //namespace LSY