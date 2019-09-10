#include "StdAfx.h"
#include "SutdaGameSound.h"
#include "SoundRes.h"
#include "SutdaGameSoundDefine.h"


CSutdaGameSound::CSutdaGameSound(void)
{
	m_nVoiceType = 1;
}

CSutdaGameSound::~CSutdaGameSound(void)
{
}

CSutdaGameSound& CSutdaGameSound::GetInstance()
{
	static CSutdaGameSound inst;
	return inst;
}

CSutdaGameSound& SutdaGameSound()
{
	return CSutdaGameSound::GetInstance();
}

void CSutdaGameSound::SetMyVoiceType(BOOL bMySex, int nSndFxKind)
{
	
	m_nVoiceType = GetVoiceType(bMySex, nSndFxKind);
}

int CSutdaGameSound::GetVoiceType(BOOL bMySex, int nSndFxKind)
{

	int nVoiceType = 1;
	if (bMySex)
	{
		nVoiceType = (nSndFxKind==0)?1:2;
	}
	else
	{
		nVoiceType = (nSndFxKind==1)?3:4;
	}

	return nVoiceType;
}

//������ ���õ� ���带 ����. 
void CSutdaGameSound::WinJokboVoice(int nJokboPoint, BOOL bSex, int nSndFxKind)
{
	int selsnum = 0;

// 	int point = GetPairPoint(Play[r_pnum].MyCard[0].CardNo, Play[r_pnum].MyCard[1].CardNo);
// 	int sndkind = Play[r_pnum].m_SelSound;
// 
// 	//��Ҹ� ����
// 	if( g_pMainDlg->CFG.bSingleVoice ) sndkind = g_MyInfo.m_SelSound;
	int nVoiceType = GetVoiceType(bSex, nSndFxKind);

	switch( nJokboPoint )
	{
	case 29:	//38
		{
			selsnum = rand()%3;
			switch( nVoiceType )
			{
			case 1:
				{
					if( selsnum == 0)			NMBASE::SOUND::g_Sound.PlayOggStream(SNDM0_20 , FALSE);
					else if( selsnum == 1 )		NMBASE::SOUND::g_Sound.PlayOggStream(SNDM0_21 , FALSE);
					else						NMBASE::SOUND::g_Sound.PlayOggStream(SNDM0_22, FALSE );
				}break;
			case 2:
				{
					if( selsnum == 0)			NMBASE::SOUND::g_Sound.PlayOggStream(SNDM1_20, FALSE);
					else if( selsnum == 1)		NMBASE::SOUND::g_Sound.PlayOggStream(SNDM1_21 , FALSE);
					else						NMBASE::SOUND::g_Sound.PlayOggStream(SNDM1_22, FALSE );
				}break;
			case 3:
				{
					if( selsnum == 0)			NMBASE::SOUND::g_Sound.PlayOggStream(SNDF0_20 , FALSE);
					else if( selsnum ==1 )		NMBASE::SOUND::g_Sound.PlayOggStream(SNDF0_21 , FALSE);
					else						NMBASE::SOUND::g_Sound.PlayOggStream(SNDF0_22 , FALSE);
				}break;
			case 4:
				{
					if( selsnum == 0)			NMBASE::SOUND::g_Sound.PlayOggStream(SNDF1_20 , FALSE);
					else if( selsnum == 1)		NMBASE::SOUND::g_Sound.PlayOggStream(SNDF1_21 , FALSE);
					else						NMBASE::SOUND::g_Sound.PlayOggStream(SNDF1_22 , FALSE);
				}break;
			}
		}break;

	case 28:			//13����
		//case 2:			//18����
		{
			selsnum = rand()%2;
			switch( nVoiceType )
			{
			case 1:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDM0_23 , FALSE);
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDM0_24 , FALSE);
				}break;
			case 2:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDM1_23 , FALSE);
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDM1_24 , FALSE);
				}break;
			case 3:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDF0_23 , FALSE);
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDF0_24 , FALSE);
				}break;
			case 4:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDF1_23 , FALSE);
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDF1_24 , FALSE);
				}break;
			}
		}break;	
	case 27:	//10��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_25 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_25 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_25 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_25 , FALSE); break;
			}
		}break;	
	case 26:	//9��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_26 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_26 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_26 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_26 , FALSE); break;
			}
		}break;	
	case 25:		//8��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_27 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_27 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_27 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_27 , FALSE); break;
			}
		}break;
	case 24:		//7��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_28 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_28 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_28 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_28 , FALSE); break;
			}
		}break;
	case 23:	//6��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_29 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_29, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_29 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_29 , FALSE); break;
			}
		}break;
	case 22:		//5��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_30, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_30 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_30 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_30 , FALSE); break;
			}
		}break;
	case 21:		//4��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_31 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_31 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_31 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_31 , FALSE); break;
			}
		}break;
	case 20:			//3��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_32 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_32, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_32 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_32, FALSE ); break;
			}
		}break;
	case 19:		//2��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_33 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_33, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_33, FALSE ); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_33 , FALSE); break;
			}
		}break;
	case 18:		//1��
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_34 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_34 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_34 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_34 , FALSE); break;
			}
		}break;
	case 30:		//������
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_35 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_35 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_35 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_35 , FALSE); break;
			}
		}break;
	case 31:		//������
		{
			selsnum = rand()%2;
			switch( nVoiceType )
			{
			case 1:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDM0_36 , FALSE);
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDM0_37 , FALSE);
				}break;
			case 2:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDM1_36 , FALSE);
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDM1_37, FALSE );
				}break;
			case 3:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDF0_36, FALSE );
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDF0_37 , FALSE);
				}break;
			case 4:
				{
					if( selsnum == 0)	NMBASE::SOUND::g_Sound.PlayOggStream(SNDF1_36 , FALSE);
					else				NMBASE::SOUND::g_Sound.PlayOggStream(SNDF1_37 , FALSE);
				}break;
			}
		}break;	
	}

}

