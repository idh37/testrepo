
#include "stdafx.h"
#include "xSprite.h"

///////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------

          <  0 �� ���� ���� ����(����� ������ ����Ÿ �κ�) >

 Byte(s)

 4 * height  - �������� ���� �������� �Ÿ��� ��� �迭
   2         - ����(pattern ) �� ---------------------------+
   2         - ����� �� ---------------+                   | ���α��� ��ŭ
   2         - �׸� ����                | ���ϼ���ŭ �ݺ�   | �ݺ�
  size       - ����Ÿ(����*2)-----------+ ------------------+

  ## ����Ÿ�� ũ��� ����*2�̴�(16��Ʈ �̹Ƿ�) ��, ���� ����Ʈ���� �ǹ��Ѵ�
  ## �������� �Ÿ� : ����Ÿ�� ó���������� �� ������ ���۵Ǵ� ��������
                     �Ÿ��� ��Ÿ����.(�������� ���̰� �ƴ�)
                     (Ŭ���ν� �ӵ��� ���̰� ó���� �����ϰ��ϱ�����)
					 (����: �Ÿ�1�� ���� 2����Ʈ������)
  ## ��������� �׸�����Ÿ���� WORD���̴�.
 --------------------------------------------------------------------*/



//#define  TRANS  0

typedef struct {
    WORD skipb;
    WORD putb;
} BLKPATTERN;

/*---------------------------------------------------------
  ���ۿ� �̹����� �����Ͽ� �����ϴ� �Լ�

  WORD *image   : �Էµ� �̹��� ������(16��Ʈ)
  int  xl       : �Է� �̹����� ���α���
  int  yl       : �Է� �̹����� ���α���
  WORD **output : ��µ� ���� �̹����� ���� ������
                  (�ݵ�� NULL �̾���Ѵ�, �����������ӿ� ����)
  WORD TRANS    : Color Key

  ���ϰ�  -->  ����� ������(���н� 0)

  ���� ) ���� �̹����� �����̹����� ������
         ���� �������Ƿ� �̸� ���� free ���־���Ѵ�.
-----------------------------------------------------------*/
unsigned long
EncodeSpr0(WORD *image, int xl, int yl, WORD **output, WORD TRANS)
{
	if(!image || xl <= 0 || yl <= 0) return 0;

    WORD i,k,cnt;
    short pNum;        // ������ ����
    BLKPATTERN pat[1024] = {0,};

    unsigned long bp=0;         // ���� �ε���
    unsigned long dsize = 0;        // ����� ��ü ũ��

	// �ӽ� ���� �Ҵ�(����ũ���� 4�� + LDT���̺����) 
	// ���α��̴� �ּ� 8�ȼ� �̻����� ����(8���� ���� ��� 4�踦 �ϴ��� ���۰� ���ڶ� �� ����)
	WORD *buf = (WORD *)malloc((max(8,xl)*4)*yl+(4*yl));
	if(buf==NULL) return 0;

    bp += yl*2;   // �� ������ �Ÿ��� ��� ������ �ǳʶ�(����: buf�� WORD�� �������̹Ƿ�... yl*2)
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
        memcpy(buf+k*2, &bp, 4); // ����: buf�� WORD�� �������̹Ƿ�... k*2

        // ���δ� ������ ����
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

    dsize=bp*2; // LDT���̺� + �̹���ũ��
    *output = (WORD *)malloc(dsize);
    memcpy(*output, buf, dsize); // �ӽù����� ������ ����
    free(buf);
    return(dsize);
}

