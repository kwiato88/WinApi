//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "ControlText.hpp"

namespace WinApi
{
namespace Control
{

void Text::setContent(const std::string& p_content)
{
	TCHAR content[_MAX_PATH] = TEXT("");
	mbstowcs(content, p_content.c_str(), _MAX_PATH - 1);
	SetWindowText(self, content);
}

}
}
