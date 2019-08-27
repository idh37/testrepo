#include "stdafx.h"
#include "TextNumber.h"
#include "../ObjectMan.h"
#include "../../tinyxml/tinyxml.h"
#include "../../Utility/MyMime64.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CTextNumber::CTextNumber(void)
:	m_i64Number(false),
	m_nMaxPlace(0),
	m_nCutCount(0),
	m_strFirstUnit(""),
	m_strPrefix(""),
	m_bUsePlus(false),
	m_bUseMinus(false),
	m_bShowZero(true),
	m_bShowZeroNumber(false),
	m_nNumberType(EIND_NUMBER),
	m_bShowZeroPlus(false)
{
}

CTextNumber::~CTextNumber()
{
}

CObjectBase *CTextNumber::NewObject(void)
{
	return new CTextNumber;
}

CObjectBase *CTextNumber::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CTextNumber *pText			= (CTextNumber *)pObject;
	pText->m_i64Number			= m_i64Number;
	pText->m_nMaxPlace			= m_nMaxPlace;
	pText->m_nCutCount			= m_nCutCount;
	pText->m_strFirstUnit		= m_strFirstUnit;
	pText->m_strPrefix			= m_strPrefix;
	pText->m_bUsePlus			= m_bUsePlus;
	pText->m_bUseMinus			= m_bUseMinus;
	pText->m_bShowZero			= m_bShowZero;
	pText->m_bShowZeroNumber	= m_bShowZeroNumber;
	pText->m_nNumberType		= m_nNumberType;
	pText->m_listUnit			= m_listUnit;
	pText->m_bShowZeroPlus		= m_bShowZeroPlus;
	pText->SetRedraw();
	return pObject;
}

size_t CTextNumber::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(INT64);						//m_i64Number
	nRet += sizeof(int);						//m_nMaxPlace
	nRet += sizeof(int);						//m_nCutCount
	nRet += sizeof(size_t);						//m_strFirstUnit의 크기
	nRet += m_strFirstUnit.size();				//m_strFirstUnit
	nRet += sizeof(size_t);						//m_strPrefix의 크기
	nRet += m_strPrefix.size();					//m_strPrefix
	nRet += sizeof(bool);						//m_bUsePlus
	nRet += sizeof(bool);						//m_bUseMinus
	nRet += sizeof(bool);						//m_bShowZero
	nRet += sizeof(bool);						//m_bShowZeroNumber
	nRet += sizeof(ENUM_IMAGENUMBER_DRAWTYPE);	//m_nNumberType
	nRet += sizeof(bool);						//m_bShowZeroPlus
	nRet += sizeof(size_t);						//m_listUint의 크기

	size_t nSize = m_listUnit.size();
	for(int i=0;i<(int)nSize;++i)
	{
		nRet += sizeof(size_t);					//Unit의 크기
		nRet += m_listUnit[i].size();			//Unit
	}
	return nRet;
}

