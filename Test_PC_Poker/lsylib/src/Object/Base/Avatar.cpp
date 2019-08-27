#include "stdafx.h"
#include "Avatar.h"
#include "DumyImage.h"
#include "../ObjectMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CAvatar::CAvatar(void)
:	m_pAvatar(NULL),
	m_pImage(NULL),
	m_dwStartTime(0),
	m_dwCurTime(0)
{
	ZeroMemory(m_AvatarInfo, sizeof(m_AvatarInfo));
	SetUseLogic(true);
}

CAvatar::~CAvatar()
{
	if(m_pAvatar)
	{
		m_pAvatar->Destroy();
		delete m_pAvatar;
		m_pAvatar = NULL;
	}

	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

bool CAvatar::Create(int nWidth, int nHeight)
{
	if(m_pAvatar == NULL)
	{
		m_pAvatar = new NMBASE::AVATAR::CAvatar;
	}

	if(!m_pAvatar->Create(nWidth, nHeight, (int*)NULL))
	{
		m_pAvatar->Destroy();
		m_pAvatar = NULL;
		return false;
	}

	m_pAvatar->SetAvatar(m_AvatarInfo);
	m_pAvatar->SetAnimationTime(100);

	if(m_pImage == NULL)
	{
		m_pImage = new CDumyImage;
	}

	if(!m_pImage->Create(nWidth, nHeight))
	{
		delete m_pImage;
		m_pImage = NULL;
		return false;
	}

	m_pImage->ClearColor(RGB(0,0,255));
	m_pImage->SetTempEventDraw(m_bEventDraw);

	AddLocalChild(m_pImage);

	SetLocalRect(CRect(0,0,(float)nWidth,(float)nHeight));

	return true;
}

bool CAvatar::SetAvatar(short *pAvatarInfo)
{
	if(m_AvatarInfo == (int*)pAvatarInfo) return false;
	if(NULL == pAvatarInfo) return false;
	bool bRet = false;
	//memcpy(m_AvatarInfo, pAvatarInfo, sizeof(short) * MAX_AVATAR_INFO);

	for(int i = 0; i < MAX_AVATAR_INFO; i++)
	{
		m_AvatarInfo[i] = (int)pAvatarInfo[i];
	}

	if(m_pAvatar && m_pAvatar->SetAvatar(pAvatarInfo))
	{
		bRet = true;
	}

	return bRet;
}

bool CAvatar::SetAvatar(int *pAvatarInfo)
{
	if(m_AvatarInfo == pAvatarInfo) return false;
	if(NULL == pAvatarInfo) return false;
	bool bRet = false;
	memcpy(m_AvatarInfo, pAvatarInfo, sizeof(int) * MAX_AVATAR_INFO);
	if(m_pAvatar && m_pAvatar->SetAvatar(pAvatarInfo))
	{
		bRet = true;
	}

	return bRet;
}

CObjectBase *CAvatar::NewObject(void)
{
	return new CAvatar;
}

CObjectBase *CAvatar::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CAvatar *pAvatar= (CAvatar *)pObject;
	if(m_pImage) pAvatar->Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	pAvatar->SetAvatar(m_AvatarInfo);

	return pObject;
}

size_t CAvatar::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(int) * MAX_AVATAR_INFO;
	return nRet;
}

size_t CAvatar::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, m_AvatarInfo, sizeof(int) * MAX_AVATAR_INFO);
	nRet += sizeof(int) * MAX_AVATAR_INFO;
	return nRet;
}

size_t CAvatar::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(m_AvatarInfo, pDest+nRet, sizeof(int) * MAX_AVATAR_INFO);
	nRet += sizeof(int) * MAX_AVATAR_INFO;

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	SetAvatar(m_AvatarInfo);

	if(IsTool()) SetMessage(true);

	return nRet;
}

bool CAvatar::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	if(IsTool())
	{
		ZeroMemory(m_AvatarInfo, sizeof(int)*MAX_AVATAR_INFO);
		m_AvatarInfo[5] = 2;
		SetAvatar(m_AvatarInfo);

		SetMessage(true);
	}

	if(nVersion <= 2)
	{
		SetUseLogic(true);
	}

	return true;
}

void CAvatar::Logic(const DWORD &dwTime)
{
	if(m_pAvatar && m_pImage)
	{
		bool bDraw = false;
		m_dwCurTime = m_pProject->GetTickCount();
		if(m_dwCurTime - m_dwStartTime >= 100)
		{
			m_dwStartTime += 100;
			bDraw = true;
		}

		if(bDraw)
		{
			NMBASE::GRAPHICGDI::CPage *pPage = m_pImage->GetPage();
			m_pImage->ClearColor(RGB(0,0,255));
			m_pAvatar->Draw(0,0,(LONG)GetWidth(),(LONG)GetHeight(),(WORD*)pPage->lpBit, pPage->lPitch,FALSE);

			m_pImage->CheckEventObjectDraw();
		}
	}
}

void CAvatar::SetLogicStartTime(const DWORD &dwStartTime)
{
	__super::SetLogicStartTime(dwStartTime);

	m_dwStartTime = 0;
	m_dwCurTime = 0;
}

void CAvatar::SetObjectProject(CObjectProject *pProject)
{
	__super::SetObjectProject(pProject);
	if(m_pImage) m_pImage->SetTempEventDraw(m_bEventDraw);
}
} //namespace LSY