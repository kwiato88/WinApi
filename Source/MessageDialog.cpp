//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "MessageDialog.hpp"
#include "StringConversion.hpp"

namespace WinApi
{
namespace
{
unsigned int toInt(MessageDialog::Icon i)
{
	switch (i)
	{
	case WinApi::MessageDialog::Icon::Question:
		return MB_ICONQUESTION;
	case WinApi::MessageDialog::Icon::Info:
		return MB_ICONINFORMATION;
	case WinApi::MessageDialog::Icon::Warn:
		return MB_ICONWARNING;
	case WinApi::MessageDialog::Icon::Error:
		return MB_ICONERROR;
	case WinApi::MessageDialog::Icon::None:
		return 0;
	default:
		return 0;
	}
}
unsigned int toInt(MessageDialog::Buttons b)
{
	switch (b)
	{
	case WinApi::MessageDialog::Buttons::Ok:
		return MB_OK;
	case WinApi::MessageDialog::Buttons::OkCancel:
		return MB_OKCANCEL;
	case WinApi::MessageDialog::Buttons::YesNo:
		return MB_YESNO;
	case WinApi::MessageDialog::Buttons::RetryCancel:
		return MB_RETRYCANCEL;
	default:
		return 0;
	}
}
MessageDialog::Button toButton(int b)
{
	if(b == IDOK)
		return MessageDialog::Button::Ok;
	if(b == IDCANCEL)
		return MessageDialog::Button::Cancel;
	if(b == IDYES)
		return MessageDialog::Button::Yes;
	if(b == IDNO)
		return MessageDialog::Button::No;
	if(b == IDRETRY)
		return MessageDialog::Button::Retry;
}
std::string cutString(std::string p_string, size_t p_length)
{
	if (p_string.size() < p_length)
		return p_string;
	else
		return p_string.substr(0, p_length);
}
}
MessageDialog::MessageDialog(Handle p_parent)
	: parent(p_parent), icon(Icon::None), buttons(Buttons::Ok)
{}

void MessageDialog::with(Icon i)
{
	icon = i;
}

void MessageDialog::with(Buttons b)
{
	buttons = b;
}

void MessageDialog::withTitle(const std::string& t)
{
	title = t;
}
void MessageDialog::withContent(const std::string& c)
{
	content = c;
}

MessageDialog::Button MessageDialog::show()
{
	static const size_t MAX_CONTENT_LENGTH = 1024;
	static const size_t MAX_TITLE_LENGTH = 64;
	TCHAR messageUString[MAX_CONTENT_LENGTH];
	stringToArray(cutString(content, MAX_CONTENT_LENGTH), messageUString);
	TCHAR titleUString[MAX_TITLE_LENGTH];
	stringToArray(cutString(title, MAX_TITLE_LENGTH), titleUString);
	return toButton(MessageBox(parent, messageUString, titleUString, toInt(icon) | toInt(buttons)));
}

std::string toStr(MessageDialog::Button b)
{
	switch (b)
	{
	case WinApi::MessageDialog::Ok:
		return "Ok";
	case WinApi::MessageDialog::Cancel:
		return "Cancel";
	case WinApi::MessageDialog::Yes:
		return "Yes";
	case WinApi::MessageDialog::No:
		return "No";
	case WinApi::MessageDialog::Retry:
		return "Retry";
	default:
		return "Invalid button";
	}
}

}
