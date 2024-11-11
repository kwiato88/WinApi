//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Windowsx.h>
#include <richedit.h>
#include <commctrl.h>

#include "ControlList.hpp"
#include "StringConversion.hpp"

namespace WinApi
{
namespace Control
{

void List::addItem(const std::string& p_item)
{
    TCHAR l_itemString[1024];
	stringToArray(p_item, l_itemString);
    SendMessage(self, LB_ADDSTRING, 0, (LPARAM)l_itemString);
}

void List::addItems(const std::vector<std::string>& p_items)
{
    for(const auto& item : p_items)
        addItem(item);
}

void List::clear()
{
    SendMessage(self, LB_RESETCONTENT, 0, 0);
}

void List::selectIndex(int p_index)
{
    if(p_index < 0 || p_index >= size())
        return;
    SendMessage(self, LB_SETCURSEL, p_index, 0);
}

int List::selectedIndex() const
{
    auto index = SendMessage(self, LB_GETCURSEL, 0, 0);
    if(index == LB_ERR)
        return -1;
    return index;
}

std::string List::selectedItem() const
{
    int index = selectedIndex();
    if(index == -1)
        return "";
    TCHAR l_itemString[1024];
    SendMessage(self, LB_GETTEXT, index, (LPARAM)l_itemString);
    return arrayToString(l_itemString);
}

std::string List::getItem(int p_index) const
{
    TCHAR l_itemString[1024];
    SendMessage(self, LB_GETTEXT, p_index, (LPARAM)l_itemString);
    return arrayToString(l_itemString);
}

void List::setItem(int p_index, const std::string& p_item)
{
    TCHAR l_itemString[1024];
    stringToArray(p_item, l_itemString);
    SendMessage(self, LB_DELETESTRING, p_index, 0);
    SendMessage(self, LB_INSERTSTRING, p_index, (LPARAM)l_itemString);
}

int List::size() const
{
    auto length = SendMessage(self, LB_GETCOUNT, 0, 0);
    if(length == LB_ERR)
        return -1;
    return length;
}

void List::scrollToLine(int p_lineNum)
{
    SendMessage(self, LB_SETTOPINDEX, p_lineNum, 0);
}

void List::scrollToBottom()
{
    scrollToLine(size() - 1);
}

void List::scrollToTop()
{
    scrollToLine(0);
}

template<typename ControlT>
class ListKeyDownMatcher
{
public:
    ListKeyDownMatcher(WORD p_keyCode, const ControlT& p_relatedObj)
        : m_keyCode(p_keyCode), m_relatedObj(p_relatedObj) {}
    bool operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const
    {
        return p_msgId == WM_VKEYTOITEM
            && LOWORD(p_wParam) == m_keyCode
            && m_relatedObj.isOwnHandle(Handle{(HWND)(p_lParam)});
    }
private:
    WORD m_keyCode;
    const ControlT& m_relatedObj;
};

WinApi::MsgMatchers::Matcher List::KeyDown(WORD p_keyCode) const
{
    return ListKeyDownMatcher<List>{p_keyCode, *this};
}

}
}
