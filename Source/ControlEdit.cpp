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
	TCHAR contentBuffer[1024] = TEXT("");
	Edit_GetText(self, contentBuffer, 1024);
	return arrayToString(contentBuffer);
}

void Edit::setContent(const std::string& p_content)
{
	TCHAR content[1024] = TEXT("");
	stringToArray(p_content, content);
	Edit_SetText(self, content);
}

void Edit::appendContent(const std::string& p_content)
{
    int nLength = getContentLength(); 
    setSelection(nLength, nLength);
    TCHAR content[1024] = TEXT("");
    stringToArray(p_content, content);
    Edit_ReplaceSel(self, content);
}

void Edit::setSelection(int p_begin, int p_end)
{
	Edit_SetSel(self, p_begin, p_end);
}

int Edit::getContentLength() const
{
	return Edit_GetTextLength(self);
}

int Edit::getLineLength(int p_lineNum) const
{
	return Edit_LineLength(self, p_lineNum);
}

void Edit::scrollToBottom()
{
	Edit_Scroll(self, getLineCount(), 0);
}

int Edit::getLineCount() const
{
	return Edit_GetLineCount(self);
}

void Edit::scrollLines(int p_lineNum)
{
	SendMessage(self, EM_LINESCROLL, p_lineNum, 0);
}

void Edit::acceptNewLine()
{
	addStyle(ES_WANTRETURN);
}

void Edit::dontAcceptNewLine()
{
	removeStyle(ES_WANTRETURN);
}

}
}
