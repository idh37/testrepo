#include "stdafx.h"
#include "ChannelModel.h"

namespace NMBASE
{
	namespace MODEL
	{

		CChannelModel::CChannelModel()
		{

		}


		CChannelModel::~CChannelModel()
		{

		}

		BOOL CChannelModel::OnAttached()
		{
			//UTIL::MM().AddNetMsgH(MSG_GAME_ALLCHANNELINFO,	Fnt(this, &CChannelModel::ChannelInfo));	

			return TRUE;
		}

		BOOL CChannelModel::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}

		//
		//LRESULT CChannelModel::ChannelInfo(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//
		//	//�����Ӵ� ���� ���� �޾Ƽ� ����
		//	CSV_ROOMOPTIONINFO msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//	
		//	int nTotalCount = *msg.CreateOptionNum;
		//
		//	// ��ȯ����
		//	/// ä�� ������ �߰��Ѵ�.
		//	//CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
		//	//ASSERT(pDataRoomOptionList);
		//	//pDataRoomOptionList->Clear();			
		//	//// 
		//	//for(int i = 0; i < nTotalCount; ++i)
		//	//{
		//	//	pDataRoomOptionList->AddData(i, &msg.pCreateOptionList[i]);
		//	//}
		//
		//	//// Frame�� View�ʿ� ���� ������ ������.
		//	//UTIL::MM().Call(MSG_VIEW_ALLCHANNELINFO, 0, pDataRoomOptionList);	
		//
		//
		//	return TRUE;
		//}

	}//namespace MODEL

}//namespace NMBASE
