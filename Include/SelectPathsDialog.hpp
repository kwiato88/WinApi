//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Windows.h>
#include <string>
#include <vector>

#include "Dialog.hpp"
#include "ControlGrid.hpp"
#include "ControlEdit.hpp"

namespace WinApi
{

enum class SelectorType
{
    Directory,
    File
};

template <SelectorType selector>
class SelectPathsDialog : public Dialog
{

public:
    SelectPathsDialog(
        InstanceHandle p_hInstance,
        Handle p_parentWindow,
		const std::string& p_pathsCopySeparator = "\n");

    void setInitalPath(const std::string& p_path);
    void setPaths(const std::vector<std::string>& p_paths);
    std::vector<std::string> getPaths() const;

private:
    void onInit() override;
	bool showContextMenu(int p_xPos, int p_yPos) override;
    void onOkClick();
    void onCancleClick();
    void onAddClick();
    void onSelectClick();
    void onDeleteClick();
	void copyAll() const;
	void copySelected() const;

    void addItemToControl(const std::string& p_newItem);

	Control::RcFileGrid m_itemsControl;
	Control::Edit m_item;
    std::vector<std::vector<std::string>> m_paths;
    std::string m_initialPath;
	std::string itemsSeparator;
};

} // namespace WinApi
