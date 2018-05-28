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
class TreeExport
{
public:
	TreeExport(char p_indnetChar)
		: indentLvl(0), indentChar(p_indnetChar)
	{}

	void append(const Node& p_node)
	{
		out += std::string(indentLvl, indentChar);
		out += p_node.m_value;
		out += '\n';
		++indentLvl;
		for (const auto& node : p_node.m_children)
			append(node);
		--indentLvl;
	}
	std::string result()
	{
		return out;
	}

private:
	int indentLvl;
	char indentChar;
	std::string out;
};
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
	TreeExport toCopy('\t');
	for(const auto& node : m_treeNodes)
		toCopy.append(node);
	Clipboard::set(Clipboard::String(toCopy.result()));
}
void TreeDialog::copySelectedSubTree()
{
	const Node* subTree = m_tree.getSelectedNode();
	if (subTree != nullptr)
	{
		TreeExport toCopy('\t');
		toCopy.append(*subTree);
		Clipboard::set(Clipboard::String(toCopy.result()));
	}
}
void TreeDialog::copySelected()
{
	const Node* subTree = m_tree.getSelectedNode();
	if (subTree != nullptr)
		Clipboard::set(Clipboard::String(subTree->m_value));
}

} // namespace WinApi
