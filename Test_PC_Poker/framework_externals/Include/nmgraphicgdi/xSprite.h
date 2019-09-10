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
		extern void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b); // 16��Ʈ(555)�� RGB�� �м��Ѵ�
		extern void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b); // 16��Ʈ(565)�� RGB�� �м��Ѵ�

		//================================= ��������Ʈ ���� 3.0 ======================================//


		typedef struct         // ��������Ʈ�� ���� ���
		{
			char  Identifier[16];      // ��������Ʈ �νĹ���
			short Version;             // ��������Ʈ ����
			short BPD;                 // ��Ʈ�� ����Ʈ��
			short PixelFormat;         // �ȼ� ����(��= 555, 565)
			short TotalSprNum;         // �� ��������Ʈ ����
			long  SceneNum;		       // ��� �������� ����
		} SPRFILEHEAD;
		typedef SPRFILEHEAD* LPSPRFILEHEAD;

		typedef struct         // �浹, ������ǥ ����ü
		{
			short attrib;              // ��ǥ�� �Ӽ�
			short x1,y1;               // �»��� ��ǥ
			short x2,y2;               // ������ ��ǥ
			char reserved[2];
		} SPRPOS;
		typedef SPRPOS* LPSPRPOS;

		typedef struct         // �� ��������Ʈ�� ���
		{
			BYTE  compress;            // ���࿩��
			char  Reserved1[1];
			WORD  colorkey;            // �÷�Ű
			short rx, ry;              // ���� �����ǥ
			short xl, yl;              // X, Y�� ����
			short mx, my;              // �߽���ǥ
			short hitNum;              // �浹��ǥ�� ����
			short atckNum;             // ������ǥ�� ����
			unsigned long dsize;       // ��������Ʈ ����Ÿ�� Byte ������(����� �ʿ�)
			short PixelFormat;         // �ȼ� ����(��= 555, 565, 4444, 5558)
			char  SprFormat;		   // ��������Ʈ ����(0:��������Ʈ 1:JPG)
			char  Dithering;		   // JPG�����ΰ�� ����� ����(0:��������� 1:�����)
			BYTE  byUserVal;           // ����� ���� ����
			char  Reserved2[3];
		} SPRHEAD30;
		typedef SPRHEAD30* LPSPRHEAD30;

		typedef struct         // �׼� ���
		{
			BYTE  id;                  // �׼� ��ȣ
			BYTE  attrib;              // �׼��� �Ӽ�
			short frameNum;            // �׼� �������� ����
		} ACTIONHEAD;
		typedef ACTIONHEAD* LPACTIONHEAD;

		typedef struct         // �� �׼� ������ ����ü
		{
			short frame;               // ������ ��ȣ
			long  delay;               // ���� ������ ī��Ʈ
			short rx, ry;              // �������� �����ǥ
			short flag;                // �������� �Ӽ� �÷���
		} ACTIONFRAME;
		typedef ACTIONFRAME* LPACTIONFRAME;

		//------------------------------------------------------------------------------

		typedef struct          // ��������Ʈ ����Ÿ ����ü(�޸𸮿�)
		{
			//int tot;                 // �� ��������Ʈ ����
			int compress;            // ���࿩��
			WORD colorkey;           // �÷�Ű
			int xl, yl;              // X, Y�� ����
			int rx, ry;              // �����ǥ
			int mx, my;              // �߽���ǥ
			int hitNum;              // �浹��ǥ�� ����
			int atckNum;             // ������ǥ�� ����
			unsigned long  dsize;    // ��������Ʈ ����Ÿ�� ������(����� �ʿ�)
			short PixelFormat;       // �ȼ� ����(��= 555, 565, 4444)
			char SprFormat;			 // ���� ����(0:��������Ʈ 1:JPG)
			char Dithering;			 // JPG�����ΰ�� ����� ����(0:��������� 1:�����)
			BYTE  byUserVal;		 // ����� ���� ����
			SPRPOS* hit;             // �浹��ǥ ����ü�� ������
			SPRPOS* atck;            // ������ǥ ����ü�� ������
			WORD*   data;            // ��������Ʈ ����Ÿ ������
			//////////////////////
			BYTE*	pAlpha;		     // ���� ���̺� ������(��������Ʈ �̹��� �ڿ� �ٱ� ������ ������ �Ҵ��� ����)
			//////////////////////
			unsigned long  jpgsize;  // JPG�����ΰ�� �̹��� ������
			BYTE*	pJpgData;		 // JPG�����ΰ�� ������ ������
			//////////////////////
		} SPRITE30;

		//------------------------------------------------------------------------------

		// ��Ƽ���� ���� Ʈ�� ���� ���ſ� ����ü
		struct QUERYMSCENE_SOUND
		{
			int   NowMScene;	// ���� ��ȸ���� ��Ƽ��(-1�� ���� �������� �ǹ���)
			int   NowTrack;		// ���� ��ȸ���� Ʈ��(-1�� ���� �������� �ǹ���)
			DWORD PlayTime;		// ���尡 �÷��� �� �ð�(������)
			char* SndFile;		// ���� ���ϸ�
			int   Volume;		// ���� ����

			QUERYMSCENE_SOUND()
			{
				Reset();
			}

			// ���� ����ü �ʱ�ȭ - �����ϱ� ���� 1ȸ ȣ��
			void Reset()
			{
				NowMScene = -1;
				NowTrack = -1;
				PlayTime = 0;
				SndFile = "";
				Volume = 100;
			}
		};

		// ��Ƽ���� ��ũ��Ʈ Ʈ�� ���� ���ſ� ����ü
		struct QUERYMSCENE_SCRIPT
		{
			int   NowMScene;	// ���� ��ȸ���� ��Ƽ��(-1�� ���� �������� �ǹ���)
			int   NowTrack;		// ���� ��ȸ���� Ʈ��(-1�� ���� �������� �ǹ���)
			DWORD PlayTime;		// ��ũ��Ʈ�� �÷��� �� �ð�(������)
			char* ScriptName;	// ��ũ��Ʈ �̸�
			char* Script;		// ��ũ��Ʈ ����

			QUERYMSCENE_SCRIPT()
			{
				Reset();
			}

			// ���� ����ü �ʱ�ȭ - �����ϱ� ���� 1ȸ ȣ��
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

			virtual void PutErrStr(char *fmt, ...) // ������Ʈ�� �Է� �Լ�
			{
				// �ƹ��͵� ����(��� ���� Ŭ�������� ó��)
			}

			std::string			m_strPath;
		public:

			//////// ���� ���� ����
			SPRFILEHEAD sfhead;                  // ��������Ʈ ���� ���
			SPRITE30 *spr;                       // ��������Ʈ ���� ����ü
			xAniFile ani;                        // ������ ���ϸ��̼� ������ Ŭ����
			xScene scene;                        // �Ź��� ���ϸ��̼� ������ Ŭ����
			xMultiScene multiscene;              // ��Ƽ�� ������ Ŭ����

			//////// ������, �Ҹ���
			xSprite();
			~xSprite();

			//////// ��������Ʈ �ε�,���̺�,�����Լ�
			BOOL Load(char *name);
			BOOL Load(char *name,  int ScreenRGBMode);
			void Remove();

			//////// ��ƿ��Ƽ �Լ�
			inline int GetTotalSpr() { return sfhead.TotalSprNum; }
			inline BOOL IsValidSpr(int spn) { return (spn >= 0 && spn < sfhead.TotalSprNum); }

			//////// Ư����� �Լ�
			BOOL Compress(LONG snum);                       // ��������Ʈ ������ ����
			BOOL DeCompress(LONG snum);                     // ��������Ʈ ������ ���� ����
			BOOL ChangePixelFormat(int NewForm);            // �ȼ� ������ �ٲ۴�(555,565,-565)
			void OptimizeSize(int snum, BOOL fixPos=TRUE);	// ��������Ʈ ����� ����ȭ�Ѵ�
			BOOL TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey);  // Ư�� �̹����� ��������Ʈ�� �����(�������� �ʰ� �����͸� �״�� ������)
			BOOL TransToSprByCopy(int xl, int yl, WORD *pImage, WORD wColorKey); // Ư�� �̹����� ��������Ʈ�� �����(���������� �Ҵ��Ͽ� ������)
			BOOL NewSpace(int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
			BOOL Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);  // ��������Ʈ �� ����
			BOOL InsertByCopy(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress);
			string &GetFilePath(void){return m_strPath;}

			//////// ��� �ִϸ��̼� ���� �Լ�
			DWORD GetScenePlayTime(int scn);					// ����� �� �÷��� Ÿ���� ����
			BOOL  IsSceneTimeOver(int scn, DWORD time);			// �־��� �ð��� ����� �÷��̰� ���� �ð��ΰ�?
			int   GetScene(char* name);							// �̸����� ��� ��ȣ�� ����(-1�� �����ϸ� ����)
			int   GetTotFrame(int scn);							// ����� �� �������� ����
			int   GetNowFrame(int scn, DWORD time, BOOL bLoop);	// �־��� �ð��� �÷��� �� �������� ���� : ������ ��� -1�� ����
			BOOL  GetFrameTime(int scn, int frame, DWORD* pSTime, DWORD* pETime);	// Ư�� �������� ���� �ð��� �� �ð��� ����

			SceneData* GetScenePt(int scn);						// ��� �������� �����͸� ����
			FrameData* GetFramePt(int scn, int frame);			// ������ �������� �����͸� ����
			CellData*  GetCellPt(int scn, int frame, int cell);	// �� �������� �����͸� ����

			//////// ��Ƽ�� �ִϸ��̼� ���� �Լ�
			DWORD GetMScenePlayTime(int mscn);					// ��Ƽ���� �� �÷��� Ÿ���� ����
			BOOL IsMSceneTimeOver(int mscn, DWORD time);		// �־��� �ð��� ��Ƽ���� �÷��̰� ���� �ð��ΰ�?

			// ���� ��� ������ �Լ���
			BOOL CalcTrackPlayTime(int mscn, int track, DWORD* pStartTime, DWORD* pEndTime);
			BOOL CalcMScenePlayTime(int mscn, DWORD* pStartTime, DWORD* pEndTime);
			BOOL UpdateMScenePlayTime(int mscn);
			void UpdateAllMScenePlayTime();

			// ���� �� ��ũ��Ʈ ���� �Լ�(FALSE�� �����Ҷ����� �ݺ� ȣ���� ��) - startime�� endtime���� �׻� �۾ƾ� ��
			BOOL QueryAllSound(QUERYMSCENE_SOUND* pQuery);
			BOOL QueryAllScript(QUERYMSCENE_SCRIPT* pQuery);
			BOOL QueryMSceneSound(QUERYMSCENE_SOUND* pQuery, int multiscn, DWORD starttime, DWORD endtime, BOOL loop=TRUE);
			BOOL QueryMSceneScript(QUERYMSCENE_SCRIPT* pQuery, int multiscn, DWORD starttime, DWORD endtime, BOOL loop=TRUE);

			/////////// static member

			static BOOL bSpriteEditMode;	// ��������Ʈ ���� ����ΰ�? - �Ϲ����� ��Ȳ������ �׻� FALSE��(�������� TRUE�� ���)

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
