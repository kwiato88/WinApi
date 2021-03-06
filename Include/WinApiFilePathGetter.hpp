//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#ifndef WINAPI_FILEPATHGETTER_HPP
#define WINAPI_FILEPATHGETTER_HPP

#include <string>
#include "WinApiTypes.hpp"

namespace WinApi
{

class FilePathGetter
{
public:
    FilePathGetter(Handle p_parrent);

    std::string getPath(const std::string& p_comment, const std::string& p_initialDir) const;

private:
    Handle m_parrent;
};

} // namespace WinApi

#endif
