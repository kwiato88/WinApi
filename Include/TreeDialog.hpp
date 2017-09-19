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
    static const int BUTTON_OK = 1;
    static const int BUTTON_CANCEL = 0;

    TreeDialog(InstanceHandle p_hInstance, Handle p_parentWindow);

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
    void onInit();
    void onOkClick();
    void onCancleClick();

	Control::Tree m_tree;
	const void* m_selectedItemUserContext;
	std::vector<Node> m_treeNodes;
};

} // namespace WinApi