size_t CTextNumber::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_i64Number, sizeof(INT64));
	nRet += sizeof(INT64);						//m_i64Number
	memcpy(pDest+nRet, &m_nMaxPlace, sizeof(int));
	nRet += sizeof(int);						//m_nMaxPlace
	memcpy(pDest+nRet, &m_nCutCount, sizeof(int));
	nRet += sizeof(int);						//m_nCutCount

	size_t nSize = m_strFirstUnit.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);						//m_strFirstUnit의 크기
	if((int)nSize != 0)
	{
		memcpy(pDest+nRet, m_strFirstUnit.c_str(), nSize);
		nRet += nSize;								//m_strFirstUnit
	}

	nSize = m_strPrefix.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);						//m_strPrefix의 크기
	if((int)nSize != 0)
	{
		memcpy(pDest+nRet, m_strPrefix.c_str(), nSize);
		nRet += nSize;							//m_strPrefix
	}

	memcpy(pDest+nRet, &m_bUsePlus, sizeof(bool));
	nRet += sizeof(bool);						//m_bUsePlus
	memcpy(pDest+nRet, &m_bUseMinus, sizeof(bool));
	nRet += sizeof(bool);						//m_bUseMinus
	memcpy(pDest+nRet, &m_bShowZero, sizeof(bool));
	nRet += sizeof(bool);						//m_bShowZero
	memcpy(pDest+nRet, &m_bShowZeroNumber, sizeof(bool));
	nRet += sizeof(bool);						//m_bShowZeroNumber
	memcpy(pDest+nRet, &m_nNumberType, sizeof(ENUM_IMAGENUMBER_DRAWTYPE));
	nRet += sizeof(ENUM_IMAGENUMBER_DRAWTYPE);	//m_nNumberType
	memcpy(pDest+nRet, &m_bShowZeroPlus, sizeof(bool));
	nRet += sizeof(bool);						//m_bShowZeroPlus

	nSize = m_listUnit.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);						//m_listUint의 크기

	size_t nSize2;
	for(int i=0;i<(int)nSize;++i)
	{
		nSize2 = m_listUnit[i].size();
		memcpy(pDest+nRet, &nSize2, sizeof(size_t));
		nRet += sizeof(size_t);					//Unit의 크기
		if((int)nSize2 != 0)
		{
			memcpy(pDest+nRet, m_listUnit[i].c_str(), nSize2);
			nRet += nSize2;						//Unit
		}
	}
	return nRet;
}

size_t CTextNumber::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_i64Number, pDest+nRet, sizeof(INT64));
	nRet += sizeof(INT64);						//m_i64Number
	memcpy(&m_nMaxPlace, pDest+nRet, sizeof(int));
	nRet += sizeof(int);						//m_nMaxPlace
	memcpy(&m_nCutCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);						//m_nCutCount

	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);						//m_strFirstUnit의 크기
	if((int)nSize != 0)
	{
		GetPointerToString(pDest+nRet, nSize, m_strFirstUnit);
		nRet += nSize;								//m_strFirstUnit
	}

	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);						//m_strPrefix의 크기
	if((int)nSize != 0)
	{
		GetPointerToString(pDest+nRet, nSize, m_strPrefix);
		nRet += nSize;							//m_strPrefix
	}

	memcpy(&m_bUsePlus, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);						//m_bUsePlus
	memcpy(&m_bUseMinus, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);						//m_bUseMinus
	memcpy(&m_bShowZero, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);						//m_bShowZero
	memcpy(&m_bShowZeroNumber, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);						//m_bShowZeroNumber
	memcpy(&m_nNumberType, pDest+nRet, sizeof(ENUM_IMAGENUMBER_DRAWTYPE));
	nRet += sizeof(ENUM_IMAGENUMBER_DRAWTYPE);	//m_nNumberType
	memcpy(&m_bShowZeroPlus, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);						//m_bShowZeroPlus

	memcpy(&nSize, pDest+nRet,sizeof(size_t));
	nRet += sizeof(size_t);						//m_listUint의 크기

	m_listUnit.resize(nSize);
	size_t nSize2;
	std::string strUnit;
	for(int i=0;i<(int)nSize;++i)
	{
		memcpy(&nSize2, pDest+nRet, sizeof(size_t));
		nRet += sizeof(size_t);					//Unit의 크기
		if((int)nSize2 != 0)
		{
			GetPointerToString(pDest+nRet, nSize2, strUnit);
			nRet += nSize2;						//Unit
		}
		else strUnit = "";

		m_listUnit[i] = strUnit;
	}
	SetRedraw();

	if(IsTool()) SetMessage(true);

	return nRet;
}

