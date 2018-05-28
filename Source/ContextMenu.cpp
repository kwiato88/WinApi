//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include "ContextMenu.hpp"
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

ContextMenu::ContextMenu(Handle p_parent)
	: parent(p_parent), self(MenuHandle(NULL)), itemId(1)
{
	self = MenuHandle(CreatePopupMenu());
	if (self == NULL)
		throw LastErrorException("Failed to create context menu");
}

ContextMenu::~ContextMenu()
{
	if (self != NULL)
		DestroyMenu(self);
}

void ContextMenu::add(const Item& p_menuItem)
{
	TCHAR label[256] = TEXT("");
	mbstowcs(label, p_menuItem.label.c_str(), 256 - 1);
	if (InsertMenu(
			self,
			-1, //add to end
			MF_BYPOSITION | MF_STRING,
			itemId,
			label) == 0)
		throw LastErrorException("Failed to insert item to context menu");
	items[itemId++] = p_menuItem;
}

void ContextMenu::addSeparator()
{
	if (InsertMenu(
			self,
			-1, //add to end
			MF_BYPOSITION | MF_SEPARATOR,
			0,
			TEXT("")) == 0)
		throw LastErrorException("Failed to insert separator to context menu");
}

void ContextMenu::show(int p_x, int p_y)
{
	//TODO: handle x, y == -1, -1
	SetForegroundWindow(parent);
	POINT pos;
	pos.x = p_x;
	pos.y = p_y;
	ClientToScreen(parent, &pos);
	unsigned int selected = TrackPopupMenu(
		self,
		TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_NONOTIFY | TPM_RETURNCMD,
		pos.x,
		pos.y,
		0,
		parent,
		nullptr);
	if (selected == 0)
		throw LastErrorException("Failed to display context menu");
	if (items.find(selected) == items.end())
		throw std::runtime_error("Context menu item idx out of range"); //TODO: replace with dediacted error, introduce WinApi::Error ???
	items[selected].handler();
}

}
