//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <string>
#include "WinApiTypes.hpp"

namespace WinApi
{

class SelectDirPathDialog
{
public:
	static const int BUTTON_OK = 1;
	static const int BUTTON_CANCEL = 0;

	SelectDirPathDialog(Handle p_parrent, const std::string& p_comment, const std::string& p_initialDir);
	int show();
	std::string getSelectedPath();

private:
	Handle m_parrent;
	std::string m_selectedPath;
	std::string m_comment;
	std::string m_initialDir;
};

}
