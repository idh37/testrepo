/**    
@file    Avatar.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   �ƹ�Ÿ ���� 
*/

#pragma once

#include "AvatarDefine.h"

namespace NMBASE
{
	namespace AVATAR
	{

		//////////////////////////////////////////////////////////////////////

		// ���� ����Ʈ�� ���� �ƹ�Ÿ ������ �����ϱ� ���Ͽ� ȣ���ϴ� �Լ�(InitAvatarModule() ������ ȣ��Ǿ�� ��)
		// ���������� ��� "daum", �ϳ����������� ��� "hanafos"�� �ѱ�(�ݸ����� ���� ȣ���� �ʿ䰡 ����)
		NMAVATAR_API void PresetAvatarModuleSetSiteName(LPCTSTR strSiteName);

		// �ƹ�Ÿ ��� �ʱ�ȭ �Լ�(����� ������ ���α׷� ����� �ڵ����� ����ȴ�)
		NMAVATAR_API BOOL InitAvatarModule();

		// �׽�Ʈ�� �ʱ�ȭ �Լ�
		NMAVATAR_API BOOL InitAvatarModule_ForTest(LPCSTR strBaseDir);

		// �ƹ�Ÿ ��� �⺻ ������ ������ �����ϴ� �Լ�
		NMAVATAR_API BOOL SetBaseAvatarDir(char *strFolder);

		// �ƹ�Ÿ ��� �⺻ ������ ��� �Լ�
		NMAVATAR_API BOOL GetBaseAvatarDir(char *strBuffer, int buffersize);

		//////////////////////////////////////////////////////////////////////
		// �ƹ�Ÿ Ŭ����

		class CAvatarResource;

		class NMAVATAR_CLASS CAvatar  
		{
		public:

			CAvatar();
			virtual ~CAvatar();

			// �ƹ�Ÿ �ʱ�ȭ �� ����
			BOOL Create();
			BOOL Create(short *pAvatarInfo);
			BOOL Create(int *pAvatarInfo);
			BOOL Create(int width, int height, short *pAvatarInfo);
			BOOL Create(int width, int height, int *pAvatarInfo);

			void ClearMember();
			void Destroy();											// �ƹ�Ÿ �ı�

			BOOL SetAvatar(short *pAvatarInfo);						// �ƹ�Ÿ ���� ����
			BOOL SetAvatar(int *pAvatarInfo);						// �ƹ�Ÿ ���� ����
			BOOL SetForceAvatar(short *pAvatarInfo, BOOL bAutoPair=FALSE);	// �ƹ�Ÿ ���� ����(������ ����, AutoPair�ɼ��� TRUE�� ��� ��Ʈ�� ���� ���� ������ �̷������ �����۸� �ڵ� ����)
			BOOL SetForceAvatar(int *pAvatarInfo, BOOL bAutoPair=FALSE);	// �ƹ�Ÿ ���� ����(������ ����, AutoPair�ɼ��� TRUE�� ��� ��Ʈ�� ���� ���� ������ �̷������ �����۸� �ڵ� ����)
			BOOL SetSmallAvatar(short *pAvatarInfo);				// ���� �ƹ�Ÿ ���� ����
			BOOL SetSmallAvatar(int *pAvatarInfo);					// ���� �ƹ�Ÿ ���� ����
			BOOL SetBackgroundVisible(BOOL bShow);					// ����� 0�� ���̾��� ǥ�� ����(Default: FALSE)
			BOOL SetSize(int width, int height);					// �ƹ�Ÿ ������ ����
			BOOL SetOffset(int ox, int oy);							// �ƹ�Ÿ ������ ����(����Ʈ 0,0)
			BOOL ResetAnimation();									// �ƹ�Ÿ ���ϸ��̼� ����
			BOOL SetAnimationTime(WORD tick);						// �ƹ�Ÿ ���ϸ��̼� Ÿ���� ����
			BOOL SetBackBitmap(HBITMAP hBmp, int sx, int sy);		// �ƹ�Ÿ ��׶��� ��Ʈ�� �̹��� ����(DC�� �׸��� ��� ��� �̹����� �����Ͽ� ������ �׸��µ� ���) 
			// ����) ��Ʈ���� ������ ���� Draw()ȣ��� ������۸��� �ʿ����(���������� �����)
			BOOL IsAvatarDownloading();								// �ƹ�Ÿ �̹����� �ٿ�ε������� �˻�

