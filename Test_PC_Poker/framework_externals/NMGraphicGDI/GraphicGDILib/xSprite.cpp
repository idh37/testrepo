
#include "StdAfx.h"
#include "JpegFile.h"	// JPEG 모듈
#include "dither.h"     // 디더링 모듈
#include "xSprite.h"


namespace NMBASE
{
	namespace GRAPHICGDI
	{

		///////////////////////////////////////////////////////////////////////////


		/*--------------------------------------------------------------------

		<  0 번 압축 포맺 구조(헤더를 제외한 데이타 부분) >

		Byte(s)

		4 * height - 각라인의 시작 오프셋의 거리가 담긴 배열
		2          - 패턴(pattern ) 수 ---------------------------+
		2          - 투명색 수 ---------------+                   | 세로길이 만큼
		2          - 그릴 점수                | 패턴수만큼 반복   | 반복
		size       - 데이타(점수*2)-----------+ ------------------+

		## 데이타의 크기는 점수*2이다(16비트 이므로) 즉, 실제 바이트수를 의미한다
		## 각라인의 거리 : 데이타의 처음에서부터 각 라인이 시작되는 곳까지의
		거리를 나타낸다.(각라인의 길이가 아님)
		(클리핑시 속도를 높이고 처리를 용이하게하기위함)
		(주의: 거리1은 실제 2바이트길이임)
		## 투명색수와 그릴데이타수는 WORD형이다.
		--------------------------------------------------------------------*/



		//#define  TRANS  0

		typedef struct {
			WORD skipb;
			WORD putb;
		} BLKPATTERN;

		/*---------------------------------------------------------
		버퍼에 이미지를 압축하여 복사하는 함수

		WORD *image   : 입력될 이미지 포인터(16비트)
		int  xl       : 입력 이미지의 가로길이
		int  yl       : 입력 이미지의 세로길이
		WORD **output : 출력될 압축 이미지를 받을 포인터
		(반드시 NULL 이어야한다, 이중포인터임에 주의)
		WORD TRANS    : Color Key

		리턴값  -->  압축된 사이즈(실패시 0)

		주의 ) 원본 이미지는 압축이미지를 받은후
		쓸모 없어지므로 이를 직접 free 해주어야한다.
		-----------------------------------------------------------*/
		unsigned long EncodeSpr0(WORD *image, int xl, int yl, WORD **output, WORD TRANS)
		{
			if(!image || xl <= 0 || yl <= 0) return 0;

			WORD i,k,cnt;
			short pNum;        // 패턴의 개수
			BLKPATTERN pat[1024] = {0,};

			unsigned long bp=0;         // 버퍼 인덱스
			unsigned long dsize = 0;        // 압축된 전체 크기

			// 임시 버퍼 할당(가로크기의 4배 + LDT테이블공간) 
			// 가로길이는 최소 8픽셀 이상으로 가정(8보다 작은 경우 4배를 하더라도 버퍼가 모자랄 수 있음)
			WORD *buf = (WORD *)malloc((max(8,xl)*4)*yl+(4*yl));
			if(buf==NULL) return 0;

			bp += yl*2;   // 각 라인의 거리가 담긴 공간은 건너띔(주의: buf가 WORD형 포인터이므로... yl*2)
			for(k=0; k<yl; k++)
			{
				for(i=0,pNum=0; i<xl;) {
					// skip bytes
					for(cnt=0; i<xl && *(image+i)==TRANS; cnt++,i++) ;
					pat[pNum].skipb = cnt;

					// put bytes
					for(cnt=0; i<xl && *(image+i)!=TRANS; cnt++,i++) ;
					pat[pNum].putb = cnt;

					pNum++;
				}

				// Line-Distance-Table
				memcpy(buf+k*2, &bp, 4); // 주의: buf가 WORD형 포인터이므로... k*2

				// 라인당 페턴의 개수
				*(buf+bp) = pNum;
				bp++;

				// Copy pattern to temporary buffers
				for(i=0;i<pNum;i++) {
					memcpy(&buf[bp], &pat[i],sizeof(BLKPATTERN)); 
					bp+=2;
					if(pat[i].putb!=0) {
						// Copy Image
						memcpy(buf+bp, image+pat[i].skipb, pat[i].putb*2); // for 16bit
						bp+=pat[i].putb;
					}

					image += pat[i].skipb + pat[i].putb;
				}
			}

			dsize=bp*2; // LDT테이블 + 이미지크기
			*output = (WORD *)malloc(dsize);
			memcpy(*output, buf, dsize); // 임시버퍼의 내용을 복사
			free(buf);
			return(dsize);
		}

