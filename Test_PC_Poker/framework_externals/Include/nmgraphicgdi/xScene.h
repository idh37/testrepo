
#pragma once

#include "GraphicGDIDefine.h"

namespace NMBASE
{
	namespace GRAPHICGDI
	{

		class SceneData;
		class FrameData;
		class CellData;


		// xScene Class
		class NMGRAPHICGDI_CLASS xScene
		{
		public:

			int  SceneVer;
			int  TotSceneNum;
			char Reserved[32];

			SceneData** ppScene;

			xScene();
			~xScene();
			void Destroy();

			// ���ϸ��̼� �����͸� �ε�, ���̺�
			BOOL Load(FILE* fp);
			BOOL Save(FILE* fp);

			// ��������Ʈ �������� ������ ������ ����ϴ� �Լ���
			void OnDeleteSpr(int snum);
			void OnMoveSpr(int snum, int tnum);

			// ��� �߰�, ����, �̵�
			void AddScene(char* name);
			void DelScene(int num);
			void MovScene(int num, int tnum);

			// �� ����� �����͸� ���ϴ� �Լ�
			SceneData* GetScenePt(char* name, int* pos=NULL);
			SceneData* GetScenePt(int num);
			FrameData* GetFramePt(int SceneNum, int FrameNum);
			CellData*  GetCellPt(int SceneNum, int FrameNum, int CellNum);
		};


		// SceneData Class
		class NMGRAPHICGDI_CLASS SceneData
		{
		public:
			// ���� ����� ������
			char SceneName[32];
			int  TotFrameNum;
			char  Reserved[32];

			// Ŭ���� ���� ��� ������
			FrameData** ppFrame;

			DWORD TotPlayTime;	// �� �÷��� �ð�(1/1000�� ����)

			SceneData();
			~SceneData();
			void Destroy();
			void Set(char* name);

			int  AddFrame(DWORD delay);
			int  AddFrame(FrameData* pFrame);
			void DelFrame(int num);
			void MovFrame(int num, int tnum);
			int  MovFrames(int totnum, int* plist, int tnum);
			int  CopyFrames(int totnum, int* plist, int tnum);
			FrameData* GetPtrFrame(int num);

			int SearchFrame(DWORD time, BOOL bLoop);
			float GetFrameDelta(int frame, DWORD time, BOOL bLoop);
			BOOL GetFrameTime(int frame, DWORD* pSTime, DWORD* pETime);

			// ������ Ž���� ����Ʈ�� ����
			BOOL BuildFrameTree();

		protected:

			DWORD* pFrameNode;	// ������ Ž���� ����Ʈ��
			DWORD* pTimeTable;	// �� �����Ӻ� ���� �ð� ���̺�

			int RecursiveBuildTree(int si, int ei, DWORD* pdata, int& pos, DWORD* delay);
		};

		// FrameData Class
		class NMGRAPHICGDI_CLASS FrameData
		{
		public:
			int   TotCellNum;
			DWORD Delay;
			int   UserVal;
			char  Reserved[8];

			CellData** ppCell;

			FrameData();
			~FrameData();
			void Destroy();
			void Set(int delay);

			int  AddCell(int sprno, int rx, int ry, char effect, char grade, WORD color, char mirror, int zoomx, int zoomy, int angle, char interpol, char bias);
			int  AddCell(CellData* pCell);
			void DelCell(int num);
			void MovCell(int num, int tnum);
			int  MovCells(int totnum, int* plist, int tnum);
			int  CopyCells(int totnum, int* plist, int tnum);
			void CopyFrom(FrameData* pSrc);

			CellData* GetPtrCell(int num);
		};

		// CelData Class
		class NMGRAPHICGDI_CLASS CellData
		{
		public:
			short SprNo;
			short Rx, Ry;
			char  Effect;
			char  Grade;
			WORD  Color;
			char  Mirror;	// 0:����, 1:�¿� ������, 2:���ϵ�����
			char  Interpol;	// ���� ���ΰ�?
			short ZoomX;
			short ZoomY;
			char  Bias;
			char  Reserved1[1];
			short RotAngle;
			char  Reserved2[14];
			//short ZoomXM;
			//short ZoomYM;

			CellData();
			~CellData();

			void ClearMember();
			void Destroy();

			void Set(int sprno, int rx, int ry, char effect, char grade, WORD color, char mirror, int zoomx, int zoomy, int angle, char interpol, char bias);
			void CopyFrom(CellData* pSrc);
		};

	}//namespace GRAPHICGDI

}//namespace NMBASE
