
#include "stdafx.h"
#include "xSprite.h"

///////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------

          <  0 번 압축 포맺 구조(헤더를 제외한 데이타 부분) >

 Byte(s)

 4 * height  - 각라인의 시작 오프셋의 거리가 담긴 배열
   2         - 패턴(pattern ) 수 ---------------------------+
   2         - 투명색 수 ---------------+                   | 세로길이 만큼
   2         - 그릴 점수                | 패턴수만큼 반복   | 반복
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
unsigned long
EncodeSpr0(WORD *image, int xl, int yl, WORD **output, WORD TRANS)
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
        for(i=0,pNum=0; i<xl;)
        {
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
        for(i=0;i<pNum;i++)
        {
            memcpy(&buf[bp], &pat[i],sizeof(BLKPATTERN)); 
			bp+=2;
            if(pat[i].putb!=0)
            {
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
    if(spr != NULL) 
	{
		for(i=0; i < sfhead.TotalSprNum; i++)
		{
	        if( spr[i].hit != NULL ) { free(spr[i].hit); spr[i].hit = NULL; }
			if( spr[i].atck != NULL ) { free(spr[i].atck); spr[i].atck = NULL; }
			if( spr[i].data != NULL ) { free(spr[i].data); spr[i].data = NULL; }
	    }
		free(spr); spr=NULL;
		ZeroMemory(&sfhead, sizeof(SPRFILEHEAD));
		ani.Destroy();
	}
}

/*--------------------------------------------------------------------------
  스프라이트 파일을 로드(스크린의 픽셀 포맷에 따라 읽는다)
  name: 파일명
  (주의: xCanvas가 먼저 초기화 되어 있어야 한다)
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
static inline 
BOOL _SPRFileSafeRead(FILE* fp, void* pBuffer, size_t size)
{
	return ( fp != NULL && pBuffer != NULL && fread(pBuffer, size, 1, fp) == 1 );
}

BOOL xSprite::Load(char *name, int ScreenRGBMode)
{
    BOOL bSucc;
	
	if( name == NULL )
	{
		//PutErrStr("파일이 없습니다(%s)", name); 
		bSucc = FALSE;
	}
	else
	{
		FILE *fp;
		
		if( (fp = fopen( name, "rb")) == NULL )
		{ 
			//PutErrStr("파일이 없습니다(%s)", name); 
			bSucc = FALSE;
		}
		else
		{
			SPRFILEHEAD tempsfhead;     // 임시 파일 헤더
			
			//기본 헤더 로드
			if( _SPRFileSafeRead(fp, &tempsfhead, sizeof(SPRFILEHEAD)) == FALSE )
			{
				//PutErrStr("스프라이트 헤더 읽기 실패(%s)", name);
				bSucc = FALSE;
			}
			//스프라이트 파일이 아니면
			else if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) 
			{
				//PutErrStr("스프라이트 파일이 아닙니다(%s)", name);
				bSucc = FALSE;
			}
			else
			{
				SPRHEAD30 shead;            // 각 스프라이트의 헤더
				
				Remove();
				
				memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD));
				
				spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
				if(spr==NULL) 
				{ 
					//PutErrStr("메모리 할당 실패(헤더)"); 
					bSucc = FALSE;
				}
				else
				{
					int i;

					bSucc = TRUE;
					ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

					for(i=0; bSucc && i < sfhead.TotalSprNum; i++)
					{
						// 각 스프라이트 마다의 헤더 로드
						if( _SPRFileSafeRead(fp, &shead, sizeof(SPRHEAD30)) == FALSE )
						{
							// 요청한 길이 만큼 로드하지 못했음!
							bSucc = FALSE;
						}
						else
						{
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
							spr[i].byUserVal = shead.byUserVal;

							///// 충돌 좌표, 공격 좌표 읽기 /////
							if( spr[i].hitNum>0 
								&& ((spr[i].hit = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].hitNum)) == NULL 
								|| _SPRFileSafeRead(fp, spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum) == FALSE ) ) 
							{
								// 충돌 좌표 읽기 실패!
								bSucc = FALSE;
							}
							else if( spr[i].atckNum>0 
								&& ((spr[i].atck = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].atckNum)) == NULL 
								|| _SPRFileSafeRead(fp, spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum) == FALSE ) ) 
							{
								// 공격 좌표 읽기 실패!
								bSucc = FALSE;
							}
							// 스프라이트 이미지 데이타 읽기
							else if((spr[i].data = (WORD *)malloc( shead.dsize )) == NULL ) 
							{
								//PutErrStr("메모리 할당 실패(이미지)"); 
								bSucc = FALSE;
							}
							// 스프라이트 파일 데이터 로드.
							else if(_SPRFileSafeRead(fp, spr[i].data, spr[i].dsize) == FALSE )
							{
								bSucc = FALSE;
							}
							else
							{
								// 압축 파일이면 
								if( spr[i].compress )
								{
									// 데이터 유효성 체크

									if( spr[i].dsize < spr[i].yl*sizeof(long) )
									{
										bSucc = FALSE;
									}
									else
									{
										int line = 0;
										WORD cnt;
										unsigned long iLineStartIdx = 0;
										unsigned long idx;
										unsigned long pbyte;

										for(line = 0;bSucc && line < spr[i].yl;line++)
										{
											iLineStartIdx = *((long *)(spr[i].data+line*(sizeof(long)/sizeof(WORD))));

											// 첫번째 라인일경우 인덱스를 저장한다.
											if( line == 0 )
											{
												idx = iLineStartIdx;
											}
											// idx 랑 시작 위치랑 다르면 오류
											if( idx != iLineStartIdx )
											{
												bSucc = FALSE;
											}
											// 라인 시작 지점이 데이터 크기보다 크면 잘못됨.
											else if( idx*sizeof(WORD) > spr[i].dsize  )
											{
												bSucc = FALSE;
											}
											else
											{
												int iLineDotCount = 0;

												cnt = spr[i].data[idx++];

												if( idx*sizeof(WORD) >= spr[i].dsize  )
												{
													bSucc = FALSE;
												}

												while(bSucc && cnt--)
												{
													iLineDotCount += spr[i].data[idx++];
													
													bSucc = (idx*sizeof(WORD) <= spr[i].dsize);
													if( bSucc  )
													{
														pbyte = spr[i].data[idx++];
														
														bSucc = (idx*sizeof(WORD) <= spr[i].dsize);
														if( bSucc  )
														{
															idx += pbyte;
															iLineDotCount += pbyte;
															
															bSucc = (idx*sizeof(WORD) <= spr[i].dsize);
														}
													}
												}

												if( bSucc )
												{
													// 한줄의 도트수를 마지막으로 계산한다.
													bSucc = ( iLineDotCount <= spr[i].xl );
												}
											}
										}
									}
								}

							}
						} // if( _SPRFileSafeRead(fp, &shead, sizeof(SPRHEAD30)) == FALSE )
					} // for(i=0; bSucc && i < sfhead.TotalSprNum; i++)

					if( bSucc == FALSE )
					{
						Remove();
					}
				} // if(spr==NULL) 
				
			} // if( _SPRFileSafeRead(&tempsfhead, sizeof(SPRFILEHEAD), 1, fp) == FALSE )
			
			fclose(fp);
		} // if( (fp = fopen( name, "rb")) == NULL )
	}
	
	return bSucc;
}
// Old Source
// BOOL xSprite::Load(char *name, int ScreenRGBMode)
// {
//     FILE *fp;
// 
//     SPRFILEHEAD tempsfhead;     // 임시 파일 헤더
// 	SPRHEAD30 shead;            // 각 스프라이트의 헤더
// 	
// 
// 	if( (fp = fopen( name, "rb")) == NULL )
// 	{ 
// 		//PutErrStr("파일이 없습니다(%s)", name); 
// 		return FALSE;
// 	}
//     
// 	fread(&tempsfhead, sizeof(SPRFILEHEAD), 1, fp); //기본 헤더 로드
// 
//     if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) //스프라이트 파일이 아니면
//     {
// 		//PutErrStr("스프라이트 파일이 아닙니다(%s)", name);
// 		fclose(fp);
//         return FALSE;
//     }
// 
// 	Remove();
// 
// 	memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD));
// 
// 	spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
// 	if(spr==NULL) 
// 	{ 
// 		//PutErrStr("메모리 할당 실패(헤더)"); 
// 		return FALSE; 
// 	}
// 	ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);
// 
//     for(int i=0; i < sfhead.TotalSprNum; i++)
//     {
// 		fread(&shead, sizeof(SPRHEAD30), 1, fp); // 각 스프라이트 마다의 헤더 로드
// 		
// 		spr[i].compress = shead.compress;
// 		spr[i].colorkey = shead.colorkey;
//         spr[i].rx       = shead.rx;
//         spr[i].ry       = shead.ry;
//         spr[i].xl       = shead.xl;
//         spr[i].yl       = shead.yl;
//         spr[i].mx       = shead.mx;
//         spr[i].my       = shead.my;
//         spr[i].hitNum   = shead.hitNum;
//         spr[i].atckNum  = shead.atckNum;
//         spr[i].hit      = NULL;
//         spr[i].atck     = NULL;
//         spr[i].dsize    = shead.dsize;
// 		spr[i].byUserVal = shead.byUserVal;
// 
// 		///// 충돌 좌표, 공격 좌표 읽기 /////
// 		if(spr[i].hitNum>0) spr[i].hit = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].hitNum);
// 		if(spr[i].atckNum>0) spr[i].atck = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].atckNum);
// 		fread(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
// 		fread(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);
// 
// 		///// 스프라이트 이미지 데이타 읽기
//         spr[i].data = (WORD *)malloc( shead.dsize );
// 		if(spr[i].data==NULL) 
// 		{ 
// 			//PutErrStr("메모리 할당 실패(이미지)"); 
// 			return FALSE; 
// 		}
//         ZeroMemory(spr[i].data, shead.dsize);
// 		fread(spr[i].data, shead.dsize, 1, fp);
//     }
// 
// 	//// 에니메이션 데이터 읽기
// 	if(sfhead.CharacterNum > 0) ani.Load(fp);
// 
// 	fclose(fp);
// 
// 	ChangePixelFormat(ScreenRGBMode);
// 
//     return TRUE;
// }

/*--------------------------------------------------------------------------
  스프라이트를 파일로 저장(화면 모드에 관계없이 RGB 5:5:5 포맷으로 저장된다)
---------------------------------------------------------------------------*/
BOOL xSprite::Save(char *name)
{
    FILE *fp = NULL;
	SPRHEAD30 shead;            // 각 스프라이트의 헤더

	if(spr==NULL || sfhead.TotalSprNum==0) 
	{
		//PutErrStr("저장할 스프라이트가 없습니다."); 
		return FALSE;
	}

	if((fp=fopen( name,"wb"))==NULL)
	{
		//PutErrStr("파일생성 실패 (%s)",name); 
		return FALSE;
	}

	//ChangePixelFormat(555); // 5:5:5로 바꾼다
    
    strcpy(sfhead.Identifier, "93XYZ sprite");
	sfhead.Version = 30;
	sfhead.BPD = 2;
	sfhead.PixelFormat = 555;
	sfhead.CharacterNum = ani.TotCharacterNum;

	fwrite( &sfhead, sizeof(SPRFILEHEAD),1,fp );         // 파일 헤더 쓰기

    for(int i=0; i < sfhead.TotalSprNum; i++)
    {
		shead.compress = spr[i].compress;
		shead.colorkey = spr[i].colorkey;
	    shead.xl       = spr[i].xl;
		shead.yl       = spr[i].yl;
		shead.rx       = spr[i].rx;
	    shead.ry       = spr[i].ry;
		shead.mx       = spr[i].mx;
	    shead.my       = spr[i].my;
		shead.hitNum   = spr[i].hitNum;
	    shead.atckNum  = spr[i].atckNum;
		//??           = spr[i].atck;
	    //??           = spr[i].hit;
		shead.dsize    = spr[i].dsize;
		shead.PixelFormat = spr[i].PixelFormat;
		shead.SprFormat = spr[i].SprFormat;
		shead.Dithering = spr[i].Dithering;
		shead.byUserVal = spr[i].byUserVal;

		if(spr[i].SprFormat == 1)
		{
			// JPG 스프라이트 포맷으로 기록
			if(!spr[i].pJpgData || spr[i].jpgsize <= 0)
			{
				//PutErrStr("JPEG 이미지 데이터가 존재하지 않습니다"); 
				fclose(fp);
				return FALSE;
			}

			// 스프라이트 사이즈가 아닌 JPEG 이미지의 사이즈를 기록한다
			shead.dsize = spr[i].jpgsize;

			// 각 스프라이트의 헤더 쓰기
			fwrite(&shead, sizeof(SPRHEAD30), 1, fp);
			// 충돌 좌표, 공격 좌표 쓰기
			if(spr[i].hitNum>0)
				fwrite(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
			if(spr[i].atckNum>0) 
				fwrite(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);
			// JPEG 이미지 쓰기
			fwrite(spr[i].pJpgData, spr[i].jpgsize, 1, fp);
		}
		else 
		{
			// 일반 스프라이트로 기록
			spr[i].SprFormat = 0;
			// 각 스프라이트의 헤더 쓰기
			fwrite(&shead, sizeof(SPRHEAD30), 1, fp);
			// 충돌 좌표, 공격 좌표 쓰기
			if(spr[i].hitNum>0)
				fwrite(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
			if(spr[i].atckNum>0) 
				fwrite(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);
			// 이미지 데이터 쓰기
			fwrite(spr[i].data, spr[i].dsize, 1, fp);
		}
    }

	//// 에니메이션 데이터 쓰기
	if(sfhead.CharacterNum > 0)
		ani.Save(fp);

	fclose(fp);

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
		if(spr[i].compress)
		{
			int  m,xl,yl;
			int  sbyte,pbyte;
			short cnt;

			xl = spr[i].xl;
			yl = spr[i].yl;
			WORD* tar = spr[i].data + yl*2; // 라인거리 데이타를 건너띔

			for(m=0; m<yl; m++)
			{
		        cnt = *tar++;      // 패턴 개수 얻기
				sbyte=0;
		        while(cnt--) {

				    sbyte+=(*(WORD*)tar); // 투명색수 얻기
		            tar++;

		            pbyte =(*(WORD*)tar); // 점수 얻기
					tar++;

					for(int k=0; k<pbyte; k++)
					{
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
			for(y=0; y<spr[i].yl; y++)
			{
				for(x=0; x<spr[i].xl; x++)
				{
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
  스프라이트 사이즈를 최적화(필요없는 공백부분을 없앤다)
---------------------------------------------------------------------------*/
/*
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
	for(i=0; i<yl; i++)
	{
		fx = FALSE;
		ex = FALSE;
		bline = TRUE;		// 공백라인 유무
		for(j=0; j<xl; j++)
		{
			if(*(spr[snum].data + (i*xl+j)) != colorkey) 
			{
				bline = FALSE;
				ex = TRUE;
				ey = TRUE; y2=i;
				if(fx==FALSE) 
				{
					if(j<x1) x1=j; 
					fx=TRUE;
				}
				if(fy==TRUE) {y1=i; fy=FALSE;}
			}
			else
			{
				if(ex==TRUE) 
				{
					if(j>x2) x2=j; 
					ex=FALSE;
				}
			}
		}
		//if(bline && fy) {y1=i;}
		if(bline && ey) {y2=i; ey=FALSE;}
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
	WORD *pNew = (WORD*)malloc((newXl*newYl)*2);
	WORD *pSor = spr[snum].data + (xl*y1);
	for(i=0; i<newYl; i++)
	{
		memcpy(pNew+(newXl*i), pSor+x1, newXl*2);
		pSor+=xl;
	}
	free(spr[snum].data);
	spr[snum].data = pNew;
	spr[snum].xl = newXl;
	spr[snum].yl = newYl;

	// 각종 좌표들을 함께 교정
	if(fixPos)
	{
		spr[snum].rx += x1; spr[snum].ry += y1;

		spr[snum].mx -= x1; spr[snum].my -= y1;

		for(i=0; i<spr[snum].hitNum; i++)
		{
			spr[snum].hit[i].x1 -= x1; spr[snum].hit[i].y1 -= y1;
			spr[snum].hit[i].x2 -= x1; spr[snum].hit[i].y2 -= y1;
		}

		for(i=0; i<spr[snum].atckNum; i++)
		{
			spr[snum].atck[i].x1 -= x1; spr[snum].atck[i].y1 -= y1;
			spr[snum].atck[i].x2 -= x1; spr[snum].atck[i].y2 -= y1;
		}
	}

	//MsgBox(NULL,"x1=%d, y1=%d, x2=%d, y2=%d", x1, y1, x2, y2);
	if(compressed) Compress(snum);
}
*/

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
	for(i=0; i<yl; i++)
	{
		fx = FALSE;
		ex = FALSE;
		bline = TRUE;		// 공백라인 유무
		for(j=0; j<xl; j++)
		{
			if(*(spr[snum].data + (i*xl+j)) != colorkey) 
			{
				bline = FALSE;
				ex = TRUE;
				ey = TRUE; y2=i;
				if(fx==FALSE) 
				{
					if(j<x1) x1=j; 
					fx=TRUE;
				}
				if(fy==TRUE) {y1=i; fy=FALSE;}

				if(j>x2) x2=j; 
			}
		}

		if(bline && ey) {y2=i; ey=FALSE;}
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
	WORD *pNew = (WORD*)malloc((newXl*newYl)*2);
	WORD *pSor = spr[snum].data + (xl*y1);
	for(i=0; i<newYl; i++)
	{
		memcpy(pNew+(newXl*i), pSor+x1, newXl*2);
		pSor+=xl;
	}
	free(spr[snum].data);
	spr[snum].data = pNew;
	spr[snum].xl = newXl;
	spr[snum].yl = newYl;

	// 각종 좌표들을 함께 교정
	if(fixPos)
	{
		spr[snum].rx += x1; spr[snum].ry += y1;

		spr[snum].mx -= x1; spr[snum].my -= y1;

		for(i=0; i<spr[snum].hitNum; i++)
		{
			spr[snum].hit[i].x1 -= x1; spr[snum].hit[i].y1 -= y1;
			spr[snum].hit[i].x2 -= x1; spr[snum].hit[i].y2 -= y1;
		}

		for(i=0; i<spr[snum].atckNum; i++)
		{
			spr[snum].atck[i].x1 -= x1; spr[snum].atck[i].y1 -= y1;
			spr[snum].atck[i].x2 -= x1; spr[snum].atck[i].y2 -= y1;
		}
	}

	//MsgBox(NULL,"x1=%d, y1=%d, x2=%d, y2=%d", x1, y1, x2, y2);
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

	newsize = spr[snum].xl*2 * spr[snum].yl;
	buf = (WORD *)malloc(newsize);
	if(buf==NULL) return FALSE;

	//ZeroMemory(buf, newsize);
	DecodeSpr0(buf, spr[snum].xl, spr[snum].yl, spr[snum].data, spr[snum].colorkey);

	free(spr[snum].data);          // 기존의 이미지를 해제
	spr[snum].compress = 0;
	spr[snum].data     = buf;      // 압축이 풀린 이미지 데이터 포인터
	spr[snum].dsize    = newsize;  // 압축이 풀린 사이즈

	return TRUE;
}

/*--------------------------------------------------------------------------
  특정 이미지를 바탕으로 스프라이트를 생성
  (이미지를 복사하지는 않는다)
---------------------------------------------------------------------------*/
BOOL xSprite::TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey)
{
	if(pImage == NULL) return FALSE;
	Remove();
	
	spr = (SPRITE30*) malloc(sizeof(SPRITE30));
	if(spr==NULL) 
	{ 
		//PutErrStr("메모리 할당 실패(At TransToSpr)"); 
		return FALSE; 
	}

	sfhead.TotalSprNum = 1;
	strcpy(sfhead.Identifier, "93XYZ sprite");
	sfhead.Version = 30;
	sfhead.BPD = 2;
	sfhead.CharacterNum = 0;

	memset(&spr[0], 0, sizeof(SPRITE30));

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
		//PutErrStr("메모리 할당 실패(At NewSpace)"); 
		return FALSE; 
	}

	sfhead.TotalSprNum = 1;
	strcpy(sfhead.Identifier, "93XYZ sprite");
	sfhead.Version = 30;
	sfhead.BPD = 2;
	sfhead.CharacterNum = 0;

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
	spr[0].pJpgData = NULL;
	spr[0].jpgsize = 0;

	if(bCompress) 
	{
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
		//PutErrStr("이미지 포인터가 NULL입니다(At Insert)"); 
		return FALSE; 
	}

	if(sfhead.TotalSprNum <= 0) // 최초로 생성된 스프라이트라면
	{
		if(NewSpace(xl, yl, pImage, wColorKey, bCompress)==FALSE) return FALSE;
		return TRUE;
	}

	if(snum > sfhead.TotalSprNum || snum < 0) 
	{ 
		//PutErrStr("스프라이트 컷 삽입 범위 초과(At Insert)"); 
		return FALSE; 
	}

	SPRITE30 *oldspr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
	if(oldspr==NULL) { 
		//PutErrStr("메모리 할당 실패(At Insert)"); 
		return FALSE; 
	}
	memcpy(oldspr, spr, sizeof(SPRITE30) * sfhead.TotalSprNum);
	
	free(spr);
	sfhead.TotalSprNum += 1;
	spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
	if(spr==NULL) { 
		//PutErrStr("메모리 할당 실패(At Insert)"); 
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
	spr[snum].pJpgData = NULL;
	spr[snum].jpgsize = 0;
	spr[snum].SprFormat = 0;

	if(bCompress) 
	{
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
	memcpy(pNewImage, pImage, xl*yl);

	if(!Insert(snum, xl, yl, pImage, wColorKey, bCompress)) {
		delete pNewImage;
		return FALSE;
	}

	return TRUE;
}
