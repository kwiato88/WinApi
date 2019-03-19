//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "SelectPathsDialog.hpp"
#include "SelectPathsDialogDef.h"
#include "WinApiDirPathGetter.hpp"
#include "WinApiFilePathGetter.hpp"
#include "DialogMsgMatchers.hpp"
#include "ContextMenu.hpp"
#include "Clipboard.hpp"

namespace WinApi
{

template <SelectorType selector>
struct SelectorTypeTraits;

template <>
struct SelectorTypeTraits<SelectorType::Directory>
{
    typedef WinApi::DirPathGetter SelectorType;
	static std::string dialogTitle() { return "Select folders"; }
	static std::string itemName() { return "folder"; }
};

template <>
struct SelectorTypeTraits<SelectorType::File>
{
    typedef WinApi::FilePathGetter SelectorType;
	static std::string dialogTitle() { return "Select files"; }
	static std::string itemName() { return "file"; }
};

template <SelectorType selector>
SelectPathsDialog<selector>::SelectPathsDialog(
        InstanceHandle p_hInstance,
        Handle p_parentWindow,
		const std::string& p_pathsCopySeparator)
    : Dialog(p_hInstance, p_parentWindow, ResourceId(ID_SELECT_PATHS_DIALOG)), itemsSeparator(p_pathsCopySeparator)
{
    registerHandler(MsgMatchers::ButtonClick(IDOK),      std::bind(&SelectPathsDialog::onOkClick, this));
    registerHandler(MsgMatchers::ButtonClick(IDCANCEL),  std::bind(&SelectPathsDialog::onCancleClick, this));
    registerHandler(MsgMatchers::ButtonClick(ID_ADD),    std::bind(&SelectPathsDialog::onAddClick, this));
    registerHandler(MsgMatchers::ButtonClick(ID_SELECT), std::bind(&SelectPathsDialog::onSelectClick, this));
    registerHandler(MsgMatchers::ButtonClick(ID_DELETE), std::bind(&SelectPathsDialog::onDeleteClick, this));
    registerHandler(MsgMatchers::KeyDown(m_itemsControl, VK_DELETE), std::bind(&SelectPathsDialog::onDeleteClick, this));
}

template <SelectorType selector>
void SelectPathsDialog<selector>::onInit()
{
	setTitle(SelectorTypeTraits<selector>::dialogTitle());
    //m_itemsControl.init(m_self, m_hInstance, 12, 74, 362, 200);
    m_itemsControl.init(getItem(ResourceId(ID_SELECT_PATHS_DIALOG_GRID)));
	m_itemsControl.addColumns({ "Path" });
    m_itemsControl.addRows(m_paths);
	m_item.init(getItem(ResourceId(ID_ITEM)));
	m_item.setContent(m_initialPath);
}

template <SelectorType selector>
void SelectPathsDialog<selector>::onOkClick()
{
    close(RESULT_OK);
}

template <SelectorType selector>
void SelectPathsDialog<selector>::onCancleClick()
{
    close(RESULT_CANCEL);
}

template <SelectorType selector>
void SelectPathsDialog<selector>::addItemToControl(const std::string& p_newItem)
{
    if(!p_newItem.empty())
    {
		m_paths.push_back({ p_newItem });
        m_itemsControl.addRows(m_paths);
    }
}

template <SelectorType selector>
void SelectPathsDialog<selector>::onAddClick()
{
    addItemToControl(m_item.getContent());
}

template <SelectorType selector>
void SelectPathsDialog<selector>::onSelectClick()
{
    typedef SelectorTypeTraits<selector>::SelectorType SelectorType;
    std::string l_path = SelectorType(m_self).getPath(
		std::string("Select " + SelectorTypeTraits<selector>::itemName()), m_item.getContent());
    if(!l_path.empty())
        m_item.setContent(l_path);
}

template <SelectorType selector>
void SelectPathsDialog<selector>::onDeleteClick()
{
    int l_itemToDelete = m_itemsControl.getSelectedRowIndex();
    if(l_itemToDelete != -1)
    {
        m_paths.erase(m_paths.begin() + l_itemToDelete);
        m_itemsControl.addRows(m_paths);
    }
}

template <SelectorType selector>
std::vector<std::string> SelectPathsDialog<selector>::getPaths() const
{
    std::vector<std::string> l_paths;
    for(const auto& path : m_paths)
        l_paths.push_back(path.front());
    return l_paths;
}

template <SelectorType selector>
void SelectPathsDialog<selector>::setPaths(const std::vector<std::string>& p_paths)
{
    m_paths.clear();
    for(const auto& path : p_paths)
		m_paths.push_back({ path });
}

template <SelectorType selector>
void SelectPathsDialog<selector>::setInitalPath(const std::string& p_path)
{
    m_initialPath = p_path;
}

template <SelectorType selector>
bool SelectPathsDialog<selector>::showContextMenu(int p_xPos, int p_yPos)
{
	if (m_itemsControl.isWithin(p_xPos, p_yPos))
	{
		ContextMenu menu(m_self);
		menu.add(ContextMenu::Item{ "Copy paths", std::bind(&SelectPathsDialog::copyAll, this) });
		menu.add(ContextMenu::Item{ "Copy selected path", std::bind(&SelectPathsDialog::copySelected, this) });
		menu.show(p_xPos, p_yPos);
		return true;
	}
	return false;
}
template <SelectorType selector>
void SelectPathsDialog<selector>::copyAll() const
{
	std::string toCopy;
	for (const auto row : m_paths)
		toCopy += row.at(0) + itemsSeparator;
	Clipboard::set(Clipboard::String(toCopy));
}
template <SelectorType selector>
void SelectPathsDialog<selector>::copySelected() const
{
	auto selected = m_itemsControl.getSelectedRowIndex();
	if (selected != -1)
		Clipboard::set(Clipboard::String(m_paths[selected].at(0)));
}

template class SelectPathsDialog<SelectorType::Directory>;
template class SelectPathsDialog<SelectorType::File>;

} // namespace WinApi
