// SoundRes.h: interface for the CGameRes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_)
#define AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���� ���ҽ��� �����ϰ� ����Ѵ�
void RegistSoundFiles();


// ����(OGG ����)
#define MUSIC_OGG01		("Sutda\\Sound\\SutdaBGM1.ogg")			// OGG ���� 1
//#define MUSIC_OGG01 ("Sutda\\voice\\man\\01\\allin1\\SutdaBGM1.ogg")
#define MUSIC_OGG02		("Sutda\\Sound\\SutdaBGM2.ogg")			// OGG ���� 2
#define MUSIC_OGG03		("Sutda\\Sound\\SutdaBGM3.ogg")			// OGG ���� 3
#define MUSIC_OGG04		("Sutda\\Sound\\SutdaBGM4.ogg")			// OGG ���� 4
#define MUSIC_OGGTheme	("Sutda\\Sound\\SutdaMainTheme.ogg")	// OGG �׸����� 1




#define SND01	("Sutda\\Sound\\1.wav")		//������ Ȯ�� ȿ���� (����)
#define SND02_1	("Sutda\\Sound\\2-1.wav")	//������ Ȯ�� ȿ���� (�ڵ�&������)(�����)
#define SND02_2	("Sutda\\Sound\\2-2.wav")	//������ Ȯ�� ȿ���� (�ڵ�&������)(�����)
#define SND03	("Sutda\\Sound\\2.wav")		//�� ������ ȿ����
#define SND04	("Sutda\\Sound\\4.wav")		//�� ��ġ�� ȿ����(����Ȯ������ �и� ������)
#define SND05	("Sutda\\Sound\\5.wav")		//�м���ȿ��(�����߿� ����)(�����)
#define SND06	("Sutda\\Sound\\6.wav")		//����ȿ��
#define SND07	("Sutda\\Sound\\7.wav")		//����ȿ����
#define SND08	("Sutda\\Sound\\8.wav")		//�ݱ�ȿ���� 
#define SND09	("Sutda\\Sound\\9.wav")		//������ȿ���� 
#define SND10	("Sutda\\Sound\\10.wav")	//Ÿ�̸�ȿ���� 
#define SND11	("Sutda\\Sound\\11.wav")	//��ưȿ���� 
#define SND12	("Sutda\\Sound\\12.wav")	//c����Ʈȿ��(�˸�~����)
#define SND13	("Sutda\\Sound\\13.wav")	//a,b����Ʈȿ��(�涯~38����)
#define SND14	("Sutda\\Sound\\14.wav")	//�ʴ�������˸�ȿ��
#define SND18	("Sutda\\Sound\\tt1.wav")	//������ ��Ʈ�� (oN-> Off)(�����)
#define SND19	("Sutda\\Sound\\tt2.wav")	//������ on (off -> on)(�����)
#define SND20	("Sutda\\Sound\\20.wav")	//49���� �� �� ���� ȿ����
#define SND21	("Sutda\\Sound\\21.wav")	//�������� �� �� ���� ȿ����
#define SND22	("Sutda\\Sound\\22.wav")	//�籸�� ȿ����(���ڸ�)
#define SND23	("Sutda\\Sound\\23.wav")	//������ ȿ����(������������ ������ ���ڸ�)
#define SND24	("Sutda\\Sound\\24.wav")	//��ĥ ������ ȿ����(���ڸ�)
#define SND25	("Sutda\\Sound\\25.wav")	//����� ���� ȿ����
#define SND26	("Sutda\\Sound\\26.wav")	//����� ���� ȿ����
#define SND30	("Sutda\\Sound\\30.wav")	//38�� �¸��� (�¸��ڸ�)
#define SND31	("Sutda\\Sound\\31.wav")	//���� �¸���(���ڸ�)
#define SND32	("Sutda\\Sound\\32.wav")	//�嶯 �¸���(���ڸ�)
#define SND33	("Sutda\\Sound\\33.wav")	//����~�涯 (���ڸ�)
#define SND34	("Sutda\\Sound\\34.wav")	//�������(���ڸ�)
#define SND35	("Sutda\\Sound\\35.wav")	//�ϱ�����(���ڸ�)
#define SND36	("Sutda\\Sound\\36.wav")	//�����й�(���ڸ�)
#define SND37	("Sutda\\Sound\\37.wav")	//�嶯�й�(���ڸ�)
#define SND38	("Sutda\\Sound\\38.wav")	//���й�(���ڸ�)
#define SND39	("Sutda\\Sound\\39.wav")	//���������(���ڸ�)
#define SND40	("Sutda\\Sound\\40.wav")	//�ϱ�������(���ڸ�)
#define SND41	("Sutda\\Sound\\41.wav")	//����ȿ��(������ ���ι���������)
#define SND42	("Sutda\\Sound\\42.wav")	//���εǾ� �Ļ�ȿ��(���ڸ�)
#define SND43	("Sutda\\Sound\\43.wav")	//�¸��õ���
#define SND44	("Sutda\\Sound\\44.wav")	//�й��Į


// ### [���� �߰�] ###
//############### ���� ###############

//## ���ȯ ���� ##//
//��������
#define SNDM0_1 ("Sutda\\voice\\man\\01\\banis\\ban.ogg")
#define SNDM0_2 ("Sutda\\voice\\man\\01\\banis\\bossban.ogg")

//�ʱ�
#define SNDM0_3 ("Sutda\\voice\\man\\01\\first\\gs1.ogg")
#define SNDM0_4 ("Sutda\\voice\\man\\01\\first\\gs2.ogg")
#define SNDM0_5 ("Sutda\\voice\\man\\01\\first\\gs3.ogg")

//����ī��
#define SNDM0_6 ("Sutda\\voice\\man\\01\\hidden\\hidden1.ogg")
#define SNDM0_7 ("Sutda\\voice\\man\\01\\hidden\\hidden2.ogg")
#define SNDM0_8 ("Sutda\\voice\\man\\01\\hidden\\hidden3.ogg")
#define SNDM0_9 ("Sutda\\voice\\man\\01\\hidden\\hidden4.ogg")

//��������
#define SNDM0_10 ("Sutda\\voice\\man\\01\\join\\j1.ogg")
#define SNDM0_11 ("Sutda\\voice\\man\\01\\join\\j2.ogg")
#define SNDM0_12 ("Sutda\\voice\\man\\01\\join\\j3.ogg")
#define SNDM0_13 ("Sutda\\voice\\man\\01\\join\\j4.ogg")
#define SNDM0_14 ("Sutda\\voice\\man\\01\\join\\j5.ogg")

//���� 
#define SNDM0_15 ("Sutda\\voice\\man\\01\\observer\\jres1.ogg")
#define SNDM0_16 ("Sutda\\voice\\man\\01\\observer\\jres2.ogg")
#define SNDM0_17 ("Sutda\\voice\\man\\01\\observer\\jres3.ogg")
#define SNDM0_18 ("Sutda\\voice\\man\\01\\observer\\ob_m.ogg")
#define SNDM0_19 ("Sutda\\voice\\man\\01\\observer\\ob_nm.ogg")

//����
#define SNDM0_20 ("Sutda\\voice\\man\\01\\value\\v_38Kwang1.ogg")
#define SNDM0_21 ("Sutda\\voice\\man\\01\\value\\v_38Kwang2.ogg")
#define SNDM0_22 ("Sutda\\voice\\man\\01\\value\\v_38Kwang3.ogg")
#define SNDM0_23 ("Sutda\\voice\\man\\01\\value\\v_Kwang1.ogg")
#define SNDM0_24 ("Sutda\\voice\\man\\01\\value\\v_Kwang2.ogg")

