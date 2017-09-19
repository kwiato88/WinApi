//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <Windows.h>
#include <boost/serialization/strong_typedef.hpp>

namespace WinApi
{

BOOST_STRONG_TYPEDEF(HWND, Handle)
BOOST_STRONG_TYPEDEF(HINSTANCE, InstanceHandle)
BOOST_STRONG_TYPEDEF(int, ResourceId)

}
