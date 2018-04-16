//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include "Control.hpp"

namespace WinApi
{
namespace Control
{

class Text : public Control
{
public:
	void setContent(const std::string& p_content);
};

}
}
