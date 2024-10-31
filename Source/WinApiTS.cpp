//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Windows.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <synchapi.h>

#include "CommonDialogs.hpp"
#include "Clipboard.hpp"
#include "WinApiShellCommand.hpp"
#include "WinApiProcess.hpp"
#include "WinApiLastErrorException.hpp"
#include "StringConversion.hpp"
#include "WinApiThread.hpp"
#include "WinApiMutex.hpp"

WinApi::InstanceHandle hModule;

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
	void wprint(const TCHAR* p)
	{
		std::wcout << getWindent() << p << std::endl;
	}
	void wprint(const TCHAR* p1, const TCHAR* p2)
	{
		std::wcout << getWindent() << p1 << p2 << std::endl;
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
	std::wstring getWindent()
	{
		std::wstring str;
		for (int i = 0; i < indent; ++i)
			str += TEXT("    ");
		return str;
	}

	int indent{0};
	std::string testName;
};

Printer out;

class Tests
{
public:
	typedef std::function<void()> Test;
	struct Matching
	{
		std::string pattern;
	};
	struct NotMatching
	{
		std::string pattern;
	};
	
	void add(const std::string& p_name, Test p_test);
	void runAll();
	void run(Matching p_pattern);
	void run(NotMatching p_pattern);
	void names();

private:
	struct TestUnit
	{
		std::string name;
		Test test;
	};
	void run(const TestUnit& p_test);
	bool matches(const std::string& p_name, const std::string& p_pattern);

	std::list<TestUnit> tests;
};

void Tests::add(const std::string& p_name, Tests::Test p_test)
{
	tests.push_back(TestUnit{ p_name, p_test });
}

void Tests::run(const TestUnit& p_test)
{
	out.testStarted(p_test.name);
	try
	{
		p_test.test();
	}
	catch (std::exception& e)
	{
		out.print("Excpetion during test: ", e.what());
	}
	out.testFinished();
}

void Tests::names()
{
	out.print("Tests names:");
	out.addIndent();
	for (const auto& test : tests)
		out.print(test.name);
	out.removeIndent();
}

void Tests::runAll()
{
	out.print("Runing all tests\n");
	for (const auto& test : tests)
		run(test);
}

bool Tests::matches(const std::string& p_name, const std::string& p_pattern)
{
	return p_name.find(p_pattern) != std::string::npos;
}

void Tests::run(Matching p_pattern)
{
	out.print(std::string("Runing tests matching pattern ") + p_pattern.pattern + "\n");
	for (const auto& test : tests)
		if (matches(test.name, p_pattern.pattern))
			run(test);
}

void Tests::run(NotMatching p_pattern)
{
	out.print(std::string("Runing tests not matching pattern ") + p_pattern.pattern + "\n");
	for (const auto& test : tests)
		if (!matches(test.name, p_pattern.pattern))
			run(test);
}

Tests testSuite;

void listDialog()
{
	WinApi::ListBoxDialog dlg(hModule, WinApi::Handle(0), "select item from list", "|");
	dlg.setItems({ "string 1", "string 2", "string3" });
	auto returnCode = dlg.show();
	auto selected = dlg.getSelectedItemIndex();

	out.print("return code: ", returnCode);
	out.print("selected item: ", selected);
}

void gridDialog()
{
	WinApi::GridDialog dlg(hModule, WinApi::Handle(0), "select row");
	dlg.setTitles({ "col 1", "col2" });
	dlg.setContent({ {"cel 11", "cell 22"}, {"cell21", "cell22"} });
	auto returnCode = dlg.show();
	auto selected = dlg.getSelectedItemIndex();

	out.print("return code: ", returnCode);
	out.print("selected: ", selected);
}

void queryDialog()
{
	WinApi::QueryDialog dlg(hModule, WinApi::Handle(0));
	dlg.setInitialResponse("initial resp");
	dlg.setQuestion("to be or not to be?");
	auto code = dlg.show();
	auto resp = dlg.getResponse();

	out.print("return code: ", code);
	out.print("response: ", resp);
}

void treeDialog()
{
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
}

void selectDirPathDailog()
{
	WinApi::SelectDirPathDialog dlg(WinApi::Handle(0), "select folder", "D:\\");
	auto code = dlg.show();
	auto selected = dlg.getSelectedPath();

	out.print("return code: ", code);
	out.print("selected path: ", selected);
}

void selectFilePathDailog()
{
	WinApi::SelectFilePathDialog dlg(WinApi::Handle(0), "select file", "D:\\");
	auto code = dlg.show();
	auto selected = dlg.getSelectedPath();

	out.print("return code: ", code);
	out.print("selected path: ", selected);
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
	selectPathsDialog<WinApi::SelectorType::Directory>();
}

