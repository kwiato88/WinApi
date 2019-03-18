//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <string>

namespace WinApi
{

class ShellCommand
{
public:
	ShellCommand(const std::string& p_path, const std::string& p_parameters);
	void execute();
private:
	std::string path;
	std::string parameters;
};

}