//������� �������� 
void CSutdaGameSound::LoseJokboVoice(int nJokboPoint, BOOL bSex, int nSndFxKind)
{
	int selsnum = 0;

// 	int point = GetPairPoint(Play[r_pnum].MyCard[0].CardNo, Play[r_pnum].MyCard[1].CardNo);
// 	int sndkind = Play[r_pnum].m_SelSound;
// 
// 	//��Ҹ� ����
// 	if( g_pMainDlg->CFG.bSingleVoice ) sndkind = g_MyInfo.m_SelSound;

	int nVoiceType = GetVoiceType(bSex, nSndFxKind);
	switch( nJokboPoint )
	{
	case 28:		//����
		//case 2:	
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_38 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_38 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_38 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_38 , FALSE); break;
			}
		}break;	//13,18
	case 27:	 //10
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_39 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_39 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_39 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_39 , FALSE); break;
			}
		}break;	
	case 26:	 //9
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_40 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_40 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_40 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_40, FALSE ); break;
			}
		}break;	
	case 25:	 //8
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_41, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_41 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_41 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_41 , FALSE); break;
			}
		}break;	
	case 24:	 	//7
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_42, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_42 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_42 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_42 , FALSE); break;
			}
		}break;
	case 23:	 	//6
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_43 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_43 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_43 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_43 , FALSE); break;
			}
		}break;
	case 22:	 	//5
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_44 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_44 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_44 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_44 , FALSE); break;
			}
		}break;
	case 21:	 	//4
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_45 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_45 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_45 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_45 , FALSE); break;
			}
		}break;
	case 20://3
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_46 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_46, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_46 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_46 , FALSE); break;
			}
		}break;	
	case 19:	 	//2
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_47 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_47 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_47 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_47, FALSE ); break;
			}
		}break;
	case 18:	//1
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_48, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_48 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_48 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_48 , FALSE); break;
			}
		}break;
	}
}