void selectFilePathsDailog()
{
	selectPathsDialog<WinApi::SelectorType::File>();
}

void messageDialog()
{
	out.print("message with warn, retry and cancel");
	auto code = WinApi::MessageDialog{ WinApi::Handle() }
	.withTitle("title")
	.withContent("something nice")
	.with(WinApi::MessageDialog::Icon::Warn)
	.with(WinApi::MessageDialog::Buttons::RetryCancel)
	.show();
	out.print("return code: ", WinApi::toStr(code));

	out.print("message with default params");
	code = WinApi::MessageDialog{ WinApi::Handle() }
	.show();
	out.print("return code: ", WinApi::toStr(code));

	out.print("message with ok");
	code = WinApi::MessageDialog{ WinApi::Handle() }
	.withTitle("title")
	.withContent("something nice")
	.with(WinApi::MessageDialog::Buttons::Ok)
	.show();
	out.print("return code: ", WinApi::toStr(code));
}

void copyToClipboard()
{
	WinApi::Clipboard::set(WinApi::Clipboard::String("Test haha!"));
}

void copyFromClipboard()
{
	std::string txt = WinApi::Clipboard::getText();
	
	out.print("text from clipboard: ", txt);
}

void shellCommand_OpenFile()
{
	WinApi::ShellCommand viewText("D:\\test.txt", "");
	viewText.execute();
}

void shellCommand_runCtags()
{
	WinApi::ShellCommand ctags("D:\\universal_ctags\\ctags.exe", "");
	ctags.execute();
}

void shellCommand_runCtagsWithHelp()
{
	WinApi::ShellCommand ctagsHelp("D:\\universal_ctags\\ctags.exe", "--help");
	ctagsHelp.execute();
}

void shellCommand_failed()
{
	try
	{
		WinApi::ShellCommand invalidApp("D:\\myProg.exe", "-a asd");
		invalidApp.execute();
	}
	catch (WinApi::Exception& e)
	{
		out.print(e.what());
	}
}

void executeProcess_runCtags()
{
	WinApi::Process app("D:\\universal_ctags\\ctags.exe");
	std::string output = "[";
	output += app.execute();
	output += "]";
	out.print("ctags.exe returned code: ", app.exitCode());
	out.print("ctags.exe out: ", output);
}

void executeProcess_runCtagsWithHelp()
{
	WinApi::Process app("D:\\universal_ctags\\ctags.exe --help");
	std::string output = "[";
	output += app.execute();
	output += "]";
	out.print("ctags.exe returned code: ", app.exitCode());
	out.print("ctags.exe out: ", output);
}

void executeProcess_withoutStdOut()
{
	WinApi::Process app("D:\\universal_ctags\\readtags.exe -t D:\\tags.txt dupa");
	std::string output = "[";
	output += app.execute();
	output += "]";
	out.print("readtags.exe returned code: ", app.exitCode());
	out.print("readtags.exe out: ", output);
}

void executeProcess_async()
{
	WinApi::Process app("D:\\universal_ctags\\ctags.exe --help");
	app.start();

	out.print("is running: ", app.isRunning());
	out.print("code: ", app.exitCode());

	std::string output = "[";
	output += app.wait();
	output += "]";
	out.print("is running: ", app.isRunning());
	out.print("code: ", app.exitCode());
	out.print("out: ", output);
}

void tstringTostring()
{
	TCHAR someStr[100] = TEXT("Hello");
	std::string outStr = WinApi::arrayToString(someStr);
	out.print("Converted std: ", outStr);

	TCHAR *someStr1 = TEXT("Hello");
	std::string outStr1 = WinApi::pointerToString<10>(someStr1);
	out.print("Converted std: ", outStr1);
}

void stringToTstring()
{
	std::string inStr = "Hello";
	TCHAR outStr[10] = TEXT("");
	WinApi::stringToArray(inStr, outStr);
	out.wprint(TEXT("Converted TCHAR: "), outStr);
}

void printInts()
{
	for(int i=0; i<10; ++i)
	{
		std::cout << i;
		Sleep(100);
	}
	std::cout << std::endl;
}
void printChars()
{
	const char* chars = "abcdeghijk";
	for(int i=0; i<10; ++i)
	{
		Sleep(100);
		std::cout << chars[i];
	}
	std::cout << std::endl;
}

void runThreads()
{
	WinApi::Thread a(&printInts), b(&printChars);
	a.start();
	b.start();
	a.join();
	b.join();
}

void terminateThread()
{
	WinApi::Thread a(&printInts);
	a.start();
	Sleep(300);
	a.stop();
}

