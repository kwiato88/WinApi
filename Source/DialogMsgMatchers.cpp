//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "DialogMsgMatchers.hpp"

namespace WinApi
{
namespace MsgMatchers
{

MsgCode::MsgCode(WORD p_expectedMsgCode)
    : m_msgCode(p_expectedMsgCode)
{}

bool MsgCode::operator()(UINT p_msgId, WPARAM p_wParam, LPARAM) const
{
    return p_msgId == WM_COMMAND && m_msgCode == LOWORD(p_wParam);
}

MsgCode ButtonClick(WORD p_buttonId)
{
	return MsgCode(p_buttonId);
}

MsgCodeAndValue::MsgCodeAndValue(WORD p_expecdedMsgCode, WORD p_expectedMsgValue)
    : m_msgCode(p_expecdedMsgCode), m_msgValue(p_expectedMsgValue)
{}

bool MsgCodeAndValue::operator()(UINT p_msgId, WPARAM p_wParam, LPARAM) const
{
    return p_msgId == WM_COMMAND && m_msgCode == LOWORD(p_wParam) && m_msgValue == HIWORD(p_wParam);
}

AllOf::AllOf(std::initializer_list<std::function<bool(UINT, WPARAM, LPARAM)>> p_matchers)
    : m_matchers(p_matchers)
{}

bool AllOf::operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const
{
    for (const auto& matcher : m_matchers)
    {
        if (!matcher(p_msgId, p_wParam, p_lParam))
            return false;
    }
    return true;
}

NotifyCode::NotifyCode(UINT p_code)
    : m_code(p_code)
{}

bool NotifyCode::operator()(UINT p_msgId, WPARAM, LPARAM p_lParam) const
{
    return p_msgId == WM_NOTIFY && ((LPNMHDR)p_lParam)->code == m_code;
}

} // namespace MsgMatchers
} // namespace WinApi
