
#include "StdAfx.h"
#include "JpegFile.h"	// JPEG ���
#include "dither.h"     // ����� ���
#include "xSprite.h"


namespace NMBASE
{
	namespace GRAPHICGDI
	{

		///////////////////////////////////////////////////////////////////////////


		/*--------------------------------------------------------------------

		<  0 �� ���� ���� ����(����� ������ ����Ÿ �κ�) >

		Byte(s)

		4 * height - �������� ���� �������� �Ÿ��� ��� �迭
		2          - ����(pattern ) �� ---------------------------+
		2          - ����� �� ---------------+                   | ���α��� ��ŭ
		2          - �׸� ����                | ���ϼ���ŭ �ݺ�   | �ݺ�
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
		unsigned long EncodeSpr0(WORD *image, int xl, int yl, WORD **output, WORD TRANS)
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
				memcpy(buf+k*2, &bp, 4); // ����: buf�� WORD�� �������̹Ƿ�... k*2

				// ���δ� ������ ����
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

		//class xDraw;
		//extern static int xDraw::PixelFormat; // ��ũ���� Pixel Format�� �˾ƾ� �ϱ� ����

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
		�ε��� ��������Ʈ�� ����
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
		��������Ʈ ������ �ε�(������ 555�� �д´�)
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
		BOOL xSprite::Load(char *name, int ScreenRGBMode)
		{
			FILE *fp = NULL;

			m_strPath = name;

			SPRFILEHEAD tempsfhead;     // �ӽ� ���� ���
			SPRHEAD30 shead;            // �� ��������Ʈ�� ���

			if( (fp = fopen( name, "rb")) == NULL ) { 
				PutErrStr("���� ���¿� �����Ͽ����ϴ�(%s)", name);
				return FALSE;
			}

			fread(&tempsfhead, sizeof(SPRFILEHEAD), 1, fp); //�⺻ ��� �ε�

			if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) {
				//��������Ʈ ������ �ƴϸ�
				PutErrStr("��������Ʈ ������ �ƴմϴ�(%s)", name);
				fclose(fp);
				return FALSE;
			}

			// ���� ���� �˻�
			if(tempsfhead.Version < 30 || tempsfhead.Version > 32) {
				PutErrStr("��������Ʈ ������ ��ġ���� �ʽ��ϴ�(%s)", name);
				fclose(fp);
				return FALSE;
			}

			Remove();

			memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD));

			spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
			if(spr==NULL) { 
				PutErrStr("�޸� �Ҵ� ����(���)"); 
				fclose(fp);
				return FALSE; 
			}
			ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

			for(int i=0; i < sfhead.TotalSprNum; i++)
			{
				fread(&shead, sizeof(SPRHEAD30), 1, fp); // �� ��������Ʈ ������ ��� �ε�

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

				///// �浹 ��ǥ, ���� ��ǥ �б� ///// 
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

				// ��������Ʈ ��������, JPEG �������� Ȯ���ϴ�
				if(spr[i].SprFormat == 1) 
				{
					//
					// JPEG �����̴�
					//

					// ����� �ɼ��� �����Ǿ� �ִٸ� ����� ����
					BOOL bDither = FALSE;
					if(spr[i].Dithering == 1) bDither = TRUE;

					//// JPEG ���� �б�
					fpos_t pos;
					fgetpos(fp, &pos);

					int width=0, height=0;
					WORD *pNewImage = Make16bitFromJpeg(fp, &width, &height, bDither);
					if(!pNewImage) {
						PutErrStr("JPEG ���� �б� ����");
						fclose(fp);
						return FALSE;
					}

					if(width != spr[i].xl || height != spr[i].yl) {
						PutErrStr("JPEG ������ �̹��� ����� ��ġ���� ����");
						fclose(fp);
						return FALSE;
					}

					spr[i].dsize = width * height * 2;
					spr[i].data = pNewImage;

					// ������� ���� ���·� �ϴ� ��ȯ
					spr[i].compress = 0;

					if(shead.compress == 1) {
						// ���� ��Ŵ
						Compress(i);
					}

					if(bSpriteEditMode) {
						// JPEG �̹��� ������ ����(���ӿ����� �ʿ� ����)
						fsetpos(fp, &pos);
						spr[i].jpgsize = shead.dsize;
						spr[i].pJpgData = new BYTE[shead.dsize];
						fread(spr[i].pJpgData, shead.dsize, 1, fp);
					}
					else {
						// ���� ��������Ʈ�� ���� ������ �̵�
						pos += shead.dsize;
						fsetpos(fp, &pos);
					}
				}
				else {
					//
					// ��������Ʈ �����̴�
					//

					///// ��������Ʈ �̹��� ����Ÿ �б�
					if(shead.dsize > 0)
						spr[i].data = (WORD *)malloc( shead.dsize );

					if(spr[i].data==NULL) { 
						PutErrStr("�޸� �Ҵ� ����(�̹���)"); 
						return FALSE; 
					}
					ZeroMemory(spr[i].data, shead.dsize);
					fread(spr[i].data, shead.dsize, 1, fp);
				}

				// ���� ���̺��� ���Ե� �������� �˻�
				if(spr[i].PixelFormat == 5558) {
					// ���� ���̺� ���� �����͸� ����
					int alphasize = spr[i].xl * spr[i].yl;
					spr[i].pAlpha = ((BYTE*)spr[i].data) + spr[i].dsize - alphasize;
				}
			}

			if(sfhead.Version <= 30) {
				//// ������ ���ϸ��̼� ������ �б�
				if(sfhead.SceneNum > 0) 
					ani.Load(fp);
			}
			else {
				//// �Ź��� ���ϸ��̼� ������ �б�
				if(sfhead.SceneNum > 0) 
					scene.Load(fp);

				//// ��Ƽ�� ������ �б�
				multiscene.Load(fp);
				UpdateAllMScenePlayTime();
			}

			fclose(fp);

			if(sfhead.PixelFormat != 4444) ChangePixelFormat(ScreenRGBMode);

			return TRUE;
		}

		/*--------------------------------------------------------------------------
		��������Ʈ ����� ����ȭ(�ʿ���� ����κ��� ���ش�)
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

			// ������ ����ȭ�� ���� ����κ� ���
			x1 = spr[snum].xl-1;
			fy = TRUE;
			for(i=0; i<yl; i++) {
				fx = FALSE;
				ex = FALSE;
				bline = TRUE;		// ������� ����
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

			// ���� ���̺� ���
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
				// ��������Ʈ �̹��� Ŭ����
				memcpy(pNew+(newXl*i), pSor+x1, newXl*2);
				pSor += xl;

				// ���� �̹��� Ŭ����
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

			// ���� ��ǥ���� �Բ� ����
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

			if(spr[snum].PixelFormat == 5558) {
				// �������̺��� ������ ��������Ʈ��� ���Ӱ� �Ҵ��Ͽ� �̹��� ����
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

			int alphasize = 0;
			if(spr[snum].PixelFormat == 5558) {
				// �������̺��� ������ ��������Ʈ
				alphasize = spr[snum].xl * spr[snum].yl;
			}

			newsize = (spr[snum].xl * spr[snum].yl * 2) + alphasize;
			buf = (WORD *)malloc(newsize);
			if(buf==NULL) return FALSE;

			//ZeroMemory(buf, newsize);
			DecodeSpr0(buf, spr[snum].xl, spr[snum].yl, spr[snum].data, spr[snum].colorkey);

			// ���� ���̺� ����
			if(spr[snum].PixelFormat == 5558) {
				BYTE* pNewAlpha = (BYTE*)buf + (spr[snum].xl * spr[snum].yl * 2);
				memcpy(pNewAlpha, spr[snum].pAlpha, alphasize);
				spr[snum].pAlpha = pNewAlpha;
			}

			free(spr[snum].data);          // ������ �̹����� ����

			spr[snum].compress = 0;
			spr[snum].data     = buf;      // ������ Ǯ�� �̹��� ������ ������
			spr[snum].dsize    = newsize;  // ������ Ǯ�� ������

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
				if(spr[i].compress) {
					int  m,xl,yl;
					int  sbyte,pbyte;
					short cnt;

					xl = spr[i].xl;
					yl = spr[i].yl;
					WORD* tar = spr[i].data + yl*2; // ���ΰŸ� ����Ÿ�� �ǳʶ�

					for(m=0; m<yl; m++) {
						cnt = *tar++;      // ���� ���� ���
						sbyte=0;
						while(cnt--) {

							sbyte+=(*(WORD*)tar); // ������� ���
							tar++;

							pbyte =(*(WORD*)tar); // ���� ���
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
				else // ������� ���� ��������Ʈ�� ���
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
		Ư�� �̹����� �������� ��������Ʈ�� ����
		(�̹����� ���������� �ʴ´�)
		---------------------------------------------------------------------------*/
		BOOL xSprite::TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey)
		{
			if(pImage == NULL) return FALSE;
			if(xl <= 0 || yl <= 0) return FALSE;

			Remove();

			spr = (SPRITE30*) malloc(sizeof(SPRITE30));
			if(spr==NULL) { 
				PutErrStr("�޸� �Ҵ� ����(At TransToSpr)"); 
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
		Ư�� �̹����� �������� ��������Ʈ�� ����
		(�̹��� ���۸� ���������� �Ҵ��Ͽ� ������)
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
		���ʷ� ��������Ʈ�� �����ϱ� ���� �޸� �Ҵ�(������ �뵵)
		---------------------------------------------------------------------------*/
		BOOL xSprite::NewSpace(int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress)
		{
			if(pImage == NULL) return FALSE;
			Remove();

			spr = (SPRITE30*) malloc(sizeof(SPRITE30));
			if(spr==NULL) { 
				PutErrStr("�޸� �Ҵ� ����(At NewSpace)"); 
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
		���ο� ��������Ʈ�� ����
		---------------------------------------------------------------------------*/
		BOOL xSprite::Insert(LONG snum, int xl, int yl, WORD *pImage, WORD wColorKey, BOOL bCompress)
		{
			if(pImage == NULL) { 
				PutErrStr("�̹��� �����Ͱ� NULL�Դϴ�(At Insert)"); 
				return FALSE; 
			}

			if(sfhead.TotalSprNum <= 0) {
				// ���ʷ� ������ ��������Ʈ���
				if(NewSpace(xl, yl, pImage, wColorKey, bCompress)==FALSE) return FALSE;
				return TRUE;
			}

			if(snum > sfhead.TotalSprNum || snum < 0) { 
				PutErrStr("��������Ʈ �� ���� ���� �ʰ�(At Insert)"); 
				return FALSE; 
			}

			SPRITE30 *oldspr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
			if(oldspr==NULL) { 
				PutErrStr("�޸� �Ҵ� ����(At Insert)"); 
				return FALSE; 
			}
			memcpy(oldspr, spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

			free(spr);
			sfhead.TotalSprNum += 1;
			spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
			if(spr==NULL) { 
				PutErrStr("�޸� �Ҵ� ����(At Insert)"); 
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
		���ο� ��������Ʈ�� ����(���ο� �޸𸮸� �Ҵ��Ͽ� ������)
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
		JPEG ���� �����ͷ� 16��Ʈ 555 �̹����� ����
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
				// ����� �ɼ��� ������ ���
				DWORD rmask = (DWORD)0x0000001F;
				DWORD gmask = (DWORD)0x000003E0;
				DWORD bmask = (DWORD)0x00007C00;
				// 24��Ʈ �̹����� ������Ͽ� 16��Ʈ(5:5:5) �̹����� ��ȯ
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
					// 16��Ʈ�� ��ȯ
					static char k=0;
					k=0;

					for(int j=0; j<xsize; j++) {
						// ����) ���⼱ RGB�������� ��Ʈ���� BGR���̴�
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


		/////////////////////////////////// ��� �ִϸ��̼� ���� ///////////////////////////////////////////


		// ����� �� �÷��� Ÿ���� ����
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

		// �̸����� ��� ��ȣ�� ����(-1�� �����ϸ� ����)
		int xSprite::GetScene(char* name)
		{
			if(scene.TotSceneNum==0) 
				return -1;

			for(int i=0; i<scene.TotSceneNum; i++)
				if(stricmp(name, (scene.ppScene[i])->SceneName) == 0) 
					return i;

			return -1;
		}

		// ����� �� �������� ����
		int xSprite::GetTotFrame(int scn)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return 0;
			return pScene->TotFrameNum;
		}

		// �־��� �ð��� �÷��� �� �������� ���� : ������ ��� -1�� ����
		int xSprite::GetNowFrame(int scn, DWORD time, BOOL bLoop)
		{
			SceneData* pScene = scene.GetScenePt(scn);
			if(!pScene) 
				return -1;
			return pScene->SearchFrame(time, bLoop);
		}

		// Ư�� �������� ���� �ð��� �� �ð��� ����
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

		/////////////////////////////////// ��Ƽ�� �ִϸ��̼� ���� ///////////////////////////////////////////

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
					// ���� ��� ������Ÿ���� 0�� ��� �ݺ� ȸ���� ���� �ڵ����� �ð��� ������
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
		//      ��������Ʈ ���� ���� ��� ��Ƽ������ ���� ���� ���� ���� �Լ�
		//--------------------------------------------------------------------------------------
		BOOL xSprite::QueryAllSound(QUERYMSCENE_SOUND* pQuery)
		{
			if(!pQuery) 
				return FALSE;

			if(pQuery->NowMScene == -1) 
				pQuery->NowMScene += 1;

			if(pQuery->NowMScene < 0 || pQuery->NowMScene >= multiscene.TotMSceneNum)
				return FALSE;

			// 0�������� �ִ� �ð� ������ ��� ���带 ����
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
		//      ��������Ʈ ���� ���� ��� ��Ƽ������ ���� ��ũ��Ʈ ���� ���� �Լ�(TRUE�� �����ϴ� ���ȿ� �ݺ� ȣ���ؾ���)
		//--------------------------------------------------------------------------------------
		BOOL xSprite::QueryAllScript(QUERYMSCENE_SCRIPT* pQuery)
		{
			if(!pQuery) 
				return FALSE;

			if(pQuery->NowMScene == -1) 
				pQuery->NowMScene += 1;

			if(pQuery->NowMScene < 0 || pQuery->NowMScene >= multiscene.TotMSceneNum)
				return FALSE;

			// 0�������� �ִ� �ð� ������ ��� ��ũ��Ʈ�� ����
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
		//      ��Ƽ�� �ִϸ��̼ǿ��� Ư�� �ð� ������ ���� ���� ���� �Լ�(TRUE�� �����ϴ� ���ȿ� �ݺ� ȣ���ؾ���)
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
					// ���� �ð��� ������ ���� ���� �ð� ���� ���� ���
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
		//      ��Ƽ�� �ִϸ��̼ǿ��� Ư�� �ð� ������ ��ũ��Ʈ ���� ���� �Լ�
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
					// ���� �ð��� ������ ���� ���� �ð� ���� ���� ���
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