#define SNDM0_25 ("Sutda\\voice\\man\\01\\value\\v_10ddang.ogg")
#define SNDM0_26 ("Sutda\\voice\\man\\01\\value\\v_9ddang.ogg")
#define SNDM0_27 ("Sutda\\voice\\man\\01\\value\\v_8ddang.ogg")
#define SNDM0_28 ("Sutda\\voice\\man\\01\\value\\v_7ddang.ogg")
#define SNDM0_29 ("Sutda\\voice\\man\\01\\value\\v_6ddang.ogg")

//11
#define SNDM0_30 ("Sutda\\voice\\man\\01\\value\\v_5ddang.ogg")
#define SNDM0_31 ("Sutda\\voice\\man\\01\\value\\v_4ddang.ogg")
#define SNDM0_32 ("Sutda\\voice\\man\\01\\value\\v_3ddang.ogg")
#define SNDM0_33 ("Sutda\\voice\\man\\01\\value\\v_2ddang.ogg")
#define SNDM0_34 ("Sutda\\voice\\man\\01\\value\\v_1ddang.ogg")

#define SNDM0_35 ("Sutda\\voice\\man\\01\\value\\v_killddang.ogg")
#define SNDM0_36 ("Sutda\\voice\\man\\01\\value\\v_amhang1.ogg")
#define SNDM0_37 ("Sutda\\voice\\man\\01\\value\\v_amhang2.ogg")
#define SNDM0_38 ("Sutda\\voice\\man\\01\\value\\l_Kwang.ogg")
#define SNDM0_39 ("Sutda\\voice\\man\\01\\value\\l_10ddang.ogg")

//21
#define SNDM0_40 ("Sutda\\voice\\man\\01\\value\\l_9ddang.ogg")
#define SNDM0_41 ("Sutda\\voice\\man\\01\\value\\l_8ddang.ogg")
#define SNDM0_42 ("Sutda\\voice\\man\\01\\value\\l_7ddang.ogg")
#define SNDM0_43 ("Sutda\\voice\\man\\01\\value\\l_6ddang.ogg")
#define SNDM0_44 ("Sutda\\voice\\man\\01\\value\\l_5ddang.ogg")

#define SNDM0_45 ("Sutda\\voice\\man\\01\\value\\l_4ddang.ogg")
#define SNDM0_46 ("Sutda\\voice\\man\\01\\value\\l_3ddang.ogg")
#define SNDM0_47 ("Sutda\\voice\\man\\01\\value\\l_2ddang.ogg")
#define SNDM0_48 ("Sutda\\voice\\man\\01\\value\\l_1ddang.ogg")
#define SNDM0_49 ("Sutda\\voice\\man\\01\\value\\n_alli.ogg")

//31
#define SNDM0_50 ("Sutda\\voice\\man\\01\\value\\n_doksa.ogg")
#define SNDM0_51 ("Sutda\\voice\\man\\01\\value\\n_gubbing.ogg")
#define SNDM0_52 ("Sutda\\voice\\man\\01\\value\\n_jangbbing.ogg")
#define SNDM0_53 ("Sutda\\voice\\man\\01\\value\\n_jangsa.ogg")
#define SNDM0_54 ("Sutda\\voice\\man\\01\\value\\n_seruyk.ogg")

#define SNDM0_55 ("Sutda\\voice\\man\\01\\value\\n_gabo.ogg")
#define SNDM0_56 ("Sutda\\voice\\man\\01\\value\\n_9top.ogg")
#define SNDM0_57 ("Sutda\\voice\\man\\01\\value\\n_8top.ogg")
#define SNDM0_58 ("Sutda\\voice\\man\\01\\value\\n_7top.ogg")
#define SNDM0_59 ("Sutda\\voice\\man\\01\\value\\n_6top.ogg")

//41
#define SNDM0_60 ("Sutda\\voice\\man\\01\\value\\n_5top.ogg")
#define SNDM0_61 ("Sutda\\voice\\man\\01\\value\\n_4top.ogg")
#define SNDM0_62 ("Sutda\\voice\\man\\01\\value\\n_3top.ogg")
#define SNDM0_63 ("Sutda\\voice\\man\\01\\value\\n_2top.ogg")
#define SNDM0_64 ("Sutda\\voice\\man\\01\\value\\n_1top.ogg")

#define SNDM0_65 ("Sutda\\voice\\man\\01\\value\\n_0top.ogg")
#define SNDM0_66 ("Sutda\\voice\\man\\01\\value\\n_sagu1.ogg")
#define SNDM0_67 ("Sutda\\voice\\man\\01\\value\\n_sagu2.ogg")
#define SNDM0_68 ("Sutda\\voice\\man\\01\\value\\n_killddang.ogg")
#define SNDM0_69 ("Sutda\\voice\\man\\01\\value\\n_amhang.ogg")

//���������ϼ�
#define SNDM0_70 ("Sutda\\voice\\man\\01\\hidden\\made1.ogg")
#define SNDM0_71 ("Sutda\\voice\\man\\01\\hidden\\made2.ogg")

//������������
#define SNDM0_72 ("Sutda\\voice\\man\\01\\hidden\\hfail1.ogg")
#define SNDM0_73 ("Sutda\\voice\\man\\01\\hidden\\hfail2.ogg")

//�����ν�
#define SNDM0_74 ("Sutda\\voice\\man\\01\\allin1\\am1.ogg")
#define SNDM0_75 ("Sutda\\voice\\man\\01\\allin1\\am2.ogg")
#define SNDM0_76 ("Sutda\\voice\\man\\01\\allin1\\am3.ogg")
#define SNDM0_77 ("Sutda\\voice\\man\\01\\allin1\\am4.ogg")

//�ڽſ��ν�
#define SNDM0_78 ("Sutda\\voice\\man\\01\\allin2\\a1.ogg")
#define SNDM0_79 ("Sutda\\voice\\man\\01\\allin2\\a2.ogg")
#define SNDM0_80 ("Sutda\\voice\\man\\01\\allin2\\a3.ogg")
#define SNDM0_81 ("Sutda\\voice\\man\\01\\allin2\\a4.ogg")
#define SNDM0_82 ("Sutda\\voice\\man\\01\\allin2\\a5.ogg")

//����
#define SNDM0_83 ("Sutda\\voice\\man\\01\\bet\\mk0_00.ogg")	//���� 	
#define SNDM0_84 ("Sutda\\voice\\man\\01\\bet\\mk0_01.ogg")	//üũ
#define SNDM0_85 ("Sutda\\voice\\man\\01\\bet\\mk0_02.ogg")	//��
#define SNDM0_86 ("Sutda\\voice\\man\\01\\bet\\mk0_03.ogg")	//��
#define SNDM0_87 ("Sutda\\voice\\man\\01\\bet\\mk0_04.ogg")	//����
#define SNDM0_88 ("Sutda\\voice\\man\\01\\bet\\mk0_05.ogg")	//����
#define SNDM0_89 ("Sutda\\voice\\man\\01\\bet\\mk0_Q.ogg")		//����
#define SNDM0_90 ("Sutda\\voice\\man\\01\\bet\\mk0_M.ogg")		//�ƽ�

#define SNDM0_91 ("Sutda\\voice\\man\\01\\bet\\mk1_00.ogg")
#define SNDM0_92 ("Sutda\\voice\\man\\01\\bet\\mk1_01.ogg")
#define SNDM0_93 ("Sutda\\voice\\man\\01\\bet\\mk1_02.ogg")
#define SNDM0_94 ("Sutda\\voice\\man\\01\\bet\\mk1_03.ogg")
#define SNDM0_95 ("Sutda\\voice\\man\\01\\bet\\mk1_04.ogg")
#define SNDM0_96 ("Sutda\\voice\\man\\01\\bet\\mk1_05.ogg")
#define SNDM0_97 ("Sutda\\voice\\man\\01\\bet\\mk1_Q.ogg")
#define SNDM0_98 ("Sutda\\voice\\man\\01\\bet\\mk1_M.ogg")

