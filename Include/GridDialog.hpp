//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#ifndef GRID_DIALOG_HPP
#define GRID_DIALOG_HPP

#include <windows.h>
#include <string>
#include <vector>

#include "Dialog.hpp"
#include "ControlGrid.hpp"

namespace WinApi
{

class GridDialog : public Dialog
{
public:
    GridDialog(InstanceHandle p_hInstance, Handle p_parentWindow, const std::string& p_title = "");

    void setTitles(const std::vector<std::string>& p_titles);
    void setContent(const std::vector<std::vector<std::string> >& p_data);
    int getSelectedItemIndex() const;

private:
    void onInit();
	void showContextMenu(int p_xPos, int p_yPos);

    void onOkClick();
    void onCancleClick();
    void onGridDoubleClick();
	void test1();
	void test2();

	Control::RcFileGrid m_gridControl;

    std::vector<std::string> m_gridLabels;
    std::vector<std::vector<std::string> > m_gridRows;
    int m_selectedItemIndex;
};

} // namespace WinApi

#endif /* GRID_DIALOG_HPP */
