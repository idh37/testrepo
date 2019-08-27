#include "stdafx.h"
#include "ImageNumber.h"
#include <algorithm>
#include "../../tinyxml/tinyxml.h"
#include "../ObjectMan.h"
#include "Image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CImageNumber::CImageNumber(void)
:	m_pImage(NULL),
	m_i64Number(0),
	m_nCutCount(0),
	m_nMaxPlace(0),
	m_nUnitStartIndex(-1),
	m_nUnitIndexGap(0),
	m_nFirstUnitIndex(-1),
	m_bShowZero(true),
	m_bShowZeroNumber(false),
	m_bRedraw(false),
	m_uFormat(DT_LEFT | DT_TOP),
	m_nNumberType(EIND_NUMBER),
	m_nPrefixIndex(-1),
	m_nPlusIndex(-1),
	m_nMinusIndex(-1),
	m_nStartIndex(0),
	m_bShowZeroPlus(false)
{
}

CImageNumber::~CImageNumber()
{
	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

bool CImageNumber::Create(LPCTSTR lpPath, int nWidth, int nHeight)
{
	if(m_pImage == NULL)
	{
		m_pImage = new CImage;
	}

	if(!m_pImage->Create(lpPath))
	{
		delete m_pImage;
		m_pImage = NULL;
		return false;
	}

	if(m_pImage->GetTotalNum() <= m_nStartIndex)
		m_nStartIndex = 0;

	SetLocalClipRect(CRect(0,0,(float)nWidth,(float)nHeight));
	SetLocalRect(CRect(0,0,(float)nWidth,(float)nHeight));

	SetRedraw();

	return true;
}

void CImageNumber::SetStartIndex(const int &nStartIndex)
{
	if(m_nStartIndex == nStartIndex) return;
	m_nStartIndex = nStartIndex;
	SetRedraw();
}

const int CImageNumber::GetTotalNum(void)
{
	return m_pImage?m_pImage->GetTotalNum():0;
}

CObjectBase *CImageNumber::NewObject(void)
{
	return new CImageNumber;
}

CObjectBase *CImageNumber::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CImageNumber *pImage = (CImageNumber *)pObject;
	//이전에 읽어들인 이미지가 있다면 지운다.
	if(pImage->m_pImage)
	{
		delete pImage->m_pImage;
		pImage->m_pImage = NULL;
	}
	//새로운 이미지를 복사해 온다.
	if(m_pImage) pImage->m_pImage = (CImage *)m_pImage->Clone();
	pImage->m_i64Number = m_i64Number;
	pImage->m_nCutCount = m_nCutCount;
	pImage->m_nMaxPlace = m_nMaxPlace;
	pImage->m_nUnitStartIndex = m_nUnitStartIndex;
	pImage->m_nUnitIndexGap = m_nUnitIndexGap;
	pImage->m_nFirstUnitIndex = m_nFirstUnitIndex;
	pImage->m_bShowZero = m_bShowZero;
	pImage->m_bShowZeroNumber = m_bShowZeroNumber;
	pImage->m_uFormat = m_uFormat;
	pImage->m_nNumberType = m_nNumberType;
	pImage->m_nPrefixIndex = m_nPrefixIndex;
	pImage->m_nPlusIndex = m_nPlusIndex;
	pImage->m_nMinusIndex = m_nMinusIndex;
	pImage->m_nStartIndex = m_nStartIndex;
	pImage->m_bShowZeroPlus = m_bShowZeroPlus;
	pImage->SetRedraw();

	return pObject;
}

std::string CImageNumber::GetFileName(void)
{
	if(m_pImage) return m_pImage->GetFileName();
	return "";
}

