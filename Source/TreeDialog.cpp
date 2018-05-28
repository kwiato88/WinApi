//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <windows.h>

#include "TreeDialog.hpp"
#include "TreeDialogDef.h"
#include "DialogMsgMatchers.hpp"
#include "ContextMenu.hpp"
#include "Clipboard.hpp"
#include "MessageDialog.hpp"

#include <iostream>

namespace WinApi
{
namespace
{
std::string indentToStr(int p_lvl)
{
	return std::string(p_lvl, '\t');
}
void appendNode(std::string& p_out, int p_lvl, const Node& p_node)
{
	p_out += indentToStr(p_lvl);
	p_out += p_node.m_value;
	p_out += '\n';
	++p_lvl;
	for (const auto& node : p_node.m_children)
		appendNode(p_out, p_lvl, node);
}
}

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
{
	std::string toCopy;
	for(const auto& node : m_treeNodes)
		appendNode(toCopy, 0, node);
	Clipboard::set(Clipboard::String(toCopy));
}
void TreeDialog::copySelectedSubTree()
{
	const Node* subTree = m_tree.getSelectedNode();
	if (subTree != nullptr)
	{
		std::string toCopy;
		appendNode(toCopy, 0, *subTree);
		Clipboard::set(Clipboard::String(toCopy));
	}
}
void TreeDialog::copySelected()
{
	const Node* subTree = m_tree.getSelectedNode();
	if (subTree != nullptr)
		Clipboard::set(Clipboard::String(subTree->m_value));
}

} // namespace WinApi