bool CTextNumber::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	char strTemp[256];
	_i64toa_s(m_i64Number, strTemp, 256, 10);

	CMyMime64 myMime;

	TiXmlElement xmlNumber("number");
	xmlNumber.SetAttribute("number", strTemp);
	xmlNumber.SetAttribute("cut", m_nCutCount);
	xmlNumber.SetAttribute("maxplace", m_nMaxPlace);
	myMime.SetString(m_strFirstUnit,FALSE,TRUE);
	xmlNumber.SetAttribute("firstunit", myMime.GetString());
	myMime.SetString(m_strPrefix,FALSE,TRUE);
	xmlNumber.SetAttribute("prefix", myMime.GetString());
	xmlNumber.SetAttribute("showzero", m_bShowZero?1:0);
	xmlNumber.SetAttribute("showzeronumber", m_bShowZeroNumber?1:0);
	xmlNumber.SetAttribute("showzeroplus", m_bShowZeroPlus?1:0);
	xmlNumber.SetAttribute("type", GetImageNumberDrawTypeToString(m_nNumberType).c_str());
	xmlNumber.SetAttribute("plus", m_bUsePlus?1:0);
	xmlNumber.SetAttribute("minus", m_bUseMinus?1:0);

	std::string strUnit = GetUnit();
	myMime.SetString(strUnit,FALSE,TRUE);
	xmlNumber.SetAttribute("unit", myMime.GetString());

	pXmlObject->InsertEndChild(xmlNumber);

	return true;
}

bool CTextNumber::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	CMyMime64 myMime;

	TiXmlElement *xmlNumber = pXmlObject->FirstChildElement("number");
	GetAttribute(xmlNumber, "number", m_i64Number);
	GetAttribute(xmlNumber, "cut", m_nCutCount);
	GetAttribute(xmlNumber, "maxplace", m_nMaxPlace);
	GetAttributeMime64(xmlNumber, "firstunit", m_strFirstUnit);
	GetAttributeMime64(xmlNumber, "prefix", m_strPrefix);
	GetAttribute(xmlNumber, "showzero", m_bShowZero);
	GetAttribute(xmlNumber, "showzeronumber", m_bShowZeroNumber);
	if(xmlNumber->Attribute("showzeroplus")) GetAttribute(xmlNumber, "showzeroplus", m_bShowZeroPlus);
	GetAttribute(xmlNumber, "type", strTemp);
	m_nNumberType = GetStringToImageNumberDrawType(strTemp);
	GetAttribute(xmlNumber, "plus", m_bUsePlus);
	GetAttribute(xmlNumber, "minus", m_bUseMinus);
	GetAttributeMime64(xmlNumber, "unit", strTemp);

	SetUnit(strTemp);
	SetRedraw();

	if(IsTool()) SetMessage(true);

	return true;
}

const std::string CTextNumber::GetUnit(void)
{
	std::string strUnit;
	int nSize = (int)m_listUnit.size();
	for(int i=0;i<nSize;++i)
	{
		strUnit += m_listUnit[i];
		if(i != (nSize - 1)) strUnit += "/";
	}
	
	return strUnit;
}

void CTextNumber::SetUnit(const std::string &strUnit)
{
	if(GetUnit() == strUnit) return;

	if(strUnit == "")
	{
		m_listUnit.clear();
		SetRedraw();
		return;
	}

	std::list<std::string> listTemp;
	std::string strTemp;

	std::string::size_type nFirst = 0;
	std::string::size_type nLast = strUnit.find('/');

	while(nLast != std::string::npos)
	{
		strTemp.clear();

		if(nFirst != nLast)
		{
			strTemp.append(strUnit.c_str()+nFirst, nLast-nFirst);
		}

		listTemp.push_back(strTemp);

		nFirst = nLast+1;
		nLast = strUnit.find('/', nFirst);
	}

	strTemp.clear();
	strTemp.append(strUnit.c_str()+nFirst, strUnit.size()-nFirst);
	listTemp.push_back(strTemp);

	m_listUnit.resize(listTemp.size());

	std::list<std::string>::iterator start = listTemp.begin();
	std::list<std::string>::iterator end = listTemp.end();

	for(int i=0;start != end;++start,++i)
	{
		m_listUnit[i] = *start;
	}
	if(m_nNumberType != EIND_NUMBER) SetRedraw();
}

