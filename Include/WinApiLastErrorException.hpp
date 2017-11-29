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

class LastErrorException : public std::runtime_error
{
public:
	LastErrorException(const std::string& p_description);
};

}
