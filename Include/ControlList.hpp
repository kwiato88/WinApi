//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "WinApiTypes.hpp"
#include "Control.hpp"
#include "DialogMsgMatchers.hpp"

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
    std::string getItem(int p_index) const;
    void setItem(int p_index, const std::string& p_item);
    int size() const;
    void scrollToLine(int p_lineNum);
    void scrollToBottom();
    void scrollToTop();

    WinApi::MsgMatchers::Matcher KeyDown(WORD p_keyCode) const;
};

}
}