void CTextNumber::SetMaxPlace(const int &nMaxPlace)
{
	if(m_nMaxPlace == nMaxPlace) return;
	m_nMaxPlace = nMaxPlace;
	if(m_nNumberType != EIND_MONEY)	SetRedraw();
}

void CTextNumber::SetCutCount(const int &nCutCount)
{
	if(m_nCutCount == nCutCount) return;
	m_nCutCount = nCutCount;
	if(m_nNumberType == EIND_MONEY)	SetRedraw();
}

void CTextNumber::SetFirstUnit(const std::string &strFirstUnit)
{
	if(m_strFirstUnit == strFirstUnit) return;
	m_strFirstUnit = strFirstUnit;
	SetRedraw();
}

void CTextNumber::SetPrefix(const std::string &strPrefix)
{
	if(m_strPrefix == strPrefix) return;
	m_strPrefix = strPrefix;
	SetRedraw();
}

void CTextNumber::SetUsePlus(const bool &bUse)
{
	if(m_bUsePlus == bUse) return;
	m_bUsePlus = bUse;
	SetRedraw();
}

void CTextNumber::SetUseMinus(const bool &bUse)
{
	if(m_bUseMinus == bUse) return;
	m_bUseMinus = bUse;
	SetRedraw();
}

void CTextNumber::SetShowZero(const bool &bShowZero)
{
	if(m_bShowZero == bShowZero) return;
	m_bShowZero = bShowZero;
	if(m_i64Number == 0) SetRedraw();
}

void CTextNumber::SetShowZeroNumber(const bool &bShowZeroNumber)
{
	if(m_bShowZeroNumber == bShowZeroNumber) return;
	m_bShowZeroNumber = bShowZeroNumber;
	if(m_nNumberType == EIND_NEONSIGN_MONEY) SetRedraw();
}

void CTextNumber::SetShowZeroPlus(const bool &bShowZeroPlus)
{
	if(m_bShowZeroPlus == bShowZeroPlus) return;
	m_bShowZeroPlus = bShowZeroPlus;
	if(m_i64Number == 0) SetRedraw();
}

void CTextNumber::SetNumberType(const ENUM_IMAGENUMBER_DRAWTYPE &nType)
{
	if(m_nNumberType == nType) return;
	m_nNumberType = nType;
	SetRedraw();
}

void CTextNumber::SetNumber(const INT64 &i64Number)
{
	if(i64Number == m_i64Number) return;
	m_i64Number = i64Number;
	SetRedraw();
}

int CTextNumber::CheckRedraw(void)
{
	INT64 i64Number = m_i64Number;
	bool bMinus = false;
	if(i64Number < 0)
	{
		if(m_bUseMinus)
		{
			i64Number *= -1;
			
		}
		else
		{
			i64Number = 0;
		}
		bMinus = true;
	}

	std::string strText;
	if(i64Number == 0 && (m_bShowZero == false))
	{
	}
	else
	{
		char strTemp[256];
		_i64toa_s(i64Number, strTemp, 256, 10);

		int nSize = (int)strlen(strTemp);
		strText.insert(0, m_strFirstUnit);

		switch(m_nNumberType)
		{
		case EIND_NUMBER:
			CheckNumberDraw(strText, strTemp, nSize);
			break;
		case EIND_COMMA_MONEY:
			CheckCommaMoneyDraw(strText, strTemp, nSize);
			break;
		case EIND_MONEY:
			CheckMoneyDraw(strText, strTemp, nSize);
			break;
		case EIND_NEONSIGN_MONEY:
			CheckNeonsignMoneyDraw(strText, strTemp, nSize);
			break;
		}

		if(bMinus && m_bUseMinus)
		{
			strText.insert(0, "-");
		}
		else if(i64Number == 0)
		{
			if(m_bShowZeroPlus)
			{
				strText.insert(0, "+");
			}
		}
		else if(!bMinus && m_bUsePlus)
		{
			strText.insert(0, "+");
		}

		strText.insert(0, m_strPrefix);
	}

	SetText(strText);
	return __super::CheckRedraw();
}

