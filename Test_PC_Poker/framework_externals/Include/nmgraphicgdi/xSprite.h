#pragma once

#include "GraphicGDIDefine.h"
#include "xAniFile.h"
#include "xScene.h"
#include "xMultiScene.h"
#include <string>
using namespace std;

namespace NMBASE
{
	namespace GRAPHICGDI
	{

		extern WORD RGB555_TO_RGB565(WORD color); // RGB555 --> RGB565
		extern WORD RGB565_TO_RGB555(WORD color); // RGB565 --> RGB555
		extern WORD RGB555_TO_BGR565(WORD color); // RGB555 --> BGR565
		extern WORD RGB565_TO_BGR565(WORD color); // RGB565 --> BGR565
		extern void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b); // 16비트(555)의 RGB를 분석한다
		extern void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b); // 16비트(565)의 RGB를 분석한다

		//================================= 스프라이트 버젼 3.0 ======================================//


		typedef struct         // 스프라이트의 파일 헤더
		{
			char  Identifier[16];      // 스프라이트 인식문자
			short Version;             // 스프라이트 버젼
			short BPD;                 // 도트당 바이트수
			short PixelFormat;         // 픽셀 포멧(값= 555, 565)
			short TotalSprNum;         // 총 스프라이트 개수
			long  SceneNum;		       // 장면 데이터의 개수
		} SPRFILEHEAD;
		typedef SPRFILEHEAD* LPSPRFILEHEAD;

		typedef struct         // 충돌, 공격좌표 구조체
		{
			short attrib;              // 좌표의 속성
			short x1,y1;               // 좌상의 좌표
			short x2,y2;               // 우하의 좌표
			char reserved[2];
		} SPRPOS;
		typedef SPRPOS* LPSPRPOS;

		typedef struct         // 각 스프라이트의 헤더
		{
			BYTE  compress;            // 압축여부
			char  Reserved1[1];
			WORD  colorkey;            // 컬러키
			short rx, ry;              // 고유 상대좌표
			short xl, yl;              // X, Y축 길이
			short mx, my;              // 중심좌표
			short hitNum;              // 충돌좌표의 개수
			short atckNum;             // 공격좌표의 개수
			unsigned long dsize;       // 스프라이트 데이타의 Byte 사이즈(압축시 필요)
			short PixelFormat;         // 픽셀 포멧(값= 555, 565, 4444, 5558)
			char  SprFormat;		   // 스프라이트 포멧(0:스프라이트 1:JPG)
			char  Dithering;		   // JPG포멧인경우 디더링 여부(0:디더링없음 1:디더링)
			BYTE  byUserVal;           // 사용자 정의 변수
			char  Reserved2[3];
		} SPRHEAD30;
		typedef SPRHEAD30* LPSPRHEAD30;

		typedef struct         // 액션 헤더
		{
			BYTE  id;                  // 액션 번호
			BYTE  attrib;              // 액션의 속성
			short frameNum;            // 액션 프레임의 개수
		} ACTIONHEAD;
		typedef ACTIONHEAD* LPACTIONHEAD;

		typedef struct         // 각 액션 프레임 구조체
		{
			short frame;               // 프레임 번호
			long  delay;               // 지연 프레임 카운트
			short rx, ry;              // 프레임의 상대좌표
			short flag;                // 프레임의 속성 플레그
		} ACTIONFRAME;
		typedef ACTIONFRAME* LPACTIONFRAME;

		//------------------------------------------------------------------------------

		typedef struct          // 스프라이트 데이타 구조체(메모리용)
		{
			//int tot;                 // 총 스프라이트 개수
			int compress;            // 압축여부
			WORD colorkey;           // 컬러키
			int xl, yl;              // X, Y축 길이
			int rx, ry;              // 상대좌표
			int mx, my;              // 중심좌표
			int hitNum;              // 충돌좌표의 개수
			int atckNum;             // 공격좌표의 개수
			unsigned long  dsize;    // 스프라이트 데이타의 사이즈(압축시 필요)
			short PixelFormat;       // 픽셀 포멧(값= 555, 565, 4444)
			char SprFormat;			 // 파일 포멧(0:스프라이트 1:JPG)
			char Dithering;			 // JPG포멧인경우 디더링 여부(0:디더링없음 1:디더링)
			BYTE  byUserVal;		 // 사용자 정의 변수
			SPRPOS* hit;             // 충돌좌표 구조체의 포인터
			SPRPOS* atck;            // 공격좌표 구조체의 포인터
			WORD*   data;            // 스프라이트 데이타 포인터
			//////////////////////
			BYTE*	pAlpha;		     // 알파 테이블 포인터(스프라이트 이미지 뒤에 붙기 때문에 별도의 할당은 없음)
			//////////////////////
			unsigned long  jpgsize;  // JPG포맷인경우 이미지 사이즈
			BYTE*	pJpgData;		 // JPG포맷인경우 데이터 포인터
			//////////////////////
		} SPRITE30;

		//------------------------------------------------------------------------------

		// 멀티씬의 사운드 트랙 정보 열거용 구조체
		struct QUERYMSCENE_SOUND
		{
			int   NowMScene;	// 현재 조회중인 멀티씬(-1은 쿼리 시작전을 의미함)
			int   NowTrack;		// 현재 조회중인 트랙(-1은 쿼리 시작전을 의미함)
			DWORD PlayTime;		// 사운드가 플레이 될 시간(딜레이)
			char* SndFile;		// 사운드 파일명
			int   Volume;		// 사운드 볼륨

			QUERYMSCENE_SOUND()
			{
				Reset();
			}

			// 열거 구조체 초기화 - 쿼리하기 전에 1회 호출
			void Reset()
			{
				NowMScene = -1;
				NowTrack = -1;
				PlayTime = 0;
				SndFile = "";
				Volume = 100;
			}
		};

		// 멀티씬의 스크립트 트랙 정보 열거용 구조체
		struct QUERYMSCENE_SCRIPT
		{
			int   NowMScene;	// 현재 조회중인 멀티씬(-1은 쿼리 시작전을 의미함)
			int   NowTrack;		// 현재 조회중인 트랙(-1은 쿼리 시작전을 의미함)
			DWORD PlayTime;		// 스크립트가 플레이 될 시간(딜레이)
			char* ScriptName;	// 스크립트 이름
			char* Script;		// 스크립트 내용

			QUERYMSCENE_SCRIPT()
			{
				Reset();
			}

			// 열거 구조체 초기화 - 쿼리하기 전에 1회 호출
			void Reset()
			{
				NowMScene = -1;
				NowTrack = -1;
				PlayTime = 0;
				ScriptName = "";
				Script = "";
			}
		};

		//------------------------------------------------------------------------------

		class NMGRAPHICGDI_CLASS xSprite
		{
		protected:

			virtual void PutErrStr(char *fmt, ...) // 에러스트링 입력 함수
			{
				// 아무것도 안함(상속 받은 클래스에서 처리)
			}

			std::string			m_strPath;
		public:

			//////// 공통 변수 선언
			SPRFILEHEAD sfhead;                  // 스프라이트 파일 헤더
			SPRITE30 *spr;                       // 스프라이트 정보 구조체
			xAniFile ani;                        // 구버전 에니메이션 데이터 클레스
			xScene scene;                        // 신버전 에니메이션 데이터 클레스
			xMultiScene multiscene;              // 멀티씬 데이터 클레스

			//////// 생성자, 소멸자
			xSprite();
			~xSprite();

			//////// 스프라이트 로드,세이브,제거함수
			BOOL Load(char *name);
			BOOL Load(char *name,  int ScreenRGBMode);
			void Remove();

			//////// 유틸리티 함수
			inline int GetTotalSpr() { return sfhead.TotalSprNum; }
			inline BOOL IsValidSpr(int spn) { return (spn >= 0 && spn < sfhead.TotalSprNum); }

			//////// 특수기능 함수
			BOOL Compress(LONG snum);                       // 스프라이트 프레임 압축
			BOOL DeCompress(LONG snum);                     // 스프라이트 프레임 압축 해제
			BOOL ChangePixelFormat(int NewForm);            // 픽셀 포맷을 바꾼다(555,565,-565)
			void OptimizeSize(int snum, BOOL fixPos=TRUE);	// 스프라이트 사이즈를 최적화한다
			BOOL TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey);  // 특정 이미지를 스프라이트로 만든다(복사하지 않고 포인터를 그대로 가져감)
			BOOL TransToSprByCopy(int xl, int yl, WORD *pImage, WORD wColorKey); // 특정 이미지를 스프라이트로 만든다(내부적으로 할당하여 복사함)
			BOOL NewSpace(int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
			BOOL Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);  // 스프라이트 컷 삽입
			BOOL InsertByCopy(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
			string &GetFilePath(void){return m_strPath;}

			//////// 장면 애니메이션 관련 함수
			DWORD GetScenePlayTime(int scn);					// 장면의 총 플레이 타임을 구함
			BOOL  IsSceneTimeOver(int scn, DWORD time);			// 주어진 시간이 장면의 플레이가 끝날 시간인가?
			int   GetScene(char* name);							// 이름으로 장면 번호를 구함(-1을 리턴하면 실패)
			int   GetTotFrame(int scn);							// 장면의 총 프레임을 구함
			int   GetNowFrame(int scn, DWORD time, BOOL bLoop);	// 주어진 시간에 플레이 될 프레임을 구함 : 실패할 경우 -1을 리턴
			BOOL  GetFrameTime(int scn, int frame, DWORD* pSTime, DWORD* pETime);	// 특정 프레임의 시작 시간과 끝 시간을 구함

			SceneData* GetScenePt(int scn);						// 장면 데이터의 포인터를 구함
			FrameData* GetFramePt(int scn, int frame);			// 프레임 데이터의 포인터를 구함
			CellData*  GetCellPt(int scn, int frame, int cell);	// 셀 데이터의 포인터를 구함

			//////// 멀티씬 애니메이션 관련 함수
			DWORD GetMScenePlayTime(int mscn);					// 멀티씬의 총 플레이 타임을 구함
			BOOL IsMSceneTimeOver(int mscn, DWORD time);		// 주어진 시간이 멀티씬의 플레이가 끝날 시간인가?

			// 내부 사용 목적의 함수들
			BOOL CalcTrackPlayTime(int mscn, int track, DWORD* pStartTime, DWORD* pEndTime);
			BOOL CalcMScenePlayTime(int mscn, DWORD* pStartTime, DWORD* pEndTime);
			BOOL UpdateMScenePlayTime(int mscn);
			void UpdateAllMScenePlayTime();

			// 사운드 및 스크립트 쿼리 함수(FALSE를 리턴할때까지 반복 호출할 것) - startime은 endtime보다 항상 작아야 함
			BOOL QueryAllSound(QUERYMSCENE_SOUND* pQuery);
			BOOL QueryAllScript(QUERYMSCENE_SCRIPT* pQuery);
			BOOL QueryMSceneSound(QUERYMSCENE_SOUND* pQuery, int multiscn, DWORD starttime, DWORD endtime, BOOL loop=TRUE);
			BOOL QueryMSceneScript(QUERYMSCENE_SCRIPT* pQuery, int multiscn, DWORD starttime, DWORD endtime, BOOL loop=TRUE);

			/////////// static member

			static BOOL bSpriteEditMode;	// 스프라이트 편집 모드인가? - 일반적인 상황에서는 항상 FALSE임(툴에서만 TRUE로 사용)

			inline static WORD RGB565(int r,int g,int b)
			{	
				return 	(WORD) (  (((WORD)(((r)/8)<<11))) | (((WORD)(((g)/4)<<5))) | ((WORD)(b)/8)  );
			}

			inline static WORD	RGB555(int r,int g,int b)
			{	
				//g>>=1;
				return 	(WORD) (  (((WORD)(((r)/8)<<10))) | (((WORD)(((g)/8)<<5))) | ((WORD)(b)/8)  );
			}

			static WORD* Make16bitFromJpeg(LPCTSTR fname, int* width, int* height, BOOL bDither);
			static WORD* Make16bitFromJpeg(FILE *fp, int* width, int* height, BOOL bDither);
		};

	}//namespace GRAPHICGDI

}//namespace NMBASE
