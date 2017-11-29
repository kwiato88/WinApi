//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <windows.h>
#include <algorithm>
#include <commctrl.h>
#include <boost/assign/list_of.hpp>

#include "ListBoxDialog.hpp"
#include "ListBoxDialogDef.h"
#include "DialogMsgMatchers.hpp"

namespace WinApi
{

namespace
{
class ItemAdder
{
public:
    ItemAdder(Handle p_list) { m_list = p_list; }

    void operator()(const std::string& p_item)
    {
		TCHAR l_itemString[1024];
		mbstowcs(l_itemString, p_item.c_str(), 1024 - 1);
        SendMessage(m_list, LB_ADDSTRING, 0, (LPARAM)l_itemString);
    }

private:
    Handle m_list;
};

typedef int Pixel;

Pixel getCharWidth()                       { return 9;/*TODO: get value from system. Try: GetDialogBaseUnits*/ }
Pixel getWindowWidthUnit()                 { return getCharWidth()/4; }
Pixel getListBoxBorderWidth()              { return getWindowWidthUnit()*2; }
Pixel getStringWidth(std::size_t p_lenght) { return getCharWidth()*p_lenght; }
Pixel getWidthToDisplay(std::size_t p_stringLength)
{
    return 2* getListBoxBorderWidth() + getStringWidth(p_stringLength);
}

struct StringLengthComparator
{
    bool operator()(const std::string& p_lhs, const std::string& p_rhs)
    {
        return p_lhs.size() < p_rhs.size();
    }
};

}

ListBoxDialog::ListBoxDialog(
        InstanceHandle p_hInstance,
        Handle p_parentparentWindow)
 : Dialog(p_hInstance, p_parentparentWindow, ResourceId(ID_LIST_BOX_DIALOG)),
   m_selectedItemIndex(-1)
{
    registerHandler(MsgMatchers::ButtonClick(IDOK),     std::bind(&ListBoxDialog::onOkClick, this));
    registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&ListBoxDialog::onCancleClick, this));
    registerHandler(MsgMatchers::MsgCodeAndValue(ID_LIST_BOX, LBN_SELCHANGE), std::bind(&ListBoxDialog::onListUpdate, this));
    registerHandler(MsgMatchers::MsgCodeAndValue(ID_LIST_BOX, LBN_DBLCLK),    std::bind(&ListBoxDialog::onListDoubleClick, this));
}

void ListBoxDialog::onInit()
{
    setListBoxItems();
    setListBoxHorizontalScroll();
    setFocusOnListBox();
}

Handle ListBoxDialog::getListBoxHandle()
{
    return getItem(ResourceId(ID_LIST_BOX));
}

void ListBoxDialog::setFocusOnListBox()
{
    SetFocus(getListBoxHandle());
}

void ListBoxDialog::setListBoxHorizontalScroll()
{
    std::size_t l_itemsMaxLength = std::max_element(m_items.begin(), m_items.end(),
        StringLengthComparator())->size();
    SendMessage(getListBoxHandle(), LB_SETHORIZONTALEXTENT, getWidthToDisplay(l_itemsMaxLength), 0);
}

void ListBoxDialog::setListBoxItems()
{
    std::for_each(m_items.begin(), m_items.end(), ItemAdder(getListBoxHandle()));
}

void ListBoxDialog::onOkClick()
{
    close(RESULT_OK);
}

void ListBoxDialog::onCancleClick()
{
    m_selectedItemIndex = -1;
    close(RESULT_CANCEL);
}

void ListBoxDialog::onListUpdate()
{
    m_selectedItemIndex = (int)SendMessage(getListBoxHandle(), LB_GETCURSEL, 0, 0);
}

void ListBoxDialog::onListDoubleClick()
{
    onListUpdate();
    close(RESULT_OK);
}

int ListBoxDialog::getSelectedItemIndex() const
{
    return m_selectedItemIndex;
}

void ListBoxDialog::setItems(const std::vector<std::string>& p_items)
{
    m_items = p_items;
}

} // namespace WinApi
