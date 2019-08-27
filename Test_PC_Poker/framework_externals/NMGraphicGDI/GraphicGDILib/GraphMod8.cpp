
//
//    압축 스프라이트 관련 함수
//

#include "stdafx.h"
#include "GraphMod8.h"

namespace NMBASE
{
	namespace GRAPHICGDI
	{

#define BEGINPUTSPR 1  // 압축 스프라이트 클리핑에 사용(라인의 출력시작부 계산중)
#define ENDPUTSPR 2    // 압축 스프라이트 클리핑에 사용(실제 출력부 처리 중)

		int	ALPHA_STARTVAL = 0; // 색을 찾아낼 범위 지정값
		int	ALPHA_ENDVAL = 255;

		// 압축 스프라이트의 각 라인의 포인터를 구한다
#define GET_CSPLINEPTR(pCsp, line)  ((pCsp) + (*(long*)((pCsp)+((line)<<2))))
#define MAX_ALPHAGRADE 7  // 최대 알파 단계
#define IsAlphaLimit(val) (((val)>=(ALPHA_STARTVAL)&& (val)<=(ALPHA_ENDVAL)) ?1:0)

		typedef struct 
		{
			char id[16];          // 알파 테이블 파일 인식문자
			BYTE grade;           // 알파단계
			DWORD size;           // 알파테이블의 크기
			char reserved[3];
		} ALPHAHEADER;

		//static BYTE pAlpha[MAX_ALPHAGRADE][256][256]; // 8비트 알파 테이블
		static BYTE* pAlpha = NULL; // 8비트 알파 테이블 포인터

		/*---------------------------------------------------------------------------
		메모리 세팅 함수(4바이트 고속 순차 세팅)

		tar : 타겟 포인터
		val : 채울 값(바이트형)
		len : 세팅할 길이
		---------------------------------------------------------------------------*/
		void __fastcall MemSet4b(char *tar, char val, int len)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   edi, tar;

				mov   bl,  val; // EAX에 val을 4바이트 복사
				mov   bh,  val;
				mov   ax,  bx;
				shl   eax, 16;
				mov   ax,  bx;

				shr   ecx, 1;
				jnc   next_c2;
				stosb;
next_c2:
				shr   ecx, 1;
				jnc   next_d2;
				stosw;
next_d2:
				rep   stosd;
			}
		}




		/*---------------------------------------------------------------------------
		더블워드 메모리 전송 함수(4바이트 고속 순차복사)

		tar, sor : 타겟과 소스 포인터
		len      : 복사할 길이
		---------------------------------------------------------------------------*/
		void __fastcall MemCpy4b(char *tar, char *sor, int len)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;
				mov   edi, tar;
				shr   ecx, 1;
				jnc   next_a;
				movsb;
next_a:
				shr   ecx, 1;
				jnc   next_b;
				movsw;
next_b:
				rep   movsd;
			}
		}


		/*---------------------------------------------------------------------------
		역-더블워드 메모리 전송 함수(4바이트 역순 고속 복사)

		tar, sor : 타겟과 소스포인터
		len      : 복사할 길이
		---------------------------------------------------------------------------*/
		void __fastcall RvrsMemCpy4b(char *tar, char *sor, int len)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;   // 소스는 순차
				mov   edi, tar;
				add   edi, len;   // 타겟은 역순

				shr   ecx, 1;
				jnc   next_a;
				lodsb;
				dec   edi;
				mov   [edi],al;

next_a:
				shr   ecx, 1;
				jnc   next_b;
				lodsw;
				xchg  ah, al;     // 바이트를 뒤집는다(WORD)
				sub   edi,2;
				mov   [edi],ax;
next_b:
				cmp   ecx, 0;     
				je    next_end;   // 이미지 개수가 0이면 종료
next_c:
				lodsd;
				bswap eax;        // 바이트를 뒤집는다(Double WORD)
				sub   edi,4;
				mov   [edi],eax;
				loop  next_c;     // 남은 개수만큼 반복

