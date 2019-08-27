/**    
@file    DataDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Data Import / Export Define
*/
#pragma once


#ifndef NMDATA_API

	#ifdef NMDATA_EXPORTS
		#define NMDATA_API		__declspec(dllexport )
	#else
		#define NMDATA_API		__declspec(dllimport )
	#endif 

#endif

#ifndef NMDATA_CLASS 
	#define NMDATA_CLASS NMDATA_API
#endif

// /
// /< CChannelData
// #define CHANNELIDLEN        30
// #define GOSERVICELEN        30
// #define IPADDRLEN           50
// #define CHANNELNAMELEN      50
// 
// //
// /< ChannelstGroupData.h
// #define SEXLEN              1
// #define GROUPIDLEN			10
// #define GROUPNAMELEN		50