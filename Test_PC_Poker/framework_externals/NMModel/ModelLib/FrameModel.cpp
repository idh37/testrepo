#include "stdafx.h"
#include "FrameModel.h"

namespace NMBASE
{
	namespace MODEL
	{

		CFrameModel::CFrameModel()
		{
		}

		CFrameModel::~CFrameModel()
		{
		}

		BOOL CFrameModel::OnAttached()
		{

			return TRUE;
		}

		BOOL CFrameModel::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}

	}//namespace MODEL

}//namespace NMBASE
