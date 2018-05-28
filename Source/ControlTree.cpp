//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "ControlTree.hpp"

namespace WinApi
{
namespace Control
{

Tree::Item::Item(Handle p_tree, HTREEITEM p_nativeItem)
	: tree(p_tree), nativeItem(p_nativeItem)
{}

Tree::Item::Item(const Node& p_node, const Item& p_parrent)
	: tree(p_parrent.tree), nativeItem()
{
	TVINSERTSTRUCT insertStruct = buildInsertStruct(p_node, p_parrent.nativeItem);
	nativeItem = TreeView_InsertItem(tree, &insertStruct);
}

Tree::Item Tree::Item::addChild(const Node& p_node)
{
	return Item(p_node, *this);
}

const Node* Tree::Item::getNode()
{
	if (nativeItem != 0)
	{
		TVITEM item;
		item.mask = TVIF_PARAM | TVIF_HANDLE;
		item.hItem = nativeItem;
		if (TreeView_GetItem(tree, &item))
			return (const Node*)(item.lParam);
	}
	return nullptr;
}

const void* Tree::Item::getUserContext()
{
	const Node* node = getNode();
	if (node != nullptr)
		return node->m_userContext;
	return nullptr;
}

TVINSERTSTRUCT Tree::Item::buildInsertStruct(const Node& p_node, HTREEITEM p_parent)
{
	TVITEM item;
	item.mask = TVIF_TEXT | TVIF_PARAM;
	item.cchTextMax = mbstowcs(nativeText, p_node.m_value.c_str(), _MAX_PATH - 1);
	item.pszText = nativeText;
	item.lParam = (LPARAM)(&p_node);

	TVINSERTSTRUCT insertItem;
	insertItem.item = item;
	insertItem.hInsertAfter = TVI_LAST;
	insertItem.hParent = p_parent;
	
	return insertItem;
}

Tree::RootItem::RootItem(Handle p_tree)
	: Item(p_tree, TVI_ROOT)
{}

void Tree::addItem(const Node& p_node, Item& p_parrent)
{
	auto isertedItem = p_parrent.addChild(p_node);
	for (const Node& child : p_node.m_children)
		addItem(child, isertedItem);
}

void Tree::addRoot(const Node& p_root)
{
	RootItem root(self);
	addItem(p_root, root);
}

const void* Tree::getSelectedItemContext()
{
	Item selectedItem(self, TreeView_GetSelection(self));
	return selectedItem.getUserContext();
}

const Node* Tree::getSelectedNode()
{
	Item selectedItem(self, TreeView_GetSelection(self));
	return selectedItem.getNode();
}

}
}
