#include "StdAfx.h"
#include "VIPJackpotInfoDlg.h"
#include "../../WidgetID.h"

CVIPJackpotInfoDlg::CVIPJackpotInfoDlg()
{

}

CVIPJackpotInfoDlg::~CVIPJackpotInfoDlg()
{

}

void CVIPJackpotInfoDlg::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) 
		return;

	m_mapEvent.clear();

	std::string strName = pObject->GetName();
	if (strName.find("Event"))
	{
		std::vector<std::string> vecStr = NMBASE::UTIL::SplitString(strName,'_');

		if (false == vecStr.empty())
		{
			std::vector<std::string>::reverse_iterator rvit = vecStr.rbegin();
			std::string strKey = *rvit;
			int nNum = atoi(strKey.c_str());
			if (nNum >= 0)
			{
				LSY::CButton* pObj = static_cast<LSY::CButton*>(pObject);
				bool bRet = m_mapEvent.insert(std::map<int, LSY::CButton*>::value_type(nNum ,pObj )).second;
				if (false == bRet)
					m_mapEvent[nNum] = pObj; 
			}
		}
	}

	__super::OnCreateChild(pObject);
}
