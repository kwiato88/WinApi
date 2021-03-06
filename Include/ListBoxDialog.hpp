//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
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
    ListBoxDialog(InstanceHandle p_hInstance, Handle p_parentWindow,
		const std::string& p_name = "", const std::string& p_itemCopySeparator = "\n");

    int getSelectedItemIndex() const;
    void setItems(const std::vector<std::string>& p_items);

private:
    void onInit() override;
	bool showContextMenu(int p_xPos, int p_yPos) override;
    void onOkClick();
    void onCancleClick();
    void onListUpdate();
    void onListDoubleClick();
    void setListBoxItems();
    void setListBoxHorizontalScroll();
    void setFocusOnListBox();
    Handle getListBoxHandle() const;
	void copyAll() const;
	void copySelected() const;

    int m_selectedItemIndex;
    std::vector<std::string> m_items;
	std::string itemsSeparator;
};

} // namespace WinApi

#endif /* LIST_BOX_DIALOG_H_ */
