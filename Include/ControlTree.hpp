//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>

#include "WinApiTypes.hpp"
#include "Control.hpp"
#include "Node.hpp"

namespace WinApi
{
namespace Control
{

class Tree : public Control
{
	class Item
	{
	public:
		Item(Handle p_tree, HTREEITEM p_nativeItem);

		Item addChild(const Node& p_node);
		const void* getUserContext() const;
		const Node* getNode() const;

	private:
		Item(const Node& p_node, const Item& p_parrent);

		TVINSERTSTRUCT buildInsertStruct(const Node& p_node, HTREEITEM p_parent);

		Handle tree = {};
		HTREEITEM nativeItem = {};	
		TCHAR nativeText[_MAX_PATH] = TEXT("");
	};

	class RootItem : public Item
	{
	public:
		RootItem(Handle p_tree);
	};

public:
	void addRoot(const Node& p_root);
	const void* getSelectedItemContext() const;
	const Node* getSelectedNode() const;

private:
	void addItem(const Node& p_node, Item& p_parrent);
};

}
}
