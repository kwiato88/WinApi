//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <string>
#include "WinApiTypes.hpp"

namespace WinApi
{

class MessageDialog
{
public:
	enum class Icon
	{
		None,
		Question,
		Info,
		Warn,
		Error
	};
	enum class Buttons
	{
		Ok,
		OkCancel,
		YesNo,
		RetryCancel
	};
	enum Button
	{
		Ok,
		Cancel,
		Yes,
		No,
		Retry
	};

	MessageDialog(Handle p_parent);
	void withTitle(const std::string&);
	void withContent(const std::string&);
	void with(Icon);
	void with(Buttons);
	Button show();

private:
	Handle parent;
	Icon icon;
	Buttons buttons;
	std::string title;
	std::string content;
};

std::string toStr(MessageDialog::Button b);

}
