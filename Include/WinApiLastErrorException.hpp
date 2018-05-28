//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <Windows.h>
#include <stdexcept>

namespace WinApi
{

class LastError
{
public:
	LastError();
	~LastError();
	
	std::string describe() const;
private:
	DWORD errorCode;
	LPTSTR errorDescription;
};

class Exception : public std::runtime_error
{
public:
	Exception(const std::string& p_what);
};

class LastErrorException : public Exception
{
public:
	LastErrorException(const std::string& p_description);
};

}