void CSutdaGameSound::NormalJokboVoice( int nJokboPoint, BOOL bSex, int nSndFxKind)
{
	int selsnum = 0;

// 	int point = GetPairPoint(Play[r_pnum].MyCard[0].CardNo, Play[r_pnum].MyCard[1].CardNo);
// 	int sndkind = Play[r_pnum].m_SelSound;

// 	��Ҹ� ����
// 		if( g_pMainDlg->CFG.bSingleVoice ) sndkind = g_MyInfo.m_SelSound;

	int nVoiceType = GetVoiceType(bSex, nSndFxKind);

	switch( nJokboPoint )
	{
	case 17:	//�˸�
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_49 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_49 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_49 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_49 , FALSE); break;
			}
		}break;

	case 16:	//����
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_50 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_50 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_50 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_50 , FALSE); break;
			}
		}break;

	case 15:	//����
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_51, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_51 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_51 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_51 , FALSE); break;
			}
		}break;

	case 14:	//���
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_52 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_52 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_52 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_52 , FALSE); break;
			}
		}break;

	case 13:	//���
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_53 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_53, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_53 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_53 , FALSE); break;
			}
		}break;

	case 12:	// ����
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_54, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_54 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_54, FALSE ); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_54 , FALSE); break;
			}
		}break;

	case 9:	//����
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_55, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_55 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_55, FALSE ); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_55 , FALSE); break;
			}
		}break;

	case 8:	//������  
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_57 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_57 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_57 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_57 , FALSE); break;
			}
		}break;

	case 7:	//�ϰ���
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_58 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_58 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_58 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_58 , FALSE); break;
			}
		}break;

	case 6:	//������
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_59 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_59, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_59 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_59 , FALSE); break;
			}
		}break;

	case 5:	//�ټ���
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_60 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_60 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_60 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_60, FALSE ); break;
			}
		}break;

	case 4:	//�ײ�
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_61 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_61 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_61, FALSE ); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_61 , FALSE); break;
			}
		}break;

	case 3:	//����
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_62 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_62 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_62 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_62 , FALSE); break;
			}
		}break;

	case 2:	//�β�
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_63 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_63 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_63 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_63 , FALSE); break;
			}
		}break;

	case 1:	//�Ѳ�
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_64, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_64, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_64 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_64 , FALSE); break;
			}
		}break;

	case 0:	//����
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_65 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_65, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_65 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_65 , FALSE); break;
			}
		}break;

	case 10:	//�籸
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_66 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_66 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_66 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_66, FALSE ); break;
			}
		}break;

	case 11:	//���ֱ����籸
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_67 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_67 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_67 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_67 , FALSE); break;
			}
		}break;

	case 30:	//������
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_68 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_68 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_68 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_68 , FALSE); break;
			}
		}break;

	case 31:	//������
		{
			switch(nVoiceType)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_69 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_69 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_69 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_69 , FALSE); break;
			}
		}break;
	}
}

