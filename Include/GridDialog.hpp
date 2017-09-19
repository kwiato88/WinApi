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
    static const int BUTTON_OK = 1;
    static const int BUTTON_CANCEL = 0;

    GridDialog(InstanceHandle p_hInstance, Handle p_parentWindow);

    void setTitles(const std::vector<std::string>& p_titles);
    void setContent(const std::vector<std::vector<std::string> >& p_data);
    int getSelectedItemIndex() const;

private:
    void onInit();

    void onOkClick();
    void onCancleClick();
    void onGridDoubleClick();

	Control::RcFileGrid m_gridControl;

    std::vector<std::string> m_gridLabels;
    std::vector<std::vector<std::string> > m_gridRows;
    int m_selectedItemIndex;
};

} // namespace WinApi

#endif /* GRID_DIALOG_HPP */
