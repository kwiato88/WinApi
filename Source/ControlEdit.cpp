//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Windowsx.h>
#include <richedit.h>
#include <commctrl.h>

#include "ControlEdit.hpp"
#include "StringConversion.hpp"

namespace WinApi
{
namespace Control
{

std::string Edit::getContent() const
{
	TCHAR contentBuffer[_MAX_PATH] = TEXT("");
	Edit_GetText(self, contentBuffer, _MAX_PATH);
	return arrayToString(contentBuffer);
}

void Edit::setContent(const std::string& p_content)
{
	TCHAR content[_MAX_PATH] = TEXT("");
	stringToArray(p_content, content);
	Edit_SetText(self, content);
}

void Edit::setSelection(int p_begin, int p_end)
{
	Edit_SetSel(self, p_begin, p_end);
}

int Edit::getLineLength(int p_lineNum) const
{
	return Edit_LineLength(self, p_lineNum);
}

}
}
