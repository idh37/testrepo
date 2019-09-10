#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CDumyImage;
class UPG_LSYLIB_EXPORT CObjectRect :public CObjectBase
{
protected:
	CDumyImage			*m_pLeft;			//������
	CDumyImage			*m_pRight;			//������
	CDumyImage			*m_pTop;			//���ʼ�
	CDumyImage			*m_pBottom;			//�Ʒ��ʼ�
	CDumyImage			*m_pLeftTop;		//������
	CDumyImage			*m_pRightTop;		//������
	CDumyImage			*m_pLeftBottom;		//�����Ʒ�
	CDumyImage			*m_pRightBottom;	//�����Ʒ�

	CObjectBase			*m_pObserver;		//������ Object
	bool				m_bObserverChange;
public:
	CObjectRect(void);
	virtual ~CObjectRect();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_OBJECTRECT;}

	bool Create(CObjectBase *pObject);
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	void SetOserverChange(const bool &bChange);

	CObjectBase	*GetObserver(void){return m_pObserver;}
public:
	//Object ���� ���� �Լ���
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

protected:
	void CheckObserver(void);
	void Release(void);
};
} //namespace LSY