#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CDumyImage;
class UPG_LSYLIB_EXPORT CCreateRect :public CObjectBase
{
protected:
	CDumyImage			*m_pLeft;			//������
	CDumyImage			*m_pRight;			//������
	CDumyImage			*m_pTop;			//���ʼ�
	CDumyImage			*m_pBottom;			//�Ʒ��ʼ�
	CDumyImage			*m_pBack;			//������

	CPoint				m_ptStartPos;
	CPoint				m_ptEndPos;
	CRect				m_rtCreateRect;
public:
	CCreateRect(void);
	virtual ~CCreateRect();

	virtual void Create(const CPoint &ptPos);
	virtual void SetEndPos(const CPoint &ptPos);

	const CRect &GetCreateRect(void){return m_rtCreateRect;}

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_CREATERECT;}
public:
	//Object ���� ���� �Լ���
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);
};
} //namespace LSY