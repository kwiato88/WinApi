//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <Windows.h>
#include "WinApiTypes.hpp"
#include "Control.hpp"

namespace WinApi
{
namespace Control
{

class CheckBox : public Control
{
public:
	bool isChecked() const;
	void check();
	void uncheck();
	void enable();
	void disable();
};

}
}