#define SNDM0_99 ("Sutda\\voice\\man\\01\\bet\\mk1_allin.ogg")

//�Ѳ�������
#define SNDM0_100 ("Sutda\\voice\\man\\01\\lose\\g1.ogg")
#define SNDM0_101 ("Sutda\\voice\\man\\01\\lose\\g2.ogg")

//�����
#define SNDM0_102 ("Sutda\\voice\\man\\01\\lose\\d1.ogg")
#define SNDM0_103 ("Sutda\\voice\\man\\01\\lose\\d2.ogg")
#define SNDM0_104 ("Sutda\\voice\\man\\01\\lose\\d3.ogg")

//�Ϲ���
#define SNDM0_105 ("Sutda\\voice\\man\\01\\lose\\b1.ogg")
#define SNDM0_106 ("Sutda\\voice\\man\\01\\lose\\b2.ogg")
#define SNDM0_107 ("Sutda\\voice\\man\\01\\lose\\b3.ogg")
#define SNDM0_108 ("Sutda\\voice\\man\\01\\lose\\b4.ogg")

//�Ѳ����̽�
#define SNDM0_109 ("Sutda\\voice\\man\\01\\win\\w1.ogg")
#define SNDM0_110 ("Sutda\\voice\\man\\01\\win\\w2.ogg")

//���̽�
#define SNDM0_111 ("Sutda\\voice\\man\\01\\win\\p1.ogg")
#define SNDM0_112 ("Sutda\\voice\\man\\01\\win\\p2.ogg")

//��ڽ�
#define SNDM0_113 ("Sutda\\voice\\man\\01\\win\\b1.ogg")
#define SNDM0_114 ("Sutda\\voice\\man\\01\\win\\b2.ogg")
#define SNDM0_115 ("Sutda\\voice\\man\\01\\win\\b3.ogg")
#define SNDM0_116 ("Sutda\\voice\\man\\01\\win\\b4.ogg")

//�ʹڽ�
#define SNDM0_117 ("Sutda\\voice\\man\\01\\win\\s1.ogg")
#define SNDM0_118 ("Sutda\\voice\\man\\01\\win\\s2.ogg")
#define SNDM0_119 ("Sutda\\voice\\man\\01\\win\\s3.ogg")

//�Ϲݽ�
#define SNDM0_120 ("Sutda\\voice\\man\\01\\win\\m1.ogg")
#define SNDM0_121 ("Sutda\\voice\\man\\01\\win\\m2.ogg")
#define SNDM0_122 ("Sutda\\voice\\man\\01\\win\\m3.ogg")

//49�ٽ�
#define SNDM0_123 ("Sutda\\voice\\man\\01\\draw\\d1.ogg")
#define SNDM0_124 ("Sutda\\voice\\man\\01\\draw\\d2.ogg")

//�����������
#define SNDM0_125 ("Sutda\\voice\\man\\01\\draw\\d3.ogg")

//�����̽��۽�
#define SNDM0_126 ("Sutda\\voice\\man\\01\\eventw\\e1.ogg")
#define SNDM0_127 ("Sutda\\voice\\man\\01\\eventw\\e2.ogg")

//�����̽¸���
#define SNDM0_128 ("Sutda\\voice\\man\\01\\eventw\\e3.ogg")
#define SNDM0_129 ("Sutda\\voice\\man\\01\\eventw\\e4.ogg")

//////////////////////////////////////////////////////////////////////////
//## ������ ##//
//��������
#define SNDM1_1 ("Sutda\\voice\\man\\02\\banis\\ban.ogg")
#define SNDM1_2 ("Sutda\\voice\\man\\02\\banis\\bossban.ogg")

//�ʱ�
#define SNDM1_3 ("Sutda\\voice\\man\\02\\first\\gs1.ogg")
#define SNDM1_4 ("Sutda\\voice\\man\\02\\first\\gs2.ogg")
#define SNDM1_5 ("Sutda\\voice\\man\\02\\first\\gs3.ogg")

//����ī��
#define SNDM1_6 ("Sutda\\voice\\man\\02\\hidden\\hidden1.ogg")
#define SNDM1_7 ("Sutda\\voice\\man\\02\\hidden\\hidden2.ogg")
#define SNDM1_8 ("Sutda\\voice\\man\\02\\hidden\\hidden3.ogg")
#define SNDM1_9 ("Sutda\\voice\\man\\02\\hidden\\hidden4.ogg")

//��������
#define SNDM1_10 ("Sutda\\voice\\man\\02\\join\\j1.ogg")
#define SNDM1_11 ("Sutda\\voice\\man\\02\\join\\j2.ogg")
#define SNDM1_12 ("Sutda\\voice\\man\02\\join\\j3.ogg")
#define SNDM1_13 ("Sutda\\voice\\man\\02\\join\\j4.ogg")
#define SNDM1_14 ("Sutda\\voice\\man\\02\\join\\j5.ogg")

//���� 
#define SNDM1_15 ("Sutda\\voice\\man\\02\\observer\\jres1.ogg")
#define SNDM1_16 ("Sutda\\voice\\man\\02\\observer\\jres2.ogg")
#define SNDM1_17 ("Sutda\\voice\\man\\02\\observer\\jres3.ogg")
#define SNDM1_18 ("Sutda\\voice\\man\\02\\observer\\ob_m.ogg")
#define SNDM1_19 ("Sutda\\voice\\man\\02\\observer\\ob_nm.ogg")

//����
#define SNDM1_20 ("Sutda\\voice\\man\\02\\value\\v_38Kwang1.ogg")
#define SNDM1_21 ("Sutda\\voice\\man\\02\\value\\v_38Kwang2.ogg")
#define SNDM1_22 ("Sutda\\voice\\man\\02\\value\\v_38Kwang3.ogg")
#define SNDM1_23 ("Sutda\\voice\\man\\02\\value\\v_Kwang1.ogg")
#define SNDM1_24 ("Sutda\\voice\\man\\02\\value\\v_Kwang2.ogg")

#define SNDM1_25 ("Sutda\\voice\\man\\02\\value\\v_10ddang.ogg")
#define SNDM1_26 ("Sutda\\voice\\man\\02\\value\\v_9ddang.ogg")
#define SNDM1_27 ("Sutda\\voice\\man\\02\\value\\v_8ddang.ogg")
#define SNDM1_28 ("Sutda\\voice\\man\\02\\value\\v_7ddang.ogg")
#define SNDM1_29 ("Sutda\\voice\\man\\02\\value\\v_6ddang.ogg")

//11
#define SNDM1_30 ("Sutda\\voice\\man\\02\\value\\v_5ddang.ogg")
#define SNDM1_31 ("Sutda\\voice\\man\\02\\value\\v_4ddang.ogg")
#define SNDM1_32 ("Sutda\\voice\\man\\02\\value\\v_3ddang.ogg")
#define SNDM1_33 ("Sutda\\voice\\man\\02\\value\\v_2ddang.ogg")
#define SNDM1_34 ("Sutda\\voice\\man\\02\\value\\v_1ddang.ogg")

#define SNDM1_35 ("Sutda\\voice\\man\\02\\value\\v_killddang.ogg")
#define SNDM1_36 ("Sutda\\voice\\man\\02\\value\\v_amhang1.ogg")
#define SNDM1_37 ("Sutda\\voice\\man\\02\\value\\v_amhang2.ogg")
#define SNDM1_38 ("Sutda\\voice\\man\\02\\value\\l_Kwang.ogg")
#define SNDM1_39 ("Sutda\\voice\\man\\02\\value\\l_10ddang.ogg")

