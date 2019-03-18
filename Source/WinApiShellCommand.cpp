//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "WinApiShellCommand.hpp"
#include "WinApiTypes.hpp"
#include "WinApiLastErrorException.hpp"
#include <shellapi.h>
#include <boost/lexical_cast.hpp> 

namespace WinApi
{

ShellCommand::ShellCommand(const std::string& p_path, const std::string& p_parameters)
	: path(p_path), parameters(p_parameters)
{}

void ShellCommand::execute()
{
	int code = (int)ShellExecuteA(
		NULL, // hwnd
		"open", // lpOperation
		(path.empty() ? NULL : path.c_str()), // lpFile
		(parameters.empty() ? NULL : parameters.c_str()), // lpParameters
		NULL, // lpDirectory
		SW_HIDE //nShowCmd
	);
	if (code <= 32)
		throw Exception(
			std::string("Failed to execute: ") + path + " with param: " + parameters
			+ ". Error code: " + boost::lexical_cast<std::string>(code));
}

}
