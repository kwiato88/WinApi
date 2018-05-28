//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <functional>
#include <map>
#include "WinApiTypes.hpp"

namespace WinApi
{

class ContextMenu
{
public:
	typedef std::function<void()> Handler;
	struct Item
	{
		std::string label;
		Handler handler;
	};

	ContextMenu(Handle p_parent);
	~ContextMenu();
	void add(const Item&);
	void addSeparator();
	void show(int p_x, int p_y);

private:
	Handle parent;
	MenuHandle self;
	unsigned int itemId;
	std::map<unsigned int, Item> items;
};

}
