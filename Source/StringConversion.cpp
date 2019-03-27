//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "StringConversion.hpp"

namespace WinApi
{
namespace
{

std::size_t stringToWchar(const std::string& p_in, wchar_t* p_out, std::size_t p_maxLen)
{
	return mbstowcs(p_out, p_in.c_str(), p_maxLen - 1);
}
std::size_t stringToChar(const std::string& p_in, char*p_out, std::size_t p_maxLen)
{
	strncpy(p_out, p_in.c_str(), p_maxLen -  1);
	return strlen(p_out);
}

}

std::size_t stringToPoiner(const std::string& p_inStr, TCHAR* p_outStr, std::size_t p_maxSize)
{
#ifdef  UNICODE
	return stringToWchar(p_inStr, p_outStr, p_maxSize);
#else
	return stringToChar(p_inStr, p_outStr, p_maxSize);
#endif
}

}
