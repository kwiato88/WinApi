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
