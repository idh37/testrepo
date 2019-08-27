
#include "StdAfx.h"
#include "TgaFile.h"


BOOL CTGAFile::Load(FILE *fp)
{	
	Destroy();

	fread(&Header, sizeof(TGA_HEADER), 1, fp);

	if(Header.Bits != 32)	return FALSE;
	if(Header.Width <= 0 || Header.Height <= 0) return FALSE;

	Width	=	Header.Width;
	Height	=	Header.Height;
	pImage	=	new DWORD[Width * Height];	//	ÀÌ¹ÌÁö

	int linesize = Width*4;
	for(int line = Height-1; line >=0; line--)	fread(pImage+(line*Width), linesize, 1, fp);

	return TRUE;
}

BOOL CTGAFile::Load(char *fn)
{
	FILE *fp;
	
	fp = fopen(fn,"rb");
	if(fp == NULL) return FALSE;

	BOOL result	= Load(fp);

	fclose(fp);

	return result;
}

WORD* CTGAFile::AllocARGB4444()
{
	if(!pImage || Width<=0 || Height<=0) return NULL;
	WORD* p = new WORD[Width*Height];
	for(UINT i=0; i<Width*Height; i++) {
		TGA_PIXEL *x = (TGA_PIXEL*)&pImage[i];
		WORD a = (WORD)x->a >> 4;
		WORD r = (WORD)x->r >> 4;
		WORD g = (WORD)x->g >> 4;
		WORD b = (WORD)x->b >> 4;
		p[i] = (WORD)(a<<12 | r<<8 | g<<4 | b);
	}
	return p;
}

WORD* CTGAFile::AllocRGB555()
{
	if(!pImage || Width<=0 || Height<=0) return NULL;
	WORD* p = new WORD[Width*Height];
	for(UINT i=0; i<Width*Height; i++) {
		TGA_PIXEL *x = (TGA_PIXEL*)&pImage[i];
		//WORD a = (WORD)x->a >> 3;
		WORD r = (WORD)x->r >> 3;
		WORD g = (WORD)x->g >> 3;
		WORD b = (WORD)x->b >> 3;
		p[i] = (WORD)(r<<10 | g<<5 | b);
	}
	return p;
}

BYTE* CTGAFile::AllocAlpha5()
{
	if(!pImage || Width<=0 || Height<=0) return NULL;
	BYTE* p = new BYTE[Width*Height];
	for(UINT i=0; i<Width*Height; i++) {
		TGA_PIXEL *x = (TGA_PIXEL*)&pImage[i];
		WORD a = (WORD)x->a >> 3;
		//WORD r = (WORD)x->r >> 3;
		//WORD g = (WORD)x->g >> 3;
		//WORD b = (WORD)x->b >> 3;
		p[i] = (BYTE)a;
	}
	return p;
}

BYTE* CTGAFile::AllocAlpha()
{
	if(!pImage || Width<=0 || Height<=0) return NULL;
	DWORD* p = new DWORD[Width*Height];
	for(UINT i=0; i<Width*Height; i++) {
		TGA_PIXEL *x = (TGA_PIXEL*)&pImage[i];
		DWORD a = (DWORD)x->a<<24;
		DWORD r = (DWORD)x->r<<16;
		DWORD g = (DWORD)x->g<<8;
		DWORD b = (DWORD)x->b;
		p[i] = (DWORD)(a | r | g | b);
	}
	return (BYTE*)p;
}