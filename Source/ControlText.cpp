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
