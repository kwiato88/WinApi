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
	int getLineLength(int p_lineNum);
};

}
}