//21
#define SNDM1_40 ("Sutda\\voice\\man\\02\\value\\l_9ddang.ogg")
#define SNDM1_41 ("Sutda\\voice\\man\\02\\value\\l_8ddang.ogg")
#define SNDM1_42 ("Sutda\\voice\\man\\02\\value\\l_7ddang.ogg")
#define SNDM1_43 ("Sutda\\voice\\man\\02\\value\\l_6ddang.ogg")
#define SNDM1_44 ("Sutda\\voice\\man\\02\\value\\l_5ddang.ogg")

#define SNDM1_45 ("Sutda\\voice\\man\\02\\value\\l_4ddang.ogg")
#define SNDM1_46 ("Sutda\\voice\\man\\02\\value\\l_3ddang.ogg")
#define SNDM1_47 ("Sutda\\voice\\man\\02\\value\\l_2ddang.ogg")
#define SNDM1_48 ("Sutda\\voice\\man\\02\\value\\l_1ddang.ogg")
#define SNDM1_49 ("Sutda\\voice\\man\\02\\value\\n_alli.ogg")

//31
#define SNDM1_50 ("Sutda\\voice\\man\\02\\value\\n_doksa.ogg")
#define SNDM1_51 ("Sutda\\voice\\man\\02\\value\\n_gubbing.ogg")
#define SNDM1_52 ("Sutda\\voice\\man\\02\\value\\n_jangbbing.ogg")
#define SNDM1_53 ("Sutda\\voice\\man\\02\\value\\n_jangsa.ogg")
#define SNDM1_54 ("Sutda\\voice\\man\\02\\value\\n_seruyk.ogg")

#define SNDM1_55 ("Sutda\\voice\\man\\02\\value\\n_gabo.ogg")
#define SNDM1_56 ("Sutda\\voice\\man\\02\\value\\n_9top.ogg")
#define SNDM1_57 ("Sutda\\voice\\man\\02\\value\\n_8top.ogg")
#define SNDM1_58 ("Sutda\\voice\\man\\02\\value\\n_7top.ogg")
#define SNDM1_59 ("Sutda\\voice\\man\\02\\value\\n_6top.ogg")

//41
#define SNDM1_60 ("Sutda\\voice\\man\\02\\value\\n_5top.ogg")
#define SNDM1_61 ("Sutda\\voice\\man\\02\\value\\n_4top.ogg")
#define SNDM1_62 ("Sutda\\voice\\man\\02\\value\\n_3top.ogg")
#define SNDM1_63 ("Sutda\\voice\\man\\02\\value\\n_2top.ogg")
#define SNDM1_64 ("Sutda\\voice\\man\\02\\value\\n_1top.ogg")

#define SNDM1_65 ("Sutda\\voice\\man\\02\\value\\n_0top.ogg")
#define SNDM1_66 ("Sutda\\voice\\man\\02\\value\\n_sagu1.ogg")
#define SNDM1_67 ("Sutda\\voice\\man\\02\\value\\n_sagu2.ogg")
#define SNDM1_68 ("Sutda\\voice\\man\\02\\value\\n_killddang.ogg")
#define SNDM1_69 ("Sutda\\voice\\man\\02\\value\\n_amhang.ogg")

//���������ϼ�
#define SNDM1_70 ("Sutda\\voice\\man\\02\\hidden\\made1.ogg")
#define SNDM1_71 ("Sutda\\voice\\man\\02\\hidden\\made2.ogg")

//������������
#define SNDM1_72 ("Sutda\\voice\\man\\02\\hidden\\hfail1.ogg")
#define SNDM1_73 ("Sutda\\voice\\man\\02\\hidden\\hfail2.ogg")

//�����ν�
#define SNDM1_74 ("Sutda\\voice\\man\\02\\allin1\\am1.ogg")
#define SNDM1_75 ("Sutda\\voice\\man\\02\\allin1\\am2.ogg")
#define SNDM1_76 ("Sutda\\voice\\man\\02\\allin1\\am3.ogg")
#define SNDM1_77 ("Sutda\\voice\\man\\02\\allin1\\am4.ogg")

//�ڽſ��ν�
#define SNDM1_78 ("Sutda\\voice\\man\\02\\allin2\\a1.ogg")
#define SNDM1_79 ("Sutda\\voice\\man\\02\\allin2\\a2.ogg")
#define SNDM1_80 ("Sutda\\voice\\man\\02\\allin2\\a3.ogg")
#define SNDM1_81 ("Sutda\\voice\\man\\02\\allin2\\a4.ogg")
#define SNDM1_82 ("Sutda\\voice\\man\\02\\allin2\\a5.ogg")

//����
#define SNDM1_83 ("Sutda\\voice\\man\\02\\bet\\mk0_00.ogg")
#define SNDM1_84 ("Sutda\\voice\\man\\02\\bet\\mk0_01.ogg")
#define SNDM1_85 ("Sutda\\voice\\man\\02\\bet\\mk0_02.ogg")
#define SNDM1_86 ("Sutda\\voice\\man\\02\\bet\\mk0_03.ogg")
#define SNDM1_87 ("Sutda\\voice\\man\\02\\bet\\mk0_04.ogg")
#define SNDM1_88 ("Sutda\\voice\\man\\02\\bet\\mk0_05.ogg")
#define SNDM1_89 ("Sutda\\voice\\man\\02\\bet\\mk0_Q.ogg")
#define SNDM1_90 ("Sutda\\voice\\man\\02\\bet\\mk0_M.ogg")

#define SNDM1_91 ("Sutda\\voice\\man\\02\\bet\\mk1_00.ogg")
#define SNDM1_92 ("Sutda\\voice\\man\\02\\bet\\mk1_01.ogg")
#define SNDM1_93 ("Sutda\\voice\\man\\02\\bet\\mk1_02.ogg")
#define SNDM1_94 ("Sutda\\voice\\man\\02\\bet\\mk1_03.ogg")
#define SNDM1_95 ("Sutda\\voice\\man\\02\\bet\\mk1_04.ogg")
#define SNDM1_96 ("Sutda\\voice\\man\\02\\bet\\mk1_05.ogg")
#define SNDM1_97 ("Sutda\\voice\\man\\02\\bet\\mk1_Q.ogg")
#define SNDM1_98 ("Sutda\\voice\\man\\02\\bet\\mk1_M.ogg")
#define SNDM1_99 ("Sutda\\voice\\man\\02\\bet\\mk1_allin.ogg")

//�Ѳ�������
#define SNDM1_100 ("Sutda\\voice\\man\\02\\lose\\g1.ogg")
#define SNDM1_101 ("Sutda\\voice\\man\\02\\lose\\g2.ogg")

//�����
#define SNDM1_102 ("Sutda\\voice\\man\\02\\lose\\d1.ogg")
#define SNDM1_103 ("Sutda\\voice\\man\\02\\lose\\d2.ogg")
#define SNDM1_104 ("Sutda\\voice\\man\\02\\lose\\d3.ogg")

//�Ϲ���
#define SNDM1_105 ("Sutda\\voice\\man\\02\\lose\\b1.ogg")
#define SNDM1_106 ("Sutda\\voice\\man\\02\\lose\\b2.ogg")
#define SNDM1_107 ("Sutda\\voice\\man\\02\\lose\\b3.ogg")
#define SNDM1_108 ("Sutda\\voice\\man\\02\\lose\\b4.ogg")

