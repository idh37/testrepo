/**    
@file    FrameModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   MainFrame ������ ó��
*/
#pragma once
#include "ModelDefine.h"

namespace NMBASE
{
	namespace MODEL
	{

		class NMMODEL_CLASS CFrameModel : public IComponent
		{
		public:
			CFrameModel();
			virtual ~CFrameModel();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:

		private:

		};

	}//namespace MODEL
}//namespace NMBASE
