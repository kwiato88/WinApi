//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <windows.h>
#include <algorithm>
#include <commctrl.h>
#include <boost/assign/list_of.hpp>

#include "GridDialog.hpp"
#include "GridDialogDef.h"
#include "DialogMsgMatchers.hpp"
#include "ContextMenu.hpp"
#include "Clipboard.hpp"

namespace WinApi
{
namespace
{
std::string rowToStr(const std::vector<std::string>& p_row, const GridDialog::Copy& p_data)
{
	std::string out;
	if (!p_row.empty())
		out += p_row.front();
	for (std::size_t i = 1; i < p_row.size(); ++i)
		(out += p_data.cellSeparator) += p_row[i];
	out += p_data.rowSeparator;
	return out;
}
}
GridDialog::GridDialog(
	InstanceHandle p_hInstance,
	Handle p_parentparentWindow,
	const std::string& p_title,
	const Copy& p_exportData)
 : Dialog(p_hInstance, p_parentparentWindow, ResourceId(ID_GRID_DIALOG), p_title),
   m_selectedItemIndex(-1), exportData(p_exportData)
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

bool GridDialog::showContextMenu(int p_xPos, int p_yPos)
{
	if (m_gridControl.isWithin(p_xPos, p_yPos))
	{
		ContextMenu menu(m_self);
		menu.add(ContextMenu::Item{ "Copy table" , std::bind(&GridDialog::copyAll, this) });
		menu.add(ContextMenu::Item{ "Copy selected row" , std::bind(&GridDialog::copySelected, this) });
		menu.show(p_xPos, p_yPos);
		return true;
	}
	return false;
}

void GridDialog::copyAll()
{
	std::string toCopy;
	for (const auto row : m_gridRows)
		toCopy += rowToStr(row, exportData);
	Clipboard::set(Clipboard::String(toCopy));
}
void GridDialog::copySelected()
{
	auto selected = m_gridControl.getSelectedRowIndex();
	if (selected != -1)
		Clipboard::set(Clipboard::String(rowToStr(m_gridRows[selected], exportData)));
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
