#include <json/json.h>
#include <stdio.h>
#include <algorithm>

#if defined(_MSC_VER)  &&  _MSC_VER >= 1310
# pragma warning( disable: 4996 )     // disable fopen deprecation warning
#endif


int main( int argc, const char *argv[] )
{
	Json::Value root;
	Json::Value encoding;
	//root["1_encoding"] = "UTF-8";
	Json::Value valueRoot = root["1_encoding"].append("UTF-8");
	std::string str = "1_en";
	root[str] = valueRoot;
	root["1_encoding"].append("asdasd");

	Json::Value plugins;
	plugins.append("python");
	plugins.append("c++");
	plugins.append("ruby");
	root["2_plug-ins"] = plugins;

	root["2_plug-ins"].append("1test_app");

	Json::Value indent;
	indent["3length"] = 3;
	indent["4use_space"] = true;
	root["indent"] = indent;

	Json::FastWriter writer1;
	Json::StyledWriter writer2;
	std::string outputConfig1 = writer1.write( root );
	std::string outputConfig2 = writer2.write( root );

	printf("%s\n", outputConfig1.c_str());
	printf("%s\n", outputConfig2.c_str());
  
	std::replace(outputConfig2.begin(), outputConfig2.end(), '\r', ' ');
	std::replace(outputConfig2.begin(), outputConfig2.end(), '\n', '\n');

	printf("%s\n", outputConfig2.c_str());

   return 0;
}