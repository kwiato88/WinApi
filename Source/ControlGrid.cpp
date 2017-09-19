//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "ControlGrid.hpp"
#include <commctrl.h>
#include <functional>
#include <boost/range/algorithm/for_each.hpp>

namespace WinApi
{
namespace Control
{

bool Grid::s_isListViewControlInitialized = false;

void Grid::initListViewControl()
{
    INITCOMMONCONTROLSEX icex;
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex); 
}

Grid::Grid()
 : Control(), m_columnsCount(0)
{
    if(!s_isListViewControlInitialized)
    {
        initListViewControl();
        s_isListViewControlInitialized = true;
    }
}

//comctl32.lib to linker
void DynamicGrid::init(Handle p_parent, InstanceHandle p_app,
                int p_posX, int p_posY, int p_sizeX, int p_sizeY)
{
    self = CreateWindow(WC_LISTVIEW, 
                        L"",
                        WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_VISIBLE,
                        p_posX, p_posY,
                        p_sizeX,
                        p_sizeY,
                        p_parent,
                        (HMENU)0,
                        p_app,
                        NULL);
}

void Grid::addColumns(const std::vector<std::string>& p_labels)
{
    boost::range::for_each(p_labels, std::bind(&Grid::addColumn, this, std::placeholders::_1));
}

void Grid::addColumn(const std::string& p_title)
{
    TCHAR l_columnLabel[64];
    mbstowcs(l_columnLabel, p_title.c_str(), 63);
    LVCOLUMN l_col;
    l_col.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
	l_col.cx = 100;
    l_col.pszText = l_columnLabel;
    ListView_InsertColumn(self, m_columnsCount++, &l_col);
}

void Grid::addRows(const std::vector<std::vector<std::string> >& p_rows)
{
    ListView_DeleteAllItems(self);
    boost::range::for_each(p_rows, std::bind(&Grid::addRow, this, std::placeholders::_1));
    setCollumnsWidth();
    ListView_SetExtendedListViewStyle(self, LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}

void Grid::addRow(const std::vector<std::string>& p_cells)
{
    int l_rowCount = ListView_GetItemCount(self);
    for(int i=0; i<p_cells.size(); ++i)
        addCellInRow(p_cells[i], l_rowCount, i);
}

void Grid::addCellInRow(const std::string& p_value, int p_row, int p_cell)
{
    TCHAR l_value[256];
    mbstowcs(l_value, p_value.c_str(), 255);
    LV_ITEM Lv_i; 
	Lv_i.mask = LVIF_TEXT;
    Lv_i.iItem = p_row;
    Lv_i.iSubItem = p_cell;
    Lv_i.pszText = l_value;
    if(p_cell == 0)
	    ListView_InsertItem(self,&Lv_i);
    else
        ListView_SetItem(self, &Lv_i);
}

int Grid::getSelectedRowIndex()
{
    return ListView_GetNextItem(self, -1, LVNI_SELECTED);
}

void Grid::setCollumnsWidth()
{
    for(int i=0; i<m_columnsCount; ++i)
        ListView_SetColumnWidth(self, i, LVSCW_AUTOSIZE_USEHEADER);
}

}// namespace Control
}// namespace WinApi
