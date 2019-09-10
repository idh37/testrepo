#pragma once

#include "../AnimationDefine.h"	//UPG�۾� �߰�

namespace effect
{


class CBetCmdEffect : public CBaseObject
{

public:
	CBetCmdEffect(void);
	~CBetCmdEffect(void);


	typedef struct _tagEFFECTPOS
	{
		int nBigPos;
		int nSmallPos;
	}SEFFECTPOS;

public:
	
	BOOL Init(int nPlayerNum, OBJECT_TYPE eAniType, int nEffectType, BOOL bLastBetMax = FALSE);
	void SetSmallCmd();
	void SetDelTime(int ndel)
	{
		if(ndel <=0)
		{
			ndel = 0;
		}
		m_nDestroyDelTime = ndel;
	};

	SEFFECTPOS m_sEffectPos[ _ANF_SEPERATOR1_ ];	// ����/��/üũ/����/��/����/���� ���� ���( 5���� �̵��Ѵ�.)
	CPoint GetEffectPos(void);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);

	int GetPlayerNumber(){return m_PlayerNumber;}

private:		
	NMBASE::SKINGDI::CNMultiScene *m_pEffectScene;

	int		Xp,Yp;
	int		eXp,eYp;
	int		m_PlayerNumber;			// �÷��̾� ��ġ
	int     m_nBetEffectBigPos;		// ū ����Ʈ ��ġ
	int     m_nBetEffectSmallPos;	// ���� ����Ʈ ��ġ

	BOOL    bDifferntBetStyle;		// 3�� 2���϶� �� ���� �ݾ�

	DWORD	m_nStartTime;
	DWORD	m_nEndTime;	
	int		m_nDestroyDelTime;	

};

}


