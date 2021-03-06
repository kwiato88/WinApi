//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Shlobj.h>
#include "SelectFilePathDialog.hpp"
#include "Dialog.hpp"
#include "StringConversion.hpp"

namespace WinApi
{

SelectFilePathDialog::SelectFilePathDialog(Handle p_parrent, const std::string& p_comment, const std::string& p_initialDir)
	: m_parrent(p_parrent), m_comment(p_comment), m_initialDir(p_initialDir)
{}

int SelectFilePathDialog::show()
{
	OPENFILENAME l_winApiFilePath;
	TCHAR l_filePath[_MAX_PATH] = TEXT("");
	TCHAR l_title[64] = TEXT("");
	TCHAR l_initialDir[_MAX_PATH] = TEXT("");
	stringToArray(m_comment, l_title);
	stringToArray(m_initialDir, l_initialDir);

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
		m_selectedPath = arrayToString(l_filePath);
		return Dialog::RESULT_OK;
	}
	
	m_selectedPath = std::string();
	return Dialog::RESULT_CANCEL;
}

std::string SelectFilePathDialog::getSelectedPath() const
{
	return m_selectedPath;
}

}
