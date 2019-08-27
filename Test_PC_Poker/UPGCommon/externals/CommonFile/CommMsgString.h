#ifndef __COMMMSGSTRING_H__ 
#define __COMMMSGSTRING_H__ 



#define CASEENUMSTRING(enums) case enums: return #enums;

std::string ToStringCommMsgIDHeader(int nSignal);


#endif // #define __COMMMSGSTRING_H__ 