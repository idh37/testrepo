//
//                                   그래픽 처리 모듈
//
#include "StdAfx.h"
#include "Graphic.h"
#include "Jpegfile.h"
#include <math.h>

namespace NMBASE
{
	namespace GRAPHICGDI
	{
		#define TO_RADIAN(x) ((x) * 0.0174773f)
		#define FLOATROUNDINT(x) ( (int)((x)+0.5f) )
		#define FLOATROUND2(x) ( floor(10.0*((x)+0.05))/10.0 )


		#define _swap(a,b) {(a)^=(b); (b)^=(a); (a)^=(b);}

		// 스프라이트 히트 테스트
		BOOL PointHitTest(xSprite *pSpr, int sprnum, int x, int y, DWORD opt)
		{
			if(pSpr==NULL) return FALSE;
			if(pSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pSpr->sfhead.TotalSprNum) return FALSE;

			int tx = x;
			int ty = y;

			if(opt & RXY)
			{
				tx -= pSpr->spr[sprnum].rx;
				ty -= pSpr->spr[sprnum].ry;
			}

			if(opt & LR) 
			{
				tx = pSpr->spr[sprnum].xl - tx;
			}

			if(opt & UD) 
			{
				ty = pSpr->spr[sprnum].yl - ty;
			}

			if(pSpr->spr[sprnum].compress) 
				return _PointHitTestCsp(pSpr->spr[sprnum].xl, pSpr->spr[sprnum].yl, pSpr->spr[sprnum].data, tx, ty);
			else 
				return _PointHitTestSpr(pSpr->spr[sprnum].xl, pSpr->spr[sprnum].yl, pSpr->spr[sprnum].data, pSpr->spr[sprnum].colorkey, tx, ty);
		}

		// 줌을 적용한 스프라이트 히트 테스트
		BOOL PointHitTest_Zoom(xSprite *pSpr, int sprnum, int x, int y, int w, int h, DWORD opt)
		{
			if(pSpr == NULL) return FALSE;
			if(pSpr->spr == NULL) return FALSE;
			if(sprnum < 0 || sprnum >= pSpr->sfhead.TotalSprNum) return FALSE;
			if(pSpr->spr[sprnum].xl == 0 || pSpr->spr[sprnum].yl == 0)
				return FALSE;
			if(w == 0 || h == 0)
				return FALSE;

			float tw = ((float)pSpr->spr[sprnum].xl / (float)w);
			float th = ((float)pSpr->spr[sprnum].yl / (float)h);

			int tx = FLOATROUNDINT(x * tw);
			int ty = FLOATROUNDINT(y * th);

			return PointHitTest(pSpr, sprnum, tx, ty, opt);
		}

		// 회전과 줌을 적용한 스프라이트 히트 테스트
		BOOL PointHitTest_RotateZoom(xSprite *pSpr, int sprnum, int x, int y, int w, int h, float angle, DWORD opt)
		{
			if(pSpr == NULL) return FALSE;
			if(pSpr->spr == NULL) return FALSE;
			if(sprnum < 0 || sprnum >= pSpr->sfhead.TotalSprNum) return FALSE;
			if(pSpr->spr[sprnum].xl == 0 || pSpr->spr[sprnum].yl == 0)
				return FALSE;
			if(w == 0 || h == 0)
				return FALSE;

			if(angle != 0) {
				POINT rp;
				rp.x = x;
				rp.y = y;

				_RotatePoint(&rp, -angle);

				x = rp.x;
				y = rp.y;
			}

			return PointHitTest_Zoom(pSpr, sprnum, x, y, w, h, opt);
		}


		//////////////////////////////////////////////////


		CPage::CPage()
		{
			lpBmpInfo = NULL;
			lpBit     = NULL;
			lPitch    = 0;
			lPitchHalf= 0;
			nBitNum   = 8;
			nBPD      = 1;
			nColorNum = 0;
			lpLogPal  = NULL;
			hPal      = NULL;
			hBmp      = NULL;

			nAllocSize = 0;

			////////////

			bEnableTempClip = FALSE;

			////////////

			TClipX1 = 0;
			TClipY1 = 0;
			TClipX2 = 0;
			TClipY2 = 0;

			MirrorX = 0;

			////////////

			bAttached = FALSE;

			////////////

			xpFlip = 0;
			ypFlip = 0;
			iZoomX = 1;

			////////////

			bScnEffect = FALSE;

			iDrawingTrackBegin = -1;
			iDrawingTrackEnd = -1;
		}


		CPage::~CPage()
		{
			Destroy();
		}

