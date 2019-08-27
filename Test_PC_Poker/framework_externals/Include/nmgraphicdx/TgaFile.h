//#ifndef __TGAFILE_H__
//#define __TGAFILE_H__
//
//
//#include <windows.h>
//#include <stdio.h>

#pragma pack(1)

typedef struct
{	
	char	idLength;
	char	ColorMapType;
	char	ImageType;
	WORD	ColorMapFirst;
	WORD	ColorMapLast;
	char	ColorMapBits;
	WORD	FirstX;
	WORD	FirstY;
	WORD	Width;
	WORD	Height;
	char	Bits;
	char	Descriptor;
} TGA_HEADER;

#pragma pack()

typedef struct
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
} TGA_PIXEL;

//---------------------------------------------------------------------------
//	TGA 클래스
//	여기서 관리하는 이미지의 최초 4바이트는 이미지의 가로,세로 사이즈 입니다.
//---------------------------------------------------------------------------

class CTGAFile
{
public:
	TGA_HEADER Header;
	DWORD	Width,Height;
	DWORD	*pImage;

	CTGAFile()	
	{
		memset(&Header, 0, sizeof(TGA_HEADER));
		pImage	=	NULL;
	};

	~CTGAFile()	
	{	
		Destroy();
	};

	void Destroy()
	{
		memset(&Header, 0, sizeof(TGA_HEADER));
		Width = Height = 0;
		if (pImage) {	
			delete [] pImage;
			pImage	=	NULL;
		}
	};

	BOOL Load(char *fn);
	BOOL Load(FILE *fp);

	WORD* AllocARGB4444();
	WORD* AllocRGB555();
	BYTE* AllocAlpha5();
	BYTE* AllocAlpha();
};



#endif