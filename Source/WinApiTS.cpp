#include <Windows.h>
#include <iostream>

#include "CommonDialogs.hpp"

class Printer
{
public:
	void addIndent()
	{
		++indent;
	}
	void removeIndent()
	{
		if (indent > 0)
			--indent;
	}
	void testStarted(const std::string& p_name)
	{
		testName = p_name;
		std::cout << getIndent() << "Start " << testName << std::endl;
		addIndent();
	}
	template<typename T>
	void print(const T& p)
	{
		std::cout << getIndent() << p << std::endl;
	}
	template<typename T1, typename T2>
	void print(const T1& p1, const T2& p2)
	{
		std::cout << getIndent() << p1 << p2 << std::endl;
	}
	void testFinished()
	{
		removeIndent();
		std::cout << getIndent() << "Finshed " << testName << std::endl;
		std::cout << std::endl;
	}
private:
	std::string getIndent()
	{
		std::string str;
		for (int i = 0; i < indent; ++i)
			str += "    ";
		return str;
	}

	int indent;
	std::string testName;
};

Printer out;
WinApi::InstanceHandle hModule;

void listDialog()
{
	out.testStarted("WinApi::ListBoxDialog");

	WinApi::ListBoxDialog dlg(hModule, WinApi::Handle(0));
	dlg.setItems({ "string 1", "string 2", "string3" });
	auto returnCode = dlg.show();
	auto selected = dlg.getSelectedItemIndex();

	out.print("return code: ", returnCode);
	out.print("selected item: ", selected);
	out.testFinished();
}

void gridDialog()
{
	out.testStarted("WinApi::GridDialog");

	WinApi::GridDialog dlg(hModule, WinApi::Handle(0));
	dlg.setTitles({ "col 1", "col2" });
	dlg.setContent({ {"cel 11", "cell 22"}, {"cell21", "cell22"} });
	auto returnCode = dlg.show();
	auto selected = dlg.getSelectedItemIndex();

	out.print("return code: ", returnCode);
	out.print("selected: ", selected);
	out.testFinished();
}

void queryDialog()
{
	out.testStarted("WinApi::QueryDialog");

	WinApi::QueryDialog dlg(hModule, WinApi::Handle(0));
	dlg.setInitialResponse("initial resp");
	dlg.setQuestion("to be or not to be?");
	auto code = dlg.show();
	auto resp = dlg.getResponse();

	out.print("return code: ", code);
	out.print("response: ", resp);
	out.testFinished();
}

void treeDialog()
{
	out.testStarted("WinApi::TreeDialog");

	WinApi::TreeDialog dlg(hModule, WinApi::Handle(0));
	int rootCon = 0;
	int child1Con = 1, child2Con = 2, child3Con = 3, child21Con = 4;
	WinApi::Node child21("child 21", &child21Con);
	WinApi::Node child1("child 1", &child1Con), child2("child 2", {child21}, &child1Con), child3("child 2", &child2Con);
	WinApi::Node root("root", { child1, child2, child3 }, &rootCon);
	dlg.setTreeNodes({ root });
	
	auto code = dlg.show();
	const int* selectedCon = dlg.getSelectedItemContext<int>();

	out.print("return code: ", code);
	out.print("selected item context: ", (selectedCon ? *selectedCon : int(99)));
	out.testFinished();
}

void selectDirPathDailog()
{
	out.testStarted("WinApi::SelectDirPathDialog");

	WinApi::SelectDirPathDialog dlg(WinApi::Handle(0), "comment", "D:\\");
	auto code = dlg.show();
	auto selected = dlg.getSelectedPath();

	out.print("return code: ", code);
	out.print("selected path: ", selected);

	out.testFinished();
}

void selectFilePathDailog()
{
	out.testStarted("WinApi::SelectFilePathDialog");

	WinApi::SelectFilePathDialog dlg(WinApi::Handle(0), "comment", "D:\\");
	auto code = dlg.show();
	auto selected = dlg.getSelectedPath();

	out.print("return code: ", code);
	out.print("selected path: ", selected);

	out.testFinished();
}

template <WinApi::SelectorType DialogType>
void selectPathsDialog()
{
	WinApi::SelectPathsDialog<DialogType> dlg(hModule, WinApi::Handle(0));
	dlg.setInitalPath("D:\\");
	dlg.setPaths({});
	auto code = dlg.show();
	auto result = dlg.getPaths();

	out.print("return code: ", code);
	out.print("selcted paths:");
	out.addIndent();
	out.print("count: ", result.size());
	for (const auto& i : result)
		out.print(i);
	out.removeIndent();
}
void selectDirPathsDailog()
{
	out.testStarted("WinApi::SelectPathsDialog<WinApi::SelectorType::Directory>");
	selectPathsDialog<WinApi::SelectorType::Directory>();
	out.testFinished();
}

void selectFilePathsDailog()
{
	out.testStarted("WinApi::SelectPathsDialog<WinApi::SelectorType::File>");
	selectPathsDialog<WinApi::SelectorType::File>();
	out.testFinished();
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main()
{
	try
	{
		//hModule = WinApi::InstanceHandle(hInstance);
		listDialog();
		gridDialog();
		queryDialog();
		treeDialog();
		selectDirPathDailog();
		selectFilePathDailog();
		selectDirPathsDailog();
		selectFilePathsDailog();
	}
	catch (std::exception& e)
	{
		out.print("unexpected exception cought: ", e.what());
	}

	return 0;
}