//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Windows.h>
#include <iostream>

#include "CommonDialogs.hpp"
#include "Clipboard.hpp"
#include "WinApiShellCommand.hpp"
#include "WinApiProcess.hpp"
#include "WinApiLastErrorException.hpp"
#include "StringConversion.hpp"

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
		std::cout << getIndent() << "Finshed " << testName << "\n" << std::endl;
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

	int indent{0};
	std::string testName;
};

Printer out;
WinApi::InstanceHandle hModule;

void listDialog()
{
	out.testStarted("WinApi::ListBoxDialog");

	WinApi::ListBoxDialog dlg(hModule, WinApi::Handle(0), "select item from list", "|");
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

	WinApi::GridDialog dlg(hModule, WinApi::Handle(0), "select row");
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

	WinApi::TreeDialog dlg(hModule, WinApi::Handle(0), "widow title", WinApi::NodeExport::Copy{ "*", ";" });
	int rootCon = 0;
	int child1Con = 1, child2Con = 2, child3Con = 3, child21Con = 4;
	WinApi::Node child21("child 21", &child21Con);
	WinApi::Node child1("child 1", &child1Con), child2("child 2", {child21}, &child2Con), child3("child 3", &child3Con);
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

	WinApi::SelectDirPathDialog dlg(WinApi::Handle(0), "select folder", "D:\\");
	auto code = dlg.show();
	auto selected = dlg.getSelectedPath();

	out.print("return code: ", code);
	out.print("selected path: ", selected);

	out.testFinished();
}

void selectFilePathDailog()
{
	out.testStarted("WinApi::SelectFilePathDialog");

	WinApi::SelectFilePathDialog dlg(WinApi::Handle(0), "select file", "D:\\");
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

void messageDialog()
{
	out.testStarted("WinApi::MessageDialog");

	out.print("message with warn, retry and cancel");
	WinApi::MessageDialog message{ WinApi::Handle() };
	message.withTitle("title");
	message.withContent("something nice");
	message.with(WinApi::MessageDialog::Icon::Warn);
	message.with(WinApi::MessageDialog::Buttons::RetryCancel);
	auto code = message.show();
	out.print("return code: ", WinApi::toStr(code));

	out.print("message with default params");
	WinApi::MessageDialog message2{ WinApi::Handle() };
	code = message2.show();
	out.print("return code: ", WinApi::toStr(code));

	out.print("message with ok");
	WinApi::MessageDialog message3{ WinApi::Handle() };
	message3.withTitle("title");
	message3.withContent("something nice");
	message3.with(WinApi::MessageDialog::Buttons::Ok);
	code = message3.show();
	out.print("return code: ", WinApi::toStr(code));

	out.testFinished();
}

void copyToClipboard()
{
	out.testStarted("copy to clipboard");

	WinApi::Clipboard::set(WinApi::Clipboard::String("Test haha!"));

	out.testFinished();
}

void copyFromClipboard()
{
	out.testStarted("copy from clipboard");

	std::string txt = WinApi::Clipboard::getText();
	
	out.print("text from clipboard: ", txt);
	out.testFinished();
}

void shellCommand1()
{
	out.testStarted("open txt file");
	WinApi::ShellCommand viewText("D:\\test.txt", "");
	viewText.execute();
	out.testFinished();
}

void shellCommand2()
{
	out.testStarted("run ctags.exe");
	WinApi::ShellCommand ctags("D:\\universal_ctags\\ctags.exe", "");
	ctags.execute();
	out.testFinished();

	out.testStarted("run ctags.exe --help");
	WinApi::ShellCommand ctagsHelp("D:\\universal_ctags\\ctags.exe", "--help");
	ctagsHelp.execute();
	out.testFinished();
}

void shellCommanddFailed()
{
	out.testStarted("open unexisting path");
	try
	{
		WinApi::ShellCommand invalidApp("D:\\myProg.exe", "-a asd");
		invalidApp.execute();
	}
	catch (WinApi::Exception& e)
	{
		out.print(e.what());
	}
	out.testFinished();
}

void executeProcess1()
{
	out.testStarted("execute ctags.exe and print output");

	WinApi::Process app("D:\\universal_ctags\\ctags.exe");
	std::string output = "[";
	output += app.execute();
	output += "]";
	out.print("ctags.exe out: ", output);

	out.testFinished();
}

void executeProcess2()
{
	out.testStarted("execute ctags.exe with help and print output");

	WinApi::Process app("D:\\universal_ctags\\ctags.exe --help");
	std::string output = "[";
	try {
		output += app.execute();
		output += "]";
		out.print("ctags.exe out: ", output);
	}
	catch (std::exception& e)
	{
		out.print("Exception: ", e.what());
	}

	out.testFinished();
}

void conversStrings()
{
	out.testStarted("TCHAR[] to std::string");

	TCHAR someStr[100] = TEXT("Hello");
	std::string outStr = WinApi::arrayToString(someStr);
	out.print("Convertes std: ", outStr);

	out.testFinished();
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main()
{
	try
	{
		//hModule = WinApi::InstanceHandle(hInstance);
		/*
		copyFromClipboard();
		copyToClipboard();
		listDialog();
		gridDialog();
		queryDialog();
		treeDialog();
		selectDirPathDailog();
		selectFilePathDailog();
		selectDirPathsDailog();
		selectFilePathsDailog();
		messageDialog();
		shellCommand1();
		shellCommand2();
		shellCommanddFailed();
		executeProcess1();
		executeProcess2();
		*/
		conversStrings();
	}
	catch (std::exception& e)
	{
		out.print("unexpected exception cought: ", e.what());
	}

	return 0;
}
