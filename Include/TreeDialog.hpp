//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#pragma once

#include <string>
#include <vector>

#include "Dialog.hpp"
#include "ControlTree.hpp"
#include "Node.hpp"

namespace WinApi
{

class TreeDialog : public Dialog
{
public:
    TreeDialog(InstanceHandle p_hInstance, Handle p_parentWindow,
		const std::string& p_title = "Select tree item", const std::string& p_exportIndentStr = "\t");

	void setTreeNodes(const std::vector<Node>& p_tree);

	template<typename T>
	const T* getSelectedItemContext() const
	{
		return static_cast<const T*>(m_selectedItemUserContext);
	}
	template<typename T>
	const T& getSelectedItemContextRef() const
	{
		const T* context = getSelectedItemContext<T>();
		if (context == nullptr)
			throw std::runtime_error("Can't cast to ref. User context is null.");
		return *context;
	}

private:
    void onInit() override;
	void showContextMenu(int p_xPos, int p_yPos) override;
    void onOkClick();
    void onCancleClick();
	void copyAll();
	void copySelectedSubTree();
	void copySelected();

	Control::Tree m_tree;
	const void* m_selectedItemUserContext;
	std::vector<Node> m_treeNodes;
	std::string indent;
};

} // namespace WinApi