next_end:
			}
		}


		/*---------------------------------------------------------------------------
		투명처리 메모리 세팅함수(memset와 비슷한 함수로 대신 쓰면 투명효과가 처리된다)

		tar    : 타겟 포인터
		val    : 세팅할 값
		len    : 처리할 길이
		pAlpha : 알파테이블(256*256배열의 포인터)
		---------------------------------------------------------------------------*/
		void __fastcall GlassSet(char *tar, char val, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   edi, tar;
				cmp   ecx, 0;     
				je    next_b;     // 이미지 개수가 0이면 점프
next_a:
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, val;    // EAX + val
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				inc   edi;
				loop  next_a;    // 이미지수만큼 반복
next_b:
			}
		}


		/*---------------------------------------------------------------------------
		투명처리 역-메모리 세팅함수(역순 세트)

		tar    : 타겟 포인터
		val    : 세팅할 값
		len    : 처리할 길이
		pAlpha : 알파테이블(256*256배열의 포인터)
		---------------------------------------------------------------------------*/
		void __fastcall RvrsGlassSet(char *tar, char val, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   edi, tar;
				add   edi, len;   // 타겟은 역순

				cmp   ecx, 0;     
				je    next_b;     // 이미지 개수가 0이면 점프
next_a:
				dec   edi;
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, val;  // EAX + ESI
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				loop  next_a;    // 이미지수만큼 반복
next_b:
			}
		}



		/*---------------------------------------------------------------------------
		투명처리 메모리복사함수(memcpy와 비슷한 함수로 대신 쓰면 투명효과가 처리된다)

		tar, sor : 배경과 스프라이트 포인터
		len      : 처리할 길이
		pAlpha   : 알파테이블(256*256배열의 포인터)
		---------------------------------------------------------------------------*/
		void __fastcall GlassCpy(char *tar, char *sor, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;
				mov   edi, tar;
				cmp   ecx, 0;     
				je    next_b;     // 이미지 개수가 0이면 점프
next_a:
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, [esi];  // EAX + ESI
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				inc   edi;
				inc   esi;
				loop  next_a;    // 이미지수만큼 반복
next_b:
			}
		}

		/*---------------------------------------------------------------------------
		투명처리 역-메모리복사함수(역순 복사)

		tar, sor : 배경과 스프라이트 포인터
		len      : 처리할 길이
		pAlpha   : 알파테이블(256*256배열의 포인터)
		---------------------------------------------------------------------------*/
		void __fastcall RvrsGlassCpy(char *tar, char *sor, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;   // 소스는 순차
				mov   edi, tar;
				add   edi, len;   // 타겟은 역순

				cmp   ecx, 0;     
				je    next_b;     // 이미지 개수가 0이면 점프
next_a:
				dec   edi;
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, [esi];  // EAX + ESI
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				inc   esi;
				loop  next_a;    // 이미지수만큼 반복
next_b:
			}
		}


		/*-------------------------------------------------------------
		RGB를 HSV로 바꿔준다(테스트용)
		-------------------------------------------------------------*/
		void Rgb2Hsv(BYTE r, BYTE g, BYTE b, BYTE* h, BYTE* s, BYTE* v)
		{
			int major, minor, diff, round;

			major=max(r,max(g,b)); minor=min(r,min(g,b));
			diff = major - minor;
			*h = *s = 0; *v = major;
			if(diff==0 || major==0) return;

			round = diff/2;
			*s = (diff*100+major/2)/major;
			if(major==r) *h = 16+(((int)g-(int)b)*16+round)/diff;
			if(major==g) *h = 48+(((int)b-(int)r)*16+round)/diff;
			if(major==b) *h = 80+(((int)r-(int)g)*16+round)/diff;
		}

		/*-------------------------------------------------------------
		근접색 찾기
		-------------------------------------------------------------*/
		BYTE NearColor(int r, int g, int b, RGBQUAD* pal) // 근접색 찾기
		{
			BYTE find;
			int i;
			int temp, distance = 32767;
			int re,ge,be;      // RGB편차
			int maxc, minc;    // RGB편차의 최대,최소
			//BYTE h,s,v,h2,s2,v2; // 비교할 HSV값
			//int he,se,ve;      // HSV편차	
			int mdif = 1000;   // 바로전의 근소값

			//Rgb2Hsv((BYTE)r,(BYTE)g,(BYTE)b, &h, &s, &v);

			for(i=0; i<256; i++)
			{
				re = abs(r - pal[i].rgbRed);
				ge = abs(g - pal[i].rgbGreen);
				be = abs(b - pal[i].rgbBlue);

				/*
				Rgb2Hsv((BYTE)pal[i].rgbRed,
				(BYTE)pal[i].rgbGreen,
				(BYTE)pal[i].rgbBlue,
				&h2, &s2, &v2);

				he = abs(h - h2);
				se = abs(s - s2);
				ve = abs(v - v2);
				*/

				maxc = max(re, max(ge,be));             // 편차의 최대 최소 구하기
				minc = min(re, min(ge,be));
				//temp = re + ge + be;                    // 일반식
				temp = re + ge + be + abs(maxc - minc); // 개선된 식
				//temp = re + ge + be + he + se + ve;     // HSV까지 고려하는 개선된 식
				//temp = re + ge + be + ve;     // HSV까지 고려하는 개선된 식

				if(temp < mdif && IsAlphaLimit(i)) // 새로 찾은 색이 범위에 만족하면
				{
					find = i;
					mdif = temp;
				}
			}
			return find;
		}

		/*-------------------------------------------------------------
		혼합색 찾기
		al : 혼합 비율(al이 클수록 c1>c2 이다, 소스가 더 투명해짐)
		-------------------------------------------------------------*/
		BYTE FindBlendColor(BYTE c1, BYTE c2, RGBQUAD* pal, BYTE al) // 혼합색 찾기
		{
			int r1,g1,b1,r2,g2,b2;
			int r,g,b;

			r1 = pal[c1].rgbRed; // RGB 분리
			g1 = pal[c1].rgbGreen;
			b1 = pal[c1].rgbBlue;
			r2 = pal[c2].rgbRed;
			g2 = pal[c2].rgbGreen;
			b2 = pal[c2].rgbBlue;

			r = ((r1*al)+(r2*(8-al)))/8;   // 혼합된 색의 RGB
			g = ((g1*al)+(g2*(8-al)))/8;
			b = ((b1*al)+(b2*(8-al)))/8;

			return NearColor(r,g,b,pal); // 가장 근접한 색을 찾아서 리턴
		}


		/*---------------------------------------------------------------------------
		알파테이블 생성
		---------------------------------------------------------------------------*/
		void CreateAlphaTable8()
		{
			if(!pAlpha) pAlpha = new BYTE[MAX_ALPHAGRADE*256*256];
		}

		/*---------------------------------------------------------------------------
		알파테이블 해제
		---------------------------------------------------------------------------*/
		void ClearAlphaTable8()
		{
			if(pAlpha) delete pAlpha;
			pAlpha = NULL;
		}

		/*---------------------------------------------------------------------------
		알파테이블 초기화 (색을 지정한 범위 내의 값으로만 얻을 수 있다)

		pal   : 파레트 테이블 포인터
		start : 파레트 검색 범위 시작번호
		eld   : 파레트 검색 범위 끝번호
		---------------------------------------------------------------------------*/
		void InitAlphaTable8(RGBQUAD* pal, int start, int end)   // 알파 테이블 초기화
		{
			ALPHA_STARTVAL = start;
			ALPHA_ENDVAL = end;

			if(!pAlpha) CreateAlphaTable8();

			for(int grade=0; grade<MAX_ALPHAGRADE; grade++) {
				for(int i=0; i<256; i++) {
					for(int j=0; j<256; j++) {
						//pAlpha[grade][i][j] = FindBlendColor(i,j,pal,grade); // 혼합색 찾기
						*(pAlpha+(256*256*grade)+(256*i)+j) = FindBlendColor(i,j,pal,grade); // 혼합색 찾기
					}
					// 0번 테이블은 필요없지만 속도관계상 계산함(바로 값을 얻기위함) <-- 메모리 낭비다!
				}
			}
		}

		/*---------------------------------------------------------------------------
		알파테이블 저장
		---------------------------------------------------------------------------*/
		int SaveAlphaTable8(char* fname)
		{
			FILE* fp;
			ALPHAHEADER ah;

			if(!pAlpha) return -2;

			fp = fopen( fname, "wb");
			if(fp == NULL ) return -1;   // File open error

			memset(&ah, 0, sizeof(ALPHAHEADER));  // Initializing header
			strncpy(ah.id,"Alpha Table",11);
			ah.grade = MAX_ALPHAGRADE;
			ah.size = 256 * 256 * MAX_ALPHAGRADE;

			fwrite(&ah,sizeof(ALPHAHEADER),1,fp); // Write header
			fwrite(pAlpha,MAX_ALPHAGRADE*256*256,1,fp); // Write pAlpha data
			fclose(fp);
			return TRUE;
		}

		/*---------------------------------------------------------------------------
		알파테이블 로드
		---------------------------------------------------------------------------*/
		int LoadAlphaTable8(char* fname)
		{
			FILE* fp;
			ALPHAHEADER ah;

			fp = fopen( fname, "rb");
			if(fp == NULL ) return -1;                          // File open error

			fread(&ah,sizeof(ALPHAHEADER),1,fp);                // Read header
			if(strncmp(ah.id,"Alpha Table",11) != 0) return -2; // Not a Alpha table

			if(!pAlpha) CreateAlphaTable8();

			fread(pAlpha,MAX_ALPHAGRADE*256*256,1,fp);           // Read data
			fclose(fp);
			return TRUE;
		}


		/*---------------------------------------------------------------------------
		반투명으로 스프라이트를 출력하는 함수
		(grade 가 클수록 더 투명해진다)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += y * xLen + x; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}

			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}

		/*------------------ 내부적으로 쓰임 -----------------------------*/
		void PutImage90d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor);
		void PutImage270d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor );

		void PutSpr90d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor);
		void PutSpr270d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor );

		void PutSprMono_90d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor, char color);
		void PutSprMono_270d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor, char color);

		/*---------------------------------------------------------------------------
		색을 채운 박스 그리기(클리핑)
		---------------------------------------------------------------------------*/
		void FillBox8( int x1, int y1, int x2, int y2, char *tar, char color )
		{
			register int i;
			register int xl;

			if(x1<_CX1) { x1 = _CX1;}
			if(x2>_CX2) { x2 = _CX2;}
			if(y1<_CY1) { y1 = _CY1;}
			if(y2>_CY2) { y2 = _CY2;}

			xl = x2-(x1-1);

			if(xl<=0) return;

			tar += _LPITCH * y1 + x1;

			for(i=y1; i<=y2; i++)
			{
				memset( tar , color, xl);
				tar += _LPITCH;
			}
		}

		/*---------------------------------------------------------------------------
		반투명색을 채운 박스 그리기(클리핑)
		---------------------------------------------------------------------------*/
		void FillBoxGlass8( int xp, int yp, int xl, int yl, char *tar, char color, int grade)
		{
			register int i;
			int x1=xp,y1=yp;
			int x2=xp+xl-1;
			int y2=yp+yl-1;
			int xLen, yLen;
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(x1<_CX1) { x1 = _CX1;}
			if(x2>_CX2) { x2 = _CX2;}
			if(y1<_CY1) { y1 = _CY1;}
			if(y2>_CY2) { y2 = _CY2;}

			xLen = x2-x1+1;
			yLen = y2-y1+1;

			if(xLen<1 || yLen<1) return;
			if(grade>=MAX_ALPHAGRADE) return;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			tar += _LPITCH * y1 + x1;

			for(i=0; i<yLen; i++)
			{
				GlassSet( tar , color, xLen, pAT);
				tar += _LPITCH;
			}
		}

		/*---------------------------------------------------------------------------
		수평선 그리기(클리핑)
		---------------------------------------------------------------------------*/
		void DrawHLine8( int x, int y, int len, char *tar, char color )
		{
			if(y < _CY1 || y > _CY2 || x+len-1 < _CX1 || x > _CX2) return;

			if(x<_CX1) x = _CX1;
			if(x + len -1 > _CX2) len = _CX2 - x +1;

			tar += _LPITCH * y + x;
			memset(tar , color, len);
		}

		/*---------------------------------------------------------------------------
		수직선 그리기(클리핑)
		---------------------------------------------------------------------------*/
		void DrawVLine8( int x, int y, int len, char *tar, char color )
		{
			register int i;
			if(x < _CX1 || x > _CX2 || y+len-1 < _CY1 || y > _CY2) return;

			if(y<_CY1) y = _CY1;
			if(y + len -1 > _CY2) len = _CY2 - y +1;

			tar += _LPITCH * y + x;

			for(i=0; i<len; i++)
			{
				*tar = color;
				tar += _LPITCH;
			}
		}

		/*---------------------------------------------------------------------------
		속이 빈 박스 그리기(클리핑)
		---------------------------------------------------------------------------*/
		void DrawBox8( int x1, int y1, int x2, int y2, char *tar, char color )
		{
			register int i,yp;
			register int xl;
			int x=0,y=0,h=0,t=0;

			//if(x1>_CX2 || x2<_CX1 || y1>_CY2 || y2<_CY1) return;

			if(x1<_CX1) { x1 = _CX1; x=1;}
			if(x2>_CX2) { x2 = _CX2; y=1;}
			if(y1<_CY1) { y1 = _CY1; h=1;}
			if(y2>_CY2) { y2 = _CY2; t=1;}

			xl = x2-x1+1;

			if(!h) memset( tar + (y1<<8) + (y1<<6) + x1 , color, xl);
			if(!t) memset( tar + (y2<<8) + (y2<<6) + x1 , color, xl);

			yp = (y1<<8) + (y1<<6);

			for(i=y1; i<=y2; i++)
			{
				if(!x) *(tar+yp+x1) = color;
				if(!y) *(tar+yp+x2) = color;
				yp+=_LPITCH;
			}
		}


		/*---------------------------------------------------------------------------
		서피스를 지정한 색으로 체움
		---------------------------------------------------------------------------*/
		void FillPage8( char *tar, char fillColor )
		{
			for(int i=0; i<_MYL; i++) memset(tar+_LPITCH*i, fillColor, _MXL);
		}


		/*---------------------------------------------------------------------------
		서피스에 점 출력
		---------------------------------------------------------------------------*/
		void PutPixel8(int x, int y, char *tar, char color )
		{
			*(tar+_LPITCH * y + x)  = color;
		}

		/*---------------------------------------------------------------------------
		특정 페이지에서 이미지를 얻음
		---------------------------------------------------------------------------*/
		void GetImage8(int x, int y, int xl, int yl, char *page, char *tar )
		{
			int i;

			page += _LPITCH * y + x;

			for(i=0; i<yl; i++)
			{
				memcpy( tar , page, xl);
				page += _LPITCH;
				tar += xl;
			}
		}


		/*---------------------------------------------------------------------------
		이미지 출력 함수
		---------------------------------------------------------------------------*/
		void PutImage8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			register int i;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp; 
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;

				tar += (yp+y) * _LPITCH + (xp+x) ;
				sor += y * xLen + x;

				for(i=0; i<yLimit; i++)
				{
					memcpy( tar , sor, xLimit);
					tar += _LPITCH;
					sor += xLen;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				tar += yp * _LPITCH + xp; 

				for(i=0; i<yLen; i++)
				{
					memcpy( tar , sor, xLen);
					tar += _LPITCH;
					sor += xLen;
				}
			}
		}


		/*--------------------------------------------------------------------------
		이미지 출력 함수(좌우 반전) : 그냥찍는 루틴에 비해 속도가 떨어짐
		--------------------------------------------------------------------------*/
		void PutImageLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			int rx, ry;              // 실제 찍힐 좌표
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; sor += y * xLen; ry=_CY1;}
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry*_LPITCH + rx + xLimit-1 ; 
				skip = _LPITCH + xLimit;

				for(i=0; i<yLimit; i++)
				{
					sor += h;

					for(j=0; j<xLimit; j++)
					{
						*tar = *sor;
						sor++;
						tar--;
					}
					tar += skip;
					sor += x;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				tar += yp * _LPITCH + xp + xLen-1;  //(== _LPITCH * yp + xp +xLen-1)
				skip = _LPITCH + xLen;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor++; tar--;
					}
					tar += skip;
				}
			}
		}


		/*--------------------------------------------------------------------------
		이미지 출력 함수(상하 반전)
		--------------------------------------------------------------------------*/
		void PutImageUD8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			int rx, ry;              // 실제 찍힐 좌표
			register int i;
			register int xLimit, yLimit;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음


			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;


			if( x || y || h || t)   // 클리핑이 발생되면
			{

				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y) + x;

				for(i=0; i<yLimit; i++)
				{
					memcpy( tar , sor, xLimit);
					tar += _LPITCH;
					sor -= xLen;
				}
			}
			else                      // 클리핑이 발생되지 않았을 경우
			{
				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1);

				for(i=0; i<yLen; i++)
				{
					memcpy( tar , sor, xLen);
					tar += _LPITCH;
					sor -= xLen;
				}
			}
		}



		/*---------------------------------------------------------------------------
		이미지를 출력하는 함수(상하 좌우 반전) : 그냥찍는 루틴에 비해 속도가 떨어짐
		---------------------------------------------------------------------------*/
		void PutImageUDLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			int rx, ry;              // 실제 찍힐 좌표
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음


			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;


			if( x || y || h || t)   // 클리핑이 발생되면
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y) + xLen-1;

				skip = _LPITCH - xLimit;
				//skip2 = x+h;

				for(i=0; i<yLimit; i++)
				{
					sor -= x;

					for(j=0; j<xLimit; j++)
					{
						*tar = *sor;
						sor--;
						tar++;
					}
					tar += skip;
					sor -= h;
				}
			}
			else                      // 클리핑이 발생되지 않았을 경우
			{
				skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;
				sor += (xLen * yLen)-1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor--; tar++;
					}
					tar += skip;
				}
			}
		}

		/*---------------------------------------------------------------------------
		각도 별로 이미지를 출력하는 함수(0,90,180,270도)
		---------------------------------------------------------------------------*/
		void PutImageAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, int ang )
		{
			if(ang==0) PutImage8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==1) PutImage90d8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==2) PutImageUDLR8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==3) PutImage270d8( xp, yp, xLen, yLen, tar, sor );
		}


		/*---------------------------------------------------------------------------
		이미지를 출력하는 함수(90도 회전)
		---------------------------------------------------------------------------*/
		void PutImage90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치(회전 후의 초과치임)
			int rx1, ry1, rx2, ry2;  // 회전 후에 실제 찍힐 좌표
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; sor += x * xLen; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1=_CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry2 * _LPITCH + rx1;

				for(i=rx1; i<=rx2; i++)
				{
					sor += t;

					for(j=ry1; j<=ry2; j++)
					{
						*tar = *sor;
						sor++;
						tar-=_LPITCH;
					}
					tar += skip;
					sor += y;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				rx1 = xp;
				ry1 = yp+xLen-1;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor++; tar-=_LPITCH;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		이미지를 출력하는 함수(270도 회전)
		---------------------------------------------------------------------------*/
		void PutImage270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치(회전 후의 초과치임)
			int rx1, ry1, rx2, ry2;  // 회전 후에 실제 찍힐 좌표
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1= _CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; sor += h * xLen; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry1 * _LPITCH + rx2;

				for(i=rx1; i<=rx2; i++)
				{
					sor += y;

					for(j=ry1; j<=ry2; j++)
					{
						*tar = *sor;
						sor++;
						tar+=_LPITCH;
					}
					tar -= skip;
					sor += t;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				rx1 = xp+yLen-1;
				ry1 = yp;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor++; tar+=_LPITCH;
					}
					tar -= skip;
				}
			}
		}

		/*---------------------------------------------------------------------------
		스프라이트를 출력하는 함수
		---------------------------------------------------------------------------*/
		void PutSpr8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)               // 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += y * xLen + x;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					inc   edi;
					loop  next_b;
					pop   ecx;

					add   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					inc   edi;
					loop  next_b2;
					pop   ecx;

					add   edi, skip;
					loop  next_a2;
				}
			}
		}

		/*---------------------------------------------------------------------------
		스프라이트를 출력하는 함수(좌우 반전)
		---------------------------------------------------------------------------*/
		void PutSprLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;
				int skip = _LPITCH + xLimit;
				int skip2 = h + x;

				tar += (yp+y) * _LPITCH + (xp+x) + xLimit-1; 
				sor += y * xLen + h;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					dec   edi;
					loop  next_b;
					pop   ecx;

					add   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH + xLen;
				tar += yp * _LPITCH + xp + xLen-1;  //(== _LPITCH * yp + xp +xLen-1)

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					dec   edi;
					loop  next_b2;
					pop   ecx;

					add   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		스프라이트를 출력하는 함수(상하 반전)
		---------------------------------------------------------------------------*/
		void PutSprUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음
			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x);
				sor += xLen * t + x;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx, xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					inc   edi;
					loop  next_b;
					pop   ecx;

					sub   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                      // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH + xLen;
				tar += (yp+yLen-1) * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					inc   edi;
					loop  next_b2;
					pop   ecx;

					sub   edi, skip;
					loop  next_a2;
				}
			}
		}



		/*---------------------------------------------------------------------------
		스프라이트를 출력하는 함수(상하 좌우 반전)
		---------------------------------------------------------------------------*/
		void PutSprUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음
			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) + xLimit-1;
				sor += xLen * t + h;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx, xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					dec   edi;
					loop  next_b;
					pop   ecx;

					sub   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                      // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH - xLen;
				tar += (yp+yLen-1) * _LPITCH + xp + xLen-1;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					dec   edi;
					loop  next_b2;
					pop   ecx;

					sub   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		스프라이트를 출력하는 함수(90도 회전)
		---------------------------------------------------------------------------*/
		void PutSpr90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치(회전 후의 초과치임)
			int rx1, ry1, rx2, ry2;  // 회전 후에 실제 찍힐 좌표
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; sor += x * xLen; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1=_CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry2 * _LPITCH + rx1;

				for(i=rx1; i<=rx2; i++)
				{
					sor += t;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++;
						tar-=_LPITCH;
					}
					tar += skip;
					sor += y;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				rx1 = xp;
				ry1 = yp+xLen-1;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++; tar-=_LPITCH;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		스프라이트를 출력하는 함수(270도 회전)
		---------------------------------------------------------------------------*/
		void PutSpr270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치(회전 후의 초과치임)
			int rx1, ry1, rx2, ry2;  // 회전 후에 실제 찍힐 좌표
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1= _CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; sor += h * xLen; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry1 * _LPITCH + rx2;

				for(i=rx1; i<=rx2; i++)
				{
					sor += y;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++;
						tar+=_LPITCH;
					}
					tar -= skip;
					sor += t;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				rx1 = xp+yLen-1;
				ry1 = yp;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++; tar+=_LPITCH;
					}
					tar -= skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		각도 별로 스프라이트를 출력하는 함수(0,90,180,270도)
		---------------------------------------------------------------------------*/
		void PutSprAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, int ang )
		{
			if(ang==0) PutSpr8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==1) PutSpr90d8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==2) PutSprUDLR8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==3) PutSpr270d8( xp, yp, xLen, yLen, tar, sor );
		}



		/*---------------------------------------------------------------------------
		단색 스프라이트를 출력하는 함수
		---------------------------------------------------------------------------*/
		void PutSprMono8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			int rx, ry;              // 실제 찍힐 좌표
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; sor += y * xLen; ry=_CY1;}
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				skip = _LPITCH - xLimit;

				for(i=0; i<yLimit; i++)
				{
					sor += x;

					for(j=0; j<xLimit; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar++;
					}
					tar += skip;
					sor += h;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar++;
					}
					tar += skip;
				}
			}

		}

		/*---------------------------------------------------------------------------
		단색으로 스프라이트를 출력하는 함수(좌우 반전)
		---------------------------------------------------------------------------*/
		void PutSprMonoLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			int rx, ry;              // 실제 찍힐 좌표
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; sor += y * xLen; ry=_CY1;}
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx + xLimit-1 ;  // _LPITCH * ry + xLimit와 같다.
				skip = _LPITCH + xLimit;

				for(i=0; i<yLimit; i++)
				{
					sor += h;

					for(j=0; j<xLimit; j++)
					{
						if(*sor != 0) *tar = color;
						sor++;
						tar--;
					}
					tar += skip;
					sor += x;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				tar += yp * _LPITCH + xp + xLen-1;  //(== _LPITCH * yp + xp +xLen-1)
				skip = _LPITCH + xLen;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if(*sor != 0) *tar = color;
						sor++; tar--;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색으로 스프라이트를 출력하는 함수(상하 반전)
		---------------------------------------------------------------------------*/
		void PutSprMonoUD8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			int rx, ry;              // 실제 찍힐 좌표
			register int i, j;
			register int xLimit, yLimit, skip, skip2;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y);

				skip = _LPITCH - xLimit;
				skip2 = xLimit + xLen + x;

				for(i=0; i<yLimit; i++)
				{
					sor += x;

					for(j=0; j<xLimit; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar++;
					}
					tar += skip;
					sor -= skip2;
				}
			}
			else                      // 클리핑이 발생되지 않았을 경우
			{
				skip = _LPITCH - xLen;
				skip2 = xLen << 1;    // xLen * 2와 같다.
				tar += yp * _LPITCH + xp;
				sor += xLen * (yLen-1);

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar++;
					}
					tar += skip;
					sor -= skip2; // xLen * 2만큼 포인터 이동
				}
			}
		}

		/*---------------------------------------------------------------------------
		단색으로 스프라이트를 출력하는 함수(상하 좌우 반전)
		---------------------------------------------------------------------------*/
		void PutSprMonoUDLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // 초과치
			int rx, ry;              // 실제 찍힐 좌표
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y) + xLen-1;

				skip = _LPITCH - xLimit;
				//skip2 = x+h;

				for(i=0; i<yLimit; i++)
				{
					sor -= x;

					for(j=0; j<xLimit; j++)
					{
						if( *sor != 0) *tar = color;
						sor--;
						tar++;
					}
					tar += skip;
					sor -= h;
				}
			}
			else                      // 클리핑이 발생되지 않았을 경우
			{
				skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;
				sor += (xLen * yLen)-1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor--; tar++;
					}
					tar += skip;
				}
			}
		}



		/*---------------------------------------------------------------------------
		단색으로 스프라이트를 출력하는 함수(90도 회전)
		---------------------------------------------------------------------------*/
		void PutSprMono90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // 초과치(회전 후의 초과치임)
			int rx1, ry1, rx2, ry2;  // 회전 후에 실제 찍힐 좌표
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; sor += x * xLen; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1=_CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry2 * _LPITCH + rx1;

				for(i=rx1; i<=rx2; i++)
				{
					sor += t;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar-=_LPITCH;
					}
					tar += skip;
					sor += y;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				rx1 = xp;
				ry1 = yp+xLen-1;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar-=_LPITCH;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색으로 스프라이트를 출력하는 함수(270도 회전)
		---------------------------------------------------------------------------*/
		void PutSprMono270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // 초과치(회전 후의 초과치임)
			int rx1, ry1, rx2, ry2;  // 회전 후에 실제 찍힐 좌표
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1= _CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; sor += h * xLen; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // 클리핑이 발생되면
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry1 * _LPITCH + rx2;

				for(i=rx1; i<=rx2; i++)
				{
					sor += y;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar+=_LPITCH;
					}
					tar -= skip;
					sor += t;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				rx1 = xp+yLen-1;
				ry1 = yp;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar+=_LPITCH;
					}
					tar -= skip;
				}
			}
		}

		/*---------------------------------------------------------------------------
		각도 별로 단색으로 스프라이트를 출력하는 함수(0,90,180,270도)
		---------------------------------------------------------------------------*/
		void PutSprMonoAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color, int ang )
		{
			if(ang==0) PutSprMono8( xp, yp, xLen, yLen, tar, sor, color );
			else if(ang==1) PutSprMono90d8( xp, yp, xLen, yLen, tar, sor, color  );
			else if(ang==2) PutSprMonoUDLR8( xp, yp, xLen, yLen, tar, sor, color  );
			else if(ang==3) PutSprMono270d8( xp, yp, xLen, yLen, tar, sor, color  );
		}

		/*---------------------------------------------------------------------------
		반투명으로 스프라이트를 출력하는 함수(좌우반전)
		(grade 가 클수록 더 투명해진다)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x) + (xLimit-1);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += y * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH + xLen;
				tar += yp * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		반투명으로 스프라이트를 출력하는 함수(상하반전)
		(grade 가 클수록 더 투명해진다)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += t * xLen + x; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}

			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH + xLen;
				tar += (yp+yLen-1) * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		반투명으로 스프라이트를 출력하는 함수(상하좌우 반전)
		(grade 가 클수록 더 투명해진다)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) + (xLimit-1);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += t * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH - xLen;
				tar += (yp+yLen-1) * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색 반투명으로 스프라이트를 출력하는 함수
		- grade : 반투명 단계(클수록 소스가 투명해진다)
		- color : 반투명 색
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += y * xLen + x; 

				_asm
				{
					//PUSHAD;
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + color
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
					//POPAD;
				}

			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + color
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색 반투명으로 스프라이트를 출력하는 함수(좌우반전)
		(grade 가 클수록 더 투명해진다)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x) + (xLimit-1);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += y * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH + xLen;
				tar += yp * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색 반투명으로 스프라이트를 출력하는 함수(상하반전)
		(grade 가 클수록 더 투명해진다)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += t * xLen + x; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}

			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH + xLen;
				tar += (yp+yLen-1) * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색 반투명으로 스프라이트를 출력하는 함수(상하좌우 반전)
		(grade 가 클수록 더 투명해진다)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 클리핑될 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[?] 번지를 찾는다.

			if( x || y || h || t)               // 클리핑이 발생되면(초과치가 있으면)
			{
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) + (xLimit-1);  // 실제 찍힐좌표를 초과치로 보정해서 주소계산
				sor += t * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0번색이면 점프

					// 알파테이블에서 색찾아 페이지에 뿌리기
					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH - xLen;
				tar += (yp+yLen-1) * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0번색이면 점프

					xor   eax,eax;               // EAX를 0으로 초기화
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////


		/*---------------------------------------------------------------------------
		압축 스프라이트를 출력하는 함수(좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // 라인의 끝으로 포인터 이동
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								RvrsMemCpy4b(tar-sbyte-temp+1, sor, temp);
								doing = ENDPUTSPR;
							}
							else RvrsMemCpy4b(tar-sbyte-pbyte+1, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y) * _LPITCH + xp + (xLen);   // 찍을 좌표계산(한도트 초과)
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기

					movzx   ecx, al;   
					shr   ecx, 1;
					jnc   next_c2;
					lodsb;
					dec   edi;
					mov   [edi],al;

next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					lodsw;
					xchg  ah, al;     // 바이트 뒤집기(WORD)
					sub   edi,2;
					mov   [edi],ax;

next_d2:
					cmp   ecx, 0;     
					je    next_f2;     // 이미지 개수가 0이면 점프
next_e2:
					lodsd;
					bswap eax;         // 바이트 뒤집기(Double WORD)
					sub   edi,4;
					mov   [edi],eax;
					loop  next_e2;     // 이미지 개수만큼 반복

next_f2:
					pop   ecx;
					loop  next_b2;     // 패턴수만큼 반복

					pop   ecx;
					add   edi, skip;
					loop  next_a2;     // Y길이만큼 반복
				}
			}
		}

		/*---------------------------------------------------------------------------
		압축 스프라이트를 출력하는 함수(상하반전)
		---------------------------------------------------------------------------*/
		void PutCspUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memcpy(tar+sbyte, sor, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memcpy(tar+sbyte, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;

next_b2:
					push  ecx;
					lodsb;            // 0번색 개수 읽기
					add   edi, eax;
					lodsb;            // 이미지 개수 읽기

					mov   ecx, eax;   
					shr   ecx, 1;
					jnc   next_c2;
					movsb;
next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					movsw;
next_d2:
					rep   movsd;

					pop   ecx;
					loop  next_b2;

					pop   ecx;
					sub   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		압축 스프라이트를 출력하는 함수(상하좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit);  // 찍을 좌표 계산(한도트초과) - 역순
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // 라인의 끝으로 포인터 이동
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								RvrsMemCpy4b(tar-sbyte-temp+1, sor, temp);
								doing = ENDPUTSPR;
							}
							else RvrsMemCpy4b(tar-sbyte-pbyte+1, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLen -1);   // 찍을 좌표계산
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기

					movzx   ecx, al;   
					shr   ecx, 1;
					jnc   next_c2;
					lodsb;
					dec   edi;
					mov   [edi],al;

next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					lodsw;
					xchg  ah, al;     // 바이트 뒤집기(WORD)
					sub   edi,2;
					mov   [edi],ax;

next_d2:
					cmp   ecx, 0;     
					je    next_f2;     // 이미지 개수가 0이면 점프
next_e2:
					lodsd;
					bswap eax;         // 바이트 뒤집기(Double WORD)
					sub   edi,4;
					mov   [edi],eax;
					loop  next_e2;     // 이미지 개수만큼 반복

next_f2:
					pop   ecx;
					loop  next_b2;     // 패턴수만큼 반복

					pop   ecx;
					sub   edi, skip;
					loop  next_a2;     // Y길이만큼 반복
				}
			}
		}



		/*---------------------------------------------------------------------------
		압축 스프라이트 출력(테스트용)
		---------------------------------------------------------------------------*/
		void PutCsp8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )               // 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(y<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memcpy(tar+sbyte, sor, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memcpy(tar+sbyte, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else                    // 클리핑이 발생되지 않았을 경우
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산
				tar += (yp+y)* _LPITCH + xp;
				sor += (*(long*)(sor+(y<<2)));    // 찍기 시작할 라인으로 이동

				int i,cnt;
				int sbyte,pbyte;

				for(i=0; i<yLimit; i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);
					sor+=2;

					sbyte=0;
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte += (BYTE)(*sor++);
						pbyte = (BYTE)(*sor++);

						memcpy(tar+sbyte, sor, pbyte);
						//MemCpy4b(tar+sbyte, sor, pbyte);
						//RvrsMemCpy4b(tar+sbyte, sor, pbyte);

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
		}

		/*---------------------------------------------------------------------------
		단색으로 압축 스프라이트를 출력하는 함수
		---------------------------------------------------------------------------*/
		void PutCspMono8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(y<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memset(tar+sbyte, color, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memset(tar+sbyte, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0번색 개수 읽기
					add   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;

					mov   bl,  color; // EAX에 color를 4바이트 복사
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					add   esi, ecx;   // 소스는 미리 이동

					shr   ecx, 1;
					jnc   next_c2;
					stosb;
next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					stosw;
next_d2:
					rep   stosd;

					pop   ecx;
					loop  next_b2;

					pop   ecx;
					add   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색으로 압축 스프라이트를 출력하는 함수(좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspMonoLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								MemSet4b(tar-sbyte-temp+1, color, temp);
								doing = ENDPUTSPR;
							}
							else MemSet4b(tar-sbyte-pbyte+1, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y) * _LPITCH + xp + (xLen);   // 찍을 좌표계산(한도트 초과)
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;

next_b2:
					cld;
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기

					movzx ecx, al;   

					mov   bl,  color; // EAX에 color를 4바이트 복사
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					//std;
					add   esi,  ecx;   // 소스를 미리 이동
					shr   ecx,  1;
					jnc   next_c2;
					dec   edi;
					mov   [edi],al;
					//

next_c2:
					shr   ecx,  1;
					jnc   next_d2;
					sub   edi,  2;
					mov   [edi],ax;

next_d2:

					cmp   ecx,  0;
					je    next_f2;
next_e2:
					sub   edi,4
						mov   [edi], eax;
					loop  next_e2;

next_f2:
					pop   ecx;
					loop  next_b2;     // 패턴수만큼 반복

					add   edi, skip;
					pop   ecx;
					loop  next_a2;     // Y길이만큼 반복
				}
			}
		}

		/*---------------------------------------------------------------------------
		단색으로 압축 스프라이트를 출력하는 함수(상하반전)
		---------------------------------------------------------------------------*/
		void PutCspMonoUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memset(tar+sbyte, color, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memset(tar+sbyte, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0번색 개수 읽기
					add   edi, eax;
					lodsb;            // 이미지 개수 읽기

					mov   ecx, eax;   

					mov   bl,  color; // EAX에 color를 4바이트 복사
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					add   esi, ecx;   // 소스는 미리 이동

					shr   ecx, 1;
					jnc   next_c2;
					stosb;
next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					stosw;
next_d2:
					rep   stosd;

					pop   ecx;
					loop  next_b2;

					pop   ecx;
					sub   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색으로 압축 스프라이트를 출력하는 함수(상하좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspMonoUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // 초과치

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // 라인의 끝으로 포인터 이동
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								MemSet4b(tar-sbyte-temp+1, color, temp);
								doing = ENDPUTSPR;
							}
							else MemSet4b(tar-sbyte-pbyte+1, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLen);   // 찍을 좌표계산(한도트 초과)
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;

next_b2:
					cld;
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기

					movzx ecx, al;   

					mov   bl,  color; // EAX에 color를 4바이트 복사
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					//std;
					add   esi,  ecx;   // 소스를 미리 이동
					shr   ecx,  1;
					jnc   next_c2;
					dec   edi;
					mov   [edi],al;
					//

next_c2:
					shr   ecx,  1;
					jnc   next_d2;
					sub   edi,  2;
					mov   [edi],ax;

next_d2:

					cmp   ecx,  0;
					je    next_f2;
next_e2:
					sub   edi,4
						mov   [edi], eax;
					loop  next_e2;

next_f2:
					pop   ecx;
					loop  next_b2;     // 패턴수만큼 반복

					sub   edi, skip;
					pop   ecx;
					loop  next_a2;     // Y길이만큼 반복

				}
			}
		}



		/*---------------------------------------------------------------------------
		반투명으로 압축 스프라이트를 출력
		---------------------------------------------------------------------------*/
		void PutCspGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(y<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassCpy(tar+sbyte, sor, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassCpy(tar+sbyte, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					add   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					inc   esi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}


		/*---------------------------------------------------------------------------
		반투명으로 압축 스프라이트를 출력(좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // 라인의 끝으로 포인터 이동
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								RvrsGlassCpy(tar-sbyte-temp+1, sor, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassCpy(tar-sbyte-pbyte+1, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y) * _LPITCH + xp + (xLimit-1);   // 좌표교정 (역순)
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					inc   esi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}


		/*---------------------------------------------------------------------------
		반투명으로 압축 스프라이트를 출력(상하반전)
		---------------------------------------------------------------------------*/
		void PutCspGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.
			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassCpy(tar+sbyte, sor, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassCpy(tar+sbyte, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동
				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					add   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					inc   esi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}


		/*---------------------------------------------------------------------------
		반투명으로 압축 스프라이트를 출력(상하좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // 라인의 끝으로 포인터 이동
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								RvrsGlassCpy(tar-sbyte-temp+1, sor, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassCpy(tar-sbyte-pbyte+1, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH - xLen;
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLimit-1);   // 좌표교정 (역순)
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					inc   esi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}

		/*---------------------------------------------------------------------------
		단색 반투명으로 압축 스프라이트를 출력
		---------------------------------------------------------------------------*/
		void PutCspMonoGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(y<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassSet(tar+sbyte, color, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassSet(tar+sbyte, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					add   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
					add   esi, ecx;   // 소스는 미리증가
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					//inc   esi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색 반투명으로 압축 스프라이트를 출력(좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspMonoGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // 라인의 끝으로 포인터 이동
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								RvrsGlassSet(tar-sbyte-temp+1, color, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassSet(tar-sbyte-pbyte+1, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y) * _LPITCH + xp + (xLimit-1);   // 좌표교정 (역순)
				sor += (*(long*)(sor+(y<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
					add   esi, ecx;   // 소스는 미리 증가
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + color
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색 반투명으로 압축 스프라이트를 출력(상하반전)
		---------------------------------------------------------------------------*/
		void PutCspMonoGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.
			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));    // 찍기 시작할 라인으로 이동

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassSet(tar+sbyte, color, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassSet(tar+sbyte, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // 실제찍을 좌표를 초과치로 보정하여 주소계산
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동
				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					add   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
					add   esi, ecx;   // 소스는 미리 증가
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					//inc   esi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}


		/*---------------------------------------------------------------------------
		단색 반투명으로 압축 스프라이트를 출력(상하좌우반전)
		---------------------------------------------------------------------------*/
		void PutCspMonoGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// 클리핑 영역 밖으로 나가면 출력하지 않음

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int xLimit = xLen -x - h;           // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;           // Y축으로 찍을 소스의 도트수 계산

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit-1);  // 찍을 좌표 계산(초과치 보정) - 역순
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동 - 순차

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // 패턴수를 읽음
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // 라인의 끝으로 포인터 이동
					while(cnt--)         // 패턴수 만큼 반복
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // 출력이 시작되는 곳을 계산
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // 출력할 상황이면
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // 찍을 도트수
								RvrsGlassSet(tar-sbyte-temp+1, color, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassSet(tar-sbyte-pbyte+1, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 위쪽과 아래쪽만 발생한 경우
			{
				int skip = _LPITCH - xLen;
				int xLimit = xLen -x - h;       // X축으로 찍을 소스의 도트수 계산
				int yLimit = yLen -y - t;       // Y축으로 찍을 소스의 도트수 계산

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLimit-1);   // 좌표교정 (역순)
				sor += (*(long*)(sor+(t<<2)));  // 찍기 시작할 라인으로 이동

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // 패턴수 읽기
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0번색 개수 읽기
					sub   edi, eax;
					lodsb;            // 이미지 개수 읽기
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // 이미지 개수가 0이면 점프
					add   esi, ecx;   // 소스는 미리 증가
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					//inc   esi;
					loop  next_c2;    // 이미지수만큼 반복
next_d2:	
					pop   ecx;
					loop  next_b2;    // 패턴수만큼 반복
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // 라인수만큼 반복
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////



		void LowZoomSpr8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMono8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMonoUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprGlass8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							//-->if(*src) *dest = (char)pAlpha[grade][*dest][*src];
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprGlassLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprGlassUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprGlassUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMonoGlass8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// 알파테이블의 pAlpha[grade] 번지를 찾는다.

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoGlassLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoGlassUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMonoGlassUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // 타겟초과치
			BYTE *pAT;                 // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void ZoomSprNoclip_Test(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int xErt,yErt;
			register int xCnt, yCnt;
			char *dest, *src;

			tar += _LPITCH * yp + xp;

			yErt = 0;
			yCnt = yLen;
			for(; yCnt; yCnt--)
			{
				yErt += tyl;
				while(yErt >= yLen)
				{
					yErt -= yLen;
					dest = tar;
					src = sor;
					xCnt = xLen;
					xErt = 0;

					for(; xCnt; xCnt--)
					{
						xErt += txl;
						while(xErt >= xLen)
						{
							xErt-=xLen;
							if(*src) *dest = *src;
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////                             /////////////////////////////////
		////////////////////////////  압축스프라이트 줌인/아웃   /////////////////////////////////
		////////////////////////////                             /////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////


		void LowZoomCsp8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}
							//dest += xErt/sorXL;
							//xErt = xErt%sorXL;

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}
							//dest += xErt/sorXL;
							//xErt = xErt%sorXL;

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest--;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMono8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspMonoUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspGlass8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspGlassLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspGlassUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspGlassUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}



		void LowZoomCspMonoGlass8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoGlassLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoGlassUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspMonoGlassUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// 알파 테이블이 존재하지 않으면 취소
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치
			BYTE *pAT;                        // 알파 테이블의 포인터 저장용

			int xErt,yErt=0;                // X,Y 에러텀
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // 패턴수 만큼 반복
						{
							sbyte = (BYTE)(*src++);  // 0번색의 개수 읽기
							pbyte = (BYTE)(*src++);  // 데이타의 개수 읽기


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

	}//namespace GRAPHICGDI

}//namespace NMBASE

