//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <string>
#include <cstring>
#include <Windows.h>

namespace WinApi
{
namespace
{

template<std::size_t MaxSize>
std::string wcharToString(const wchar_t* p_string)
{
	char buffer[MaxSize];
	wcstombs(buffer, p_string, MaxSize - 1);
	return buffer;
}

inline std::string charToString(const char* p_string)
{
	return p_string;
}

}

template<std::size_t MaxSize>
std::string arrayToString(const TCHAR (&p_string)[MaxSize])
{
	return pointerToString<MaxSize>(p_string);
}

template<std::size_t MaxSize>
std::string pointerToString(const TCHAR* p_string)
{
#ifdef  UNICODE
	return wcharToString<MaxSize>(p_string);
#else
	return charToString(p_string);
#endif
}

template<std::size_t MaxSize>
void stringToArray(const std::string& p_inStr, TCHAR(&p_outStr)[MaxSize])
{
	return stringToPoiner(p_inStr, p_outStr, MaxSize);
}

void stringToPoiner(const std::string& p_inStr, TCHAR* p_outStr, std::size_t p_maxSize);

}
