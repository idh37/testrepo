#pragma once
#include "EffectDraw.h"
#include <map>

class CEffectDrawMan
{
	//�Ƕ�����Ʈ�� ���� ����Ʈ�� ���ʿ��� ������ ���ΰ�
	//�ϴ��� �� Ŭ�������� ���� ���Ұ����� �����ϰ� ��������
public:
	typedef map<int, CEffectDraw*> EffectList;

	CEffectDrawMan(void);
	~CEffectDrawMan(void);
	void Init(NMBASE::GRAPHICGDI::CPage* pPage);	
	void Draw(CDC *pDC);	
	void Reset();
	void PlayAnimation( eNUM_EFFECT eType, int nPnum = 0 );
	void PlayAnimation( eNUM_EFFECT eType, char* pID );
	void StopAnimation( eNUM_EFFECT eType, int nPnum = 0 );
	void StopAllAnimation();

protected:
	EffectList m_mapEffectList[MAX_PLAYER];
	NMBASE::GRAPHICGDI::CPage* m_pPage;
	void SetEffect( int nPnum, eNUM_EFFECT eType );

	CPoint ptThank[MAX_PLAYER];
	CPoint ptThankBak[MAX_PLAYER];
	CPoint ptStop_StopBak[MAX_PLAYER];
	CPoint ptResult[MAX_PLAYER];
	CPoint ptAllIn[MAX_PLAYER];
};