//�Ѳ����̽�
#define SNDM1_109 ("Sutda\\voice\\man\\02\\win\\w1.ogg")
#define SNDM1_110 ("Sutda\\voice\\man\\02\\win\\w2.ogg")

//���̽�
#define SNDM1_111 ("Sutda\\voice\\man\\02\\win\\p1.ogg")
#define SNDM1_112 ("Sutda\\voice\\man\\02\\win\\p2.ogg")

//��ڽ�
#define SNDM1_113 ("Sutda\\voice\\man\\02\\win\\b1.ogg")
#define SNDM1_114 ("Sutda\\voice\\man\\02\\win\\b2.ogg")
#define SNDM1_115 ("Sutda\\voice\\man\\02\\win\\b3.ogg")
#define SNDM1_116 ("Sutda\\voice\\man\\02\\win\\b4.ogg")

//�ʹڽ�
#define SNDM1_117 ("Sutda\\voice\\man\\02\\win\\s1.ogg")
#define SNDM1_118 ("Sutda\\voice\\man\\02\\win\\s2.ogg")
#define SNDM1_119 ("Sutda\\voice\\man\\02\\win\\s3.ogg")

//�Ϲݽ�
#define SNDM1_120 ("Sutda\\voice\\man\\02\\win\\m1.ogg")
#define SNDM1_121 ("Sutda\\voice\\man\\02\\win\\m2.ogg")
#define SNDM1_122 ("Sutda\\voice\\man\\02\\win\\m3.ogg")

//49�ٽ�
#define SNDM1_123 ("Sutda\\voice\\man\\02\\draw\\d1.ogg")
#define SNDM1_124 ("Sutda\\voice\\man\\02\\draw\\d2.ogg")

//�����������
#define SNDM1_125 ("Sutda\\voice\\man\\02\\draw\\d3.ogg")

//�����̽��۽�
#define SNDM1_126 ("Sutda\\voice\\man\\02\\eventw\\e1.ogg")
#define SNDM1_127 ("Sutda\\voice\\man\\02\\eventw\\e2.ogg")

//�����̽¸���
#define SNDM1_128 ("Sutda\\voice\\man\\02\\eventw\\e3.ogg")
#define SNDM1_129 ("Sutda\\voice\\man\\02\\eventw\\e4.ogg")



//////////////////////////////////////////////////////////////////////////
/////// ���� ���� ///////
//## ���� ##//
//��������
#define SNDF0_1 ("Sutda\\voice\\woman\\01\\banis\\ban.ogg")
#define SNDF0_2 ("Sutda\\voice\\woman\\01\\banis\\bossban.ogg")

//�ʱ�
#define SNDF0_3 ("Sutda\\voice\\woman\\01\\first\\gs1.ogg")
#define SNDF0_4 ("Sutda\\voice\\woman\\01\\first\\gs2.ogg")
#define SNDF0_5 ("Sutda\\voice\\woman\\01\\first\\gs3.ogg")

//����ī��
#define SNDF0_6 ("Sutda\\voice\\woman\\01\\hidden\\hidden1.ogg")
#define SNDF0_7 ("Sutda\\voice\\woman\\01\\hidden\\hidden2.ogg")
#define SNDF0_8 ("Sutda\\voice\\woman\\01\\hidden\\hidden3.ogg")
#define SNDF0_9 ("Sutda\\voice\\woman\\01\\hidden\\hidden4.ogg")

//��������
#define SNDF0_10 ("Sutda\\voice\\woman\\01\\join\\j1.ogg")
#define SNDF0_11 ("Sutda\\voice\\woman\\01\\join\\j2.ogg")
#define SNDF0_12 ("Sutda\\voice\\woman\\01\\join\\j3.ogg")
#define SNDF0_13 ("Sutda\\voice\\woman\\01\\join\\j4.ogg")
#define SNDF0_14 ("Sutda\\voice\\woman\\01\\join\\j5.ogg")

//���� 
#define SNDF0_15 ("Sutda\\voice\\woman\\01\\observer\\jres1.ogg")
#define SNDF0_16 ("Sutda\\voice\\woman\\01\\observer\\jres2.ogg")
#define SNDF0_17 ("Sutda\\voice\\woman\\01\\observer\\jres3.ogg")
#define SNDF0_18 ("Sutda\\voice\\woman\\01\\observer\\ob_m.ogg")
#define SNDF0_19 ("Sutda\\voice\\woman\\01\\observer\\ob_nm.ogg")

//����
#define SNDF0_20 ("Sutda\\voice\\woman\\01\\value\\v_38Kwang1.ogg")
#define SNDF0_21 ("Sutda\\voice\\woman\\01\\value\\v_38Kwang2.ogg")
#define SNDF0_22 ("Sutda\\voice\\woman\\01\\value\\v_38Kwang3.ogg")
#define SNDF0_23 ("Sutda\\voice\\woman\\01\\value\\v_Kwang1.ogg")
#define SNDF0_24 ("Sutda\\voice\\woman\\01\\value\\v_Kwang2.ogg")

#define SNDF0_25 ("Sutda\\voice\\woman\\01\\value\\v_10ddang.ogg")
#define SNDF0_26 ("Sutda\\voice\\woman\\01\\value\\v_9ddang.ogg")
#define SNDF0_27 ("Sutda\\voice\\woman\\01\\value\\v_8ddang.ogg")
#define SNDF0_28 ("Sutda\\voice\\woman\\01\\value\\v_7ddang.ogg")
#define SNDF0_29 ("Sutda\\voice\\woman\\01\\value\\v_6ddang.ogg")

//11
#define SNDF0_30 ("Sutda\\voice\\woman\\01\\value\\v_5ddang.ogg")
#define SNDF0_31 ("Sutda\\voice\\woman\\01\\value\\v_4ddang.ogg")
#define SNDF0_32 ("Sutda\\voice\\woman\\01\\value\\v_3ddang.ogg")
#define SNDF0_33 ("Sutda\\voice\\woman\\01\\value\\v_2ddang.ogg")
#define SNDF0_34 ("Sutda\\voice\\woman\\01\\value\\v_1ddang.ogg")

#define SNDF0_35 ("Sutda\\voice\\woman\\01\\value\\v_killddang.ogg")
#define SNDF0_36 ("Sutda\\voice\\woman\\01\\value\\v_amhang1.ogg")
#define SNDF0_37 ("Sutda\\voice\\woman\\01\\value\\v_amhang2.ogg")
#define SNDF0_38 ("Sutda\\voice\\woman\\01\\value\\l_Kwang.ogg")
#define SNDF0_39 ("Sutda\\voice\\woman\\01\\value\\l_10ddang.ogg")

//21
#define SNDF0_40 ("Sutda\\voice\\woman\\01\\value\\l_9ddang.ogg")
#define SNDF0_41 ("Sutda\\voice\\woman\\01\\value\\l_8ddang.ogg")
#define SNDF0_42 ("Sutda\\voice\\woman\\01\\value\\l_7ddang.ogg")
#define SNDF0_43 ("Sutda\\voice\\woman\\01\\value\\l_6ddang.ogg")
#define SNDF0_44 ("Sutda\\voice\\woman\\01\\value\\l_5ddang.ogg")

#define SNDF0_45 ("Sutda\\voice\\woman\\01\\value\\l_4ddang.ogg")
#define SNDF0_46 ("Sutda\\voice\\woman\\01\\value\\l_3ddang.ogg")
#define SNDF0_47 ("Sutda\\voice\\woman\\01\\value\\l_2ddang.ogg")
#define SNDF0_48 ("Sutda\\voice\\woman\\01\\value\\l_1ddang.ogg")
#define SNDF0_49 ("Sutda\\voice\\woman\\01\\value\\n_alli.ogg")

