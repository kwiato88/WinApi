//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Windows.h>
#include "Clipboard.hpp"
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

template<typename T>
class LockedData
{
public:
	LockedData(const LockedData&) = delete;
	LockedData& operator=(const LockedData&) = delete;
	LockedData(GlobalHandle p_handle)
		: handle(p_handle), data(nullptr)
	{
		data = static_cast<T*>(GlobalLock(handle));
		if(data == nullptr)
			throw LastErrorException("Failed to lock mem");
	}
	T* get()
	{
		return data;
	}
	~LockedData()
	{
		if (data != nullptr)
			GlobalUnlock(handle);
	}
private:
	GlobalHandle handle;
	T* data;
};

Clipboard::String::String(const std::string& p_value)
	: value(p_value)
{
	handle = GlobalHandle(GlobalAlloc(GMEM_MOVEABLE, value.size() + 1));
	if (handle == NULL)
		throw LastErrorException("Failed to allocate mem for string");
	LockedData<char> data(handle);
	memcpy(data.get(), value.c_str(), value.size() + 1);
}

Clipboard::String::String(GlobalHandle p_handle)
	: handle(p_handle)
{
	if (handle == NULL)
		throw LastErrorException("Failed to get text from clipboard");
	LockedData<const char> data(handle);
	value = std::string(data.get());
}

Clipboard::String::String(const String& p_other)
	: handle(0), value(p_other.value)
{}

Clipboard::String::String(const std::string& p_value, NoAlloc)
	: handle(0), value(p_value)
{}

Clipboard::String::operator std::string () const
{
	return value;
}

class Clipboard::Lock
{
public:
	Lock()
	{
		if (OpenClipboard(0) == 0)
			throw LastErrorException("Failed to open clipboard");
	}
	~Lock()
	{
		CloseClipboard();
	}
};

void Clipboard::set(const String& p_value)
{
	Lock guard;
	EmptyClipboard();
	if(SetClipboardData(CF_TEXT, p_value.handle) == NULL)
		throw LastErrorException("Failed to set text to clipboard");
}

Clipboard::String Clipboard::getText()
{
	Lock guard;
	if (IsClipboardFormatAvailable(CF_TEXT))
		return String(GlobalHandle(GetClipboardData(CF_TEXT)));
	return String("", String::NoAlloc());
}

}
