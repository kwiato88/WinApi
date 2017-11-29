//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Shlobj.h>
#include "SelectFilePathDialog.hpp"
#include "Dialog.hpp"

namespace WinApi
{

SelectFilePathDialog::SelectFilePathDialog(Handle p_parrent, const std::string& p_comment, const std::string& p_initialDir)
	: m_parrent(p_parrent), m_comment(p_comment), m_initialDir(p_initialDir)
{}

int SelectFilePathDialog::show()
{
	OPENFILENAME l_winApiFilePath;
	TCHAR l_filePath[_MAX_PATH] = TEXT("");
	TCHAR l_title[12] = TEXT("");
	TCHAR l_initialDir[_MAX_PATH] = TEXT("");
	mbstowcs(l_title, m_comment.c_str(), 11);
	mbstowcs(l_initialDir, m_initialDir.c_str(), _MAX_PATH - 1);

	ZeroMemory(&l_winApiFilePath, sizeof(l_winApiFilePath));
	l_winApiFilePath.lStructSize = sizeof(l_winApiFilePath);
	l_winApiFilePath.hwndOwner = m_parrent;
	l_winApiFilePath.lpstrFilter = TEXT("All Files\0*.*\0\0");
	l_winApiFilePath.nMaxFile = _MAX_PATH;
	l_winApiFilePath.lpstrFile = l_filePath;
	l_winApiFilePath.lpstrDefExt = TEXT("");
	l_winApiFilePath.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT; //https://msdn.microsoft.com/pl-pl/library/windows/desktop/ms646839(v=vs.85).aspx
	l_winApiFilePath.lpstrTitle = l_title;
	if (!m_initialDir.empty())
		l_winApiFilePath.lpstrInitialDir = l_initialDir;

	if (GetOpenFileName(&l_winApiFilePath))
	{
		char l_filePathBuff[_MAX_PATH];
		wcstombs(l_filePathBuff, l_filePath, _MAX_PATH - 1);
		m_selectedPath = l_filePathBuff;
		return Dialog::RESULT_OK;
	}
	
	m_selectedPath = std::string();
	return Dialog::RESULT_CANCEL;
}

std::string SelectFilePathDialog::getSelectedPath()
{
	return m_selectedPath;
}

}
