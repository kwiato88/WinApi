//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <string>
#include "WinApiTypes.hpp"

namespace WinApi
{

class Pipe
{
public:
	Pipe();
	void readNotIneritable();
	void writeNotIneritable();
	void put(const std::string& p_str);
	std::string get();

protected:
	HANDLE read;
	HANDLE write;
};

class ChildOut : public Pipe
{
public:
	ChildOut();
	void apply(STARTUPINFO& p_startup);
};

class ChildIn : public Pipe
{
public:
	ChildIn();
	void apply(STARTUPINFO& p_startup);
};

class Process
{
public:
	Process(const std::string& p_command);

	std::string execute();
	void start();
	std::string wait();
private:
	std::string command;
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;
	ChildOut processOut;
}; 

}