//31
#define SNDF0_50 ("Sutda\\voice\\woman\\01\\value\\n_doksa.ogg")
#define SNDF0_51 ("Sutda\\voice\\woman\\01\\value\\n_gubbing.ogg")
#define SNDF0_52 ("Sutda\\voice\\woman\\01\\value\\n_jangbbing.ogg")
#define SNDF0_53 ("Sutda\\voice\\woman\\01\\value\\n_jangsa.ogg")
#define SNDF0_54 ("Sutda\\voice\\woman\\01\\value\\n_seruyk.ogg")

#define SNDF0_55 ("Sutda\\voice\\woman\\01\\value\\n_gabo.ogg")
#define SNDF0_56 ("Sutda\\voice\\woman\\01\\value\\n_9top.ogg")
#define SNDF0_57 ("Sutda\\voice\\woman\\01\\value\\n_8top.ogg")
#define SNDF0_58 ("Sutda\\voice\\woman\\01\\value\\n_7top.ogg")
#define SNDF0_59 ("Sutda\\voice\\woman\\01\\value\\n_6top.ogg")

//41
#define SNDF0_60 ("Sutda\\voice\\woman\\01\\value\\n_5top.ogg")
#define SNDF0_61 ("Sutda\\voice\\woman\\01\\value\\n_4top.ogg")
#define SNDF0_62 ("Sutda\\voice\\woman\\01\\value\\n_3top.ogg")
#define SNDF0_63 ("Sutda\\voice\\woman\\01\\value\\n_2top.ogg")
#define SNDF0_64 ("Sutda\\voice\\woman\\01\\value\\n_1top.ogg")

#define SNDF0_65 ("Sutda\\voice\\woman\\01\\value\\n_0top.ogg")
#define SNDF0_66 ("Sutda\\voice\\woman\\01\\value\\n_sagu1.ogg")
#define SNDF0_67 ("Sutda\\voice\\woman\\01\\value\\n_sagu2.ogg")
#define SNDF0_68 ("Sutda\\voice\\woman\\01\\value\\n_killddang.ogg")
#define SNDF0_69 ("Sutda\\voice\\woman\\01\\value\\n_amhang.ogg")

//���������ϼ�
#define SNDF0_70 ("Sutda\\voice\\woman\\01\\hidden\\made1.ogg")
#define SNDF0_71 ("Sutda\\voice\\woman\\01\\hidden\\made2.ogg")

//������������
#define SNDF0_72 ("Sutda\\voice\\woman\\01\\hidden\\hfail1.ogg")
#define SNDF0_73 ("Sutda\\voice\\woman\\01\\hidden\\hfail2.ogg")

//�����ν�
#define SNDF0_74 ("Sutda\\voice\\woman\\01\\allin1\\am1.ogg")
#define SNDF0_75 ("Sutda\\voice\\woman\\01\\allin1\\am2.ogg")
#define SNDF0_76 ("Sutda\\voice\\woman\\01\\allin1\\am3.ogg")
#define SNDF0_77 ("Sutda\\voice\\woman\\01\\allin1\\am4.ogg")

//�ڽſ��ν�
#define SNDF0_78 ("Sutda\\voice\\woman\\01\\allin2\\a1.ogg")
#define SNDF0_79 ("Sutda\\voice\\woman\\01\\allin2\\a2.ogg")
#define SNDF0_80 ("Sutda\\voice\\woman\\01\\allin2\\a3.ogg")
#define SNDF0_81 ("Sutda\\voice\\woman\\01\\allin2\\a4.ogg")
#define SNDF0_82 ("Sutda\\voice\\woman\\01\\allin2\\a5.ogg")

//����
#define SNDF0_83 ("Sutda\\voice\\woman\\01\\bet\\mk0_00.ogg")
#define SNDF0_84 ("Sutda\\voice\\woman\\01\\bet\\mk0_01.ogg")
#define SNDF0_85 ("Sutda\\voice\\woman\\01\\bet\\mk0_02.ogg")
#define SNDF0_86 ("Sutda\\voice\\woman\\01\\bet\\mk0_03.ogg")
#define SNDF0_87 ("Sutda\\voice\\woman\\01\\bet\\mk0_04.ogg")
#define SNDF0_88 ("Sutda\\voice\\woman\\01\\bet\\mk0_05.ogg")
#define SNDF0_89 ("Sutda\\voice\\woman\\01\\bet\\mk0_Q.ogg")
#define SNDF0_90 ("Sutda\\voice\\woman\\01\\bet\\mk0_M.ogg")

#define SNDF0_91 ("Sutda\\voice\\woman\\01\\bet\\mk1_00.ogg")
#define SNDF0_92 ("Sutda\\voice\\woman\\01\\bet\\mk1_01.ogg")
#define SNDF0_93 ("Sutda\\voice\\woman\\01\\bet\\mk1_02.ogg")
#define SNDF0_94 ("Sutda\\voice\\woman\\01\\bet\\mk1_03.ogg")
#define SNDF0_95 ("Sutda\\voice\\woman\\01\\bet\\mk1_04.ogg")
#define SNDF0_96 ("Sutda\\voice\\woman\\01\\bet\\mk1_05.ogg")
#define SNDF0_97 ("Sutda\\voice\\woman\\01\\bet\\mk1_Q.ogg")
#define SNDF0_98 ("Sutda\\voice\\woman\\01\\bet\\mk1_M.ogg")

#define SNDF0_99 ("Sutda\\voice\\woman\\01\\bet\\mk1_allin.ogg")

//�Ѳ�������
#define SNDF0_100 ("Sutda\\voice\\woman\\01\\lose\\g1.ogg")
#define SNDF0_101 ("Sutda\\voice\\woman\\01\\lose\\g2.ogg")

//�����
#define SNDF0_102 ("Sutda\\voice\\woman\\01\\lose\\d1.ogg")
#define SNDF0_103 ("Sutda\\voice\\woman\\01\\lose\\d2.ogg")
#define SNDF0_104 ("Sutda\\voice\\woman\\01\\lose\\d3.ogg")

//�Ϲ���
#define SNDF0_105 ("Sutda\\voice\\woman\\01\\lose\\b1.ogg")
#define SNDF0_106 ("Sutda\\voice\\woman\\01\\lose\\b2.ogg")
#define SNDF0_107 ("Sutda\\voice\\woman\\01\\lose\\b3.ogg")
#define SNDF0_108 ("Sutda\\voice\\woman\\01\\lose\\b4.ogg")

//�Ѳ����̽�
#define SNDF0_109 ("Sutda\\voice\\woman\\01\\win\\w1.ogg")
#define SNDF0_110 ("Sutda\\voice\\woman\\01\\win\\w2.ogg")

//���̽�
#define SNDF0_111 ("Sutda\\voice\\woman\\01\\win\\p1.ogg")
#define SNDF0_112 ("Sutda\\voice\\woman\\01\\win\\p2.ogg")

//��ڽ�
#define SNDF0_113 ("Sutda\\voice\\woman\\01\\win\\b1.ogg")
#define SNDF0_114 ("Sutda\\voice\\woman\\01\\win\\b2.ogg")
#define SNDF0_115 ("Sutda\\voice\\woman\\01\\win\\b3.ogg")
#define SNDF0_116 ("Sutda\\voice\\woman\\01\\win\\b4.ogg")

//�ʹڽ�
#define SNDF0_117 ("Sutda\\voice\\woman\\01\\win\\s1.ogg")
#define SNDF0_118 ("Sutda\\voice\\woman\\01\\win\\s2.ogg")
#define SNDF0_119 ("Sutda\\voice\\woman\\01\\win\\s3.ogg")

