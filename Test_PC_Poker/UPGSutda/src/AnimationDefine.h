#pragma once

//UPG�۾� BaseObject���ִ� enum ENUM_ANIFILEID�� ����� �ű�
// ������Ʈ ����

enum ENUM_ANIFILEID
{
	/////////////////////////////	
	ANF_NONE = -1,	
		
	ANF_BET_CMD = 10,			// ���� Animation
	ANF_BET_HALF,
	ANF_BET_CALL,
	ANF_BET_CHECK,
	ANF_BET_DADANG,
	ANF_BET_PING,
	ANF_BET_DIE,
	ANF_BET_ALLIN,
	
	ANF_BET_RAISE_X1,
	ANF_BET_RAISE_X2,
	ANF_BET_RAISE_X3,

	ANF_PLAYER_WINEFFECT,		// �¸� ȿ��
	ANF_PLAYER_ALLINMARK,		// ���� ��ũ
	ANF_PLAYER_JEKICHARGE,		// �������
	ANF_PLAYER_CUTINGSELECT,	// Ŀ�� ������ �̹���.	
	ANF_PLAYER_CUTINGSELECTED,	// Ŀ�� ���� �Ϸ�
	ANF_PLAYER_CUTTINGNUMBER,	// Ŀ�� Ƚ�� ,�н�		
	ANF_PLAYER_TIMEBAR,			// Ÿ�ӹ�
	ANF_PLAYER_JOKBOEFFECT,		// ���� ����Ʈ
	ANF_PLAYER_CHANCEITEM,		// ����������	
	ANF_PLAYER_OBSERVER,		// ������ �̹���
	
	ANF_SDLG_GRADE,
	ANF_CUTINFO_BD,	
	ANF_HANDO,					//�����ѵ�.
	ANF_PLAYER_COMPLETEMADE_EFT,	// ���̵�����Ʈ
	ANF_BET_FULL,
};
