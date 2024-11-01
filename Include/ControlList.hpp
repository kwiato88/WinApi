//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "WinApiTypes.hpp"
#include "Control.hpp"

namespace WinApi
{
namespace Control
{

class List : public Control
{
public:
    void addItem(const std::string& p_item);
    void addItems(const std::vector<std::string>& p_items);
    void clear();
    int selectedIndex() const;
    void selectIndex(int p_index);
    std::string selectedItem() const;
    int size() const;
};

}
}
