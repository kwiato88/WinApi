//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <Shlobj.h>
#include "SelectDirPathDialog.hpp"

namespace WinApi
{

SelectDirPathDialog::SelectDirPathDialog(Handle p_parrent, const std::string& p_comment, const std::string& p_initialDir)
	: m_parrent(p_parrent), m_comment(p_comment), m_initialDir(p_initialDir)
{}

static int CALLBACK browseCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData != NULL)
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	return 0;
}

int SelectDirPathDialog::show()
{
	BROWSEINFO l_winApiDirPath;
	TCHAR l_dirPath[MAX_PATH] = TEXT("");
	TCHAR l_initialDir[_MAX_PATH] = TEXT("");
	TCHAR l_comment[64] = TEXT("");
	mbstowcs(l_comment, m_comment.c_str(), 63);
	mbstowcs(l_initialDir, m_initialDir.c_str(), _MAX_PATH - 1);

	ZeroMemory(&l_winApiDirPath, sizeof(l_winApiDirPath));
	l_winApiDirPath.hwndOwner = m_parrent;
	l_winApiDirPath.pszDisplayName = l_dirPath;
	l_winApiDirPath.lpszTitle = l_comment;
	l_winApiDirPath.iImage = -1;
	if (!m_initialDir.empty())
	{
		l_winApiDirPath.lpfn = browseCallback;
		l_winApiDirPath.lParam = (LPARAM)l_initialDir;
	}

	LPITEMIDLIST lpItem = SHBrowseForFolder(&l_winApiDirPath);
	if (lpItem != NULL)
	{
		if (SHGetPathFromIDList(lpItem, l_dirPath))
		{
			char l_dirPathBuff[_MAX_PATH];
			wcstombs(l_dirPathBuff, l_dirPath, _MAX_PATH - 1);
			m_selectedPath = l_dirPathBuff;
			return BUTTON_OK;
		}
	}

	m_selectedPath = std::string();
	return BUTTON_CANCEL;
}

std::string SelectDirPathDialog::getSelectedPath()
{
	return m_selectedPath;
}

}
