//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <boost/lexical_cast.hpp>
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

LastError::LastError()
	: errorCode(GetLastError()), errorDescription(NULL)
{
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorDescription,
		0,
		NULL);
}

LastError::~LastError()
{
	if (errorDescription != NULL)
		LocalFree(errorDescription);
}

std::string LastError::describe() const
{
	std::string message = "Error code: " + boost::lexical_cast<std::string>(errorCode);
	if (errorDescription == NULL)
		return message;
	
	char descriptionBuffer[1024];
	wcstombs(descriptionBuffer, errorDescription, 1024 - 1);
	return (message + ". Error message: " + descriptionBuffer);
}

LastErrorException::LastErrorException(const std::string& p_description)
	: std::runtime_error(p_description + ". " + LastError().describe())
{}

}