void CSutdaGameSound::OnBetVoice(BOOL bSex, int nSndFxKind, RAISE_LAST_CMD eBetKind, BOOL bHiddenBet)
{
// 	if(m_nVoiceType==0) 
// 		return; //������ �����̸� �Ҹ��� ���� �ʴ´�.

	int nsndkind = GetVoiceType(bSex, nSndFxKind);

	if (nsndkind<=0)
		nsndkind = 1;
// 	//��Ҹ� ����
// 	if( g_pMainDlg->CFG.bSingleVoice ) nsndkind = g_MyInfo.m_SelSound;

	//////////////////////////////////////////////////////////////
	//## ������� �˻� 
//	BOOL ballinbet = FALSE;


// 	if( g_RI.Rule == 1 && m_CardNum == 2 ) ballinbet = TRUE;
// 	else if( (g_RI.Rule == 0 || m_bRematch) && m_CardNum == 1 ) ballinbet = TRUE;

	//����������ϰ�� Ư���� ������ �ش�.
	if( bHiddenBet ) 
		nsndkind += 4;	
	//////////////////////////////////////////////////////////////		

	switch(eBetKind)	//���ÿ� ����
	{
	case RAISE_LAST_CMD_FOLD:	//���� 
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_83 , FALSE); break;	//���� 1
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_83, FALSE ); break;	//����2
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_83 , FALSE); break;	//����1
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_83 , FALSE); break;	//����2

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_91 , FALSE); break;	//
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_91 , FALSE); break;	//
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_91 , FALSE); break;	//
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_91 , FALSE); break;	//

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_83 , FALSE); break;	//�⺻(����1)
			}
		}break;
	case RAISE_LAST_CMD_CHECK:	//üũ
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_84 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_84, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_84 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_84 , FALSE); break;

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_92, FALSE ); break;	//
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_92 , FALSE); break;	//
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_92 , FALSE); break;	//
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_92 , FALSE); break;	//

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_84, FALSE ); break;
			}
		}break;
	case RAISE_LAST_CMD_CALL:	//��
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_85 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_85, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_85 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_85 , FALSE); break;

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_93 , FALSE); break;	//
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_93 , FALSE); break;	//
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_93 , FALSE); break;	//
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_93 , FALSE); break;	//

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_85 , FALSE); break;
			}
		}break;
	case RAISE_LAST_CMD_PING:	//�� 
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_86 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_86, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_86 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_86 , FALSE); break;

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_94, FALSE ); break;	//
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_94 , FALSE); break;	//
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_94 , FALSE); break;	//
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_94 , FALSE); break;	//

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_86 , FALSE); break;
			}
		}break;
	case RAISE_LAST_CMD_DADANG:	//����
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_87, FALSE ); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_87 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_87 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_87 , FALSE); break;

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_95 , FALSE); break;	//
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_95 , FALSE); break;	//
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_95, FALSE ); break;	//
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_95 , FALSE); break;	//

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_87 , FALSE); break;
			}
		}break;
	case RAISE_LAST_CMD_HALF:	//Ǯ(����,��)
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_88 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_88 , FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_88 , FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_88 , FALSE); break;

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_96 , FALSE); break;	//
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_96 , FALSE); break;	//
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_96 , FALSE); break;	//
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_96 , FALSE); break;	//

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_88 , FALSE);  break;
			}
		}break;
	case RAISE_LAST_CMD_ALLIN:	//���ι��ý�
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_99 , FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_99, FALSE ); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_99, FALSE ); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_99 , FALSE); break;

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_99 , FALSE); break;	//
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_99, FALSE); break;	//
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_99 , FALSE); break;	//
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_99, FALSE ); break;	//

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_99 , FALSE);  break;
			}
		}break;
	case RAISE_LAST_CMD_MAX_BET:	//�ƽ����ý�
		{
			switch(nsndkind)
			{
			case 1:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_90, FALSE); break;
			case 2:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_90, FALSE); break;
			case 3:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_90, FALSE); break;
			case 4:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_90, FALSE); break;

			case 5:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_98, FALSE); break;
			case 6:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDM1_98, FALSE); break;
			case 7:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF0_98, FALSE); break;
			case 8:  NMBASE::SOUND::g_Sound.PlayOggStream( SNDF1_98, FALSE); break;

			default : NMBASE::SOUND::g_Sound.PlayOggStream( SNDM0_90 , FALSE);  break;
			}
		}break;
	}
}