		void CPage::Destroy()
		{
			if(bAttached) Detach();

			nColorNum = 0;
			nAllocSize = 0;
			bScnEffect = FALSE;

			if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
			//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
			if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; }
			if(hPal) { DeleteObject(hPal); hPal=NULL;}
			if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}
		}

		// 임시클리핑 영역을 설정한다
		void CPage::SetTempClip(int x1, int y1, int x2, int y2)
		{
			if(x1 > x2) _swap(x1, x2);
			if(y1 > y2) _swap(y1, y2);

			x1 = min(max(x1, 0), width-1);
			x2 = min(max(x2, 0), width-1);
			y1 = min(max(y1, 0), height-1);
			y2 = min(max(y2, 0), height-1);

			TClipX1 = x1;
			TClipY1 = y1;
			TClipX2 = x2;
			TClipY2 = y2;
		}

		// 임시 클리핑 영역의 활성화 여부를 설정
		void CPage::EnableTempClip(BOOL bEnable)
		{
			bEnableTempClip = bEnable;
		}

		// 미러찍기 기준 좌표를 설정한다
		void CPage::SetMirrorX(int x)
		{
			MirrorX = x;
		}

		/*-----------------------------------------------------------------------------------------
		페이지를 초기화 한다
		------------------------------------------------------------------------------------------*/
		BOOL CPage::Init(LONG xl, LONG yl, INT bit, RGBQUAD *lpPal, LPCTSTR palfname)
		{
			if(bAttached) Detach();

			//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
			if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
			if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}

			switch(bit)
			{
			case 4  : nBPD=1; nColorNum=16;  break;
			case 8  : nBPD=1; nColorNum=256; break;
			case 16 : nBPD=2; nColorNum=0;   break;
			case 24 : nBPD=3; nColorNum=0;   break;
			default : return FALSE;
			}

			nBitNum = bit;
			width = xl;
			lPitch = xl*nBPD;
			height = yl;

			lPitch = (lPitch+3)&~3;	// Pitch가 4의 배수가 되게 한다(while(lPitch%4) lPitch++;식과 같다)
			// 여기서의 lPitch란 바이트수이다(점의 개수는 아니다)
			lPitchHalf = lPitch/nBPD; // lPitch는 바이트수이므로 8,16,23비트 모드의 점의 개수를 얻는다.

			// 비트맵 인포 구조체를 위한 메모리 할당
			nAllocSize = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * nColorNum;
			//if(nColorNum != 0) nAllocSize -= sizeof(RGBQUAD);
			nAllocSize -= sizeof(RGBQUAD);
			lpBmpInfo = (LPBITMAPINFO)new char[nAllocSize + sizeof(RGBQUAD)];
			if(lpBmpInfo == NULL) return FALSE;

			// 가상 페이지 생성(비트맵 이미지 포인터)
			//lpBit = (WORD*)new char[lPitch * yl];
			//if(lpBit == NULL) return FALSE;

			// 비트맵 인포 해더 생성
			lpBmpInfo->bmiHeader.biSize	         = sizeof (BITMAPINFOHEADER);
			lpBmpInfo->bmiHeader.biWidth         = width;      //width;
			lpBmpInfo->bmiHeader.biHeight        = -height;    // 음수면 그림이 뒤집혀 출력된다
			lpBmpInfo->bmiHeader.biPlanes        = 1;          // 항상1
			lpBmpInfo->bmiHeader.biBitCount	     = nBitNum;    // 도트당 비트수(16비트는 5:6:5)
			lpBmpInfo->bmiHeader.biCompression	 = 0;          // BI_RGB와 같다(비압축)
			lpBmpInfo->bmiHeader.biSizeImage     = lPitch*yl;  // lPitch; // 항상 4의배수
			lpBmpInfo->bmiHeader.biXPelsPerMeter = 0;
			lpBmpInfo->bmiHeader.biYPelsPerMeter = 0;
			lpBmpInfo->bmiHeader.biClrUsed	     = 0;          // 사용된 총 칼라수(항상 0으로 고정)
			lpBmpInfo->bmiHeader.biClrImportant	 = 0;          // 중요한 색상수(항상 0으로 고정)

			// 파레트 세팅
			if( nColorNum != 0) {
				if(palfname) OpenPal(palfname);
				else SetPal(lpPal);
			}

			// DIB를 생성
			ReCreateDIBSection();

			// 그래픽 라입 초기화
			InitGrpLib(xl, yl, lPitchHalf, 555);
			InitAlphaTable(555);

			return TRUE;
		}

		BOOL CPage::Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch)
		{
			if(lpBmpInfo) return FALSE;
			if(hBmp) return FALSE;
			if(bAttached) return FALSE;

			nBPD=2; nColorNum=0;
			nBitNum = 16;
			width = xl;
			lPitch = lpitch;
			height = yl;
			//while(lPitch%4) lPitch++; // Pitch가 4의 배수가 되게 한다
			lPitchHalf = lPitch/nBPD; // lPitch는 바이트수이므로 8,16,23비트 모드의 점의 개수를 얻는다.
			lpBmpInfo = NULL;
			lpBit = (WORD*)lpbit;

			InitGrpLib(xl, yl, lPitch/2, 555);

			bAttached = TRUE;
			return TRUE;
		}

		BOOL CPage::Detach()
		{
			if(!bAttached) return FALSE;

			lpBmpInfo = NULL;
			hBmp = NULL;
			lpBit = NULL;
			width = height = 0;
			lPitch = lPitchHalf = 0;
			bAttached = FALSE;

			return TRUE;
		}

		BOOL CPage::ReCreateDIBSection()
		{
			if(hBmp) DeleteObject(hBmp);

			// DIB를 생성
			HWND ActiveWindow = GetActiveWindow();
			HDC ScreenDC = GetDC(ActiveWindow);
			hBmp = CreateDIBSection(ScreenDC, lpBmpInfo, DIB_RGB_COLORS, (VOID**)&lpBit, NULL, 0);
			ReleaseDC(ActiveWindow, ScreenDC);

			return (hBmp != NULL);
		}

		BOOL CPage::OpenPal(LPCTSTR filename)
		{
			if(nColorNum<=0) return FALSE;

			FILE *fp;
			RGBQUAD rgbpal[256];
			if( (fp = fopen( filename, "rb")) == NULL ) return FALSE;

			fread(rgbpal, nColorNum*sizeof(RGBQUAD), 1, fp);
			fclose(fp);

			return SetPal(rgbpal);
		}


		/*-----------------------------------------------------------------------------------------
		페이지를 비트맵 파일로 저장한다
		------------------------------------------------------------------------------------------*/
		BOOL CPage::SaveToBitmap(LPSTR fname)
		{
			if(!fname) return FALSE;
			if(!lpBit || !lpBmpInfo) return FALSE;
			if(nAllocSize <= 0) return FALSE;
			if(nBitNum != 16) return FALSE;

			FILE *fp = fopen(fname, "wb");
			if(!fp) return FALSE;

			int npitch = width * 3;
			while(npitch%4) npitch++;
			int allocsize = npitch * height;
			BYTE *pimage = new BYTE[allocsize];
			memset(pimage, 0, allocsize);
			BYTE *bit = pimage;

			// 16비트 이미지를 24비트로 변환(이미지를 뒤집음)
			for(int i=height-1; i>=0; i--) {
				for(int j=0; j<width; j++) {
					WORD color = *((WORD*)lpBit + (i * lPitchHalf + j));
					// 주의) BGR순이다
					*(bit+(j*3)) = ((BYTE)B555(color)) << 3;	// B
					*(bit+(j*3)+1) = ((BYTE)G555(color)) << 3;	// G
					*(bit+(j*3)+2) = ((BYTE)R555(color)) << 3;	// R
				}
				bit += npitch;
			}

			BITMAPFILEHEADER	bmfHeader = {0,};
			BITMAPINFOHEADER    bmiHeader = {0,};
			memcpy(&bmiHeader, &lpBmpInfo->bmiHeader, sizeof(BITMAPINFOHEADER));

			bmiHeader.biBitCount = 24;
			bmiHeader.biHeight = height;

			bmfHeader.bfType = ((WORD)('M'<<8 | 'B'));	// bmp포맷 표시자;
			bmfHeader.bfSize = 14 + sizeof(BITMAPINFOHEADER) + allocsize;
			bmfHeader.bfOffBits = 14 + sizeof(BITMAPINFOHEADER);
			fwrite(&bmfHeader, 14, 1, fp);
			fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
			fwrite(pimage, allocsize, 1, fp);

			fclose(fp);

			if(pimage)
			{
				delete [] pimage;
			}

			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		페이지를 JPG 파일로 저장한다(quality = 0-100)
		------------------------------------------------------------------------------------------*/
		BOOL CPage::SaveToJpeg(LPSTR fname, int quality)
		{
			if(!fname) return FALSE;
			if(!lpBit || !lpBmpInfo) return FALSE;
			if(nAllocSize <= 0) return FALSE;
			if(nBitNum != 16) return FALSE;

			int npitch = width * 3;
			//while(npitch%4) npitch++;	// 4바이트 정렬코드
			int allocsize = npitch * height;
			BYTE *pimage = new BYTE[allocsize];
			memset(pimage, 0, allocsize);

			BYTE *bit = pimage;

			// 16비트 이미지를 24비트로 변환(이미지를 뒤집지 않아야한다)
			for(int i=0; i<height; i++) {
				for(int j=0; j<width; j++) {
					WORD color = *((WORD*)lpBit + (i * lPitchHalf + j));
					// RGB
					*(bit+(j*3)) = ((BYTE)R555(color)) << 3;	// R
					*(bit+(j*3)+1) = ((BYTE)G555(color)) << 3;	// G
					*(bit+(j*3)+2) = ((BYTE)B555(color)) << 3;	// B
				}
				bit += npitch;
			}

			BOOL bSuccess = JpegFile::RGBToJpegFile(fname,		// path
				pimage,		// RGB buffer
				width,		// pixels
				height,		// rows
				TRUE,		// TRUE = RGB, FALSE = Grayscale
				quality);	// 0 - 100

			if(pimage)
			{
				delete [] pimage;
			}

			return bSuccess;
		}

		/*-----------------------------------------------------------------------------------------
		페이지의 파레트를 설정한다

		- 인수 : RGBQUAD의 포인터
		------------------------------------------------------------------------------------------*/
		BOOL CPage::SetPal(RGBQUAD *lpPal)
		{
			if( nColorNum == 0)	
				return TRUE;        //파레트가 필요없다면

			if(lpLogPal) 
			{
				free( lpLogPal );	// 메모리 해제
				lpLogPal=NULL;
			} 

			lpLogPal = (LPLOGPALETTE)malloc( sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum );

			if(NULL == lpLogPal)
				return FALSE;

			lpLogPal->palVersion    = 0x300;	    // 팔레트 버젼 입력
			lpLogPal->palNumEntries = nColorNum;    // 팔레트 색상수 입력

			int i;
			if(NULL == lpPal)	// 파레트 포인터가 NULL이면 기본 파레트를 생성한다
			{             
				for(i=0; i<nColorNum; i++)
				{
					lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed      = (BYTE)(((i>>5) & 0x07) * 255/7);
					lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen    = (BYTE)(((i>>2) & 0x07) * 255/7);
					lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue     = (BYTE)(((i>>0) & 0x03) * 255/3);
					lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = (BYTE)0;
				}
			}
			else
			{
				for(i=0; i<nColorNum; i++)
				{
					lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed   = lpPal[i].rgbRed;
					lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen = lpPal[i].rgbGreen;
					lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue  = lpPal[i].rgbBlue;
					lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = 0;
				}
			}

			if(hPal) 
			{
				DeleteObject(hPal); 
				hPal=NULL;
			}

			hPal = CreatePalette( lpLogPal );		

			if(hPal == NULL)
				return FALSE;

			return TRUE;
		}

		BOOL CPage::SetLogPal(LPLOGPALETTE lpPal)
		{
			if( nColorNum == 0)	//파레트가 필요없다면
				return TRUE;

			if(lpLogPal!=NULL) { free( lpLogPal );  lpLogPal=NULL; } // 메모리 해제

			LONG size = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum;
			lpLogPal = (LPLOGPALETTE)malloc( size );

			if(NULL == lpLogPal)
				return FALSE;

			memcpy(lpLogPal, lpPal, size);

			if(hPal) { DeleteObject(hPal); hPal=NULL;}
			hPal = CreatePalette( lpLogPal );		

			if(hPal == NULL) return FALSE;
			return TRUE;
		}


		BOOL CPage::SetPalHandle(HPALETTE h)
		{
			if(h==NULL) return FALSE;
			if(hPal) { DeleteObject(hPal); hPal=NULL;}
			hPal = h;
			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		GDI 표면을 특정 칼라로 채운다
		------------------------------------------------------------------------------------------*/
		BOOL CPage::FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor)
		{
			BOOL check;
			check = FloodFill(hTarDC, nXStart, nYStart, crColor);
			if(check != 0)return TRUE;
			else return FALSE;
		}

		/*-----------------------------------------------------------------------------------------
		다른 페이지의 내용을 현재 페이지 표면에 뿌린다
		------------------------------------------------------------------------------------------*/
		BOOL CPage::PutFromPage(LONG xp, LONG yp, CPage *pPage, int blendgrade)
		{
			int w = pPage->width;
			int h = pPage->height;
			if(lpBit == NULL) return FALSE;
			if(pPage->lpBit == NULL) return FALSE;
			if(w==0 || h==0) return FALSE;

			InitGrpLib(width, height, lPitchHalf, 555);
			if(blendgrade >= 31) _PutImage(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit);
			else _PutImage_GB(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, blendgrade);

			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		다른 페이지의 내용을 컬러키를 적용하여 현재 페이지 표면에 뿌린다
		------------------------------------------------------------------------------------------*/
		BOOL CPage::PutFromPageWithColorKey(LONG xp, LONG yp, CPage *pPage, WORD colorkey, int blendgrade)
		{
			int w = pPage->width;
			int h = pPage->height;
			if(lpBit == NULL) return FALSE;
			if(pPage->lpBit == NULL) return FALSE;
			if(w==0 || h==0) return FALSE;

			InitGrpLib(width, height, lPitchHalf, 555);
			if(blendgrade >= 31) _PutSpr(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, colorkey);
			else _PutSpr_GB(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, colorkey, blendgrade);

			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		페이지가 뿌려질 클라이언트의 특정 좌표를 페이지의 좌표로 변환시킨다
		------------------------------------------------------------------------------------------*/
		void CPage::ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y)
		{
			*X = (xp-xpFlip)/iZoomX;
			*Y = (yp-ypFlip)/iZoomX;
		}

		/*-----------------------------------------------------------------------------------------
		페이지를 플립할 GDI표면 위치(클라이언트 영역)을 설정한다
		------------------------------------------------------------------------------------------*/
		void CPage::SetFlipPos(LONG xp, LONG yp, int zoom)
		{
			xpFlip = xp;
			ypFlip = yp;
			iZoomX = zoom;
		}

		/*-----------------------------------------------------------------------------------------
		페이지의 내용을 GDI 표면에 정수배로 뿌린다(SetPagePos()로 위치를 바꿀수 있다)
		------------------------------------------------------------------------------------------*/
		BOOL CPage::Flip(HWND hwnd)
		{
			// 페이지가 초기화 되지 않았다면
			if(  lpBit == NULL ) return FALSE;
			HDC hDC = GetDC(hwnd);
			BOOL rtn = Flip(hDC);
			ReleaseDC(hwnd, hDC);
			return rtn;
		}

		BOOL CPage::Flip(HDC hdc)
		{
			int rtn;

			// 페이지가 초기화 되지 않았다면
			if(  lpBit == NULL ) return FALSE;

			HDC hDC = hdc;

			if( nColorNum != 0 && hPal != NULL)	//파레트가 필요하다면
			{
				HPALETTE hOldPal;
				hOldPal = SelectPalette( hDC, hPal, TRUE ); //팔레트 선택
				if(hOldPal == NULL) return FALSE;
				rtn = RealizePalette( hDC );				 //팔레트 활성화
				if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
			}

			int xl = width * iZoomX;
			int yl = height * iZoomX;

			rtn = StretchDIBits( hDC,
				xpFlip,
				ypFlip,
				xl,
				yl,
				0,
				0,
				width,
				height,
				lpBit,
				lpBmpInfo,
				DIB_RGB_COLORS,
				SRCCOPY );

			if(rtn == GDI_ERROR) return FALSE;
			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		페이지의 내용을 GDI 표면에 정수배 확대로 뿌린다(SetFlipPos()를 미리실행)
		------------------------------------------------------------------------------------------*/
		BOOL CPage::FlipToGDI( HDC hdc, LONG xp, LONG yp, int zoom)
		{
			SetFlipPos(xp, yp, zoom);
			return Flip(hdc);
		}

		BOOL CPage::FlipToGDI( HWND hwnd, LONG xp, LONG yp, int zoom)
		{
			SetFlipPos(xp, yp, zoom);
			return Flip(hwnd);
		}

		/*-----------------------------------------------------------------------------------------
		페이지의 내용을 GDI 표면에 Stretch로 뿌린다

		- 소스나 타겟의 RECT가 NULL이면 전체 영역으로 설정된다
		------------------------------------------------------------------------------------------*/
		BOOL CPage::StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc )
		{
			int rtn;

			// 페이지가 초기화 되지 않았다면
			if(  lpBit == NULL ) return FALSE;

			HDC hDC = GetDC(hwnd);

			if( nColorNum != 0 && hPal != NULL)	//파레트가 필요하다면
			{
				HPALETTE hOldPal;
				hOldPal = SelectPalette( hDC, hPal, TRUE ); //팔레트 선택
				if(hOldPal == NULL) return FALSE;
				rtn = RealizePalette( hDC );				 //팔레트 활성화
				if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
			}

			//int xl = width * iZoomX;
			//int yl = height * iZoomX;

			int sx, sy, sxl, syl;
			int dx, dy, dxl, dyl;

			if(rcDest==NULL) 
			{ 
				RECT rect;
				GetClientRect(hwnd, &rect);
				dx=0; dy=0; dxl=rect.right-rect.left+1; dyl=rect.bottom-rect.top+1;
			}
			else
			{
				dx = rcDest->left; dy = rcDest->top;
				dxl = rcDest->right - rcDest->left + 1; dyl = rcDest->bottom - rcDest->top + 1;
			}

			if(rcSrc==NULL) 
			{ 
				sx=0; sy=0; sxl=width; syl=height;
			}
			else
			{
				sx = rcSrc->left; sy = rcSrc->top;
				sxl = rcSrc->right - rcSrc->left + 1; syl = rcSrc->bottom - rcSrc->top + 1;
			}

			sy = height-sy-syl; // 그림이 뒤집혀 있으므로 좌표를 뒤집는다

			rtn = StretchDIBits( hDC,
				dx, dy, dxl, dyl,
				sx, sy, sxl, syl,
				lpBit,
				lpBmpInfo,
				DIB_RGB_COLORS,
				SRCCOPY );

			ReleaseDC(hwnd, hDC);

			if(rtn == GDI_ERROR) return FALSE;
			return TRUE;
		}


		/////////////////////
		//  드로잉 관련
		/////////////////////

		/*-----------------------------------------------------------------------------------------
		페이지에 수평선을 그린다(flag가 TRUE이면 XOR로 그린다)
		------------------------------------------------------------------------------------------*/
		void CPage::HLine(int x, int y, int len, WORD color, BOOL flag)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				if(flag==FALSE) DrawHLine( x, y, len, (WORD*)lpBit, color );    // 단색 칼라 드로잉
				else if(flag==TRUE) DrawHLineXOR( x, y, len, (WORD*)lpBit);           // XOR 드로잉
			}
			else if(nBitNum == 8) {
				DrawHLine8(x, y, len, (char*)lpBit, (char)color);
			}
		}

		/*-----------------------------------------------------------------------------------------
		페이지에 수직선을 그린다(flag가 TRUE이면 XOR로 그린다)
		------------------------------------------------------------------------------------------*/
		void CPage::VLine(int x, int y, int len, WORD color, BOOL flag)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				if(flag==FALSE) DrawVLine( x, y, len, (WORD*)lpBit, color );    // 단색 칼라 드로잉
				else if(flag==TRUE) DrawVLineXOR( x, y, len, (WORD*)lpBit);           // XOR 드로잉
			}
			else if(nBitNum == 8) {
				DrawVLine8(x, y, len, (char*)lpBit, (char)color);
			}
		}

		/*-----------------------------------------------------------------------------------------
		페이지에 선을 그린다(브레제남 알고리즘)
		------------------------------------------------------------------------------------------*/
		void CPage::DrawLine(int x1, int y1, int x2, int y2, WORD color)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				DrawBresLine( x1, y1, x2, y2, (WORD*)lpBit, color );    // 선 그리기(브레제남 알고리즘)
			}
		}

		void CPage::DrawLineXOR(int x1, int y1, int x2, int y2)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				DrawBresLineXOR( x1, y1, x2, y2, (WORD*)lpBit);    // 선 그리기(브레제남 알고리즘)
			}
		}

		void CPage::DrawLineGB(int x1, int y1, int x2, int y2, WORD color, int grade)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				DrawBresLineGB( x1, y1, x2, y2, (WORD*)lpBit, color, grade );    // 선 그리기(브레제남 알고리즘)
			}
		}

		/*-----------------------------------------------------------------------------------------
		페이지에 속이 빈 박스를 그린다(flag가 TRUE이면 XOR로 그린다)
		------------------------------------------------------------------------------------------*/
		void CPage::DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2) _swap(x1,x2);
			if(y1>y2) _swap(y1,y2);

			if(nBitNum == 16) {
				if(flag==FALSE) DrawBox( x1, y1, x2, y2, (WORD*)lpBit, color );    // 단색 칼라 드로잉
				else if(flag==TRUE) DrawBoxXOR( x1, y1, x2, y2, (WORD*)lpBit);           // XOR 드로잉
			}
			else if(nBitNum == 8) {
				DrawBox8(x1, y1, x2, y2, (char*)lpBit, (char)color);
			}
		}

		/*-----------------------------------------------------------------------------------------
		페이지에 속이 찬 박스를 그린다
		------------------------------------------------------------------------------------------*/
		void CPage::DrawFillBox(int x1, int y1, int x2, int y2, WORD color)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2) _swap(x1,x2);
			if(y1>y2) _swap(y1,y2);

			if(nBitNum == 16) {
				FillBox( x1, y1, x2, y2, (WORD*)lpBit, color );
			}
			else if(nBitNum == 8) {
				FillBox8( x1, y1, x2, y2, (char*)lpBit, (char)color );
			}
		}

		/*-----------------------------------------------------------------------------------------
		페이지에 속이 찬 반투명 박스를 그린다
		------------------------------------------------------------------------------------------*/
		void CPage::DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2) _swap(x1,x2);
			if(y1>y2) _swap(y1,y2);

			if(nBitNum == 16) {
				FillBoxGB( x1, y1, x2, y2, (WORD*)lpBit, color, grade);
			}
			else if(nBitNum == 8) {
				FillBoxGlass8(x1, y1, x2, y2, (char*)lpBit, (char)color, grade);
			}
		}

		/*-----------------------------------------------------------------------------------------
		선택 박스를 그린다
		------------------------------------------------------------------------------------------*/
		void CPage::DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
		{
			if(lpBit==NULL)
			{
				return;
			}

			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2)
				_swap(x1,x2);
			if(y1>y2)
				_swap(y1,y2);

			if(nBitNum == 16) 
			{
				if(flag) 
				{
					DrawXORSelectBox(x1, y1, x2, y2, (WORD*)lpBit);
				}
				else
				{
					DrawSelectBox(x1, y1, x2, y2, *(WORD*)lpBit, color);
				}
			}
			else if(nBitNum == 8) 
			{
			}
		}

		/*--------------------------------------------------------------------------------------
		회전 X-Box 출력 함수
		----------------------------------------------------------------------------------------*/
		void CPage::DrawRotateXBox(int x1, int y1, int x2, int y2, float angle, WORD color)
		{
			POINT pt[4];
			pt[0].x = x1;	pt[0].y = y1;
			pt[1].x = x2;	pt[1].y = y1;
			pt[2].x = x2;	pt[2].y = y2;
			pt[3].x = x1;	pt[3].y = y2;

			for(int i=1; i<=4; i++) {
				if(angle != 0) {
					if(i < 4)
						_AxisRotatePoint(&pt[i], angle, pt[0]);
				}

				int n = i%4;

				DrawLine(pt[i-1].x, pt[i-1].y, pt[n].x, pt[n].y, color);
			}

			DrawLine(pt[0].x, pt[0].y, pt[2].x, pt[2].y, color);
			DrawLine(pt[1].x, pt[1].y, pt[3].x, pt[3].y, color);
		}


		///////////////////////////
		// 8비트 이미지 출력 함수 
		//////////////////////////


		/*--------------------------------------------------------------------------------------
		8비트 자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt, int grade, BYTE color)
		{
			if(pSpr8==NULL) return FALSE;
			if(pSpr8->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
			PutSprAuto(x, y, &pSpr8->spr[sprnum], opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		8비트 자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, SPRITE8 *pSpr, int opt, int grade, BYTE color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 8) return FALSE;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			// 뒤집기 모드이면(상대좌표 포함)
			if(opt & MIRROR)
			{
				opt = (~(opt&MIRROR))&opt;
				opt |= LR;
				if(opt & RXY)
				{
					x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
					y += pSpr->ry;
					opt = (~(opt&RXY))&opt;
				}
				else x = MirrorX + (MirrorX - (x + pSpr->xl));
			}
			else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }


			if(!(opt&GB) && !(opt&GLOW))  // 일반출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) PutCsp8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if((opt&LR) && !(opt&UD)) PutCspLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) PutCspUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else PutCspUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSpr8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if((opt&LR) && !(opt&UD)) PutSprLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) PutSprUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else PutSprUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
				}
			}
			else if(!(opt&GB) && (opt&GLOW))  // 일반 단색 출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) PutCspMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) PutCspMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) PutCspMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else PutCspMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSprMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) PutSprMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) PutSprMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else PutSprMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
				}
			}
			else if((opt&GB) && !(opt&GLOW)) // 반투명출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) PutCspGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) PutCspGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) PutCspGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else PutCspGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSprGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) PutSprGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) PutSprGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else PutSprGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
				}
			}
			else if((opt&GB) && (opt&GLOW)) // 단색 반투명출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) PutCspMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) PutCspMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) PutCspMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else PutCspMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSprMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) PutSprMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) PutSprMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else PutSprMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
				}
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		8비트 Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int tx, int ty, int opt, int grade, BYTE color)
		{
			if(pSpr8==NULL) return FALSE;
			if(pSpr8->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
			ZoomSprAuto(x, y, &pSpr8->spr[sprnum], tx, ty, opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		8비트 Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, SPRITE8 *pSpr, int tx, int ty, int opt, int grade, BYTE color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 8) return FALSE;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% 기준으로 확대,축소
			if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// 상대좌표를 적용
			if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// 캐릭터를 좌표의 중심에 위치시킨다

			// 1x Zoom
			if(tx == pSpr->xl && ty == pSpr->yl) { PutSprAuto(x, y, pSpr, opt, grade, color); return TRUE; }

			if(!(opt&GB) && !(opt&GLOW))  // 일반출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCsp8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if((opt&LR) && !(opt&UD)) LowZoomCspLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else LowZoomCspUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSpr8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if((opt&LR) && !(opt&UD)) LowZoomSprLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else LowZoomSprUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
				}
			}
			else if(!(opt&GB) && (opt&GLOW))  // 일반 단색 출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCspMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) LowZoomCspMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else LowZoomCspMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSprMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) LowZoomSprMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else LowZoomSprMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
				}
			}
			else if((opt&GB) && !(opt&GLOW)) // 반투명출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCspGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomCspGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else LowZoomCspGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSprGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomSprGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else LowZoomSprGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
				}
			}
			else if((opt&GB) && (opt&GLOW)) // 단색 반투명출력인가?
			{
				if(pSpr->compress) // 압축인가?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCspMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomCspMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else LowZoomCspMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSprMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomSprMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else LowZoomSprMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
				}
			}

			return TRUE;
		}


		/////////////////////////////
		//  16비트 스프라이트 관련
		/////////////////////////////

		/*--------------------------------------------------------------------------------------
		자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, CPage *pPage, int opt, int grade, WORD color)
		{
			SPRITE30 *pSprite = new SPRITE30;
			pSprite->xl = pPage->width;
			pSprite->yl = pPage->height;
			pSprite->compress = 0;
			pSprite->data = (WORD *)pPage->lpBit;
			pSprite->colorkey = xSprite::RGB555(0,0,255);
			pSprite->pAlpha = NULL;

			BOOL bRet = PutSprAuto(x, y, pSprite, opt, grade, color);

			delete pSprite;

			return bRet;
		}

		/*--------------------------------------------------------------------------------------
		자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			PutSprAuto(x, y, &pXSpr->spr[sprnum], opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if((pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// 알파 테이블이 없는 포맷이라면 효과 플래그 삭제
				opt = (~(opt&AB))&opt;
			}
			else if(pSpr->PixelFormat != 5558 || pSpr->pAlpha==NULL) {
				// 알파 테이블이 없는 포맷이라면 효과 플래그 삭제
				opt = (~(opt&AB))&opt;
			}
			// 뒤집기 모드이면(상대좌표 포함)
			if(opt & MIRROR)
			{
				opt = (~(opt&MIRROR))&opt;
				//opt &= ~MIRROR;
				opt |= LR;
				if(opt & RXY)
				{
					x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
					opt = (~(opt&RXY))&opt;
				}
				else x = MirrorX + (MirrorX - (x + pSpr->xl));
				y+=pSpr->ry;
			}
			else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			if(pSpr->compress)
			{
				if(opt&LR)
				{
					opt = (~(opt&LR))&opt;
					if(opt==NULL) _PutCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&HB) _PutCspLR_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&GB) _PutCspLR_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&LE) _PutCspLR_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&DE) _PutCspLR_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&AB) _PutCspLR_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&GLOW) _PutCspLR_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
					else if(opt&GRAYSCL) _PutCspLR_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&MONO) _PutCspLR_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
					else _PutCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
				}
				else if(opt&UD)
				{
					opt = (~(opt&UD))&opt;
					if(opt==NULL) _PutCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&HB) _PutCspUD_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&GB) _PutCspUD_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&LE) _PutCspUD_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&DE) _PutCspUD_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&AB) _PutCspUD_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&GLOW) _PutCspUD_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
					else if(opt&GRAYSCL) _PutCspUD_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&MONO) _PutCspUD_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
					else _PutCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
				}
				else
				{
					if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&ANTI) _PutCsp_Anti(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&HB) _PutCsp_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&GB) _PutCsp_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&LE) _PutCsp_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&DE) _PutCsp_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&AB) _PutCsp_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&GLOW) _PutCsp_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
					else if(opt&GRAYSCL)
					{	
						if( pSpr->pAlpha )
						{
							_PutCsp_Grayscl_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha,grade);
						}
						else
						{
							_PutCsp_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
						}

					}
					else if(opt&MONO)
					{
						if( pSpr->pAlpha )
						{
							_PutCsp_Mono_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,  pSpr->pAlpha, color,grade);
						}
						else
						{
							_PutCsp_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
						}

					}
					else _PutCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
				}
			}
			else
			{
				if(opt==NULL) _PutSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
				else if(opt&HB) _PutSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, 15);	// GB효과로 임시로 사용
				else if(opt&GB) _PutSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, grade);
				else if(opt&LE) _PutSpr_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, grade);
				else if(opt&DE) _PutSpr_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, grade);
				else if(opt&AB) _PutSpr_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
				else if(opt&LR) _PutSprLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
				else if(opt&UD) _PutSprUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
				else if(opt&GRAYSCL)
				{
					if( pSpr->pAlpha )
					{
						_PutSpr_Grayscl_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,pSpr->pAlpha,grade);
					}
					else
					{
						_PutSpr_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,grade);
					}
				}
				else if(opt&MONO)
				{
					if( pSpr->pAlpha )
					{
						_PutSpr_Mono_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,pSpr->pAlpha, color,grade);
					}
					else
					{
						_PutSpr_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, color,grade);
					}
				}
				else _PutSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, CPage *pPage, int tx, int ty, int opt, int grade, WORD color)
		{
			SPRITE30 *pSprite = new SPRITE30;
			pSprite->xl = pPage->width;
			pSprite->yl = pPage->height;
			pSprite->compress = 0;
			pSprite->data = (WORD *)pPage->lpBit;
			pSprite->colorkey = xSprite::RGB555(0,0,255);
			pSprite->pAlpha = NULL;

			BOOL bRet = ZoomSprAuto(x, y, pSprite, tx, ty, opt, grade, color);

			delete pSprite;

			return bRet;
		}

		/*--------------------------------------------------------------------------------------
		Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			ZoomSprAuto(x, y, &pXSpr->spr[sprnum], tx, ty, opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if((pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// 알파 테이블이 없는 포맷이라면 효과 플래그 삭제
				opt = (~(opt&AB))&opt;
			}
			else if(pSpr->PixelFormat != 5558 || pSpr->pAlpha==NULL) {
				// 알파 테이블이 없는 포맷이라면 효과 플래그 삭제
				opt = (~(opt&AB))&opt;
			}


			if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% 기준으로 확대,축소
			if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// 상대좌표를 적용
			if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// 캐릭터를 좌표의 중심에 위치시킨다

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			// 1x Zoom
			if(tx == pSpr->xl && ty == pSpr->yl) { 
				PutSprAuto(x, y, pSpr, opt, grade, color); 
				return TRUE; 
			}

			if(pSpr->compress)
			{
				if(opt&LR)
				{
					opt = (~(opt&LR))&opt;
					if(opt==NULL) _ZoomCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&HB) _ZoomCspLR_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&GB) _ZoomCspLR_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&LE) _ZoomCspLR_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&DE) _ZoomCspLR_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&AB) _ZoomCspLR_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->pAlpha, grade);
					else _ZoomCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
				}
				else if(opt&UD)
				{
					opt = (~(opt&UD))&opt;
					if(opt==NULL) _ZoomCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&HB) _ZoomCspUD_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&GB) _ZoomCspUD_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&LE) _ZoomCspUD_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&DE) _ZoomCspUD_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&AB) _ZoomCspUD_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->pAlpha, grade);
					else _ZoomCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
				}
				else
				{
					if(opt==NULL) _ZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&HB) _ZoomCsp_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&GB) _ZoomCsp_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&LE) _ZoomCsp_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&DE) _ZoomCsp_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&AB) _ZoomCsp_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&MONO) _ZoomCsp_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade,color);
					else if(opt&GRAYSCL) _ZoomCsp_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data,  pSpr->pAlpha , grade);
					else _ZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
				}
			}
			else
			{
				if(opt&LR)
				{
					opt = (~(opt&LR))&opt;
					if(opt==NULL) _ZoomSprLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
					else if(opt&HB) _ZoomSprLR_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, 15);	// GB효과 함수로 출력(임시 땜빵)
					else if(opt&GB) _ZoomSprLR_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&LE) _ZoomSprLR_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&DE) _ZoomSprLR_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&AB) _ZoomSprLR_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
					else _ZoomSprLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);

				}
				else if(opt&UD)
				{	
					opt = (~(opt&UD))&opt;
					if(opt==NULL) _ZoomSprUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
					else if(opt&HB) _ZoomSprUD_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, 15);	// GB효과 함수로 출력(임시 땜빵)
					else if(opt&GB) _ZoomSprUD_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&LE) _ZoomSprUD_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&DE) _ZoomSprUD_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&AB) _ZoomSprUD_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
					else _ZoomSprUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);

				}
				else if(opt==NULL) _ZoomSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
				else if(opt&HB) _ZoomSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, 15);	// GB효과 함수로 출력(임시 땜빵)
				else if(opt&GB) _ZoomSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
				else if(opt&LE) _ZoomSpr_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
				else if(opt&DE) _ZoomSpr_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
				else if(opt&AB) _ZoomSpr_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
				else if(opt&MONO) _ZoomSpr_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade,color);
				else if(opt&GRAYSCL) _ZoomSpr_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data,  pSpr->pAlpha , grade);
				else _ZoomSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Rotate & Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::RotateZoomSprAuto(int x, int y, CPage *pPage, int tx, int ty, float angle, int opt, int grade, WORD color)
		{
			SPRITE30 *pSprite = new SPRITE30;
			pSprite->xl = pPage->width;
			pSprite->yl = pPage->height;
			pSprite->compress = 0;
			pSprite->data = (WORD *)pPage->lpBit;
			pSprite->colorkey = xSprite::RGB555(0,0,255);
			pSprite->pAlpha = NULL;

			BOOL bRet = RotateZoomSprAuto(x, y, pSprite, tx, ty, angle, opt, grade, color);

			delete pSprite;

			return bRet;
		}

		/*--------------------------------------------------------------------------------------
		Rotate & Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::RotateZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			RotateZoomSprAuto(x, y, &pXSpr->spr[sprnum], tx, ty, angle, opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Rotate & Zoom 스프라이트 출력 함수
		----------------------------------------------------------------------------------------*/
		BOOL CPage::RotateZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, float angle, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if((pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// 알파 테이블이 없는 포맷이라면 효과 플래그 삭제
				opt = (~(opt&AB))&opt;
			}
			else if(pSpr->PixelFormat != 5558 || pSpr->pAlpha==NULL) {
				// 알파 테이블이 없는 포맷이라면 효과 플래그 삭제
				opt = (~(opt&AB))&opt;
			}

			if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% 기준으로 확대,축소
			if(opt & RXY) { x += pSpr->rx*tx/pSpr->xl; y += pSpr->ry*ty/pSpr->yl; }	// 상대좌표를 적용
			if(opt & CENTER) { 
				// 캐릭터를 좌표의 중심에 위치시킨다
				x -= (tx>>1); 
				y -= (ty>>1);

				// 회전된 만큼 좌표를 보정한다
				if(angle != 0) {
					POINT dt, odt;
					dt.x = -(tx>>1);
					dt.y = -(ty>>1);

					odt = dt;

					_RotatePoint(&dt, angle);

					x += dt.x - odt.x;
					y += dt.y - odt.y;
				}
			}

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			if(angle == 0.0f) {
				if(tx == pSpr->xl && ty == pSpr->yl) { 
					// 회전 및 줌 미적용인 경우 일반 스프라이트 출력
					PutSprAuto(x, y, pSpr, opt, grade, color); 
					return TRUE; 
				}
				else {
					// 줌만 적용된 경우 줌 스프라이트 출력
					ZoomSprAuto(x, y, pSpr, tx, ty, opt, grade, color);
					return TRUE;
				}
			}

			if(pSpr->compress)
			{
				/*
				if(opt&LR)
				{
				opt = (~(opt&LR))&opt;
				//_RotateZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, angle, pSpr->data);
				}
				else if(opt&UD)
				{
				opt = (~(opt&UD))&opt;
				//_RotateZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, angle, pSpr->data);
				}
				else
				{
				// 아직 미구현
				}

				if(pSpr->data) {
				// 압축된 스프라이트는 줌 및 회전을 지원하지 않음(쉽게 알 수 있도록 X박스로 표시함)
				DrawRotateXBox(x, y, x+tx-1, y+ty-1, angle, RGBmix(31,0,0));
				}
				*/
				_SetRenderTarget((WORD*)lpBit);
				_SetTexture(pSpr->data, pSpr->xl, pSpr->yl, pSpr->pAlpha);
				_SetTextureColorKey(pSpr->colorkey);
				_SetBlendingFunc_HLineTexture(CSPHLineTexture_GB);
				_SetTextureAlpha(grade);

				if(opt==NULL) _SetBlendingFunc_HLineTexture(CSPHLineTexture);
				else if(opt&HB) _SetBlendingFunc_HLineTexture(CSPHLineTexture_HB);
				else if(opt&GB) _SetBlendingFunc_HLineTexture(CSPHLineTexture_GB);
				else if(opt&LE) _SetBlendingFunc_HLineTexture(CSPHLineTexture_LE);
				else if(opt&DE) _SetBlendingFunc_HLineTexture(CSPHLineTexture_DE);
				else if(opt&AB && pSpr->pAlpha) _SetBlendingFunc_HLineTexture(CSPHLineTexture_AB); 
				else _SetBlendingFunc_HLineTexture(CSPHLineTexture);
				_RotateZoomSpr(x, y, tx, ty, angle);


			}
			else
			{
				_SetRenderTarget((WORD*)lpBit);
				_SetTexture(pSpr->data, pSpr->xl, pSpr->yl, pSpr->pAlpha);
				_SetTextureColorKey(pSpr->colorkey);
				_SetBlendingFunc_HLineTexture(HLineTexture_GB);
				_SetTextureAlpha(grade);

				if(opt==NULL) _SetBlendingFunc_HLineTexture(HLineTexture);
				else if(opt&HB) _SetBlendingFunc_HLineTexture(HLineTexture_HB);
				else if(opt&GB) _SetBlendingFunc_HLineTexture(HLineTexture_GB);
				else if(opt&LE) _SetBlendingFunc_HLineTexture(HLineTexture_LE);
				else if(opt&DE) _SetBlendingFunc_HLineTexture(HLineTexture_DE);
				else if(opt&AB && pSpr->pAlpha) _SetBlendingFunc_HLineTexture(HLineTexture_AB); 
				else _SetBlendingFunc_HLineTexture(HLineTexture);

				_RotateZoomSpr(x, y, tx, ty, angle);
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		자동 이미지 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutImageAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			PutImageAuto(x, y, &pXSpr->spr[sprnum], opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		자동 이미지 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			if(pSpr->compress)
			{
				//if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data);
			}
			else
			{
				_PutImage(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			}

			return TRUE;
		}


		///////// 애니메이션 관련


		//--------------------------------------------------------------------------------------
		//      장면 애니메이션 출력 함수
		//--------------------------------------------------------------------------------------
		BOOL CPage::DrawScene(int x, int y, xSprite *pXSpr, int scn, DWORD time, BOOL loop, int n_opt, int n_grade, WORD n_color)
		{
			if(!pXSpr) 
				return FALSE;

			SceneData* pScene = pXSpr->scene.GetScenePt(scn);

			if(!pScene)
				return FALSE;

			// 주어진 시간에 플레이 되어야 할 프레임을 찾는다
			int frame = pScene->SearchFrame(time, loop);

			if(frame < 0)
				return FALSE;

			// 프레임 보간용 델타값을 구함
			float delta = pScene->GetFrameDelta(frame, time, loop);

			return DrawFrameOfScene(x, y, pXSpr, scn, frame, delta, n_opt, n_grade, n_color);
		}

		//--------------------------------------------------------------------------------------
		//      장면 애니메이션의 특정 프레임 출력 함수
		//--------------------------------------------------------------------------------------
		BOOL CPage::DrawFrameOfScene(int x, int y, xSprite *pXSpr, int scn, int frame, float fdelta, int n_opt, int n_grade, WORD n_color)
		{
			SceneData* pScene = pXSpr->scene.GetScenePt(scn);

			if(!pScene) 
				return FALSE;

			if(pScene->TotFrameNum == 0)
				return FALSE;

			FrameData* pFrame = pScene->GetPtrFrame(frame);

			if(!pFrame)
				return FALSE;

			if(pFrame->TotCellNum == 0)
				return FALSE;

			for(int i=0; i<pFrame->TotCellNum; i++)
			{
				DRAWINGCELLDESC desc;

				// 셀 출력 정보를 얻음
				if(!GetDrawingCellInfo(x, y, pXSpr, scn, frame, i, fdelta, &desc))
					continue;

				// 오버라이드 옵션이 설정되어 있다면 그 값으로 대체
				if(n_opt != 0) {
					if(n_opt & LR || n_opt & UD || n_opt & TEMPCLIP || n_opt & MIRROR || n_opt & PERCENT || n_opt & RXY || n_opt & CENTER) {
						// 조합 가능한 부가 옵션인 경우
						desc.opt |= n_opt;
					}

					if(n_opt & HB || n_opt & GB || n_opt & GLOW || n_opt & GRAYSCL || n_opt & MONO || n_opt & ANTI || n_opt & LE || n_opt & DE || n_opt & AB) {
						// 조합이 불가능한 효과 옵션인 경우
						desc.opt = n_opt;
						desc.grade = n_grade; 
						desc.color = n_color;
					}
				}

				if(pXSpr->spr[desc.sprno].xl == desc.width && pXSpr->spr[desc.sprno].yl == desc.height && desc.angle == 0) {
					PutSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, desc.opt, desc.grade, desc.color);
				}
				else if(desc.angle == 0){
					if(desc.width != 0 && desc.height != 0)
						ZoomSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, (int)desc.width, (int)desc.height, desc.opt, desc.grade, desc.color);
				} 
				else {
					if(desc.width != 0 && desc.height != 0)
						RotateZoomSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, (int)desc.width, (int)desc.height, (float)desc.angle, desc.opt, desc.grade, desc.color);
				}
			}

			return TRUE;
		}

		void CPage::SetSceneEffect(SCNEFFECTDESC* pEffect)
		{
			if(!pEffect) {
				bScnEffect = FALSE;
				return;
			}

			bScnEffect = TRUE;
			ScnEffect = *pEffect;
		}

		//--------------------------------------------------------------------------------------
		//      장면의 특정 프레임 내의 셀 출력 정보를 얻음
		//--------------------------------------------------------------------------------------
		BOOL CPage::GetDrawingCellInfo(int x, int y, xSprite *pXSpr, int scn, int frame, int cell, float fdelta, DRAWINGCELLDESC* pDesc)
		{
			if(!pXSpr) return FALSE;

			SceneData* pScene = pXSpr->scene.GetScenePt(scn);
			if(!pScene) return FALSE;

			FrameData* pFrame = pScene->GetPtrFrame(frame);
			if(!pFrame) return FALSE;

			CellData* pCell = pFrame->GetPtrCell(cell);
			if(!pCell) return FALSE;

			if(pCell->SprNo < 0 || pCell->SprNo >= pXSpr->sfhead.TotalSprNum)
				return FALSE;

			float xp = (float)(x + pCell->Rx);
			float yp = (float)(y + pCell->Ry);
			int sprno = pCell->SprNo;
			float w = (float)pXSpr->spr[sprno].xl;
			float h = (float)pXSpr->spr[sprno].yl;
			int opt = 0;
			float grade = (float)pCell->Grade;
			WORD color = pCell->Color;

			switch(pCell->Effect) 
			{
			case 1 : opt |= HB; break;
			case 2 : opt |= GB; break;
			case 3 : opt |= LE; break;
			case 4 : opt |= DE; break;
			case 5 : opt |= GLOW; break;
			case 6 : opt |= MONO; break;
			case 7 : opt |= GRAYSCL; break;
			case 8 : opt |= AB; break;
			}

			if(pCell->Mirror == 1) {
				// 좌우 뒤집기
				if(opt & LR) opt &= ~LR;
				else opt |= LR;
			}
			if(pCell->Mirror == 2) {
				// 상하 뒤집기
				if(opt & UD) opt &= ~UD;
				else opt |= UD;
			}

			float zoomx = pCell->ZoomX;
			float zoomy = pCell->ZoomY;
			float angle = (float)pCell->RotAngle;

			if(pCell->Interpol == 1) {
				// 인터폴레이션
				int nf = frame + 1;
				if(nf >= pScene->TotFrameNum) nf = 0;

				// 다음 프레임의 프레임 포인터와 셀 포인터를 얻음
				FrameData* pFrame2 = pScene->GetPtrFrame(nf);
				CellData* pCell2 = pFrame2->GetPtrCell(cell);

				if(pCell2) {

					float delta = 0;
					float bias = (float)(pCell->Bias / 100.0);

					if(bias < 0.0001f && bias > -0.0001f) bias = 0;
					if(bias > 1.0f) bias = 1.0f;
					if(bias < -1.0f) bias = -1.0f;

					if(bias == 0) {
						delta = fdelta;
					}
					else if(bias < 0) {
						float ang = fdelta * TO_RADIAN(90);
						float m = (float)(1.0 - cos(ang));
						delta = fdelta + ((fdelta - m) * bias);
					}
					else {
						float ang = fdelta * TO_RADIAN(90);
						float m = (float)sin(ang);
						delta = fdelta + ((m - fdelta) * bias);
					}

					// 좌표 보간
					xp += ((pCell2->Rx - pCell->Rx) * delta);
					yp += ((pCell2->Ry - pCell->Ry) * delta);

					// 알파값 보간
					grade += FLOATROUNDINT((pCell2->Grade - pCell->Grade) * delta);

					// 컬러 보간
					if(pCell2->Effect == 5 || pCell2->Effect == 6) {
						int r1 = (int)R555(pCell->Color);
						int g1 = (int)G555(pCell->Color);
						int b1 = (int)B555(pCell->Color);
						int r2 = (int)R555(pCell2->Color);
						int g2 = (int)G555(pCell2->Color);
						int b2 = (int)B555(pCell2->Color);

						int r3 = r1 + FLOATROUNDINT((r2 - r1) * delta);
						int g3 = g1 + FLOATROUNDINT((g2 - g1) * delta);
						int b3 = b1 + FLOATROUNDINT((b2 - b1) * delta);

						color = RGBmix(r3, g3, b3);
					}

					// 줌 비율 보간
					zoomx += ((pCell2->ZoomX - pCell->ZoomX) * delta);
					zoomy += ((pCell2->ZoomY - pCell->ZoomY) * delta);

					// 회전 비율 보간
					angle += ((pCell2->RotAngle - pCell->RotAngle) * delta);
				}
			}

			// 확대 축소 적용 전의 출력 좌표
			float oxp = xp;
			float oyp = yp;

			if(zoomx != 100 || zoomy != 100) {	
				if(zoomx >= 0) w = (pXSpr->spr[sprno].xl * zoomx/100.0f);
				if(zoomy >= 0) h = (pXSpr->spr[sprno].yl * zoomy/100.0f);

				// 중심좌표를 기준으로 확대/축소되도록 보정
				float mx = (float)pXSpr->spr[sprno].mx;
				float my = (float)pXSpr->spr[sprno].my;
				xp += mx - (mx * zoomx/100.0f);
				yp += my - (my * zoomy/100.0f);
			}

			if(angle != 0) {
				// 중심 좌표를 기준으로 회전되도록 보정
				float mx = (float)pXSpr->spr[sprno].mx;
				float my = (float)pXSpr->spr[sprno].my;

				_AxisRotatePoint(&xp, &yp, angle, (oxp + mx), (oyp + my));
			}

			// 씬 그리기 이펙트가 적용되어 있다면
			if(bScnEffect) {
				if(ScnEffect.alpha >= 0 && ScnEffect.alpha < 31) {
					if(!(opt & GB || opt & LE || opt & DE || opt & AB)) {
						// 투명도가 적용되지 않는 상태인 경우 다단계 반투명을 강제 적용
						grade = 31;
						opt |= GB;
					}

					grade = (grade * (ScnEffect.alpha / 31.0f));
				}

				if(ScnEffect.zoomx >= 0 && ScnEffect.zoomx != 100) {
					w = (w * (ScnEffect.zoomx / 100.0f));
					// 씬 출력 좌표를 기준으로 확대/축소되도록 보정
					float mx = (float)(ScnEffect.mx - xp);
					xp += mx - (mx * ScnEffect.zoomx/100.0f);
				}

				if(ScnEffect.zoomy >= 0 && ScnEffect.zoomy != 100) {
					h = (h * (ScnEffect.zoomy / 100.0f));
					// 씬 출력 좌표를 기준으로 확대/축소되도록 보정
					float my = (float)(ScnEffect.my - yp);
					yp += my - (my * ScnEffect.zoomy/100.0f);
				}

				if(ScnEffect.angle != 0) {
					// 씬 출력 좌표를 기준으로 회전되도록 보정
					_AxisRotatePoint(&xp, &yp, (float)ScnEffect.angle, (float)ScnEffect.mx, (float)ScnEffect.my);
					angle += (float)ScnEffect.angle;
				}
			}


			pDesc->xp = float(xp);
			pDesc->yp = float(yp);
			pDesc->sprno = sprno;
			pDesc->width = float(w);
			pDesc->height = float(h);
			pDesc->angle = angle;
			pDesc->opt = opt;
			pDesc->grade = FLOATROUNDINT(grade);
			pDesc->color = color;

			return TRUE;
		}

		//--------------------------------------------------------------------------------------
		//      멀티씬에서 그려질 트랙의 범위 설정(모든 멀티씬이 영향 받음)
		//--------------------------------------------------------------------------------------
		void CPage::SetDrawingTrackScope(int begin/*=-1*/, int end/*=-1*/)
		{
			iDrawingTrackBegin = begin;
			iDrawingTrackEnd = end;
		}

		//--------------------------------------------------------------------------------------
		//      멀티씬 애니메이션 출력 함수
		//--------------------------------------------------------------------------------------
		BOOL CPage::DrawMultiScene(int x, int y, xSprite *pXSpr, int multiscn, DWORD time, BOOL loop, int n_opt, int n_grade, WORD n_color)
		{
			if(!pXSpr) 
				return FALSE;

			MSceneData* pMScene = pXSpr->multiscene.GetMScenePt(multiscn);

			if(!pMScene)
				return FALSE;

			if(loop && pMScene->TotPlayTime > 0) {
				if(time > 0 ) {
					time = time % pMScene->TotPlayTime;
					if(time == 0)
						time = pMScene->TotPlayTime;
				}
				else {
					time = time % pMScene->TotPlayTime;
				}
			}

			pMScene->UpdateAffectClipboxTrack();

			for(int track=0; track<pMScene->TotTrackNum; track++) 
			{
				// 임의로 설정한 트랙 유효 범위를 초과하는지 검사
				if(iDrawingTrackBegin >= 0 && track < iDrawingTrackBegin)
					continue;
				if(iDrawingTrackEnd >= 0 && track > iDrawingTrackEnd)
					continue;

				TrackData* pTrack = pMScene->GetPtrTrack(track);
				if(pTrack) {
					// 숨김 트랙이면 스킵
					if(pTrack->bHideTrack)
						continue;

					switch(pTrack->TrackType)
					{
					case TTYPE_SCENE:
						{
							DWORD stime, etime;
							if(pXSpr->CalcTrackPlayTime(multiscn, track, &stime, &etime)) {
								if(time < stime || time > etime) 
									continue;

								BOOL bSetTempClip = FALSE;
								// 임시 클리핑 영역 설정 상태 저장
								RECT orgtmpclip;
								BOOL orgbclip;

								if(pTrack->AffectClipboxTrack >= 0) {
									TrackData* pClipTrack = pMScene->GetPtrTrack(pTrack->AffectClipboxTrack);
									if(pClipTrack && !pClipTrack->bHideTrack && pClipTrack->TrackType == TTYPE_CLIPBOX) {
										TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pClipTrack->pData;
										DWORD t1 = pClipTrack->Delay;
										DWORD t2 = pClipTrack->Delay + pCbxData->LifeTime;
										if(time >= t1 && time <= t2) {
											CPI_CLIPBOX cpi;
											if(pClipTrack->GetInterpol_Clipbox(time, &cpi)) {
												bSetTempClip = TRUE;
												// 임시 클리핑 영역 설정 상태 저장
												orgtmpclip.left = TClipX1;
												orgtmpclip.top = TClipY1;
												orgtmpclip.right = TClipX2;
												orgtmpclip.bottom = TClipY2;
												orgbclip = bEnableTempClip;
												// 임시 클리핑 영역을 클립박스에 맞추어 설정
												OffsetRect(&cpi.Rect, x, y);
												if(bEnableTempClip) {
													// 이미 클리핑이 설정되어 있다면 클립박스 영역도 제한 받는다
													SetTempClip(max(cpi.Rect.left, TClipX1), 
														max(cpi.Rect.top, TClipY1), 
														min(cpi.Rect.right, TClipX2), 
														min(cpi.Rect.bottom, TClipY2));
												}
												else {
													SetTempClip(cpi.Rect.left, cpi.Rect.top, cpi.Rect.right, cpi.Rect.bottom);
												}

												EnableTempClip(TRUE);
											}
										}
									}
								}

								CPI_SCENE cpi;
								if(pTrack->GetInterpol_Scene(time, &cpi)) {	
									TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
									BOOL bscnloop = TRUE;
									if(pScnData->LifeTime != 0) {
										// 라이프타임이 0이 아닌 경우는 반복 회수만큼 플레이한 이후에 마지막 프레임만 표시되도록 한다
										SceneData* pScene = pXSpr->GetScenePt(pScnData->SceneNo);
										if(pScene) 
											if(cpi.DeltaTime >  pScene->TotPlayTime * pScnData->RepeatCnt)
												bscnloop = FALSE;
									}

									if(cpi.Alpha != 31 || cpi.ZoomX != 100 || cpi.ZoomY != 100 || cpi.Angle != 0) {
										SCNEFFECTDESC effect;
										effect.mx = x + cpi.Pos.x + pScnData->CenterX;
										effect.my = y + cpi.Pos.y + pScnData->CenterY;
										effect.alpha = cpi.Alpha;
										effect.zoomx = cpi.ZoomX;
										effect.zoomy = cpi.ZoomY;
										effect.angle = cpi.Angle;

										SetSceneEffect(&effect);
									}
									else {
										SetSceneEffect(NULL);
									}

									DrawScene(x + cpi.Pos.x, y + cpi.Pos.y, pXSpr, pScnData->SceneNo, cpi.DeltaTime, bscnloop, n_opt, n_grade, n_color);

									if(bScnEffect)
										SetSceneEffect(NULL);
								}

								if(bSetTempClip) {
									SetTempClip(orgtmpclip.left, orgtmpclip.top, orgtmpclip.right, orgtmpclip.bottom);
									EnableTempClip(orgbclip);
								}
							}
						} break;

					case TTYPE_SOUND:
						{
							//TRACK_SOUND* pSndData = (TRACK_SOUND*)pTrack->pData;
						} break;

					case TTYPE_CLIPBOX:
						{
							//TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pTrack->pData;
						} break;

					case TTYPE_SCRIPT:
						{
							//TRACK_SCRIPT* pScrData = (TRACK_SCRIPT*)pTrack->pData;
						} break;

					default:
						break;
					}
				}
			}

			return TRUE;
		}

		//--------------------------------------------------------------------------------------
		//      멀티씬의 특정 씬 트랙의 셀 출력 정보를 얻는 함수
		//--------------------------------------------------------------------------------------
		BOOL CPage::GetMSceneDrawingCellInfo(int x, int y, xSprite *pXSpr, int multiscn, int track, int cell, DWORD time, BOOL loop, DRAWINGCELLDESC* pDesc)
		{
			if(!pXSpr) 
				return FALSE;

			MSceneData* pMScene = pXSpr->multiscene.GetMScenePt(multiscn);

			if(!pMScene)
				return FALSE;

			if(loop && pMScene->TotPlayTime > 0)
			{
				if(time > 0 )
				{
					time = time % pMScene->TotPlayTime;
					if(time == 0)
					{
						time = pMScene->TotPlayTime;
					}
				}
				else
				{
					time = time % pMScene->TotPlayTime;
				}
			}


			TrackData* pTrack = pMScene->GetPtrTrack(track);
			if(!pTrack || !pTrack->pData) 
				return FALSE;

			if(pTrack->TrackType != TTYPE_SCENE)
				return FALSE;

			CPI_SCENE cpi;
			if(!pTrack->GetInterpol_Scene(time, &cpi)) 
				return FALSE;

			TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
			int scn = pScnData->SceneNo;

			SceneData* pScene = pXSpr->scene.GetScenePt(scn);

			if(!pScene)
				return FALSE;

			if(time < pTrack->Delay)
				return FALSE;

			if(pScnData->LifeTime == 0)
			{
				if(time > pTrack->Delay + (pScene->TotPlayTime * pScnData->RepeatCnt))
					return FALSE;
			}
			else 
			{
				if(time > pTrack->Delay + pScnData->LifeTime)
					return FALSE;
			}

			BOOL bscnloop = TRUE;
			if(pScnData->LifeTime != 0)
			{
				// 라이프타임이 0이 아닌 경우는 반복 회수만큼 플레이한 이후에 마지막 프레임만 표시되도록 한다
				SceneData* pScene2 = pXSpr->GetScenePt(pScnData->SceneNo);

				if(pScene2) 
				{
					if(cpi.DeltaTime >  pScene2->TotPlayTime * pScnData->RepeatCnt)
					{
						bscnloop = FALSE;
					}
				}
			}

			// 시간을 씬 기준으로 변환
			time -= pTrack->Delay;

			// 주어진 시간에 플레이 되어야 할 프레임을 찾는다
			int frame = pScene->SearchFrame(time, bscnloop);

			if(frame < 0)
				return FALSE;

			// 프레임 보간용 델타값을 구함
			float delta = pScene->GetFrameDelta(frame, time, bscnloop);

			if(cpi.Alpha != 31 || cpi.ZoomX != 100 || cpi.ZoomY != 100 || cpi.Angle != 0)
			{
				SCNEFFECTDESC effect;
				effect.mx = x + cpi.Pos.x + pScnData->CenterX;
				effect.my = y + cpi.Pos.y + pScnData->CenterY;
				effect.alpha = cpi.Alpha;
				effect.zoomx = cpi.ZoomX;
				effect.zoomy = cpi.ZoomY;
				effect.angle = cpi.Angle;

				SetSceneEffect(&effect);
			}
			else
			{
				SetSceneEffect(NULL);
			}

			// 셀 출력 정보를 얻음
			if(!GetDrawingCellInfo(x + cpi.Pos.x, y + cpi.Pos.y, pXSpr, scn, frame, cell, delta, pDesc))
				return FALSE;

			if(bScnEffect)
				SetSceneEffect(NULL);

			return TRUE;
		}

		WORD GetRGBmix(int nR, int nG, int nB)
		{
			return RGBmix(nR, nG, nB);
		}

	}//namespace GRAPHICGDI

}//namespace NMBASE

