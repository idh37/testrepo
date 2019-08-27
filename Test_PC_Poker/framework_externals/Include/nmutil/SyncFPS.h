#pragma once

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		class NMUTIL_CLASS CSyncFPS  
		{
		public:
			int FPS;
			UINT PreTime;
			UINT NowTime;
			double ElapseTime;
			double ProcessTime;

			CSyncFPS();
			virtual ~CSyncFPS();

			void Init(int fps);
			int FrameSync(BOOL bWait = TRUE);
		};

	}//namespace UTIL

}//namespace NMBASE