		/*---------------------------------------------------------
		압축스프라이트를 버퍼에 디코딩하는 함수
		(버퍼의 크기는 '가로길이*세로길이' 이어야한다)
		-----------------------------------------------------------*/
		void DecodeSpr0( WORD *tar, int xl, int yl, WORD *sor, WORD TRANS)
		{
			int  i,j;
			int  sbyte,pbyte;
			short cnt;

			for(i=0; i<yl; i++) // 우선 모두 투명색을 세팅한다
			{
				for(j=0; j<xl; j++) *(tar+(xl*i)+j) = TRANS; 
			}

			sor += yl*2; // 라인거리 데이타를 건너띔
			// 클리핑이 없으므로 필요 없음

			//sor += *((long*)sor);    // 찍기 시작할 라인으로 이동

			for(i=0;i<yl;i++,tar+=xl) 
			{
				cnt = *sor;      // 패턴 개수 얻기
				sor++;
				sbyte=0;
				while(cnt--) {

					sbyte+=(*(WORD*)sor); // 투명색수 얻기
					sor++;

					pbyte =(*(WORD*)sor); // 점수 얻기
					sor++;

					memcpy(tar+sbyte, sor, pbyte<<1); // 이미지 찍어주기 (16비트이므로 *2)

					sor+=pbyte;
					sbyte+=pbyte;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////

		//class xDraw;
		//extern static int xDraw::PixelFormat; // 스크린의 Pixel Format을 알아야 하기 때문

		WORD RGB555_TO_RGB565(WORD color) // RGB555 --> RGB565
		{
			WORD r = (color & 0x7C00)<<1;
			WORD g = (color & 0x03E0)<<1;
			WORD b = (color & 0x001F);
			return (r|g|b);
		}

		WORD RGB565_TO_RGB555(WORD color) // RGB565 --> RGB555
		{
			WORD r = (color & 0xF800)>>1;
			WORD g = (color & 0x03E0)>>1;
			WORD b = (color & 0x07E0);
			return (r|g|b);
		}

		WORD RGB555_TO_BGR565(WORD color) // RGB555 --> BGR565
		{
			WORD r = (color & 0x7C00)>>10;
			WORD g = (color & 0x03E0)<<1;
			WORD b = (color & 0x001F)<<11;
			return (b|g|r);
		}

		WORD RGB565_TO_BGR565(WORD color) // RGB565 --> BGR565
		{
			WORD r = (color & 0xF800)>>11;
			WORD g = (color & 0x03E0);
			WORD b = (color & 0x07E0)<<11;
			return (b|g|r);
		}

		void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b) // 16비트(555)의 RGB를 분석한다
		{
			*r = (color & 0x7C00)>>10;
			*g = (color & 0x03E0)>>5;
			*b = (color & 0x001F);
		}

		void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b) // 16비트(565)의 RGB를 분석한다
		{
			*r = (color & 0xF800)>>11;
			*g = (color & 0x03E0)>>5; 
			*b = (color & 0x07E0);
			// 참고) 565에선 Green이 두배이므로 쉬프트를 한번 더해야 
			//       RGB 조합시에 정확한 색이 나온다
			//       그러나 여기선 한번 더 하지 않았다.
		}


		//================================ 스프라이트 버젼 3.0 클래스 =================================//

		BOOL xSprite::bSpriteEditMode = FALSE;

		xSprite::xSprite()
		{
			ZeroMemory(&sfhead, sizeof(SPRFILEHEAD));
			spr=NULL;
		}

		xSprite::~xSprite()
		{
			Remove();
		}


		/*--------------------------------------------------------------------------
		로딩된 스프라이트를 삭제
		---------------------------------------------------------------------------*/
		void xSprite::Remove()
		{
			int i;
			if(spr != NULL)  {
				ani.Destroy();
				scene.Destroy();
				multiscene.Destroy();

				for(i=0; i < sfhead.TotalSprNum; i++) {
					if( spr[i].hit != NULL ) { free(spr[i].hit); spr[i].hit = NULL; }
					if( spr[i].atck != NULL ) { free(spr[i].atck); spr[i].atck = NULL; }
					if( spr[i].data != NULL ) { free(spr[i].data); spr[i].data = NULL; }
					if( spr[i].pJpgData != NULL) { free(spr[i].pJpgData); spr[i].pJpgData = NULL; }
				}
				free(spr); spr=NULL;
				ZeroMemory(&sfhead, sizeof(SPRFILEHEAD));
			}
		}

		/*--------------------------------------------------------------------------
		스프라이트 파일을 로드(무조건 555로 읽는다)
		---------------------------------------------------------------------------*/
		BOOL xSprite::Load(char *name)
		{
			return(Load(name, 555));
		}

		/*--------------------------------------------------------------------------
		스프라이트 파일을 로드
		name: 파일명
		ScreenRGBMode: 지정한 픽셀 포맷으로 읽어들인다
		---------------------------------------------------------------------------*/
		BOOL xSprite::Load(char *name, int ScreenRGBMode)
		{
			FILE *fp = NULL;

			m_strPath = name;

			SPRFILEHEAD tempsfhead;     // 임시 파일 헤더
			SPRHEAD30 shead;            // 각 스프라이트의 헤더

			if( (fp = fopen( name, "rb")) == NULL ) { 
				PutErrStr("파일 오픈에 실패하였습니다(%s)", name);
				return FALSE;
			}

			fread(&tempsfhead, sizeof(SPRFILEHEAD), 1, fp); //기본 헤더 로드

			if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) {
				//스프라이트 파일이 아니면
				PutErrStr("스프라이트 파일이 아닙니다(%s)", name);
				fclose(fp);
				return FALSE;
			}

			// 파일 버전 검사
			if(tempsfhead.Version < 30 || tempsfhead.Version > 32) {
				PutErrStr("스프라이트 버전이 일치하지 않습니다(%s)", name);
				fclose(fp);
				return FALSE;
			}

			Remove();

			memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD));

			spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
			if(spr==NULL) { 
				PutErrStr("메모리 할당 실패(헤더)"); 
				fclose(fp);
				return FALSE; 
			}
			ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

