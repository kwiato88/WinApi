#include "Control.hpp"

namespace WinApi
{
namespace Control
{

Control::Control()
	: self(0)
{}

void Control::setFocus()
{
	SetFocus(self);
}
bool Control::isOwnHandle(Handle p_handle) const
{
	return self == p_handle;
}
void Control::init(Handle p_self)
{
	self = p_self;
}

}
}
