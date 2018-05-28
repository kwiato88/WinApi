//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <windows.h>

#include "TreeDialog.hpp"
#include "TreeDialogDef.h"
#include "DialogMsgMatchers.hpp"
#include "ContextMenu.hpp"
#include "Clipboard.hpp"

#include <iostream>

namespace WinApi
{

TreeDialog::TreeDialog(
        InstanceHandle p_hInstance,
        Handle p_parentWindow,
		const std::string& p_title)
 : Dialog(p_hInstance, p_parentWindow, ResourceId(ID_TREE_DIALOG), p_title),
	m_selectedItemUserContext(nullptr)
{
    registerHandler(MsgMatchers::ButtonClick(IDOK),     std::bind(&TreeDialog::onOkClick, this));
    registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&TreeDialog::onCancleClick, this));
}

void TreeDialog::onInit()
{
	m_tree.init(getItem(ResourceId(ID_TREE)));
	for (const Node& node : m_treeNodes)
		m_tree.addRoot(node);
}

void TreeDialog::onOkClick()
{
	m_selectedItemUserContext = m_tree.getSelectedItemContext();
    close(RESULT_OK);
}

void TreeDialog::onCancleClick()
{
	m_selectedItemUserContext = nullptr;
    close(RESULT_CANCEL);
}

void TreeDialog::setTreeNodes(const std::vector<Node>& p_tree)
{
	m_treeNodes = p_tree;
}

void TreeDialog::showContextMenu(int p_xPos, int p_yPos)
{
	ContextMenu menu(m_self);
	menu.add(ContextMenu::Item{ "Copy tree", std::bind(&TreeDialog::copyAll, this) });
	menu.add(ContextMenu::Item{ "Copy sub-tree", std::bind(&TreeDialog::copySelectedSubTree, this) });
	menu.add(ContextMenu::Item{ "Copy selected item", std::bind(&TreeDialog::copySelected, this) });
	menu.show(p_xPos, p_yPos);
}
void TreeDialog::copyAll()
{}
void TreeDialog::copySelectedSubTree()
{}
void TreeDialog::copySelected()
{}

} // namespace WinApi
