//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "Control.hpp"

namespace WinApi
{
namespace Control
{
namespace
{
struct Point
{
	int x;
	int y;

	bool operator==(const Point& p_other) const
	{
		return x == p_other.x && y == p_other.y;
	}
	bool operator<(const Point& p_other) const
	{
		return x < p_other.x && y < p_other.y;
	}
	bool operator <= (const Point& p_other) const
	{
		return this->operator<(p_other) || this->operator==(p_other);
	}
};
}
Control::Control()
	: self(0)
{}

void Control::setFocus()
{
	SetFocus(self);
}
bool Control::isFocused() const
{
	return GetFocus() == self;
}
bool Control::isOwnHandle(Handle p_handle) const
{
	return self == p_handle;
}
RECT Control::getRectangle() const
{
	RECT rec;
	if (GetWindowRect(self, &rec) == 0)
	{
		rec.bottom = 0;
		rec.left = 0;
		rec.right = 0;
		rec.top = 0;
	}
	return rec;
}
bool Control::isWithin(int p_xPos, int p_yPos) const
{
	RECT rec = getRectangle();
	Point topLeft{ rec.left , rec.top };
	Point bottomRight{ rec.right, rec.bottom };
	Point other{ p_xPos, p_yPos };
	return topLeft <= other && other < bottomRight;
	
}
void Control::init(Handle p_self)
{
	self = p_self;
}

void Control::addStyle(DWORD p_style)
{
	SetWindowLongPtr(self, GWL_STYLE, GetWindowLongPtr(self, GWL_STYLE) | p_style);
}

void Control::removeStyle(DWORD p_style)
{
	SetWindowLongPtr(self, GWL_STYLE, GetWindowLongPtr(self, GWL_STYLE) & ~p_style);
}

}
}
