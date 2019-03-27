//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "ControlText.hpp"
#include "StringConversion.hpp"

namespace WinApi
{
namespace Control
{

void Text::setContent(const std::string& p_content)
{
	TCHAR content[1024] = TEXT("");
	stringToArray(p_content, content);
	SetWindowText(self, content);
}

}
}
