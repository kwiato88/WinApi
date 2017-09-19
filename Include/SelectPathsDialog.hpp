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
    static const int BUTTON_OK = 1;
    static const int BUTTON_CANCEL = 0;

    SelectPathsDialog(
        InstanceHandle p_hInstance,
        Handle p_parentWindow);

    void setInitalPath(const std::string& p_path);
    void setPaths(const std::vector<std::string>& p_paths);
    std::vector<std::string> getPaths();

private:

    void onInit();
    void onOkClick();
    void onCancleClick();
    void onAddClick();
    void onSelectClick();
    void onDeleteClick();

    void addItemToControl(const std::string& p_newItem);

	Control::RcFileGrid m_itemsControl;
	Control::Edit m_item;
    std::vector<std::vector<std::string>> m_paths;
    std::string m_initialPath;
};

} // namespace WinApi