void lockedPrintInts(WinApi::Mutex& mutex)
{
	WinApi::ScopedLock lock(mutex);
	printInts();
}
void lockedPrintChars(WinApi::Mutex& mutex)
{
	WinApi::ScopedLock lock(mutex);
	printChars();
}
void lockUnlockMutex()
{
	WinApi::Mutex mutex;
	WinApi::Thread a([&](){lockedPrintInts(mutex);}), b([&](){lockedPrintChars(mutex);});
	a.start();
	b.start();
	a.join();
	b.join();
}

bool areParamsOk(int argc, char* argv[])
{
	if (argc == 1)
		return true;
	if (argc == 2)
		return true;
	if (argc == 3)
		return std::string(argv[1]) == "not";
	return false;
}
enum class RunMode
{
	All,
	Matching,
	NotMatching,
	PrintNames,
	Help
};
std::string helpText(const char* p_appName)
{
	std::ostringstream buff;
	buff << "Usage: " << p_appName << " [ testNamePattern | not testNamePattern | printNames | help ]\n";
	return buff.str();
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char* argv[])
{
	if (!areParamsOk(argc, argv))
	{
		std::cerr << "Invalid params" << std::endl;
		std::cerr << helpText(argv[0]) << std::endl;
		return 1;
	}

	std::string testsNamesPattern;
	RunMode mode = RunMode::All;
	if (argc == 2 && std::string(argv[1]) != "printNames")
	{
		mode = RunMode::Matching;
		testsNamesPattern = argv[1];
	}
	if (argc == 2 && std::string(argv[1]) == "printNames")
	{
		mode = RunMode::PrintNames;
	}
	if (argc == 2 && std::string(argv[1]) == "help")
	{
		mode = RunMode::Help;
	}
	if (argc == 3)
	{
		mode = RunMode::NotMatching;
		testsNamesPattern = argv[2];
	}

	if (mode == RunMode::Help)
	{
		std::cout << helpText(argv[0]) << std::endl;
		return 0;
	}
	try
	{
		//hModule = WinApi::InstanceHandle(hInstance);
		
		testSuite.add("copy from clipboard", &copyFromClipboard);
		testSuite.add("copy to clipboard", &copyToClipboard);
		testSuite.add("WinApi::ListBoxDialog", &listDialog);
		testSuite.add("WinApi::GridDialog", &gridDialog);
		testSuite.add("WinApi::QueryDialog", &queryDialog);
		testSuite.add("WinApi::TreeDialog", &treeDialog);
		testSuite.add("WinApi::SelectDirPathDialog", &selectDirPathDailog);
		testSuite.add("WinApi::SelectFilePathDialog", &selectFilePathDailog);
		testSuite.add("WinApi::SelectPathsDialog<WinApi::SelectorType::Directory>", &selectDirPathsDailog);
		testSuite.add("WinApi::SelectPathsDialog<WinApi::SelectorType::File>", &selectFilePathsDailog);
		testSuite.add("WinApi::MessageDialog", &messageDialog);
		testSuite.add("WinApi::ShellCommand: open txt file", &shellCommand_OpenFile);
		testSuite.add("WinApi::ShellCommand: run ctags.exe", &shellCommand_runCtags);
		testSuite.add("WinApi::ShellCommand: run ctags.exe with help", &shellCommand_runCtagsWithHelp);
		testSuite.add("WinApi::ShellCommand: failed execution", &shellCommand_failed);
		testSuite.add("WinApi::Process: run ctags.exe", &executeProcess_runCtags);
		testSuite.add("WinApi::Process: run ctags.exe with help", &executeProcess_runCtagsWithHelp);
		testSuite.add("WinApi::Process: run readtags.exe with with no output", &executeProcess_withoutStdOut);
		testSuite.add("WinApi::Process: run async", &executeProcess_async);
		testSuite.add("convert string: tstr -> str", &tstringTostring);
		testSuite.add("convert string: str -> tstr", &stringToTstring);
		testSuite.add("WinApi::Thread: start threads", &runThreads);
		testSuite.add("WinApi::Thread: stop threads", &terminateThread);
		testSuite.add("WinApi::Thread: use mutex", &lockUnlockMutex);
		if (mode == RunMode::PrintNames)
			testSuite.names();
		if(mode == RunMode::All)
			testSuite.runAll();
		if(mode == RunMode::Matching)
			testSuite.run(Tests::Matching{ testsNamesPattern });
		if(mode == RunMode::NotMatching)
			testSuite.run(Tests::NotMatching{ testsNamesPattern });
	}
	catch (std::exception& e)
	{
		out.print("unexpected exception cought: ", e.what());
	}

	return 0;
}