//������ ����ش�.
void CSutdaGameSound::OnActionVoice( int r_kind, int r_snum, int r_randnum, int r_option  )
{
	//��Ȳ�� ���� ���� ������ġ�� �˾Ƴ���. 
	int startidx = SoundIndex[r_kind];

	int playidx = 0;	//0: ���� 1,2 : ����, 3,4:����
	//�������� ���ð��
	if( r_randnum > 0) playidx = startidx + rand() % r_randnum;
	else playidx = startidx;

	//��Ȳ�� �ɼ��� ���� ���
	if( r_option != -1 )
	{
		switch( r_kind )
		{
		case AV_WATCHSUBSCRIPT:
			{
				if( r_option > 4) r_option = 4;

				playidx = startidx + r_option;	//r_option�� ���� ������ ���´�.
			}break;
		}
	}

// 	//��Ҹ� ����
// 	if( g_pMainDlg->CFG.bSingleVoice ) r_snum = g_MyInfo.m_SelSound;
	r_snum = m_nVoiceType;

	//���尡 �߸�������� �ڽ��� ����� �ٲ��ش�.
	if( r_snum < 1 || r_snum > 4 ) 
		r_snum = 1;

	//��θ� �˾ƿ´�. 
	const char* filename = NULL;
	switch( r_snum )
	{
	case 1:  filename = pMan1[playidx]; break;		//����1
	case 2:  filename = pMan2[playidx]; break;		//����2
	case 3:  filename = pWoMan1[playidx]; break;	//����1
	case 4:  filename = pWoMan2[playidx]; break;	//����2
	}

	//����� �ȵ����� ����� �Ѵ�.
	if( NMBASE::SOUND::g_Sound.IsRegistered(filename) == FALSE )
		NMBASE::SOUND::g_Sound.RegistOggStream( filename, NMBASE::SOUND::SNDPRIORITY_MID, FALSE );

	//���带 �����Ѵ�.
	NMBASE::SOUND::g_Sound.PlayOggStream( filename, FALSE );
}


//���п� ���� ������ ����ش�.
void CSutdaGameSound::OnResultVoice(BOOL bMyWin, BOOL bMyFold)
{

// #ifdef _DEBUG
// 	m_Bugmsg.Format("������� : %d, ���� : %d", m_vWinandLossVoice , m_NowWinpnum);
// #endif

	//���� �̰��� ��� 
	//if( m_NowWinpnum == 0 )
	if (bMyWin)
	{
		if( m_vWinandLossVoice == END_ROOMJACKPOT ) 
			OnActionVoice( AV_ROOMJACKPT_WIN, m_nVoiceType, 2 );
		else if( m_vWinandLossVoice == END_ALLIN ) 
			OnActionVoice( AV_OTHER_ALLIN, m_nVoiceType, 4 );
		else if( m_vWinandLossVoice == END_SORT ) 
			OnActionVoice( AV_SORT_WIN, m_nVoiceType, 2 );
		else if( m_vWinandLossVoice == END_DRAW ) 
			OnActionVoice( AV_DRAW_WIN, m_nVoiceType, 2 );
		else if( m_vWinandLossVoice == END_BIG ) 
			OnActionVoice( AV_BIG_WIN, m_nVoiceType, 4 );
		else if( m_vWinandLossVoice == END_SMALL ) 
			OnActionVoice( AV_SMALL_WIN, m_nVoiceType, 3 );
		else /*if( m_vWinandLossVoice == END_NORMAL )*/ 
			OnActionVoice( AV_NORMAL_WIN, m_nVoiceType, 3 );
	}
	//���� �������
	else 
	{
		//�����ϰ��� ���⸦ �߾ ������ ���´�. 
		if( m_vWinandLossVoice == END_ALLIN )
			OnActionVoice( AV_MY_ALLIN, m_nVoiceType, 5 );
		//else //if( strlen(Play[0].UI.ID) > 0 && Play[0].JoinState == 1 && Play[0].LastState != GCMND_FOLD ) //$$$$$
		else if (bMyFold==FALSE)
		{
			if( m_vWinandLossVoice == END_SORT ) 
				OnActionVoice( AV_SORT_LOSS, m_nVoiceType, 2 );
			else if( m_vWinandLossVoice == END_DRAW ) 
				OnActionVoice( AV_DRAW_LOSS, m_nVoiceType, 3 );
			else /*if( m_vWinandLossVoice == END_NORMAL )*/
				OnActionVoice( AV_NORMAL_LOSS, m_nVoiceType, 4 );
		}

	
	}
}

void CSutdaGameSound::SetEndActionVoice(int r_order)
{	
	//�켱������ ������(���������� ������) ���ð�� �ٲ��ش�.
	if( m_vWinandLossVoice < r_order ) 
	{
		m_vWinandLossVoice = r_order;
	}

}

void CSutdaGameSound::OnResetGame()
{
	m_vWinandLossVoice = 0;
}
