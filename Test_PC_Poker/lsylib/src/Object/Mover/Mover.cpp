#include "stdafx.h"
#include "Mover.h"
#include "../Base/ObjectBase.h"
#include "../ObjectMan.h"
#include "../../Functor/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CMover::CMover()
:	m_bAnimation(false),
	m_dwCurTime(0),
	m_dwMoveTime(0),
	m_dwDelayTime(0),
	m_ptStart(CPointZero),
	m_ptEnd(CPointZero),
	m_ptCur(CPointZero),
	m_pParent(NULL),
	m_bSetEndPos(false),
	m_bFirst(true),
	m_bAutoDelete(true)
{
}

CMover::~CMover()
{
	if(m_bAnimation)
	{
		CObjectProject *pProject = m_pParent->GetObjectProject();
		if(pProject)
		{
			pProject->SubLogicObject(this);
		}
		m_bAnimation = false;
	}
}

void CMover::Create(const CPoint &ptPos, const CPoint &ptEnd, const DWORD &dwMoveTime, const DWORD &dwDelayTime/* = 0*/, const bool &bSetEndPos /* = false */, const bool &bAutoDelete /*=true*/)
{
	m_ptStart		= ptPos;
	m_ptEnd			= ptEnd;
	m_dwMoveTime	= dwMoveTime;
	m_dwDelayTime	= dwDelayTime;
	m_bSetEndPos	= bSetEndPos;
	m_bAutoDelete	= bAutoDelete;
}

void CMover::SetParent(CObjectBase *pObjectBase)
{
	m_pParent = pObjectBase;
}

void CMover::Start(void)
{
	ASSERT(m_pParent != NULL);

	CObjectProject *pProject = m_pParent->GetObjectProject();
	if(pProject)
	{
		pProject->AddLogicObject(this);
		m_bAnimation = true;
	}
}

void CMover::End(bool bSetCurPos, bool bSetForceEnd/* = false*/)
{
	if(m_pParent == NULL || m_bAnimation == false) return;

	if(bSetForceEnd)
		m_ptCur = m_ptEnd;

	CObjectProject *pProject = m_pParent->GetObjectProject();
	if(pProject)
	{
		pProject->SubLogicObject(this);
	}

	if(bSetCurPos)
	{
		//마지막 위치를 셋팅한다.
		m_pParent->SetLocalPos(m_pParent->GetLocalPos() + m_ptCur);
		m_pParent->SetMove(CPointZero);
	}

	m_bAnimation = false;
}

void CMover::Logic(const DWORD &dwTime)
{
	//대기 시간 보다 작으면 리턴한다.
	if(dwTime < m_dwDelayTime)
	{
		return;
	}

	DWORD dwDelta = dwTime - m_dwDelayTime;
	if(m_bFirst)
	{
		m_bFirst = false;
		//시작 시점을 알려줌
		CMO_MoveStart msg(this);
		m_cMessage.Call(&msg);
	}

	if(dwDelta >= m_dwMoveTime)
	{
		m_ptCur = m_ptEnd;
		m_bAnimation = false;

		CObjectProject *pProject = m_pParent->GetObjectProject();
		if(pProject)
		{
			pProject->SubLogicObject(this);
		}

		if(m_bSetEndPos)
		{
			m_pParent->SetLocalPos(m_pParent->GetLocalPos() + m_ptCur);
			m_pParent->SetMove(CPointZero);
		}
		else
		{
			m_pParent->SetMove(m_ptCur);
		}

		if(m_bAutoDelete)
		{
			m_pParent->SetMover(NULL);
		}

		//종료 시점을 알려줌
		CMO_MoveEnd msg(this);
		m_cMessage.Call(&msg);

		if(m_bAutoDelete)
		{
			delete this;
		}
	}
	else
	{
		float fDelta = (float)dwDelta/(float)m_dwMoveTime;
		LogicMover(fDelta);
		m_pParent->SetMove(m_ptCur);
	}
}

void CMover::LogicMover(float &fDelta)
{
}

CUniformMotionMover::CUniformMotionMover()
{
}

CUniformMotionMover::~CUniformMotionMover()
{
}

void CUniformMotionMover::Create(const CPoint &ptPos, const CPoint &ptEnd, const DWORD &dwMoveTime, const DWORD &dwDelayTime/* = 0*/, const bool &bSetEndPos/* = false*/, const bool &bAutoDelete/* = true*/)
{
	CMover::Create(ptPos, ptEnd, dwMoveTime, dwDelayTime, bSetEndPos, bAutoDelete);
	m_ptGab = ptEnd - ptPos;
}

void CUniformMotionMover::LogicMover(float &fDelta)
{
	m_ptCur = m_ptStart + m_ptGab * fDelta;
}
} //namespace LSY