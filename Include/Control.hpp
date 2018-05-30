//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include "WinApiTypes.hpp"

namespace WinApi
{
namespace Control
{

class Control
{
public:
	Control(const Control&) = delete;
	Control& operator=(const Control&) = delete;
	void init(Handle p_self);
	void setFocus();
	bool isOwnHandle(Handle p_handle) const;
	bool isWithin(int p_xPos, int p_yPos) const;

protected:
	Control();
	virtual ~Control() = default;

	RECT getRectangle() const;

	Handle self;
};

}
}