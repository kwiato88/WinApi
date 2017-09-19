#include "WinApiFilePathGetter.hpp"
#include "SelectFilePathDialog.hpp"

namespace WinApi
{

FilePathGetter::FilePathGetter(Handle p_parrent)
    : m_parrent(p_parrent)
{}

std::string FilePathGetter::getPath(const std::string& p_comment, const std::string& p_initialDir) const
{
	SelectFilePathDialog dialog(m_parrent, p_comment, p_initialDir);
	dialog.show();
	return dialog.getSelectedPath();
}

} // namespace WinApi
