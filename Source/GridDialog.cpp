//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <windows.h>
#include <algorithm>
#include <commctrl.h>
#include <boost/assign/list_of.hpp>

#include "GridDialog.hpp"
#include "GridDialogDef.h"
#include "DialogMsgMatchers.hpp"

#include <iostream>

namespace WinApi
{

GridDialog::GridDialog(
        InstanceHandle p_hInstance,
        Handle p_parentparentWindow)
 : Dialog(p_hInstance, p_parentparentWindow, ResourceId(ID_GRID_DIALOG)),
   m_selectedItemIndex(-1)
{
    registerHandler(MsgMatchers::ButtonClick(IDOK),     std::bind(&GridDialog::onOkClick, this));
    registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&GridDialog::onCancleClick, this));
    registerHandler(MsgMatchers::DoubleClick(m_gridControl), std::bind(&GridDialog::onGridDoubleClick, this));
}

void GridDialog::onInit()
{
    //m_gridControl.init(m_self, m_hInstance, 5, 5, 1015, 220);
    m_gridControl.init(getItem(ResourceId(ID_GRID_DIALOG_GRID)));
    m_gridControl.addColumns(m_gridLabels);
    m_gridControl.addRows(m_gridRows);
    m_gridControl.setFocus();
}

void GridDialog::onOkClick()
{
    m_selectedItemIndex = m_gridControl.getSelectedRowIndex();
    close(RESULT_OK);
}

void GridDialog::onCancleClick()
{
    m_selectedItemIndex = -1;
    close(RESULT_CANCEL);
}

void GridDialog::onGridDoubleClick()
{
    onOkClick();
}

int GridDialog::getSelectedItemIndex() const
{
    return m_selectedItemIndex;
}

void GridDialog::setTitles(const std::vector<std::string>& p_titles)
{
    m_gridLabels = p_titles;
}

void GridDialog::setContent(const std::vector<std::vector<std::string> >& p_data)
{
    m_gridRows = p_data;
}

} // namespace WinApi
