#pragma once

#include "../Base/IObject.h"
#include "../../Utility/Point.h"

namespace LSY
{
class IObject;
class CObjectBase;
class UPG_LSYLIB_EXPORT CMover : public IObject
{
protected:
	bool		m_bAnimation;
	bool		m_bFirst;
	
	DWORD		m_dwCurTime;

	DWORD		m_dwDelayTime;
	DWORD		m_dwMoveTime;

	LPARAM		m_lParam;

	CPoint		m_ptStart;
	CPoint		m_ptEnd;
	CPoint		m_ptCur;

	CObjectBase	*m_pParent;
	//종료 시점에 마지막 위치로 Parent를 설정할지의 여부
	//이값이 셋팅되어 있지 않으면 Parent의 m_ptMove값이 변경된 상태 그래로 임
	//셋팅되어 있다면 Parent값의 m_ptLocalPos를 강제로 변경하며 m_ptMove는 CPointZero로 셋팅됨
	bool		m_bSetEndPos;
	bool		m_bAutoDelete;
public:
	CMover();
	virtual ~CMover();

	virtual void Create(const CPoint &ptPos, const CPoint &ptEnd, const DWORD &dwMoveTime, const DWORD &dwDelayTime = 0, const bool &bSetEndPos = false, const bool &bAutoDelete = true);

	virtual void Logic(const DWORD &dwTime);
	virtual void LogicMover(float &fDelta);

	virtual void SetParent(CObjectBase *pObjectBase);
	virtual void Start(void);
	virtual void End(bool bSetCurPos, bool bSetForceEnd = false);

	virtual const CPoint &GetStartPos(void){return m_ptStart;}
	virtual const CPoint &GetEndPos(void){return m_ptEnd;}
	virtual const CPoint &GetCurPos(void){return m_ptCur;}
	virtual CObjectBase *GetParent(void){return m_pParent;}

	virtual const bool &IsAnimation(void){return m_bAnimation;}
	virtual const bool &IsSetEndPos(void){return m_bSetEndPos;}
	virtual const bool &IsAutoDelete(void){return m_bAutoDelete;}
};

class UPG_LSYLIB_EXPORT CUniformMotionMover : public CMover
{
protected:
	CPoint	m_ptGab;
public:
	CUniformMotionMover();
	virtual ~CUniformMotionMover();

	virtual void Create(const CPoint &ptPos, const CPoint &ptEnd, const DWORD &dwMoveTime, const DWORD &dwDelayTime = 0, const bool &bSetEndPos = false, const bool &bAutoDelete = true);

	virtual void LogicMover(float &fDelta);
};
} //namespace LSY