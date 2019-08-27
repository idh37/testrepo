/**    
@file    ChannelModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   채널 데이터 처리
*/
#pragma once
#include "ModelDefine.h"

namespace NMBASE
{
	namespace MODEL
	{

		class NMMODEL_CLASS CChannelModel : public IComponent
		{
		public:
			CChannelModel();
			virtual ~CChannelModel();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:
			virtual LRESULT	 ChannelInfo(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< 채널 정보

		private:

		};

	}//namespace MODEL
}//namespace NMBASE
