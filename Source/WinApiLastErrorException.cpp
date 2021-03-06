//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <boost/lexical_cast.hpp>
#include "WinApiLastErrorException.hpp"
#include "StringConversion.hpp"

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
	
	return (message + ". Error message: " + pointerToString<1024>(errorDescription));
}

Exception::Exception(const std::string& p_what)
	: std::runtime_error(p_what)
{}

LastErrorException::LastErrorException(const std::string& p_description)
	: Exception(p_description + ". " + LastError().describe())
{}

}
