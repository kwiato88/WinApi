//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <Windows.h>
#include <string>
#include "WinApiTypes.hpp"
#include "Control.hpp"

namespace WinApi
{
namespace Control
{

class Edit : public Control
{
public:
	std::string getContent() const;
	void setContent(const std::string& p_content);
	void setSelection(int p_begin, int p_end);
	int getLineLength(int p_lineNum) const;
	void scrollToBottom();
	void scrollLines(int p_lineNum);
	int getLineCount() const;
};

}
}
