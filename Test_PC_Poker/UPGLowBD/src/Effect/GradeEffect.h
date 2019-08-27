#pragma once

#include "../AnimationDefine.h"	//UPG작업 추가

namespace effect
{

class CGradeEffect :public CBaseObject
{
public:

	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;	
	DWORD nDelay;	

	NMBASE::GRAPHICGDI::CPage		*m_pPage;

	CGradeEffect();
	virtual ~CGradeEffect();

	BOOL Init(int index , NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);
};


}