			for(int i=0; i < sfhead.TotalSprNum; i++)
			{
				fread(&shead, sizeof(SPRHEAD30), 1, fp); // 각 스프라이트 마다의 헤더 로드

				int k = (int)sizeof(SPRHEAD30);

				spr[i].compress = shead.compress;
				spr[i].colorkey = shead.colorkey;
				spr[i].rx       = shead.rx;
				spr[i].ry       = shead.ry;
				spr[i].xl       = shead.xl;
				spr[i].yl       = shead.yl;
				spr[i].mx       = shead.mx;
				spr[i].my       = shead.my;
				spr[i].hitNum   = shead.hitNum;
				spr[i].atckNum  = shead.atckNum;
				spr[i].hit      = NULL;
				spr[i].atck     = NULL;
				spr[i].dsize    = shead.dsize;
				spr[i].PixelFormat = shead.PixelFormat;
				spr[i].SprFormat = shead.SprFormat;
				spr[i].Dithering = shead.Dithering;
				spr[i].byUserVal = shead.byUserVal;
				spr[i].pAlpha = NULL;
				spr[i].pJpgData = NULL;
				spr[i].jpgsize = 0;

				///// 충돌 좌표, 공격 좌표 읽기 ///// 
				if(spr[i].hitNum>0)
				{
					spr[i].hit = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].hitNum);
				}

				if(spr[i].atckNum>0) 
				{
					spr[i].atck = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].atckNum);
				}

				if(spr[i].hit)
					fread(spr[i].hit,  sizeof(SPRPOS)*spr[i].hitNum, 1, fp);

				if(spr[i].atck)
					fread(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);

				// 스프라이트 포맷인지, JPEG 포맷인지 확인하다
				if(spr[i].SprFormat == 1) 
				{
					//
					// JPEG 포맷이다
					//

					// 디더링 옵션이 설정되어 있다면 디더링 적용
					BOOL bDither = FALSE;
					if(spr[i].Dithering == 1) bDither = TRUE;

					//// JPEG 포맷 읽기
					fpos_t pos;
					fgetpos(fp, &pos);

					int width=0, height=0;
					WORD *pNewImage = Make16bitFromJpeg(fp, &width, &height, bDither);
					if(!pNewImage) {
						PutErrStr("JPEG 포맷 읽기 실패");
						fclose(fp);
						return FALSE;
					}

					if(width != spr[i].xl || height != spr[i].yl) {
						PutErrStr("JPEG 포맷의 이미지 사이즈가 일치하지 않음");
						fclose(fp);
						return FALSE;
					}

					spr[i].dsize = width * height * 2;
					spr[i].data = pNewImage;

					// 압축되지 않은 상태로 일단 전환
					spr[i].compress = 0;

					if(shead.compress == 1) {
						// 압축 시킴
						Compress(i);
					}

					if(bSpriteEditMode) {
						// JPEG 이미지 원본을 보관(게임에서는 필요 없음)
						fsetpos(fp, &pos);
						spr[i].jpgsize = shead.dsize;
						spr[i].pJpgData = new BYTE[shead.dsize];
						fread(spr[i].pJpgData, shead.dsize, 1, fp);
					}
					else {
						// 다음 스프라이트로 파일 포인터 이동
						pos += shead.dsize;
						fsetpos(fp, &pos);
					}
				}
				else {
					//
					// 스프라이트 포맷이다
					//

					///// 스프라이트 이미지 데이타 읽기
					if(shead.dsize > 0)
						spr[i].data = (WORD *)malloc( shead.dsize );

					if(spr[i].data==NULL) { 
						PutErrStr("메모리 할당 실패(이미지)"); 
						return FALSE; 
					}
					ZeroMemory(spr[i].data, shead.dsize);
					fread(spr[i].data, shead.dsize, 1, fp);
				}

				// 알파 테이블이 포함된 포맷인지 검사
				if(spr[i].PixelFormat == 5558) {
					// 알파 테이블 시작 포인터를 지정
					int alphasize = spr[i].xl * spr[i].yl;
					spr[i].pAlpha = ((BYTE*)spr[i].data) + spr[i].dsize - alphasize;
				}
			}

			if(sfhead.Version <= 30) {
				//// 구버전 에니메이션 데이터 읽기
				if(sfhead.SceneNum > 0) 
					ani.Load(fp);
			}
			else {
				//// 신버전 에니메이션 데이터 읽기
				if(sfhead.SceneNum > 0) 
					scene.Load(fp);

				//// 멀티씬 데이터 읽기
				multiscene.Load(fp);
				UpdateAllMScenePlayTime();
			}

			fclose(fp);

			if(sfhead.PixelFormat != 4444) ChangePixelFormat(ScreenRGBMode);

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		스프라이트 사이즈를 최적화(필요없는 공백부분을 없앤다)
		---------------------------------------------------------------------------*/
		void xSprite::OptimizeSize(int snum, BOOL fixPos)
		{
			if(snum >= sfhead.TotalSprNum || snum < 0) return;
			if(spr[snum].data == NULL) return;

			BOOL compressed=FALSE;
			int i,j;
			int x1=0, y1=0, x2=0, y2=0;
			BOOL fx=FALSE, ex=FALSE, fy=FALSE, ey=FALSE;
			BOOL bline = TRUE;
			int xl=spr[snum].xl;
			int yl=spr[snum].yl;
			WORD colorkey = spr[snum].colorkey;

			if(spr[snum].compress) {compressed=TRUE; DeCompress(snum);}

			// 사이즈 최적화를 위한 공백부분 계산
			x1 = spr[snum].xl-1;
			fy = TRUE;
			for(i=0; i<yl; i++) {
				fx = FALSE;
				ex = FALSE;
				bline = TRUE;		// 공백라인 유무
				for(j=0; j<xl; j++) {
					if(*(spr[snum].data + (i*xl+j)) != colorkey) {
						bline = FALSE;
						ex = TRUE;
						ey = TRUE; y2=i;
						if(fx==FALSE) {
							if(j<x1) x1=j; 
							fx=TRUE;
						}
						if(fy==TRUE) { y1=i; fy=FALSE; }
						if(j>x2) x2=j; 
					}
				}

				if(bline && ey) { y2=i; ey=FALSE; }
			}

			if(x1>=x2 || y1>=y2) {
				if(compressed) Compress(snum);
				return;
			}
			if(x2 >= spr[snum].xl && y2 >= spr[snum].yl) {
				if(compressed) Compress(snum);
				return;
			}

			int newXl = x2-x1+1;
			int newYl = y2-y1+1;

			// 알파 테이블 계산
			int alphasize = 0;
			if(spr[snum].PixelFormat == 5558) {
				alphasize = newXl * newYl;
			}

			WORD *pNew = (WORD*)malloc((newXl*newYl*2) + alphasize);
			WORD *pSor = spr[snum].data + (xl*y1);

			BYTE* pNewAlpha = (alphasize > 0)?((BYTE*)pNew + (newXl*newYl*2)):(NULL);
			BYTE* pSorAlpha = spr[snum].pAlpha + (xl*y1);

			for(i=0; i<newYl; i++)
			{
				// 스프라이트 이미지 클리핑
				memcpy(pNew+(newXl*i), pSor+x1, newXl*2);
				pSor += xl;

				// 알파 이미지 클리핑
				if(alphasize > 0) {
					memcpy(pNewAlpha+(newXl*i), pSorAlpha+x1, newXl);
					pSorAlpha += xl;
				}
			}

			free(spr[snum].data);

			spr[snum].data = pNew;
			spr[snum].pAlpha = pNewAlpha;
			spr[snum].xl = newXl;
			spr[snum].yl = newYl;

			// 각종 좌표들을 함께 교정
			if(fixPos) {
				spr[snum].rx += x1; spr[snum].ry += y1;
				spr[snum].mx -= x1; spr[snum].my -= y1;

				for(i=0; i<spr[snum].hitNum; i++) {
					spr[snum].hit[i].x1 -= x1; spr[snum].hit[i].y1 -= y1;
					spr[snum].hit[i].x2 -= x1; spr[snum].hit[i].y2 -= y1;
				}

				for(i=0; i<spr[snum].atckNum; i++) {
					spr[snum].atck[i].x1 -= x1; spr[snum].atck[i].y1 -= y1;
					spr[snum].atck[i].x2 -= x1; spr[snum].atck[i].y2 -= y1;
				}
			}
			if(compressed) Compress(snum);
		}

		/*--------------------------------------------------------------------------
		스프라이트 압축
		---------------------------------------------------------------------------*/	
		BOOL xSprite::Compress(LONG snum)
		{
			WORD *buf = NULL;           // 압축된 이미지가 저장될 버퍼
			long newsize=0;

			if(spr==NULL) return FALSE;
			if(snum < 0 || snum >= sfhead.TotalSprNum) return FALSE;
			if(spr[snum].data == NULL) return FALSE;
			if(spr[snum].compress == 1) return FALSE;

			newsize = EncodeSpr0(spr[snum].data, spr[snum].xl, spr[snum].yl, &buf, spr[snum].colorkey);
			if(newsize == 0) return FALSE;

			if(spr[snum].PixelFormat == 5558) {
				// 알파테이블을 포함한 스프라이트라면 새롭게 할당하여 이미지 복사
				int alphasize = spr[snum].xl * spr[snum].yl;

				BYTE* tmpbuf = (BYTE*)malloc(newsize + alphasize);
				BYTE* pNewAlpha = tmpbuf + newsize;

				memcpy(tmpbuf, buf, newsize);
				memcpy(pNewAlpha, spr[snum].pAlpha, alphasize);

				free(buf);

				buf = (WORD*)tmpbuf;
				newsize = newsize + alphasize;

				spr[snum].pAlpha = pNewAlpha;
			}

			free(spr[snum].data);          // 기존의 이미지를 해제

			spr[snum].compress = 1;
			spr[snum].data     = buf;      // 새로 압축된 이미지
			spr[snum].dsize    = newsize;  // 압축된 사이즈

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		스프라이트 압축 해제
		---------------------------------------------------------------------------*/
		BOOL xSprite::DeCompress(LONG snum)
		{
			WORD *buf = NULL;           // 압축이 풀린 이미지가 저장될 버퍼
			long newsize=0;

			if(spr==NULL) return FALSE;
			if(snum < 0 || snum >= sfhead.TotalSprNum) return FALSE;
			if(spr[snum].data == NULL) return FALSE;
			if(spr[snum].compress == 0) return FALSE;

			int alphasize = 0;
			if(spr[snum].PixelFormat == 5558) {
				// 알파테이블을 포함한 스프라이트
				alphasize = spr[snum].xl * spr[snum].yl;
			}

			newsize = (spr[snum].xl * spr[snum].yl * 2) + alphasize;
			buf = (WORD *)malloc(newsize);
			if(buf==NULL) return FALSE;

			//ZeroMemory(buf, newsize);
			DecodeSpr0(buf, spr[snum].xl, spr[snum].yl, spr[snum].data, spr[snum].colorkey);

			// 알파 테이블 복사
			if(spr[snum].PixelFormat == 5558) {
				BYTE* pNewAlpha = (BYTE*)buf + (spr[snum].xl * spr[snum].yl * 2);
				memcpy(pNewAlpha, spr[snum].pAlpha, alphasize);
				spr[snum].pAlpha = pNewAlpha;
			}

			free(spr[snum].data);          // 기존의 이미지를 해제

			spr[snum].compress = 0;
			spr[snum].data     = buf;      // 압축이 풀린 이미지 데이터 포인터
			spr[snum].dsize    = newsize;  // 압축이 풀린 사이즈

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		스프라이트를 특정 픽셀 포맷으로 바꾼다
		(NewForm = 555, 565, -565)
		---------------------------------------------------------------------------*/
		BOOL xSprite::ChangePixelFormat(int NewForm)
		{
			int i,x,y;
			if(NewForm == sfhead.PixelFormat) return TRUE;

			if(NewForm!=555 && NewForm!=565 && NewForm!=-565) return FALSE;

			WORD color;

			for(i=0; i < sfhead.TotalSprNum; i++) // 총 스프라이트 수만큼
			{
				color = spr[i].colorkey; // 투명색을 얻는다.

				// 투명색도 픽셀 포맷을 변환한다
				if(sfhead.PixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
					spr[i].colorkey = RGB565_TO_RGB555(color);
				if(sfhead.PixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
					spr[i].colorkey = RGB555_TO_RGB565(color);
				if(sfhead.PixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
					spr[i].colorkey = RGB555_TO_BGR565(color);
				if(sfhead.PixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
					spr[i].colorkey = RGB565_TO_BGR565(color);

				// 이미지 픽셀 포맷 변환(압축된 스프라이트일 경우)
				if(spr[i].compress) {
					int  m,xl,yl;
					int  sbyte,pbyte;
					short cnt;

					xl = spr[i].xl;
					yl = spr[i].yl;
					WORD* tar = spr[i].data + yl*2; // 라인거리 데이타를 건너띔

					for(m=0; m<yl; m++) {
						cnt = *tar++;      // 패턴 개수 얻기
						sbyte=0;
						while(cnt--) {

							sbyte+=(*(WORD*)tar); // 투명색수 얻기
							tar++;

							pbyte =(*(WORD*)tar); // 점수 얻기
							tar++;

							for(int k=0; k<pbyte; k++) {
								color = *tar;
								if(sfhead.PixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
									*tar = RGB565_TO_RGB555(color);
								if(sfhead.PixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
									*tar = RGB555_TO_RGB565(color);
								if(sfhead.PixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
									*tar = RGB555_TO_BGR565(color);
								if(sfhead.PixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
									*tar = RGB565_TO_BGR565(color);
								tar++;
							}
							//tar+=pbyte;
							sbyte+=pbyte;
						}
					}
				}
				else // 압축되지 않은 스프라이트일 경우
				{
					for(y=0; y<spr[i].yl; y++) {
						for(x=0; x<spr[i].xl; x++) {
							WORD* tar = (spr[i].data + (spr[i].xl*y + x));
							color = *tar;
							if(sfhead.PixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
								*tar = RGB565_TO_RGB555(color);
							if(sfhead.PixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
								*tar = RGB555_TO_RGB565(color);
							if(sfhead.PixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
								*tar = RGB555_TO_BGR565(color);
							if(sfhead.PixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
								*tar = RGB565_TO_BGR565(color);
						}
					}
				}
			}

			sfhead.PixelFormat = NewForm;

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		특정 이미지를 바탕으로 스프라이트를 생성
		(이미지를 복사하지는 않는다)
		---------------------------------------------------------------------------*/
		BOOL xSprite::TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey)
		{
			if(pImage == NULL) return FALSE;
			if(xl <= 0 || yl <= 0) return FALSE;

			Remove();

			spr = (SPRITE30*) malloc(sizeof(SPRITE30));
			if(spr==NULL) { 
				PutErrStr("메모리 할당 실패(At TransToSpr)"); 
				return FALSE; 
			}

			sfhead.TotalSprNum = 1;
			strcpy(sfhead.Identifier, "93XYZ sprite");
			sfhead.Version = 30;
			sfhead.BPD = 2;
			sfhead.SceneNum = 0;

			memset(&spr[0], 0, sizeof(SPRITE30));

			//spr[0].tot = 1;
			spr[0].compress = 0;
			spr[0].colorkey = wColorKey;
			spr[0].xl       = xl;
			spr[0].yl       = yl;
			spr[0].rx       = 0;
			spr[0].ry       = 0;
			spr[0].mx       = xl/2;
			spr[0].my       = yl/2;
			spr[0].hitNum   = 0;
			spr[0].atckNum  = 0;
			spr[0].hit      = NULL;
			spr[0].atck     = NULL;
			spr[0].dsize    = xl*2 * yl;
			spr[0].data     = pImage;
			spr[0].PixelFormat = 555;
			spr[0].SprFormat = 0;
			spr[0].Dithering = 0;
			spr[0].byUserVal = 0;

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		특정 이미지를 바탕으로 스프라이트를 생성
		(이미지 버퍼를 내부적으로 할당하여 복사함)
		---------------------------------------------------------------------------*/
		BOOL xSprite::TransToSprByCopy(int xl, int yl, WORD *pImage, WORD wColorKey)
		{
			if(pImage == NULL) return FALSE;
			if(xl <= 0 || yl <= 0) return FALSE;

			WORD* pNewImage = new WORD[xl * yl];
			memcpy(pNewImage, pImage, sizeof(WORD)*(xl*yl));

			if(!TransToSpr(xl, yl, pNewImage, wColorKey))
			{
				if(pNewImage)
				{
					delete [] pNewImage;
				}

				return FALSE;
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		최초로 스프라이트를 삽입하기 위해 메모리 할당(내부적 용도)
		---------------------------------------------------------------------------*/
		BOOL xSprite::NewSpace(int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress)
		{
			if(pImage == NULL) return FALSE;
			Remove();

			spr = (SPRITE30*) malloc(sizeof(SPRITE30));
			if(spr==NULL) { 
				PutErrStr("메모리 할당 실패(At NewSpace)"); 
				return FALSE; 
			}

			sfhead.TotalSprNum = 1;
			strcpy(sfhead.Identifier, "93XYZ sprite");
			sfhead.Version = 30;
			sfhead.BPD = 2;
			sfhead.SceneNum = 0;

			memset(&spr[0], 0, sizeof(SPRITE30));

			//spr[0].tot = 1;
			spr[0].compress = 0;
			spr[0].colorkey = wColorKey;
			spr[0].xl       = xl;
			spr[0].yl       = yl;
			spr[0].rx       = 0;
			spr[0].ry       = 0;
			spr[0].mx       = xl/2;
			spr[0].my       = yl/2;
			spr[0].hitNum   = 0;
			spr[0].atckNum  = 0;
			spr[0].hit      = NULL;
			spr[0].atck     = NULL;
			spr[0].dsize    = xl*2 * yl;
			spr[0].data     = pImage;
			spr[0].PixelFormat = 555;
			spr[0].SprFormat = 0;
			spr[0].Dithering = 0;
			spr[0].byUserVal = 0;
			spr[0].pAlpha = NULL;
			spr[0].pJpgData = NULL;
			spr[0].jpgsize = 0;

			if(bCompress) {
				if(Compress(0)==FALSE) return FALSE;
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		새로운 스프라이트를 삽입
		---------------------------------------------------------------------------*/
		BOOL xSprite::Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress)
		{
			if(pImage == NULL) { 
				PutErrStr("이미지 포인터가 NULL입니다(At Insert)"); 
				return FALSE; 
			}

			if(sfhead.TotalSprNum <= 0) {
				// 최초로 생성된 스프라이트라면
				if(NewSpace(xl, yl, pImage, wColorKey, bCompress)==FALSE) return FALSE;
				return TRUE;
			}

			if(snum > sfhead.TotalSprNum || snum < 0) { 
				PutErrStr("스프라이트 컷 삽입 범위 초과(At Insert)"); 
				return FALSE; 
			}

			SPRITE30 *oldspr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
			if(oldspr==NULL) { 
				PutErrStr("메모리 할당 실패(At Insert)"); 
				return FALSE; 
			}
			memcpy(oldspr, spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

			free(spr);
			sfhead.TotalSprNum += 1;
			spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
			if(spr==NULL) { 
				PutErrStr("메모리 할당 실패(At Insert)"); 
				return FALSE; 
			}

			int i;
			for(i=0; i<snum; i++) memcpy(spr+i, oldspr+i, sizeof(SPRITE30));
			for(i=snum+1; i<sfhead.TotalSprNum; i++) memcpy(spr+i, oldspr+i-1, sizeof(SPRITE30));

			if(oldspr!=NULL) free(oldspr);
			//spr[snum].tot = 1;
			memset(&spr[snum], 0, sizeof(SPRITE30));

			spr[snum].compress = 0;
			spr[snum].colorkey = wColorKey;
			spr[snum].xl       = xl;
			spr[snum].yl       = yl;
			spr[snum].rx       = 0;
			spr[snum].ry       = 0;
			spr[snum].mx       = xl/2;
			spr[snum].my       = yl/2;
			spr[snum].hitNum   = 0;
			spr[snum].atckNum  = 0;
			spr[snum].hit      = NULL;
			spr[snum].atck     = NULL;
			spr[snum].dsize    = xl*2 * yl;
			spr[snum].data     = pImage;
			spr[snum].PixelFormat = 555;
			spr[snum].Dithering = 0;
			spr[snum].byUserVal = 0;
			spr[snum].pAlpha = NULL;
			spr[snum].pJpgData = NULL;
			spr[snum].jpgsize = 0;
			spr[snum].SprFormat = 0;

			if(bCompress) {
				if(Compress(snum)==FALSE) return FALSE;
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		새로운 스프라이트를 삽입(새로운 메모리를 할당하여 복사함)
		---------------------------------------------------------------------------*/
		BOOL xSprite::InsertByCopy(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress)
		{
			if(!pImage) return FALSE;
			if(xl * yl <= 0) return FALSE;

			WORD* pNewImage = new WORD[xl*yl];
			memcpy(pNewImage, pImage, xl*yl*2);

			if(!Insert(snum, xl, yl, pNewImage, wColorKey, bCompress))
			{
				if(pNewImage)
				{
					delete [] pNewImage;
				}
				return FALSE;
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		JPEG 파일 포인터로 16비트 555 이미지를 생성
		---------------------------------------------------------------------------*/
		WORD* xSprite::Make16bitFromJpeg(LPCTSTR fname, int* width, int* height, BOOL bDither)
		{
			FILE *fp = fopen(fname,"rb");
			if(!fp) return NULL;
			WORD* pRtn = Make16bitFromJpeg(fp, width, height, bDither);
			fclose(fp);
			return pRtn;
		}

		WORD* xSprite::Make16bitFromJpeg(FILE *fp, int* width, int* height, BOOL bDither)
		{
			*width = 0;
			*height = 0;

			BYTE *pImage24 = JpegFile::JpegFileToRGB(fp, (UINT*)width, (UINT*)height);
			if(!pImage24) return NULL;

			int xsize = *width;
			int ysize = *height;
			if(xsize <= 0 || ysize <= 0) return NULL;

			WORD* pNew = NULL;

			if(bDither) {
				// 디더링 옵션이 지정된 경우
				DWORD rmask = (DWORD)0x0000001F;
				DWORD gmask = (DWORD)0x000003E0;
				DWORD bmask = (DWORD)0x00007C00;
				// 24비트 이미지를 디더링하여 16비트(5:5:5) 이미지로 변환
				setup_dither_mask(rmask, gmask, bmask, 2);
				dither_rgb24(pImage24, xsize, ysize, (BYTE**)&pNew);
			}
			else {
				pNew = new WORD[xsize * ysize];

				BYTE* bit = pImage24;
				WORD* now = (WORD*)pNew;
				WORD color16;
				BYTE r,g,b;

				for(int i=0; i<ysize; i++) {
					// 16비트로 변환
					static char k=0;
					k=0;

					for(int j=0; j<xsize; j++) {
						// 주의) 여기선 RGB순이지만 비트맵은 BGR순이다
						r = *(bit+(j*3));
						g = *(bit+(j*3)+1);
						b = *(bit+(j*3)+2);

						color16 = RGB555(r,g,b);

						*((WORD*)now+j) = color16;
					}
					now += xsize;
					bit += xsize*3;
				}
			}

			delete pImage24;

			return pNew;
		}


		/////////////////////////////////// 장면 애니메이션 관련 ///////////////////////////////////////////


		// 장면의 총 플레이 타임을 구함
		DWORD xSprite::GetScenePlayTime(int scn)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return 0;
			return pScene->TotPlayTime;
		}

		BOOL xSprite::IsSceneTimeOver(int scn, DWORD time)
		{
			if(time >= GetScenePlayTime(scn))
				return TRUE;

			return FALSE;
		}

		// 이름으로 장면 번호를 구함(-1을 리턴하면 실패)
		int xSprite::GetScene(char* name)
		{
			if(scene.TotSceneNum==0) 
				return -1;

			for(int i=0; i<scene.TotSceneNum; i++)
				if(stricmp(name, (scene.ppScene[i])->SceneName) == 0) 
					return i;

			return -1;
		}

		// 장면의 총 프레임을 구함
		int xSprite::GetTotFrame(int scn)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return 0;
			return pScene->TotFrameNum;
		}

		// 주어진 시간에 플레이 될 프레임을 구함 : 실패할 경우 -1을 리턴
		int xSprite::GetNowFrame(int scn, DWORD time, BOOL bLoop)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return -1;
			return pScene->SearchFrame(time, bLoop);
		}

		// 특정 프레임의 시작 시간과 끝 시간을 구함
		BOOL xSprite::GetFrameTime(int scn, int frame, DWORD* pSTime, DWORD* pETime)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return FALSE;
			return pScene->GetFrameTime(frame, pSTime, pETime);
		}

		SceneData* xSprite::GetScenePt(int scn)
		{
			return scene.GetScenePt(scn);
		}

		FrameData* xSprite::GetFramePt(int scn, int frame)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return 0;
			return pScene->GetPtrFrame(frame);
		}

		CellData* xSprite::GetCellPt(int scn, int frame, int cell)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return 0;
			FrameData* pFrame = pScene->GetPtrFrame(frame);
			if(!pFrame) 
				return 0;
			return pFrame->GetPtrCell(cell);
		}

		/////////////////////////////////// 멀티씬 애니메이션 관련 ///////////////////////////////////////////

		DWORD xSprite::GetMScenePlayTime(int mscn)
		{
			MSceneData* pMScene = multiscene.GetMScenePt(mscn);
			if(!pMScene)
				return 0;

			return pMScene->TotPlayTime;
		}

		BOOL xSprite::IsMSceneTimeOver(int mscn, DWORD time)
		{
			if(time >= GetMScenePlayTime(mscn))
				return TRUE;

			return FALSE;
		}


		BOOL xSprite::CalcTrackPlayTime(int mscn, int track, DWORD* pStartTime, DWORD* pEndTime)
		{
			TrackData* pTrack = multiscene.GetTrackPt(mscn, track);

			if(!pTrack) 
				return FALSE;

			switch(pTrack->TrackType)
			{
			case TTYPE_SCENE:
				{
					TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
					*pStartTime = pTrack->Delay;
					// 씬의 경우 라이프타임이 0인 경우 반복 회수에 따라 자동으로 시간이 결정됨
					if(pScnData->LifeTime == 0)
						*pEndTime = pTrack->Delay + (GetScenePlayTime(pScnData->SceneNo) * pScnData->RepeatCnt);
					else 
						*pEndTime = pTrack->Delay + pScnData->LifeTime;
				} break;

			case TTYPE_SOUND:
				{
					TRACK_SOUND* pSndData = (TRACK_SOUND*)pTrack->pData;
					*pStartTime = pTrack->Delay;
					*pEndTime = pTrack->Delay;
				} break;

			case TTYPE_CLIPBOX:
				{
					TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pTrack->pData;
					*pStartTime = pTrack->Delay;
					*pEndTime = pTrack->Delay + pCbxData->LifeTime;
				} break;

			case TTYPE_SCRIPT:
				{
					TRACK_SCRIPT* pScrData = (TRACK_SCRIPT*)pTrack->pData;
					*pStartTime = pTrack->Delay;
					*pEndTime = pTrack->Delay;
				} break;

			default:
				{
					*pStartTime = pTrack->Delay;
					*pEndTime = pTrack->Delay;
				} break;
			}

			return TRUE;
		}

		BOOL xSprite::CalcMScenePlayTime(int mscn, DWORD* pStartTime, DWORD* pEndTime)
		{
			MSceneData* pMScene = multiscene.GetMScenePt(mscn);

			if(!pMScene) 
				return FALSE;

			DWORD stime = (DWORD)0xFFFFFFFF;
			DWORD etime = 0;

			for(int i=0; i<pMScene->TotTrackNum; i++)
			{
				DWORD t1 = 0;
				DWORD t2 = 0;

				if(!CalcTrackPlayTime(mscn, i, &t1, &t2))
					continue;

				if(t1 < stime)
					stime = t1;
				if(t2 > etime)
					etime = t2;
			}

			if(stime == 0xFFFFFFFF) {
				*pStartTime = 0;
				*pEndTime = 0;
				return FALSE;
			}

			*pStartTime = stime;
			*pEndTime = etime;

			return TRUE;
		}

		BOOL xSprite::UpdateMScenePlayTime(int mscn)
		{
			MSceneData* pMScene = multiscene.GetMScenePt(mscn);
			if(!pMScene)
				return FALSE;

			DWORD stime, etime;

			if(!CalcMScenePlayTime(mscn, &stime, &etime))
				return FALSE;

			pMScene->TotPlayTime = etime;

			return TRUE;
		}

		void xSprite::UpdateAllMScenePlayTime()
		{
			for(int i=0; i<multiscene.TotMSceneNum; i++)
			{
				UpdateMScenePlayTime(i);
			}
		}


		//--------------------------------------------------------------------------------------
		//      스프라이트 파일 내의 모든 멀티씬에서 사용된 사운드 정보 열거 함수
		//--------------------------------------------------------------------------------------
		BOOL xSprite::QueryAllSound(QUERYMSCENE_SOUND* pQuery)
		{
			if(!pQuery) 
				return FALSE;

			if(pQuery->NowMScene == -1) 
				pQuery->NowMScene += 1;

			if(pQuery->NowMScene < 0 || pQuery->NowMScene >= multiscene.TotMSceneNum)
				return FALSE;

			// 0에서부터 최대 시간 범위의 모든 사운드를 쿼리
			while(!QueryMSceneSound(pQuery, pQuery->NowMScene, 0, (DWORD)0xFFFFFFFF, FALSE))
			{
				pQuery->NowMScene += 1;
				pQuery->NowTrack = -1;

				if(pQuery->NowMScene >= multiscene.TotMSceneNum)
					return FALSE;
			}

			return TRUE;
		}

		//--------------------------------------------------------------------------------------
		//      스프라이트 파일 내의 모든 멀티씬에서 사용된 스크립트 정보 열거 함수(TRUE를 리턴하는 동안에 반복 호출해야함)
		//--------------------------------------------------------------------------------------
		BOOL xSprite::QueryAllScript(QUERYMSCENE_SCRIPT* pQuery)
		{
			if(!pQuery) 
				return FALSE;

			if(pQuery->NowMScene == -1) 
				pQuery->NowMScene += 1;

			if(pQuery->NowMScene < 0 || pQuery->NowMScene >= multiscene.TotMSceneNum)
				return FALSE;

			// 0에서부터 최대 시간 범위의 모든 스크립트를 쿼리
			while(!QueryMSceneScript(pQuery, pQuery->NowMScene, 0, (DWORD)0xFFFFFFFF, FALSE))
			{
				pQuery->NowMScene += 1;
				pQuery->NowTrack = -1;

				if(pQuery->NowMScene >= multiscene.TotMSceneNum)
					return FALSE;
			}

			return TRUE;
		}

		//--------------------------------------------------------------------------------------
		//      멀티씬 애니메이션에서 특정 시간 범위의 사운드 정보 열거 함수(TRUE를 리턴하는 동안에 반복 호출해야함)
		//--------------------------------------------------------------------------------------
		BOOL xSprite::QueryMSceneSound(QUERYMSCENE_SOUND* pQuery, int multiscn, DWORD starttime, DWORD endtime, BOOL loop)
		{
			if(!pQuery) 
				return FALSE;

			MSceneData* pMScene = multiscene.GetMScenePt(multiscn);

			if(!pMScene)
				return FALSE;

			if(starttime >= endtime)
				return FALSE;

			BOOL bOverRun = FALSE;

			if(loop && pMScene->TotPlayTime > 0) {
				starttime = starttime % pMScene->TotPlayTime;
				endtime = endtime % pMScene->TotPlayTime;

				if(starttime > endtime)
					bOverRun = TRUE;
			}

			pQuery->NowTrack += 1;
			if(pQuery->NowTrack < 0 || pQuery->NowTrack >= pMScene->TotTrackNum)
				return FALSE;

			for(; pQuery->NowTrack < pMScene->TotTrackNum; pQuery->NowTrack++) {
				TrackData* pTrack = pMScene->GetPtrTrack(pQuery->NowTrack);
				if(!pTrack || !pTrack->pData)
					continue;

				if(pTrack->TrackType != TTYPE_SOUND)
					continue;

				if(bOverRun){
					// 현재 시간이 루프를 돌아 이전 시간 보다 작은 경우
					if(pTrack->Delay < starttime && pTrack->Delay > endtime)
						continue;
				}
				else {
					if(pTrack->Delay < starttime || pTrack->Delay > endtime) 
						continue;
				}

				TRACK_SOUND* pSndData = (TRACK_SOUND*)pTrack->pData;

				pQuery->NowMScene = multiscn;
				pQuery->PlayTime = pTrack->Delay;
				pQuery->SndFile = pSndData->SndFile;
				pQuery->Volume = pSndData->Volume;

				return TRUE;
			}

			return FALSE;
		}

		//--------------------------------------------------------------------------------------
		//      멀티씬 애니메이션에서 특정 시간 범위의 스크립트 정보 열거 함수
		//--------------------------------------------------------------------------------------
		BOOL xSprite::QueryMSceneScript(QUERYMSCENE_SCRIPT* pQuery, int multiscn, DWORD starttime, DWORD endtime, BOOL loop)
		{
			if(!pQuery) 
				return FALSE;

			MSceneData* pMScene = multiscene.GetMScenePt(multiscn);

			if(!pMScene)
				return FALSE;

			if(starttime >= endtime)
				return FALSE;

			BOOL bOverRun = FALSE;

			if(loop && pMScene->TotPlayTime > 0) {
				starttime = starttime % pMScene->TotPlayTime;
				endtime = endtime % pMScene->TotPlayTime;

				if(starttime > endtime)
					bOverRun = TRUE;
			}

			pQuery->NowTrack += 1;
			if(pQuery->NowTrack < 0 || pQuery->NowTrack >= pMScene->TotTrackNum)
				return FALSE;

			for(; pQuery->NowTrack < pMScene->TotTrackNum; pQuery->NowTrack++) {
				TrackData* pTrack = pMScene->GetPtrTrack(pQuery->NowTrack);
				if(!pTrack || !pTrack->pData)
					continue;

				if(pTrack->TrackType != TTYPE_SCRIPT)
					continue;

				if(bOverRun){
					// 현재 시간이 루프를 돌아 이전 시간 보다 작은 경우
					if(pTrack->Delay < starttime && pTrack->Delay > endtime)
						continue;
				}
				else {
					if(pTrack->Delay < starttime || pTrack->Delay > endtime) 
						continue;
				}

				TRACK_SCRIPT* pScrData = (TRACK_SCRIPT*)pTrack->pData;

				pQuery->NowMScene = multiscn;
				pQuery->PlayTime = pTrack->Delay;
				pQuery->ScriptName = pScrData->ScriptName;
				pQuery->Script = pTrack->pstrScript;

				return TRUE;
			}

			return FALSE;
		}

	}//namespace GRAPHICGDI

}//namespace NMBASE
