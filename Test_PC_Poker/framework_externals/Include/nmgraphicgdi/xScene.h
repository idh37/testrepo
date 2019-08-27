
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

			// 에니메이션 데이터를 로드, 세이브
			BOOL Load(FILE* fp);
			BOOL Save(FILE* fp);

			// 스프라이트 데이터의 변동이 있을때 사용하는 함수들
			void OnDeleteSpr(int snum);
			void OnMoveSpr(int snum, int tnum);

			// 장면 추가, 삭제, 이동
			void AddScene(char* name);
			void DelScene(int num);
			void MovScene(int num, int tnum);

			// 각 노드의 포인터를 구하는 함수
			SceneData* GetScenePt(char* name, int* pos=NULL);
			SceneData* GetScenePt(int num);
			FrameData* GetFramePt(int SceneNum, int FrameNum);
			CellData*  GetCellPt(int SceneNum, int FrameNum, int CellNum);
		};


		// SceneData Class
		class NMGRAPHICGDI_CLASS SceneData
		{
		public:
			// 파일 저장용 데이터
			char SceneName[32];
			int  TotFrameNum;
			char  Reserved[32];

			// 클래스 내부 사용 데이터
			FrameData** ppFrame;

			DWORD TotPlayTime;	// 총 플레이 시간(1/1000초 단위)

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

			// 프레임 탐색용 이진트리 생성
			BOOL BuildFrameTree();

		protected:

			DWORD* pFrameNode;	// 프레임 탐색용 이진트리
			DWORD* pTimeTable;	// 각 프레임별 누적 시간 테이블

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
			char  Mirror;	// 0:정상, 1:좌우 뒤집기, 2:상하뒤집기
			char  Interpol;	// 보간 셀인가?
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