//�Ϲݽ�
#define SNDF0_120 ("Sutda\\voice\\woman\\01\\win\\m1.ogg")
#define SNDF0_121 ("Sutda\\voice\\woman\\01\\win\\m2.ogg")
#define SNDF0_122 ("Sutda\\voice\\woman\\01\\win\\m3.ogg")

//49�ٽ�
#define SNDF0_123 ("Sutda\\voice\\woman\\01\\draw\\d1.ogg")
#define SNDF0_124 ("Sutda\\voice\\woman\\01\\draw\\d2.ogg")

//�����������
#define SNDF0_125 ("Sutda\\voice\\woman\\01\\draw\\d3.ogg")

//�����̽��۽�
#define SNDF0_126 ("Sutda\\voice\\woman\\01\\eventw\\e1.ogg")
#define SNDF0_127 ("Sutda\\voice\\woman\\01\\eventw\\e2.ogg")

//�����̽¸���
#define SNDF0_128 ("Sutda\\voice\\woman\\01\\eventw\\e3.ogg")
#define SNDF0_129 ("Sutda\\voice\\woman\\01\\eventw\\e4.ogg")

//////////////////////////////////////////////////////////////////////////
//## �ֶ� ##//
//��������
#define SNDF1_1 ("Sutda\\voice\\woman\\02\\banis\\ban.ogg")
#define SNDF1_2 ("Sutda\\voice\\woman\\02\\banis\\bossban.ogg")

//�ʱ�
#define SNDF1_3 ("Sutda\\voice\\woman\\02\\first\\gs1.ogg")
#define SNDF1_4 ("Sutda\\voice\\woman\\02\\first\\gs2.ogg")
#define SNDF1_5 ("Sutda\\voice\\woman\\02\\first\\gs3.ogg")

//����ī��
#define SNDF1_6 ("Sutda\\voice\\woman\\02\\hidden\\hidden1.ogg")
#define SNDF1_7 ("Sutda\\voice\\woman\\02\\hidden\\hidden2.ogg")
#define SNDF1_8 ("Sutda\\voice\\woman\\02\\hidden\\hidden3.ogg")
#define SNDF1_9 ("Sutda\\voice\\woman\\02\\hidden\\hidden4.ogg")

//��������
#define SNDF1_10 ("Sutda\\voice\\woman\\02\\join\\j1.ogg")
#define SNDF1_11 ("Sutda\\voice\\woman\\02\\join\\j2.ogg")
#define SNDF1_12 ("Sutda\\voice\\woman\\02\\join\\j3.ogg")
#define SNDF1_13 ("Sutda\\voice\\woman\\02\\join\\j4.ogg")
#define SNDF1_14 ("Sutda\\voice\\woman\\02\\join\\j5.ogg")

//���� 
#define SNDF1_15 ("Sutda\\voice\\woman\\02\\observer\\jres1.ogg")
#define SNDF1_16 ("Sutda\\voice\\woman\\02\\observer\\jres2.ogg")
#define SNDF1_17 ("Sutda\\voice\\woman\\02\\observer\\jres3.ogg")
#define SNDF1_18 ("Sutda\\voice\\\\woman\\\\02\\\\observer\\\\ob_m.ogg")
#define SNDF1_19 ("Sutda\\voice\\woman\\02\\observer\\ob_nm.ogg")

//����
#define SNDF1_20 ("Sutda\\voice\\woman\\02\\value\\v_38Kwang1.ogg")
#define SNDF1_21 ("Sutda\\voice\\woman\\02\\value\\v_38Kwang2.ogg")
#define SNDF1_22 ("Sutda\\voice\\woman\\02\\value\\v_38Kwang3.ogg")
#define SNDF1_23 ("Sutda\\voice\\woman\\02\\value\\v_Kwang1.ogg")
#define SNDF1_24 ("Sutda\\voice\\woman\\02\\value\\v_Kwang2.ogg")

#define SNDF1_25 ("Sutda\\voice\\woman\\02\\value\\v_10ddang.ogg")
#define SNDF1_26 ("Sutda\\voice\\woman\\02\\value\\v_9ddang.ogg")
#define SNDF1_27 ("Sutda\\voice\\woman\\02\\value\\v_8ddang.ogg")
#define SNDF1_28 ("Sutda\\voice\\woman\\02\\value\\v_7ddang.ogg")
#define SNDF1_29 ("Sutda\\voice\\woman\\02\\value\\v_6ddang.ogg")

//11
#define SNDF1_30 ("Sutda\\voice\\woman\\02\\value\\v_5ddang.ogg")
#define SNDF1_31 ("Sutda\\voice\\woman\\02\\value\\v_4ddang.ogg")
#define SNDF1_32 ("Sutda\\voice\\woman\\02\\value\\v_3ddang.ogg")
#define SNDF1_33 ("Sutda\\voice\\woman\\02\\value\\v_2ddang.ogg")
#define SNDF1_34 ("Sutda\\voice\\woman\\02\\value\\v_1ddang.ogg")

#define SNDF1_35 ("Sutda\\voice\\woman\\02\\value\\v_killddang.ogg")
#define SNDF1_36 ("Sutda\\voice\\woman\\02\\value\\v_amhang1.ogg")
#define SNDF1_37 ("Sutda\\voice\\woman\\02\\value\\v_amhang2.ogg")
#define SNDF1_38 ("Sutda\\voice\\woman\\02\\value\\l_Kwang.ogg")
#define SNDF1_39 ("Sutda\\voice\\woman\\02\\value\\l_10ddang.ogg")

//21
#define SNDF1_40 ("Sutda\\voice\\woman\\02\\value\\l_9ddang.ogg")
#define SNDF1_41 ("Sutda\\voice\\woman\\02\\value\\l_8ddang.ogg")
#define SNDF1_42 ("Sutda\\voice\\woman\\02\\value\\l_7ddang.ogg")
#define SNDF1_43 ("Sutda\\voice\\woman\\02\\value\\l_6ddang.ogg")
#define SNDF1_44 ("Sutda\\voice\\woman\\02\\value\\l_5ddang.ogg")

#define SNDF1_45 ("Sutda\\voice\\woman\\02\\value\\l_4ddang.ogg")
#define SNDF1_46 ("Sutda\\voice\\woman\\02\\value\\l_3ddang.ogg")
#define SNDF1_47 ("Sutda\\voice\\woman\\02\\value\\l_2ddang.ogg")
#define SNDF1_48 ("Sutda\\voice\\woman\\02\\value\\l_1ddang.ogg")
#define SNDF1_49 ("Sutda\\voice\\woman\\02\\value\\n_alli.ogg")

//31
#define SNDF1_50 ("Sutda\\voice\\woman\\02\\value\\n_doksa.ogg")
#define SNDF1_51 ("Sutda\\voice\\woman\\02\\value\\n_gubbing.ogg")
#define SNDF1_52 ("Sutda\\voice\\woman\\02\\value\\n_jangbbing.ogg")
#define SNDF1_53 ("Sutda\\voice\\woman\\02\\value\\n_jangsa.ogg")
#define SNDF1_54 ("Sutda\\voice\\woman\\02\\value\\n_seruyk.ogg")

#define SNDF1_55 ("Sutda\\voice\\woman\\02\\value\\n_gabo.ogg")
#define SNDF1_56 ("Sutda\\voice\\woman\\02\\value\\n_9top.ogg")
#define SNDF1_57 ("Sutda\\voice\\woman\\02\\value\\n_8top.ogg")
#define SNDF1_58 ("Sutda\\voice\\woman\\02\\value\\n_7top.ogg")
#define SNDF1_59 ("Sutda\\voice\\woman\\02\\value\\n_6top.ogg")