size_t CImageNumber::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);			//이름크기
	if(m_pImage) nRet += m_pImage->GetFileName().size();	//이름
	nRet += sizeof(INT64);			//m_i64Number
	nRet += sizeof(int);			//m_nCutCount
	nRet += sizeof(int);			//m_nMaxPlace
	nRet += sizeof(int);			//m_nUnitStartIndex
	nRet += sizeof(int);			//m_nUnitIndexGap
	nRet += sizeof(int);			//m_nFirstUnitIndex
	nRet += sizeof(bool);			//m_bShowZero
	nRet += sizeof(bool);			//m_bShowZeroNumber
	nRet += sizeof(UINT);			//m_uFormat
	nRet += sizeof(ENUM_IMAGENUMBER_DRAWTYPE);//m_nNumberType
	nRet += sizeof(int);			//m_nPrefixIndex
	nRet += sizeof(int);			//m_nPlusIndex
	nRet += sizeof(int);			//m_nMinusIndex
	nRet += sizeof(int);			//m_nStartIndex
	nRet += sizeof(bool);
	return nRet;
}

size_t CImageNumber::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = 0;
	if(m_pImage) nSize = m_pImage->GetFileName().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);			//이름크기
	if(m_pImage)
	{
		memcpy(pDest+nRet, m_pImage->GetFileName().c_str(), nSize);
		nRet += nSize;	//이름
	}
	memcpy(pDest+nRet, &m_i64Number, sizeof(INT64));
	nRet += sizeof(INT64);			//m_i64Number
	memcpy(pDest+nRet, &m_nCutCount, sizeof(int));
	nRet += sizeof(int);			//m_nCutCount
	memcpy(pDest+nRet, &m_nMaxPlace, sizeof(int));
	nRet += sizeof(int);			//m_nMaxPlace
	memcpy(pDest+nRet, &m_nUnitStartIndex, sizeof(int));
	nRet += sizeof(int);			//m_nUnitStartIndex
	memcpy(pDest+nRet, &m_nUnitIndexGap, sizeof(int));
	nRet += sizeof(int);			//m_nUnitIndexGap
	memcpy(pDest+nRet, &m_nFirstUnitIndex, sizeof(int));
	nRet += sizeof(int);			//m_nFirstUnitIndex
	memcpy(pDest+nRet, &m_bShowZero, sizeof(bool));
	nRet += sizeof(bool);			//m_bShowZero
	memcpy(pDest+nRet, &m_bShowZeroNumber, sizeof(bool));
	nRet += sizeof(bool);			//m_bShowZeroNumber
	memcpy(pDest+nRet, &m_uFormat, sizeof(UINT));
	nRet += sizeof(UINT);			//m_uFormat
	memcpy(pDest+nRet, &m_nNumberType, sizeof(ENUM_IMAGENUMBER_DRAWTYPE));
	nRet += sizeof(ENUM_IMAGENUMBER_DRAWTYPE);	//m_nNumberType
	memcpy(pDest+nRet, &m_nPrefixIndex, sizeof(int));
	nRet += sizeof(int);			//m_nPrefixIndex
	memcpy(pDest+nRet, &m_nPlusIndex, sizeof(int));
	nRet += sizeof(int);			//m_nPlusIndex
	memcpy(pDest+nRet, &m_nMinusIndex, sizeof(int));
	nRet += sizeof(int);			//m_nMinusIndex
	memcpy(pDest+nRet, &m_nStartIndex, sizeof(int));
	nRet += sizeof(int);			//m_nStartIndex
	memcpy(pDest+nRet, &m_bShowZeroPlus, sizeof(bool));
	nRet += sizeof(bool);			//m_bShowZeroPlus
	return nRet;
}

