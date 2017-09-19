#ifndef LIST_BOX_DIALOG_HPP
#define LIST_BOX_DIALOG_HPP

#include <windows.h>
#include <string>
#include <vector>

#include "Dialog.hpp"

namespace WinApi
{

class ListBoxDialog : public Dialog
{// TODO: refactor to use edit control
public:
    static const int BUTTON_OK = 1;
    static const int BUTTON_CANCEL = 0;

    ListBoxDialog(InstanceHandle p_hInstance, Handle p_parentWindow);

    int getSelectedItemIndex() const;
    void setItems(const std::vector<std::string>& p_items);

private:
    void onInit();
    void onOkClick();
    void onCancleClick();
    void onListUpdate();
    void onListDoubleClick();
    void setListBoxItems();
    void setListBoxHorizontalScroll();
    void setFocusOnListBox();
    Handle getListBoxHandle();

    int m_selectedItemIndex;
    std::vector<std::string> m_items;
};

} // namespace WinApi

#endif /* LIST_BOX_DIALOG_H_ */