//41
#define SNDF1_60 ("Sutda\\voice\\woman\\02\\value\\n_5top.ogg")
#define SNDF1_61 ("Sutda\\voice\\woman\\02\\value\\n_4top.ogg")
#define SNDF1_62 ("Sutda\\voice\\woman\\02\\value\\n_3top.ogg")
#define SNDF1_63 ("Sutda\\voice\\woman\\02\\value\\n_2top.ogg")
#define SNDF1_64 ("Sutda\\voice\\woman\\02\\value\\n_1top.ogg")

#define SNDF1_65 ("Sutda\\voice\\woman\\02\\value\\n_0top.ogg")
#define SNDF1_66 ("Sutda\\voice\\woman\\02\\value\\n_sagu1.ogg")
#define SNDF1_67 ("Sutda\\voice\\woman\\02\\value\\n_sagu2.ogg")
#define SNDF1_68 ("Sutda\\voice\\woman\\02\\value\\n_killddang.ogg")
#define SNDF1_69 ("Sutda\\voice\\woman\\02\\value\\n_amhang.ogg")

//���������ϼ�
#define SNDF1_70 ("Sutda\\voice\\woman\\02\\hidden\\made1.ogg")
#define SNDF1_71 ("Sutda\\voice\\woman\\02\\hidden\\made2.ogg")

//������������
#define SNDF1_72 ("Sutda\\voice\\woman\\02\\hidden\\hfail1.ogg")
#define SNDF1_73 ("Sutda\\voice\\woman\\02\\hidden\\hfail2.ogg")

//�����ν�
#define SNDF1_74 ("Sutda\\voice\\woman\\02\\allin1\\am1.ogg")
#define SNDF1_75 ("Sutda\\voice\\woman\\02\\allin1\\am2.ogg")
#define SNDF1_76 ("Sutda\\voice\\woman\\02\\allin1\\am3.ogg")
#define SNDF1_77 ("Sutda\\voice\\woman\\02\\allin1\\am4.ogg")

//�ڽſ��ν�
#define SNDF1_78 ("Sutda\\voice\\woman\\02\\allin2\\a1.ogg")
#define SNDF1_79 ("Sutda\\voice\\woman\\02\\allin2\\a2.ogg")
#define SNDF1_80 ("Sutda\\voice\\woman\\02\\allin2\\a3.ogg")
#define SNDF1_81 ("Sutda\\voice\\woman\\02\\allin2\\a4.ogg")
#define SNDF1_82 ("Sutda\\voice\\woman\\02\\allin2\\a5.ogg")

//����
#define SNDF1_83 ("Sutda\\voice\\woman\\02\\bet\\mk0_00.ogg")
#define SNDF1_84 ("Sutda\\voice\\woman\\02\\bet\\mk0_01.ogg")
#define SNDF1_85 ("Sutda\\voice\\woman\\02\\bet\\mk0_02.ogg")
#define SNDF1_86 ("Sutda\\voice\\woman\\02\\bet\\mk0_03.ogg")
#define SNDF1_87 ("Sutda\\voice\\woman\\02\\bet\\mk0_04.ogg")
#define SNDF1_88 ("Sutda\\voice\\woman\\02\\bet\\mk0_05.ogg")
#define SNDF1_89 ("Sutda\\voice\\woman\\02\\bet\\mk0_Q.ogg")
#define SNDF1_90 ("Sutda\\voice\\woman\\02\\bet\\mk0_M.ogg")

#define SNDF1_91 ("Sutda\\voice\\woman\\02\\bet\\mk1_00.ogg")
#define SNDF1_92 ("Sutda\\voice\\woman\\02\\bet\\mk1_01.ogg")
#define SNDF1_93 ("Sutda\\voice\\woman\\02\\bet\\mk1_02.ogg")
#define SNDF1_94 ("Sutda\\voice\\woman\\02\\bet\\mk1_03.ogg")
#define SNDF1_95 ("Sutda\\voice\\woman\\02\\bet\\mk1_04.ogg")
#define SNDF1_96 ("Sutda\\voice\\woman\\02\\bet\\mk1_05.ogg")
#define SNDF1_97 ("Sutda\\voice\\woman\\02\\bet\\mk1_Q.ogg")
#define SNDF1_98 ("Sutda\\voice\\woman\\02\\bet\\mk1_M.ogg")
#define SNDF1_99 ("Sutda\\voice\\woman\\02\\bet\\mk1_allin.ogg")

//�Ѳ�������
#define SNDF1_100 ("Sutda\\voice\\woman\\02\\lose\\g1.ogg")
#define SNDF1_101 ("Sutda\\voice\\woman\\02\\lose\\g2.ogg")

//�����
#define SNDF1_102 ("Sutda\\voice\\woman\\02\\lose\\d1.ogg")
#define SNDF1_103 ("Sutda\\voice\\woman\\02\\lose\\d2.ogg")
#define SNDF1_104 ("Sutda\\voice\\woman\\02\\lose\\d3.ogg")

//�Ϲ���
#define SNDF1_105 ("Sutda\\voice\\woman\\02\\lose\\b1.ogg")
#define SNDF1_106 ("Sutda\\voice\\woman\\02\\lose\\b2.ogg")
#define SNDF1_107 ("Sutda\\voice\\woman\\02\\lose\\b3.ogg")
#define SNDF1_108 ("Sutda\\voice\\woman\\02\\lose\\b4.ogg")

//�Ѳ����̽�
#define SNDF1_109 ("Sutda\\voice\\woman\\02\\win\\w1.ogg")
#define SNDF1_110 ("Sutda\\voice\\woman\\02\\win\\w2.ogg")

//���̽�
#define SNDF1_111 ("Sutda\\voice\\woman\\02\\win\\p1.ogg")
#define SNDF1_112 ("Sutda\\voice\\woman\\02\\win\\p2.ogg")

//��ڽ�
#define SNDF1_113 ("Sutda\\voice\\woman\\02\\win\\b1.ogg")
#define SNDF1_114 ("Sutda\\voice\\woman\\02\\win\\b2.ogg")
#define SNDF1_115 ("Sutda\\voice\\woman\\02\\win\\b3.ogg")
#define SNDF1_116 ("Sutda\\voice\\woman\\02\\win\\b4.ogg")

//�ʹڽ�
#define SNDF1_117 ("Sutda\\voice\\woman\\02\\win\\s1.ogg")
#define SNDF1_118 ("Sutda\\voice\\woman\\02\\win\\s2.ogg")
#define SNDF1_119 ("Sutda\\voice\\woman\\02\\win\\s3.ogg")

//�Ϲݽ�
#define SNDF1_120 ("Sutda\\voice\\woman\\02\\win\\m1.ogg")
#define SNDF1_121 ("Sutda\\voice\\woman\\02\\win\\m2.ogg")
#define SNDF1_122 ("Sutda\\voice\\woman\\02\\win\\m3.ogg")

//49�ٽ�
#define SNDF1_123 ("Sutda\\voice\\woman\\02\\draw\\d1.ogg")
#define SNDF1_124 ("Sutda\\voice\\woman\\02\\draw\\d2.ogg")

//�����������
#define SNDF1_125 ("Sutda\\voice\\woman\\02\\draw\\d3.ogg")

//�����̽��۽�
#define SNDF1_126 ("Sutda\\voice\\woman\\02\\eventw\\e1.ogg")
#define SNDF1_127 ("Sutda\\voice\\woman\\02\\eventw\\e2.ogg")

//�����̽¸���
#define SNDF1_128 ("Sutda\\voice\\woman\\02\\eventw\\e3.ogg")
#define SNDF1_129 ("Sutda\\voice\\woman\\02\\eventw\\e4.ogg")



#endif // !defined(AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_)