/*---------------------------------------------------------
  ���ེ������Ʈ�� ���ۿ� ���ڵ��ϴ� �Լ�
  (������ ũ��� '���α���*���α���' �̾���Ѵ�)
-----------------------------------------------------------*/
void DecodeSpr0( WORD *tar, int xl, int yl, WORD *sor, WORD TRANS)
{
    int  i,j;
    int  sbyte,pbyte;
    short cnt;

	for(i=0; i<yl; i++) // �켱 ��� ������� �����Ѵ�
	{
		for(j=0; j<xl; j++) *(tar+(xl*i)+j) = TRANS; 
	}

    sor += yl*2; // ���ΰŸ� ����Ÿ�� �ǳʶ�
                 // Ŭ������ �����Ƿ� �ʿ� ����

	//sor += *((long*)sor);    // ��� ������ �������� �̵�

    for(i=0;i<yl;i++,tar+=xl) 
	{
        cnt = *sor;      // ���� ���� ���
        sor++;
        sbyte=0;
        while(cnt--) {

            sbyte+=(*(WORD*)sor); // ������� ���
            sor++;

            pbyte =(*(WORD*)sor); // ���� ���
            sor++;

            memcpy(tar+sbyte, sor, pbyte<<1); // �̹��� ����ֱ� (16��Ʈ�̹Ƿ� *2)

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

void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b) // 16��Ʈ(555)�� RGB�� �м��Ѵ�
{
	*r = (color & 0x7C00)>>10;
	*g = (color & 0x03E0)>>5;
	*b = (color & 0x001F);
}

void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b) // 16��Ʈ(565)�� RGB�� �м��Ѵ�
{
	*r = (color & 0xF800)>>11;
	*g = (color & 0x03E0)>>5; 
	*b = (color & 0x07E0);
	// ����) 565���� Green�� �ι��̹Ƿ� ����Ʈ�� �ѹ� ���ؾ� 
	//       RGB ���սÿ� ��Ȯ�� ���� ���´�
	//       �׷��� ���⼱ �ѹ� �� ���� �ʾҴ�.
}


//================================ ��������Ʈ ���� 3.0 Ŭ���� =================================//


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
  �ε��� ��������Ʈ�� ����
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
  ��������Ʈ ������ �ε�(��ũ���� �ȼ� ���˿� ���� �д´�)
  name: ���ϸ�
  (����: xCanvas�� ���� �ʱ�ȭ �Ǿ� �־�� �Ѵ�)
---------------------------------------------------------------------------*/
BOOL xSprite::Load(char *name)
{
	return(Load(name, 555));
}

/*--------------------------------------------------------------------------
  ��������Ʈ ������ �ε�
  name: ���ϸ�
  ScreenRGBMode: ������ �ȼ� �������� �о���δ�
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
		//PutErrStr("������ �����ϴ�(%s)", name); 
		bSucc = FALSE;
	}
	else
	{
		FILE *fp;
		
		if( (fp = fopen( name, "rb")) == NULL )
		{ 
			//PutErrStr("������ �����ϴ�(%s)", name); 
			bSucc = FALSE;
		}
		else
		{
			SPRFILEHEAD tempsfhead;     // �ӽ� ���� ���
			
			//�⺻ ��� �ε�
			if( _SPRFileSafeRead(fp, &tempsfhead, sizeof(SPRFILEHEAD)) == FALSE )
			{
				//PutErrStr("��������Ʈ ��� �б� ����(%s)", name);
				bSucc = FALSE;
			}
			//��������Ʈ ������ �ƴϸ�
			else if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) 
			{
				//PutErrStr("��������Ʈ ������ �ƴմϴ�(%s)", name);
				bSucc = FALSE;
			}
			else
			{
				SPRHEAD30 shead;            // �� ��������Ʈ�� ���
				
				Remove();
				
				memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD));
				
				spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
				if(spr==NULL) 
				{ 
					//PutErrStr("�޸� �Ҵ� ����(���)"); 
					bSucc = FALSE;
				}
				else
				{
					int i;

					bSucc = TRUE;
					ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

					for(i=0; bSucc && i < sfhead.TotalSprNum; i++)
					{
						// �� ��������Ʈ ������ ��� �ε�
						if( _SPRFileSafeRead(fp, &shead, sizeof(SPRHEAD30)) == FALSE )
						{
							// ��û�� ���� ��ŭ �ε����� ������!
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

							///// �浹 ��ǥ, ���� ��ǥ �б� /////
							if( spr[i].hitNum>0 
								&& ((spr[i].hit = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].hitNum)) == NULL 
								|| _SPRFileSafeRead(fp, spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum) == FALSE ) ) 
							{
								// �浹 ��ǥ �б� ����!
								bSucc = FALSE;
							}
							else if( spr[i].atckNum>0 
								&& ((spr[i].atck = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].atckNum)) == NULL 
								|| _SPRFileSafeRead(fp, spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum) == FALSE ) ) 
							{
								// ���� ��ǥ �б� ����!
								bSucc = FALSE;
							}
							// ��������Ʈ �̹��� ����Ÿ �б�
							else if((spr[i].data = (WORD *)malloc( shead.dsize )) == NULL ) 
							{
								//PutErrStr("�޸� �Ҵ� ����(�̹���)"); 
								bSucc = FALSE;
							}
							// ��������Ʈ ���� ������ �ε�.
							else if(_SPRFileSafeRead(fp, spr[i].data, spr[i].dsize) == FALSE )
							{
								bSucc = FALSE;
							}
							else
							{
								// ���� �����̸� 
								if( spr[i].compress )
								{
									// ������ ��ȿ�� üũ

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

											// ù��° �����ϰ�� �ε����� �����Ѵ�.
											if( line == 0 )
											{
												idx = iLineStartIdx;
											}
											// idx �� ���� ��ġ�� �ٸ��� ����
											if( idx != iLineStartIdx )
											{
												bSucc = FALSE;
											}
											// ���� ���� ������ ������ ũ�⺸�� ũ�� �߸���.
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
													// ������ ��Ʈ���� ���������� ����Ѵ�.
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
//     SPRFILEHEAD tempsfhead;     // �ӽ� ���� ���
// 	SPRHEAD30 shead;            // �� ��������Ʈ�� ���
// 	
// 
// 	if( (fp = fopen( name, "rb")) == NULL )
// 	{ 
// 		//PutErrStr("������ �����ϴ�(%s)", name); 
// 		return FALSE;
// 	}
//     
// 	fread(&tempsfhead, sizeof(SPRFILEHEAD), 1, fp); //�⺻ ��� �ε�
// 
//     if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) //��������Ʈ ������ �ƴϸ�
//     {
// 		//PutErrStr("��������Ʈ ������ �ƴմϴ�(%s)", name);
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
// 		//PutErrStr("�޸� �Ҵ� ����(���)"); 
// 		return FALSE; 
// 	}
// 	ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);
// 
//     for(int i=0; i < sfhead.TotalSprNum; i++)
//     {
// 		fread(&shead, sizeof(SPRHEAD30), 1, fp); // �� ��������Ʈ ������ ��� �ε�
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
// 		///// �浹 ��ǥ, ���� ��ǥ �б� /////
// 		if(spr[i].hitNum>0) spr[i].hit = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].hitNum);
// 		if(spr[i].atckNum>0) spr[i].atck = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].atckNum);
// 		fread(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
// 		fread(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);
// 
// 		///// ��������Ʈ �̹��� ����Ÿ �б�
//         spr[i].data = (WORD *)malloc( shead.dsize );
// 		if(spr[i].data==NULL) 
// 		{ 
// 			//PutErrStr("�޸� �Ҵ� ����(�̹���)"); 
// 			return FALSE; 
// 		}
//         ZeroMemory(spr[i].data, shead.dsize);
// 		fread(spr[i].data, shead.dsize, 1, fp);
//     }
// 
// 	//// ���ϸ��̼� ������ �б�
// 	if(sfhead.CharacterNum > 0) ani.Load(fp);
// 
// 	fclose(fp);
// 
// 	ChangePixelFormat(ScreenRGBMode);
// 
//     return TRUE;
// }

/*--------------------------------------------------------------------------
  ��������Ʈ�� ���Ϸ� ����(ȭ�� ��忡 ������� RGB 5:5:5 �������� ����ȴ�)
---------------------------------------------------------------------------*/
BOOL xSprite::Save(char *name)
{
    FILE *fp = NULL;
	SPRHEAD30 shead;            // �� ��������Ʈ�� ���

	if(spr==NULL || sfhead.TotalSprNum==0) 
	{
		//PutErrStr("������ ��������Ʈ�� �����ϴ�."); 
		return FALSE;
	}

	if((fp=fopen( name,"wb"))==NULL)
	{
		//PutErrStr("���ϻ��� ���� (%s)",name); 
		return FALSE;
	}

	//ChangePixelFormat(555); // 5:5:5�� �ٲ۴�
    
    strcpy(sfhead.Identifier, "93XYZ sprite");
	sfhead.Version = 30;
	sfhead.BPD = 2;
	sfhead.PixelFormat = 555;
	sfhead.CharacterNum = ani.TotCharacterNum;

	fwrite( &sfhead, sizeof(SPRFILEHEAD),1,fp );         // ���� ��� ����

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
			// JPG ��������Ʈ �������� ���
			if(!spr[i].pJpgData || spr[i].jpgsize <= 0)
			{
				//PutErrStr("JPEG �̹��� �����Ͱ� �������� �ʽ��ϴ�"); 
				fclose(fp);
				return FALSE;
			}

			// ��������Ʈ ����� �ƴ� JPEG �̹����� ����� ����Ѵ�
			shead.dsize = spr[i].jpgsize;

			// �� ��������Ʈ�� ��� ����
			fwrite(&shead, sizeof(SPRHEAD30), 1, fp);
			// �浹 ��ǥ, ���� ��ǥ ����
			if(spr[i].hitNum>0)
				fwrite(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
			if(spr[i].atckNum>0) 
				fwrite(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);
			// JPEG �̹��� ����
			fwrite(spr[i].pJpgData, spr[i].jpgsize, 1, fp);
		}
		else 
		{
			// �Ϲ� ��������Ʈ�� ���
			spr[i].SprFormat = 0;
			// �� ��������Ʈ�� ��� ����
			fwrite(&shead, sizeof(SPRHEAD30), 1, fp);
			// �浹 ��ǥ, ���� ��ǥ ����
			if(spr[i].hitNum>0)
				fwrite(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
			if(spr[i].atckNum>0) 
				fwrite(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);
			// �̹��� ������ ����
			fwrite(spr[i].data, spr[i].dsize, 1, fp);
		}
    }

	//// ���ϸ��̼� ������ ����
	if(sfhead.CharacterNum > 0)
		ani.Save(fp);

	fclose(fp);

    return TRUE;
}

/*--------------------------------------------------------------------------
  ��������Ʈ�� Ư�� �ȼ� �������� �ٲ۴�
  (NewForm = 555, 565, -565)
---------------------------------------------------------------------------*/
BOOL xSprite::ChangePixelFormat(int NewForm)
{
	int i,x,y;
	if(NewForm == sfhead.PixelFormat) return TRUE;

	if(NewForm!=555 && NewForm!=565 && NewForm!=-565) return FALSE;

	WORD color;
	
    for(i=0; i < sfhead.TotalSprNum; i++) // �� ��������Ʈ ����ŭ
    {
		color = spr[i].colorkey; // ������� ��´�.

		// ������� �ȼ� ������ ��ȯ�Ѵ�
		if(sfhead.PixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
					spr[i].colorkey = RGB565_TO_RGB555(color);
		if(sfhead.PixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
					spr[i].colorkey = RGB555_TO_RGB565(color);
		if(sfhead.PixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
					spr[i].colorkey = RGB555_TO_BGR565(color);
		if(sfhead.PixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
					spr[i].colorkey = RGB565_TO_BGR565(color);
		
		// �̹��� �ȼ� ���� ��ȯ(����� ��������Ʈ�� ���)
		if(spr[i].compress)
		{
			int  m,xl,yl;
			int  sbyte,pbyte;
			short cnt;

			xl = spr[i].xl;
			yl = spr[i].yl;
			WORD* tar = spr[i].data + yl*2; // ���ΰŸ� ����Ÿ�� �ǳʶ�

			for(m=0; m<yl; m++)
			{
		        cnt = *tar++;      // ���� ���� ���
				sbyte=0;
		        while(cnt--) {

				    sbyte+=(*(WORD*)tar); // ������� ���
		            tar++;

		            pbyte =(*(WORD*)tar); // ���� ���
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
		else // ������� ���� ��������Ʈ�� ���
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
  ��������Ʈ ����� ����ȭ(�ʿ���� ����κ��� ���ش�)
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

	// ������ ����ȭ�� ���� ����κ� ���
	x1 = spr[snum].xl-1;
	fy = TRUE;
	for(i=0; i<yl; i++)
	{
		fx = FALSE;
		ex = FALSE;
		bline = TRUE;		// ������� ����
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

	// ���� ��ǥ���� �Բ� ����
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

	// ������ ����ȭ�� ���� ����κ� ���
	x1 = spr[snum].xl-1;
	fy = TRUE;
	for(i=0; i<yl; i++)
	{
		fx = FALSE;
		ex = FALSE;
		bline = TRUE;		// ������� ����
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

	// ���� ��ǥ���� �Բ� ����
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
  ��������Ʈ ����
---------------------------------------------------------------------------*/	
BOOL xSprite::Compress(LONG snum)
{
	WORD *buf = NULL;           // ����� �̹����� ����� ����
	long newsize=0;

	if(spr==NULL) return FALSE;
	if(snum < 0 || snum >= sfhead.TotalSprNum) return FALSE;
	if(spr[snum].data == NULL) return FALSE;
	if(spr[snum].compress == 1) return FALSE;

	newsize = EncodeSpr0(spr[snum].data, spr[snum].xl, spr[snum].yl, &buf, spr[snum].colorkey);
	if(newsize == 0) return FALSE;
	
	free(spr[snum].data);          // ������ �̹����� ����
	spr[snum].compress = 1;
	spr[snum].data     = buf;      // ���� ����� �̹���
	spr[snum].dsize    = newsize;  // ����� ������

	return TRUE;
}

/*--------------------------------------------------------------------------
  ��������Ʈ ���� ����
---------------------------------------------------------------------------*/
BOOL xSprite::DeCompress(LONG snum)
{
	WORD *buf = NULL;           // ������ Ǯ�� �̹����� ����� ����
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

	free(spr[snum].data);          // ������ �̹����� ����
	spr[snum].compress = 0;
	spr[snum].data     = buf;      // ������ Ǯ�� �̹��� ������ ������
	spr[snum].dsize    = newsize;  // ������ Ǯ�� ������

	return TRUE;
}

/*--------------------------------------------------------------------------
  Ư�� �̹����� �������� ��������Ʈ�� ����
  (�̹����� ���������� �ʴ´�)
---------------------------------------------------------------------------*/
BOOL xSprite::TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey)
{
	if(pImage == NULL) return FALSE;
	Remove();
	
	spr = (SPRITE30*) malloc(sizeof(SPRITE30));
	if(spr==NULL) 
	{ 
		//PutErrStr("�޸� �Ҵ� ����(At TransToSpr)"); 
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
  ���ʷ� ��������Ʈ�� �����ϱ� ���� �޸� �Ҵ�(������ �뵵)
---------------------------------------------------------------------------*/
BOOL xSprite::NewSpace(int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress)
{
	if(pImage == NULL) return FALSE;
	Remove();
	
	spr = (SPRITE30*) malloc(sizeof(SPRITE30));
	if(spr==NULL) { 
		//PutErrStr("�޸� �Ҵ� ����(At NewSpace)"); 
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
  ���ο� ��������Ʈ�� ����
---------------------------------------------------------------------------*/
BOOL xSprite::Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress)
{
	if(pImage == NULL) { 
		//PutErrStr("�̹��� �����Ͱ� NULL�Դϴ�(At Insert)"); 
		return FALSE; 
	}

	if(sfhead.TotalSprNum <= 0) // ���ʷ� ������ ��������Ʈ���
	{
		if(NewSpace(xl, yl, pImage, wColorKey, bCompress)==FALSE) return FALSE;
		return TRUE;
	}

	if(snum > sfhead.TotalSprNum || snum < 0) 
	{ 
		//PutErrStr("��������Ʈ �� ���� ���� �ʰ�(At Insert)"); 
		return FALSE; 
	}

	SPRITE30 *oldspr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
	if(oldspr==NULL) { 
		//PutErrStr("�޸� �Ҵ� ����(At Insert)"); 
		return FALSE; 
	}
	memcpy(oldspr, spr, sizeof(SPRITE30) * sfhead.TotalSprNum);
	
	free(spr);
	sfhead.TotalSprNum += 1;
	spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
	if(spr==NULL) { 
		//PutErrStr("�޸� �Ҵ� ����(At Insert)"); 
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
  ���ο� ��������Ʈ�� ����(���ο� �޸𸮸� �Ҵ��Ͽ� ������)
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