size_t CImageNumber::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	size_t nSize = 0;
	std::string strFileName;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);			//이름크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize;	//이름
	}
	memcpy(&m_i64Number, pDest+nRet, sizeof(INT64));
	nRet += sizeof(INT64);			//m_i64Number
	memcpy(&m_nCutCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nCutCount
	memcpy(&m_nMaxPlace, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nMaxPlace
	memcpy(&m_nUnitStartIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nUnitStartIndex
	memcpy(&m_nUnitIndexGap, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nUnitIndexGap
	memcpy(&m_nFirstUnitIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nFirstUnitIndex
	memcpy(&m_bShowZero, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_bShowZero
	memcpy(&m_bShowZeroNumber, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_bShowZeroNumber
	memcpy(&m_uFormat, pDest+nRet, sizeof(UINT));
	nRet += sizeof(UINT);			//m_uFormat
	memcpy(&m_nNumberType, pDest+nRet, sizeof(ENUM_IMAGENUMBER_DRAWTYPE));
	nRet += sizeof(ENUM_IMAGENUMBER_DRAWTYPE);			//m_nNumberType
	memcpy(&m_nPrefixIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nPrefixIndex
	memcpy(&m_nPlusIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nPlusIndex
	memcpy(&m_nMinusIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nMinusIndex
	memcpy(&m_nStartIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);			//m_nStartIndex
	memcpy(&m_bShowZeroPlus, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);			//m_bShowZeroPlus

	Create(strFileName.c_str(), (int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	SetRedraw();

	if(IsTool()) SetMessage(true);

	return nRet;
}

bool CImageNumber::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	char strTemp[256];
	_i64toa_s(m_i64Number, strTemp, 256, 10);

	TiXmlElement xmlFile("file");
	xmlFile.SetAttribute("path", m_pImage?m_pImage->GetFileName().c_str():"");
	xmlFile.SetAttribute("startindex", m_nStartIndex);
	xmlFile.SetAttribute("number", strTemp);
	xmlFile.SetAttribute("cut", m_nCutCount);
	xmlFile.SetAttribute("maxplace", m_nMaxPlace);
	xmlFile.SetAttribute("unitstart", m_nUnitStartIndex);
	xmlFile.SetAttribute("unitindexgap", m_nUnitIndexGap);
	xmlFile.SetAttribute("firstunit", m_nFirstUnitIndex);
	xmlFile.SetAttribute("showzero", m_bShowZero?1:0);
	xmlFile.SetAttribute("showzeronumber", m_bShowZeroNumber?1:0);
	xmlFile.SetAttribute("showzeroplus", m_bShowZeroPlus?1:0);
	xmlFile.SetAttribute("format", m_uFormat);
	xmlFile.SetAttribute("type", GetImageNumberDrawTypeToString(m_nNumberType).c_str());
	xmlFile.SetAttribute("prefix", m_nPrefixIndex);
	xmlFile.SetAttribute("plus", m_nPlusIndex);
	xmlFile.SetAttribute("minus", m_nMinusIndex);
	pXmlObject->InsertEndChild(xmlFile);
	return true;
}

bool CImageNumber::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	std::string strPath;
	TiXmlElement *pXmlFile = pXmlObject->FirstChildElement("file");
	GetAttribute(pXmlFile, "path", strPath);
	if(pXmlFile->Attribute("startindex")) GetAttribute(pXmlFile, "startindex", m_nStartIndex);
	else m_nStartIndex = 0;
	GetAttribute(pXmlFile, "number", m_i64Number);
	GetAttribute(pXmlFile, "cut", m_nCutCount);
	GetAttribute(pXmlFile, "maxplace", m_nMaxPlace);
	GetAttribute(pXmlFile, "unitstart", m_nUnitStartIndex);
	GetAttribute(pXmlFile, "unitindexgap", m_nUnitIndexGap);
	GetAttribute(pXmlFile, "firstunit", m_nFirstUnitIndex);
	GetAttribute(pXmlFile, "showzero", m_bShowZero);
	GetAttribute(pXmlFile, "showzeronumber", m_bShowZeroNumber);
	if(pXmlFile->Attribute("showzeroplus")) GetAttribute(pXmlFile, "showzeroplus", m_bShowZeroPlus);
	GetAttribute(pXmlFile, "format", m_uFormat);
	GetAttribute(pXmlFile, "type", strTemp);
	m_nNumberType = GetStringToImageNumberDrawType(strTemp);
	GetAttribute(pXmlFile, "prefix", m_nPrefixIndex);
	GetAttribute(pXmlFile, "plus", m_nPlusIndex);
	GetAttribute(pXmlFile, "minus", m_nMinusIndex);

	Create(strPath.c_str(), (int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	SetRedraw();

	if(IsTool()) SetMessage(true);

	return true;
}

void CImageNumber::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bRedraw) CheckRedraw();
	__super::Draw(pPage);
}

void CImageNumber::SetNumber(const INT64 &i64Number)
{
	if(i64Number == m_i64Number) return;
	m_i64Number = i64Number;
	SetRedraw();
}

void CImageNumber::ClearImage(void)
{
	std::list<CImage *>::iterator start = m_listImage.begin();
	std::list<CImage *>::iterator end = m_listImage.end();

	for(;start != end;++start)
	{
		delete *start;
	}

	m_listImage.clear();
}

void CImageNumber::CheckRedraw(void)
{
	SetRedraw(false);
	if(m_pImage == NULL) return;
	ClearImage();
	INT64 i64Number = m_i64Number;
	bool bMinus = false;
	if(i64Number < 0)
	{
		if(m_nMinusIndex == -1)
		{
			i64Number = 0;
		}
		else
		{
			i64Number *= -1;
		}
		bMinus = true;
	}

	if(i64Number == 0 && !m_bShowZero) return;

	char strTemp[256];
	_i64toa_s(i64Number, strTemp, 256, 10);

	int nSize = (int)strlen(strTemp);
	if(m_nFirstUnitIndex >= 0 && m_nFirstUnitIndex < m_pImage->GetTotalNum())
	{
		CImage *pImage = (CImage *)m_pImage->Clone();
		pImage->SetIndex(m_nFirstUnitIndex);
		m_listImage.push_front(pImage);		
	}

	switch(m_nNumberType)
	{
	case EIND_NUMBER:
		CheckNumberDraw(strTemp, nSize);
		break;
	case EIND_COMMA_MONEY:
		CheckCommaMoneyDraw(strTemp, nSize);
		break;
	case EIND_MONEY:
		CheckMoneyDraw(strTemp, nSize);
		break;
	case EIND_NEONSIGN_MONEY:
		CheckNeonsignMoneyDraw(strTemp, nSize);
		break;
	}

	if(bMinus && m_nMinusIndex != -1)
	{
		if(m_pImage->IsPossibilityIndex(m_nMinusIndex))
		{
			CImage *pImage = (CImage *)m_pImage->Clone();
			pImage->SetIndex(m_nMinusIndex);
			m_listImage.push_front(pImage);
		}
	}
	else if(i64Number == 0)
	{
		if(m_bShowZeroPlus && m_pImage->IsPossibilityIndex(m_nPlusIndex))
		{
			CImage *pImage = (CImage *)m_pImage->Clone();
			pImage->SetIndex(m_nPlusIndex);
			m_listImage.push_front(pImage);
		}
	}
	else if(!bMinus && m_nPlusIndex != -1)
	{
		if(m_pImage->IsPossibilityIndex(m_nPlusIndex))
		{
			CImage *pImage = (CImage *)m_pImage->Clone();
			pImage->SetIndex(m_nPlusIndex);
			m_listImage.push_front(pImage);
		}
	}

	if(m_pImage->IsPossibilityIndex(m_nPrefixIndex))
	{
		CImage *pImage = (CImage *)m_pImage->Clone();
		pImage->SetIndex(m_nPrefixIndex);
		m_listImage.push_front(pImage);
	}

	CSize szSize = GetImageSize();
	CPoint ptPos = CPointZero;
	if(m_uFormat & DT_RIGHT)
	{
		ptPos.x = m_rtLocalRect.GetWidth() - (float)szSize.cx;
	}
	else if(m_uFormat & DT_CENTER)
	{
		ptPos.x = (m_rtLocalRect.GetWidth() - (float)szSize.cx)/2.0f;
	}

	if(m_uFormat & DT_BOTTOM)
	{
		ptPos.y = m_rtLocalRect.GetHeight() - (float)szSize.cy;
	}
	else if(m_uFormat & DT_VCENTER)
	{
		ptPos.y = (m_rtLocalRect.GetHeight() - (float)szSize.cy)/2.0f;
	}

	std::list<CImage *>::iterator start = m_listImage.begin();
	std::list<CImage *>::iterator end = m_listImage.end();
	for(;start != end;++start)
	{
		(*start)->SetLocalPos(ptPos);
		ptPos.x += (*start)->GetWidth();
		AddLocalChild(*start);
	}
}

CSize CImageNumber::GetImageSize(void)
{
	if(m_bRedraw) CheckRedraw();
	CSize szSize(0,0);
	CSize szTotal(0,0);
	std::list<CImage *>::iterator start = m_listImage.begin();
	std::list<CImage *>::iterator end = m_listImage.end();
	for(;start != end;++start)
	{
		szSize.cx = (LONG)(*start)->GetWidth();
		szSize.cy = (LONG)(*start)->GetHeight();

		szTotal.cx += szSize.cx;
		if(szTotal.cy < szSize.cy) szTotal.cy = szSize.cy;
	}
	return szTotal;
}

void CImageNumber::CheckNumberDraw(char *strNumber,int &nSize)
{
	CImage *pImage = NULL;
	int nMax = nSize-1;
	int nIndex = 0;
	if(m_nMaxPlace != 0 && nMax > m_nMaxPlace)
	{
		nIndex = m_nStartIndex + 9;
		if(m_pImage->IsPossibilityIndex(nIndex))
		{
			for(int i=0;i<=m_nMaxPlace;++i)
			{
				pImage = (CImage *)m_pImage->Clone();
				pImage->SetIndex(m_nStartIndex + 9);
				m_listImage.push_front(pImage);
			}
		}
	}
	else
	{
		for(int i=0;i<=nMax;++i)
		{
			nIndex = m_nStartIndex + strNumber[nMax-i] - '0';
			if(m_pImage->IsPossibilityIndex(nIndex))
			{
				pImage = (CImage *)m_pImage->Clone();
				pImage->SetIndex(nIndex);
				m_listImage.push_front(pImage);
			}
		}
	}
}

void CImageNumber::CheckCommaMoneyDraw(char *strNumber,int &nSize)
{
	CImage *pImage = NULL;
	int nMax = nSize-1;
	int nEnd = nMax;
	if(m_nMaxPlace != 0 && nMax > m_nMaxPlace)
	{
		int nIndex = m_nStartIndex + 9;
		if(m_pImage->IsPossibilityIndex(nIndex))
		{
			for(int i=0;i<=m_nMaxPlace;++i)
			{
				if(i!=0 && i%3 == 0 && m_pImage->IsPossibilityIndex(m_nUnitStartIndex))
				{
					pImage = (CImage *)m_pImage->Clone();
					pImage->SetIndex(m_nUnitStartIndex);
					m_listImage.push_front(pImage);
				}

				pImage = (CImage *)m_pImage->Clone();
				pImage->SetIndex(m_nStartIndex+9);
				m_listImage.push_front(pImage);
			}
		}
	}
	else
	{
		int nIndex = 0;
		for(int i=0;i<=nMax;++i)
		{
			if(i!=0 && i%3 == 0 && m_pImage->IsPossibilityIndex(m_nUnitStartIndex))
			{
				pImage = (CImage *)m_pImage->Clone();
				pImage->SetIndex(m_nUnitStartIndex);
				m_listImage.push_front(pImage);
			}

			nIndex = m_nStartIndex + strNumber[nMax-i] - '0';
			if(m_pImage->IsPossibilityIndex(nIndex))
			{
				pImage = (CImage *)m_pImage->Clone();
				pImage->SetIndex(nIndex);
				m_listImage.push_front(pImage);
			}
		}
	}
}

void CImageNumber::CheckMoneyDraw(char *strNumber,int &nSize)
{
	CImage *pImage = NULL;
	int nFirstNotZero = 0;
	int nUnitIndex = -2;
	int i,j;
	int nMax = nSize-1;
	int nStart = 0;

	//최대 표시 단위의 갯수가 0이면 모두 표시한다.
	//최대 표시 단위의 갯수가 0보다 크다면 해당 시작값을 찾는다. 
	if(m_nCutCount != 0)
	{
		//0이 아닌 값을 가진 단우의 총 수를 구한다.
		int nTotalUnit = 0;
		for(i=0;i<=nMax;i += 4)
		{
			//단위에 숫자가 4개 존재하는지를 검사한다.
			//4개가 존재 한다면
			if(i+4 <= nMax)
			{
				nFirstNotZero = -1;
				for(j=3;j>=0;--j)
				{
					if(strNumber[nMax-(i+j)] != '0')
					{
						nFirstNotZero = j;
						break;
					}
				}

				//0이 아닌 숫자가 나오지 않는다면 다음 단위를 검사한다.
				if(nFirstNotZero == -1)
				{
					continue;
				}
				else
				{
					++nTotalUnit;
				}
			}
			else
			{
				++nTotalUnit;
			}
		}

		//만약 0이 아닌 단위의 갯수가 요구 값보다 크다면 시작값을 찾는다.
		if(nTotalUnit > m_nCutCount)
		{
			int nUnit = 0;
			for(i=0;i<=nMax;i += 4)
			{
				//단위에 숫자가 4개 존재하는지를 검사한다.
				//4개가 존재 한다면
				if(i+4 <= nMax)
				{
					nFirstNotZero = -1;
					for(j=3;j>=0;--j)
					{
						if(strNumber[nMax-(i+j)] != '0')
						{
							nFirstNotZero = j;
							break;
						}
					}

					//0이 아닌 숫자가 나오지 않는다면 다음 단위를 검사한다.
					if(nFirstNotZero == -1)
					{
						continue;
					}
					else
					{
						++nUnit;
					}
				}
				else
				{
					++nUnit;
				}

				if((m_nCutCount-1) == (nTotalUnit - nUnit)) break;
			}

			nStart = i;
		}
	}


	for(i=0;i<=nMax;i += 4)
	{
		//단위를 증가 시큰다.
		++nUnitIndex;
		if(i<nStart)
		{
			continue;
		}

		//단위에 숫자가 4개 존재하는지를 검사한다.
		//4개가 존재 한다면
		if(i+4 <= nMax)
		{
			//첫번째 0인 아닌 숫자가 나오는 위치를 찾는다.
			nFirstNotZero = -1;
			for(j=3;j>=0;--j)
			{
				if(strNumber[nMax-(i+j)] != '0')
				{
					nFirstNotZero = j;
					break;
				}
			}

			//0이 아닌 숫자가 나오지 않는다면
			//단위만 증가시킨다.
			if(nFirstNotZero == -1)
			{
				continue;
			}
			else
			{
				int nIndex = 0;
				if(i!=0 && m_nUnitStartIndex >= 0)
				{
					nIndex = m_nUnitStartIndex + nUnitIndex*m_nUnitIndexGap;
					if(m_pImage->IsPossibilityIndex(nIndex))
					{
						pImage = (CImage *)m_pImage->Clone();
						pImage->SetIndex(nIndex);
						m_listImage.push_front(pImage);
					}
				}

				for(j=0;j<=nFirstNotZero;++j)
				{
					nIndex = m_nStartIndex + strNumber[nMax-(i+j)] - '0';
					if(m_pImage->IsPossibilityIndex(nIndex))
					{
						pImage = (CImage *)m_pImage->Clone();
						pImage->SetIndex(nIndex);
						m_listImage.push_front(pImage);
					}
				}
			}
		}
		else
		{
			int nIndex = 0;
			if(i!=0 && m_nUnitStartIndex >= 0)
			{
				nIndex = m_nUnitStartIndex + nUnitIndex*m_nUnitIndexGap;
				if(m_pImage->IsPossibilityIndex(nIndex))
				{
					pImage = (CImage *)m_pImage->Clone();
					pImage->SetIndex(nIndex);
					m_listImage.push_front(pImage);
				}
			}

			for(j=0;j<=nMax-i;++j)
			{
				nIndex = m_nStartIndex + strNumber[nMax-(i+j)] - '0';
				if(m_pImage->IsPossibilityIndex(nIndex))
				{
					pImage = (CImage *)m_pImage->Clone();
					pImage->SetIndex(nIndex);
					m_listImage.push_front(pImage);
				}
			}
		}
	}
}

void CImageNumber::CheckNeonsignMoneyDraw(char *strNumber,int &nSize)
{
	CImage *pImage = NULL;
	int nFirstNotZero = 0;
	int nUnitIndex = -2;
	int i,j;
	int nMax = nSize-1;
	int nShowPlace = nMax;
	bool bShowNine = false;
	if(m_nMaxPlace != 0 )
	{
		if(m_nMaxPlace <= nMax)
		{
			bShowNine = true;
		}

		nShowPlace = m_nMaxPlace-1;
	}

	int nIndex = 0;
	for(i=0;i<=nShowPlace;i += 4)
	{
		//단위를 증가 시킨다.
		++nUnitIndex;
		if(i!=0 && m_nUnitStartIndex >= 0)
		{
			nIndex = m_nUnitStartIndex + nUnitIndex*m_nUnitIndexGap;
			if(m_pImage->IsPossibilityIndex(nIndex))
			{
				pImage = (CImage *)m_pImage->Clone();
				pImage->SetIndex(nIndex);
				m_listImage.push_front(pImage);
			}
		}

		//단위에 숫자가 4개 존재하는지를 검사한다.
		//4개가 존재 한다면
		if(i+4 <= nMax)
		{
			//첫번째 0인 아닌 숫자가 나오는 위치를 찾는다.
			nFirstNotZero = -1;
			for(j=3;j>=0;--j)
			{
				if(strNumber[nMax-(i+j)] != '0')
				{
					nFirstNotZero = j;
					break;
				}
			}

			//0이 아닌 숫자가 나오지 않는다면
			//단위만 증가시킨다.
			if(nFirstNotZero == -1)
			{
				for(j=0;j<=3;++j)
				{
					if(i+j > nShowPlace) break;
					nIndex = m_nStartIndex + bShowNine?9:0;
					if(m_pImage->IsPossibilityIndex(nIndex))
					{
						pImage = (CImage *)m_pImage->Clone();
						pImage->SetIndex(nIndex);
						m_listImage.push_front(pImage);
						if((bShowNine == false) && (m_bShowZeroNumber == false)) pImage->SetShow(false);
					}
				}
			}
			else
			{
				for(j=0;j<=3;++j)
				{
					if(i+j > nShowPlace) break;
					nIndex = m_nStartIndex + bShowNine?9:(strNumber[nMax-(i+j)] - '0');
					if(m_pImage->IsPossibilityIndex(nIndex))
					{
						pImage = (CImage *)m_pImage->Clone();
						pImage->SetIndex(nIndex);
						m_listImage.push_front(pImage);
						if((bShowNine == false) && (nFirstNotZero < j) && (m_bShowZeroNumber == false)) pImage->SetShow(false);
					}
				}
			}
		}
		else
		{
			bool bShowFalse = false;
			for(j=0;j<=3;++j)
			{
				if(i+j > nShowPlace) break;
				if((nMax - (i+j)) < m_nStartIndex)
				{
					nIndex = m_nStartIndex;
					bShowFalse = true;
				}
				else
				{
					nIndex = m_nStartIndex + bShowNine?9:(strNumber[nMax-(i+j)] - '0');
					bShowFalse = false;
				}

				if(m_pImage->IsPossibilityIndex(nIndex))
				{
					pImage = (CImage *)m_pImage->Clone();
					pImage->SetIndex(nIndex);
					if(bShowFalse && (m_bShowZeroNumber == false)) pImage->SetShow(false);
					m_listImage.push_front(pImage);
				}
			}
		}
	}
}

void CImageNumber::SetMaxPlace(const int &nMaxPlace)
{
	if(m_nMaxPlace == nMaxPlace) return;
	m_nMaxPlace = nMaxPlace;
	if(m_nNumberType != EIND_MONEY)
		SetRedraw();
}

void CImageNumber::SetCutCount(const int &nCutCount)
{
	if(m_nCutCount == nCutCount) return;
	m_nCutCount = nCutCount;
	if(m_nNumberType == EIND_MONEY)
		SetRedraw();
}

void CImageNumber::SetUnitStartIndex(const int &nUnitStartIndex)
{
	if(m_nUnitStartIndex == nUnitStartIndex) return;
	m_nUnitStartIndex = nUnitStartIndex;
	if(m_nNumberType != EIND_NUMBER)
		SetRedraw();
}

void CImageNumber::SetUnitIndexGap(const int &nUnitIndexGap)
{
	if(m_nUnitIndexGap == nUnitIndexGap) return;
	m_nUnitIndexGap = nUnitIndexGap;
	if(m_nNumberType == EIND_MONEY || m_nNumberType == EIND_NEONSIGN_MONEY)
		SetRedraw();
}

void CImageNumber::SetFirstUnitIndex(const int &nFirstUnitIndex)
{
	if(m_nFirstUnitIndex == nFirstUnitIndex) return;
	m_nFirstUnitIndex = nFirstUnitIndex;
	SetRedraw();
}

void CImageNumber::SetPrefixIndex(const int &nPrefixIndex)
{
	if(m_nPrefixIndex == nPrefixIndex) return;
	m_nPrefixIndex = nPrefixIndex;
	SetRedraw();
}

void CImageNumber::SetPlusIndex(const int &nPlusIndex)
{
	if(m_nPlusIndex == nPlusIndex) return;
	m_nPlusIndex = nPlusIndex;
	SetRedraw();
}

void CImageNumber::SetMinusIndex(const int &nMinusIndex)
{
	if(m_nMinusIndex == nMinusIndex) return;
	m_nMinusIndex = nMinusIndex;
	SetRedraw();
}

void CImageNumber::SetShowZero(const bool &bShowZero)
{
	if(m_bShowZero == bShowZero) return;
	m_bShowZero = bShowZero;
	if(m_i64Number == 0) SetRedraw();
}

void CImageNumber::SetShowZeroNumber(const bool &bShowZeroNumber)
{
	if(m_bShowZeroNumber == bShowZeroNumber) return;
	m_bShowZeroNumber = bShowZeroNumber;
	if(m_nNumberType == EIND_NEONSIGN_MONEY) SetRedraw();
}

void CImageNumber::SetShowZeroPlus(const bool &bShowZeroPlus)
{
	if(m_bShowZeroPlus == bShowZeroPlus) return;
	m_bShowZeroPlus = bShowZeroPlus;
	if(m_i64Number == 0) SetRedraw();
}

void CImageNumber::SetFormat(const UINT &uFormat)
{
	if(m_uFormat == uFormat) return;
	m_uFormat = uFormat;
	SetRedraw();
}

void CImageNumber::SetNumberType(const ENUM_IMAGENUMBER_DRAWTYPE &nType)
{
	if(m_nNumberType == nType) return;
	m_nNumberType = nType;
	SetRedraw();
}
} //namespace LSY