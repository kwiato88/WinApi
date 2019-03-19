//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#ifndef GRID_HPP
#define GRID_HPP

#include <windows.h>
#include <string>
#include <vector>
#include "WinApiTypes.hpp"
#include "Control.hpp"

namespace WinApi
{
namespace Control
{

class Grid : public Control
{
public:
    Grid();
    virtual ~Grid() {}

    void addColumns(const std::vector<std::string>& p_labels);
    void addRows(const std::vector<std::vector<std::string> >& p_rows);
    int getSelectedRowIndex() const;

private:
    static void initListViewControl();
    static bool s_isListViewControlInitialized;

    void addColumn(const std::string& p_title);
    void addRow(const std::vector<std::string>& p_cells);
    void addCellInRow(const std::string& p_value, int p_row, int p_cell);
    void setCollumnsWidth();

    int m_columnsCount;
};

class RcFileGrid : public Grid
{
};
class DynamicGrid : public Grid
{
public:
    void init(Handle p_parent, InstanceHandle p_app,
        int p_posX, int p_posY, int p_sizeX, int p_sizeY);
};

}
}//namespace WinApi

#endif
