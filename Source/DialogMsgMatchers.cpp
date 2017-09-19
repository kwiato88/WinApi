//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "DialogMsgMatchers.hpp"

namespace WinApi
{
namespace MsgMatchers
{

MsgCode::MsgCode(WORD p_expectedMsgCode)
    : m_msgCode(p_expectedMsgCode)
{}

bool MsgCode::operator()(WORD p_msgCode, WORD p_msgValue) const
{
    return m_msgCode == p_msgCode;
}

MsgCode ButtonClick(WORD p_buttonId)
{
	return MsgCode(p_buttonId);
}

MsgCodeAndValue::MsgCodeAndValue(WORD p_expecdedMsgCode, WORD p_expectedMsgValue)
    : m_msgCode(p_expecdedMsgCode), m_msgValue(p_expectedMsgValue)
{}

bool MsgCodeAndValue::operator()(WORD p_msgCode, WORD p_msgValue) const
{
    return m_msgCode == p_msgCode && m_msgValue == p_msgValue;
}

} // namespace MsgMatchers
} // namespace WinApi
