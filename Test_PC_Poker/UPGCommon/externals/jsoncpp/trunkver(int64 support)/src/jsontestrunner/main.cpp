// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

/* This executable is used for testing parser/writer using real JSON files.
 */


#include <json/json.h>
#include <algorithm> // sort
#include <stdio.h>
#include <map>

#if defined(_MSC_VER)  &&  _MSC_VER >= 1310
# pragma warning( disable: 4996 )     // disable fopen deprecation warning
#endif


int main( int argc, const char *argv[] )
{
	Json::Value root;
	//Json::Value encoding;
	root["encoding"] = 1234567890123;
	//Json::Value plugins;
	//plugins.append("python");
	//plugins.append("c++");
	//plugins.append("ruby");
	//root["plug-ins"] = plugins;
	//Json::Value indent;
	//indent["length"] = 3;
	//indent["use_space"] = true;
	//root["indent"] = indent;

	Json::FastWriter writer1;
	//Json::StyledWriter writer2;
	std::string outputConfig1 = writer1.write( root );
	//std::string outputConfig2 = writer2.write( root );


	Json::Reader reader;
	Json::Value root2;
	reader.parse(outputConfig1, root2);
	bool isInt = root2["encoding"].isInt();
	bool isUInt = root2["encoding"].isUInt();

	__int64 llValue = root2["encoding"].asInt64();
	unsigned __int64 ullValue = root2["encoding"].asUInt64();
   return 0;
}

