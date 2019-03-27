//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "WinApiProcess.hpp"
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

Pipe::Pipe()
{
	SECURITY_ATTRIBUTES inheritable;
	inheritable.nLength = sizeof(SECURITY_ATTRIBUTES);
	inheritable.bInheritHandle = TRUE;
	inheritable.lpSecurityDescriptor = NULL;

	if(CreatePipe(&read, &write, &inheritable, 0) == 0)
		throw LastErrorException("Failed to create pipe");
}

void Pipe::readNotIneritable()
{
	if(SetHandleInformation(read, HANDLE_FLAG_INHERIT, 0) == 0 )
		throw LastErrorException("Failed to set pipe read not iheritable");
}

void Pipe::writeNotIneritable()
{
	if(SetHandleInformation(write, HANDLE_FLAG_INHERIT, 0) == 0 )
		throw LastErrorException("Failed to set pipe write not iheritable");
}

void Pipe::closeWrite()
{
	CloseHandle(write);
}

void Pipe::put(const std::string& p_str)
{
	//TODO:
}

bool Pipe::hasDataToRead()
{
	DWORD leftData = 0;
	bool isPeekOk = PeekNamedPipe(read, NULL, NULL, NULL, &leftData, NULL);
	return isPeekOk && leftData > 0;
}

std::string Pipe::get()
{
	DWORD readLength = 0;
	char buff[1024];
	bool isReadOk = false;
	std::string output;
	do
	{
		isReadOk = ReadFile(read, buff, 1024, &readLength, NULL);
		if (isReadOk && readLength > 0)
			output.append(buff, readLength);
	}
	while (isReadOk && readLength !=0 && hasDataToRead());
	return output;
}

ChildOut::ChildOut()
	: Pipe()
{
	readNotIneritable();
}

void ChildOut::apply(STARTUPINFO& p_startup)
{
	p_startup.hStdOutput = write;
	p_startup.hStdError = write;
	p_startup.dwFlags |= STARTF_USESTDHANDLES;
}

void ChildOut::readBuffer()
{
	output += get();
}

std::string ChildOut::getOutput()
{
	return output;
}

ChildIn::ChildIn()
	: Pipe()
{
	writeNotIneritable();
}

void ChildIn::apply(STARTUPINFO& p_startup)
{
	p_startup.hStdInput = read;
	p_startup.dwFlags |= STARTF_USESTDHANDLES;
}

Process::Process(const std::string& p_command)
	: command(p_command)
{
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
}

std::string Process::execute()
{
	start();
	return wait();
}

void Process::start()
{
	TCHAR cmd[1024] = TEXT("");
	mbstowcs(cmd, command.c_str(), 1024 - 1);

	processOut.apply(startupInfo);
	prosessIn.apply(startupInfo);

	if(CreateProcess(
		NULL,	       // No module name (use command line)
		cmd,           // Command line
		NULL,          // Process handle not inheritable
		NULL,          // Thread handle not inheritable
		TRUE,          // Set handle inheritance to TRUE
		0,             // No creation flags
		NULL,          // Use parent's environment block
		NULL,          // Use parent's starting directory 
		&startupInfo,
		&processInfo) == 0)
	{
		throw LastErrorException("Failed to execute: " + command);
	}
	prosessIn.closeWrite();
}

std::string Process::wait()
{
	processOut.readBuffer();
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	processOut.closeWrite();
	processOut.readBuffer();
	return processOut.getOutput();
}

}
