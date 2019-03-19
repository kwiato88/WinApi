//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <string>
#include "WinApiTypes.hpp"

namespace WinApi
{

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
}; 

}
