#pragma once

#include "../AnimationDefine.h"	//UPG�۾� �߰�

namespace effect
{

// ��ħ , ���� , ���� ǥ��
class CCuttingStageIndicateEffect :public CBaseObject
{
public:
	CCuttingStageIndicateEffect();
	virtual ~CCuttingStageIndicateEffect();	

	void	Init();	
	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);
};


}


