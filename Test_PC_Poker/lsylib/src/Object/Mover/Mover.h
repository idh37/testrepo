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
	//���� ������ ������ ��ġ�� Parent�� ���������� ����
	//�̰��� ���õǾ� ���� ������ Parent�� m_ptMove���� ����� ���� �׷��� ��
	//���õǾ� �ִٸ� Parent���� m_ptLocalPos�� ������ �����ϸ� m_ptMove�� CPointZero�� ���õ�
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