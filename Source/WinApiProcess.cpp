//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "WinApiProcess.hpp"
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

Process::Process(const std::string& p_command)
	: command(p_command)
{
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));
}

std::string Process::execute()
{
	start();
	return wait();
}

void Process::start()
{
	TCHAR cmd[_MAX_PATH] = TEXT("");
	mbstowcs(cmd, command.c_str(), _MAX_PATH - 1);

	if(CreateProcess(
		NULL,	       // No module name (use command line)
		cmd,           // Command line
		NULL,          // Process handle not inheritable
		NULL,          // Thread handle not inheritable
		FALSE,         // Set handle inheritance to FALSE
		0,             // No creation flags
		NULL,          // Use parent's environment block
		NULL,          // Use parent's starting directory 
		&startupInfo,  // Pointer to STARTUPINFO structure
		&processInfo) == 0)
	{
		throw LastErrorException("Failed to execute: " + command);
	}
}

std::string Process::wait()
{
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return "";
}

}
