// ObjectDef.h: interface for the CBaseObject class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

// ������Ʈ ����
enum ENUM_ANIFILE
{
	/////////////////////////////
	//�̺�Ʈ ���� �κ� 
	ANF_EVENTNONE = 0,
	ANF_EVENTCOIN,			//���Ĩ �̺�Ʈ ����
	ANF_EVENTSTAGE,			//���Ĩ �̺�Ʈ ���� Ȱ��ȭ
	ANF_EVENTLEVELUP,		//�������� �̺�Ʈ ����
	ANF_STEP_EVENTLEVELUP,	//�������� �ܰ躰 ����
	ANF_GETBULLET,			//�������� ����� ���� 
	ANF_EVENTSTAGETEXT,		// ����̺�Ʈ �������� �˸� �ؽ�Ʈ
	//////////////////////////////////////

	/////////////////////////////////////
	//���� Ŀ��� �κ�
	ANF_BET_CMD = 10,
	ANF_BET_ALLIN,
	ANF_BET_HALF,
	ANF_BET_CALL,
	ANF_BET_CHECK,
	ANF_BET_DADANG,
	ANF_BET_PING,
	ANF_BET_DIE,
	
	//������ : �ƽ� ����Ʈ �߰�
	ANF_BET_MAX,
	ANF_BET_FULL,
	ANF_TOTAL,

	//ANF_BET_MAX,
	/////////////////////////////////////

	ANF_ALLINCHARGE_INFO = 100,	//����õ�� ���� ���������� 2006.3.2
	ANF_SUPERAVATA,				//���� �ƹ�Ÿ ���� ȿ��
	ANF_SUMMERAVATA,			//[��� �̺�Ʈ] 2006.07.12
	ANF_VSMODEGAMESTARTCOUNT,	//VS��� ���� ���� ȿ��
	ANF_HIDDEN,					//���� ȿ��
	ANF_PREMSUPERALLINCHARGE,	//�����̾� ���� ����

	ANF_SDLG_GRADE,			//�̿��� ǥ��

	ANF_PCROOMEVENT,		//pc�� �̺�Ʈ
	ANF_HANDO,		//�����ѵ�.
	ANF_PLAYER_COMPLETEMADE_EFT,	// ���̵�����Ʈ
	ANF_PLAYER_CHANCEITEM,			// �����ƹ�Ÿ ȹ�� ����Ʈ

	
};
