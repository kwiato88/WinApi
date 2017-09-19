#include "WinApiDirPathGetter.hpp"
#include "SelectDirPathDialog.hpp"

namespace WinApi
{

DirPathGetter::DirPathGetter(Handle p_parrent)
    : m_parrent(p_parrent)
{}

std::string DirPathGetter::getPath(const std::string& p_comment, const std::string& p_initialDir) const
{
	SelectDirPathDialog dialog(m_parrent, p_comment, p_initialDir);
	dialog.show();
	return dialog.getSelectedPath();
}

} // namespace WinApi
