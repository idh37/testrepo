#pragma once

#include "../AnimationDefine.h"	//UPG�۾� �߰�

namespace effect
{

//������ �˷��ִ� �޼��� 
class CCuttingStageMsgEffect : public CBaseObject // �ٲ� ī�带 �������ּ���
{
public:	

	CCuttingStageMsgEffect();
	virtual ~CCuttingStageMsgEffect();

	int		nChangeType;	//1 ��ħ, 2 ���� 3 ����
	NMBASE::SKINGDI::CNScene *m_pScene;
	DWORD	nDelayTime;	

	void Init(int changetype );
	virtual void DrawObject(CDC *pDC);
	virtual void Destroy();
	virtual int  ProcessObject();
};


}



