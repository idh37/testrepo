#pragma once


#include "GraphicDXDefine.h"
#include "xAniFile.h"

//// ������ ��������Ʈ���� ���� ����ü
typedef struct
{
	int SprNo;
	int Rx, Ry;
	int Flag;
} PLAYSTUFF;

//// ���ϸ��̼� ���� Ŭ����
class NMGRAPHICDX_CLASS xAniMan
{
public:
	// �� ����� ������
	xAniFile*	pAni;
	Character*	pChar;
	Group*		pGroup;
	Action*		pAction;
	Frame*		pFrame;
	Cell*		pCell;

	// ���� ������ �� ��庰 �ε�����ȣ
	int nNowChar, nNowGroup, nNowAction, nNowFrame, nNowCell;

	// ���� ������ ����� ������� ����
	int nCharNum, nGroupNum, nActionNum, nFrameNum, nCellNum;

	// ���ϸ��̼� ��� ��� ���� ����
	BOOL bLoop;
	BOOL bForward;

	// ���ϸ��̼ǿ� ����
	int iDelayCnt;

public:
	xAniMan();
	~xAniMan();
	void Clear();

	// ���ϸ��̼� ���� Ŭ������ �ʱ�ȭ�Ѵ�
	BOOL Init(xAniFile* pAniFile);

	// ���ϸ��̼� �÷���
	int  PlayAction(int x, int y);
	BOOL GetPlayStuff(PLAYSTUFF* Ps, int* cellnum);

	// ���ϸ��̼� �÷��� �ɼ� ���� �Լ�
	void SetForward();
	void SetBackward();
	void SetLoop(BOOL loop);


	///////////// Low Level Functions /////////////

	// �� ����� ������ �ʱ�ȭ�Ѵ�( resetchild�� TRUE�̸� ���� ��嵵 �ʱ�ȭ �Ѵ� )
	void ResetCharacter(BOOL resetchild = TRUE);
	void ResetGroup(BOOL resetchild = TRUE);
	void ResetAction(BOOL resetchild = TRUE);
	void ResetFrame(BOOL resetchild = TRUE);
	void ResetCell();

	// �� ��忡 ���ϸ��̼� ��带 �����Ѵ�
	// (���� ��尡 �ݵ�� �����Ǿ� �־�� �Ѵ�)
	// ( reset�� TRUE�̸� ��������� ���� �ʱ�ȭ�Ѵ�)
	BOOL SetCharacter(int num, BOOL reset=TRUE);
	BOOL SetCharacter(char* name, BOOL reset=TRUE);	// Override Function
	BOOL SetGroup(int num, BOOL reset=TRUE);
	BOOL SetGroup(char* name, BOOL reset=TRUE);		// Override Function
	BOOL SetAction(int num, BOOL reset=TRUE);
	BOOL SetAction(char* name, BOOL reset=TRUE);	// Override Function
	BOOL SetFrame(int num, BOOL reset=TRUE);
	BOOL SetCell(int num);

	// ���� ����� ������ ��´�
	BOOL GetNowFrameInfo(int* delay, int* totcell);
	BOOL GetNowFrameInfo(int FrameNum, int* delay, int* totcell);				// Override Function
	BOOL GetNowCellInfo(int* sprno, int* rx, int* ry, int* flag);
	BOOL GetNowCellInfo(int CellNum, int* sprno, int* rx, int* ry, int* flag);	// Override Function
	BOOL GetNowCellInfo(PLAYSTUFF* Ps, int* totcell);							// Override Function
};