#pragma once


//UPG�۾� BaseObject���ִ� enum OBJECT_TYPE�� ����� �ű�
// ������Ʈ ����
enum OBJECT_TYPE
{
	/////////////////////////////	
	ANF_UNKNOWN = -1,	
	//////////////////////////////////////
	ANF_BET_CMD,				// ���� Animation
	ANF_BET_HALF,
	ANF_BET_CALL,
	ANF_BET_CHECK,
	ANF_BET_DADANG,
	ANF_BET_PING,
	ANF_BET_DIE,
	ANF_BET_ALLIN,
		
	ANF_BET_LIMIT_SMALL_BET,
	ANF_BET_LIMIT_BIG_BET,
	ANF_BET_LIMIT_RAISE,	

	_ANF_SEPERATOR1_,

	ANF_BG_STAGESTATE,			//��ħ ���� ���� 		
	ANF_BG_CUTINGINFO,		    //Ŀ�� ����� ��� �˸�
	ANF_CUTINFO_NUM,

	ANF_PLAYER_WINEFFECT,		//�¸� ȿ��
	ANF_PLAYER_ALLINMARK,		//���� ��ũ
	ANF_PLAYER_ALLINANGEL,		//����õ�� (��������)
	ANF_PLAYER_JEKICHARGE,		//�������

	ANF_PLAYER_BLIND,			//�ο�ٵ��� ����ε�

	ANF_HIGHVALUE,				//����( ���� ), ������, ���
	ANF_PLAYER_CHANCEITEM,		//����������	
	
	ANF_PLAYER_OBSERVER,		//������ �̹���
	ANF_PLAYER_COMPLETEMADE_EFT,//���̵� �ϼ� ȿ��

	ANF_SDLG_GRADE,

	//������ : �ƽ� ����Ʈ �߰�
	ANF_BET_MAX,
	ANF_BET_FULL,
	ANF_TOTAL
};