			// �ƹ�Ÿ�� DC�� �׸���(��׶��� ��Ʈ���� �������� �ʴ� ���� Mask�� �����ϱ� ������ �ӵ��� ����)
			BOOL Draw(int xp, int yp, HDC hdcDest, BOOL bMirror);	
			// �ƹ�Ÿ�� �޸𸮿� �׸���(����Ʈ�� 16bit RGB555)
			BOOL Draw(int xp, int yp, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat = APF_RGB555);
			// �ƹ�Ÿ�� DC�� �׸���( �ε巴�� Ȯ��/��Ұ� ����� )
			BOOL Draw_SmoothZoom(int xp, int yp, HDC hdcDest, int tx, int ty, COLORREF crBackColor, BOOL bMirror);

			//////// static members
			static char* GetAvatarStatus();							// �ƹ�Ÿ DLL�� ���¸� ��Ʈ������ ����

		protected:
			BOOL SetAvatarUnsignedShort(HAVATAR h, unsigned short *pLayerItem);
			BOOL SetAvatarInt(HAVATAR h, int *pLayerItem);
			BOOL raw_SetAvatar(HAVATAR h, int *pLayerItem);
			BOOL PrepareResource(CAvatarResource* pRes, int *item, BOOL bSmall);
			BOOL PrepareResource(HAVATAR h, int *item, BOOL bSmall);
			static char* GetAvatarStatusString();
			BOOL DrawAvatarToDC_SmoothZoom(int xp, int yp, HAVATAR h, HDC hdcDest, int tx, int ty, COLORREF crBackColor, BOOL bMirror);
			BOOL DrawAvatarToMemory(int xp, int yp, HAVATAR h, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat);
			BOOL DrawAvatarToDC(int xp, int yp, HAVATAR h, HDC hdcDest, BOOL bMirror);
			BOOL IsAvatarDownloading(HAVATAR h);
			BOOL SetAvatarBackBitmap(HAVATAR h, HBITMAP hBmp, int srcx, int srcy);
			BOOL SetAvatarAnimationTime(HAVATAR h, WORD tick);
			BOOL ResetAvatarAnimation(HAVATAR h);
			BOOL SetAvatarSize(HAVATAR h, int width, int height);
			BOOL SetAvatarOffset(HAVATAR h, int ox, int oy);
			BOOL SetBackgroundVisible(HAVATAR h, BOOL bShow);
			BOOL SetSmallAvatarEx(HAVATAR h, unsigned short *pLayerItem);
			BOOL SetSmallAvatar(HAVATAR h, short *pLayerItem);
			BOOL SetSmallAvatar(HAVATAR h, int *pLayerItem);
			BOOL SetForceAvatar(HAVATAR h, short *pLayerItem, BOOL bAutoPair);
			BOOL SetForceAvatar(HAVATAR h, int *pLayerItem, BOOL bAutoPair);
			BOOL DestroyAvatarResource(HAVATAR h);
			HAVATAR CreateAvatarResource();

		protected:

			HAVATAR hAvatar;

			BOOL m_bCheckMir;
			BOOL m_bLastDrawMir;

			int m_OrgAvatarInfo[MAX_LAYER_4HERE];
			int m_MirAvatarInfo[MAX_LAYER_4HERE];

			void PrepareMirror(BOOL bMirror);
		};

	}//namespace AVATAR
}//namespace NMBASE