void CTextNumber::CheckNumberDraw(std::string &strText, char *strNumber,int &nSize)
{
	int nMax = nSize-1;
	int nIndex = 0;
	if(m_nMaxPlace != 0 && nMax > m_nMaxPlace)
	{
		strText.insert(0, "9");
	}
	else
	{
		for(int i=0;i<=nMax;++i)
		{
			strText.insert(0, 1, strNumber[nMax-i]);
		}
	}
}

void CTextNumber::CheckCommaMoneyDraw(std::string &strText, char *strNumber,int &nSize)
{
	int nMax = nSize-1;
	int nEnd = nMax;
	int nUnitSize = m_listUnit.size();
	if(m_nMaxPlace != 0 && nMax > m_nMaxPlace)
	{
		for(int i=0;i<=m_nMaxPlace;++i)
		{
			if(i!=0 && i%3 == 0 && nUnitSize > 0)
			{
				strText.insert(0, m_listUnit[0]);
			}

			strText.insert(0,"9");
		}
	}
	else
	{
		int nIndex = 0;
		for(int i=0;i<=nMax;++i)
		{
			if(i!=0 && i%3 == 0 && nUnitSize > 0)
			{
				strText.insert(0, m_listUnit[0]);
			}

			strText.insert(0,1,strNumber[nMax-i]);
		}
	}
}

void CTextNumber::CheckMoneyDraw(std::string &strText, char *strNumber,int &nSize)
{
	int nFirstNotZero = 0;
	int nUnitIndex = -2;
	int i,j;
	int nMax = nSize-1;
	int nStart = 0;
	int nUnitSize = m_listUnit.size();

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
				if(i!=0 && (nUnitSize > 0) && (nUnitIndex < nUnitSize))
				{
					strText.insert(0, m_listUnit[nUnitIndex]);
				}

				for(j=0;j<=nFirstNotZero;++j)
				{
					strText.insert(0, 1, strNumber[nMax-(i+j)]);
				}
			}
		}
		else
		{
			int nIndex = 0;
			if(i!=0 && (nUnitSize > 0) && (nUnitIndex < nUnitSize))
			{
				strText.insert(0, m_listUnit[nUnitIndex]);
			}

			for(j=0;j<=nMax-i;++j)
			{
				strText.insert(0, 1, strNumber[nMax-(i+j)]);
			}
		}
	}
}

void CTextNumber::CheckNeonsignMoneyDraw(std::string &strText, char *strNumber,int &nSize)
{
	int nFirstNotZero = 0;
	int nUnitIndex = -2;
	int i,j;
	int nMax = nSize-1;
	int nShowPlace = nMax;
	bool bShowNine = false;
	int nUnitSize = m_listUnit.size();

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
		if(i!=0 && (nUnitSize > 0) && (nUnitIndex < nUnitSize))
		{
			strText.insert(0, m_listUnit[nUnitIndex]);
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
					if((bShowNine == false) && (m_bShowZeroNumber == false)) strText.insert(0, " ");
					else strText.insert(0, bShowNine?"9":"0");
				}
			}
			else
			{
				for(j=0;j<=3;++j)
				{
					if(i+j > nShowPlace) break;
					if((bShowNine == false) && (nFirstNotZero < j) && (m_bShowZeroNumber == false)) strText.insert(0, " ");
					else if(bShowNine) strText.insert(0, "9");
					else strText.insert(0, 1, strNumber[nMax-(i+j)]);
				}
			}
		}
		else
		{
			bool bShowFalse = false;
			std::string strAdd;
			for(j=0;j<=3;++j)
			{
				if(i+j > nShowPlace) break;
				if((nMax - (i+j)) < 0)
				{
					strAdd = "0";
					bShowFalse = true;
				}
				else
				{
					if(bShowNine) strAdd = "9";
					else strAdd = strNumber[nMax-(i+j)];
					bShowFalse = false;
				}

				if(bShowFalse && (m_bShowZeroNumber == false)) strText.insert(0, " ");
				else strText.insert(0, strAdd);
			}
		}
	}
}
} //namespace LSY