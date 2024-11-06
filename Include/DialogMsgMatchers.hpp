//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#ifndef WINAPI_DIALOGMSGMATCHERS_HPP
#define WINAPI_DIALOGMSGMATCHERS_HPP

#include <Windows.h>
#include <commctrl.h>
#include <functional>
#include "WinApiTypes.hpp"

namespace WinApi
{
namespace MsgMatchers
{

using Matcher = std::function<bool(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam)>;

class Message
{
public:
    Message(UINT p_msgId);
    bool operator()(UINT p_msgId, WPARAM, LPARAM) const;
private:
    UINT m_msgId;
};

class CmdCode
{
public:
    CmdCode(WORD p_expectedMsgCode);
    bool operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const;
private:
    WORD m_msgCode;
};

CmdCode ButtonClick(WORD p_buttonId);

class CmdCodeAndValue
{
public:
    CmdCodeAndValue(WORD p_expecdedMsgCode, WORD p_expectedMsgValue);
    bool operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const;
private:
    WORD m_msgCode;
    WORD m_msgValue;
};

class AllOf
{
public:
    AllOf(std::initializer_list<Matcher> p_matchers);
    bool operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const;
private:
    std::vector<Matcher> m_matchers;
};

template <typename T>
class NotifyFromObject
{
public:
    NotifyFromObject(const T& p_relatedObj)
        : m_relatedObject(p_relatedObj){}
    bool operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const
    {
        return p_msgId == WM_NOTIFY
            && m_relatedObject.isOwnHandle(Handle{((LPNMHDR)p_lParam)->hwndFrom});
    }
private:
    const T& m_relatedObject;
};

template<typename T>
NotifyFromObject<T> notifyFromObject(const T& p_relatedObject)
{
	return NotifyFromObject<T>(p_relatedObject);
}

class NotifyCode
{
public:
    NotifyCode(UINT p_code);
    bool operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const;
private:
    UINT m_code;
};

template<typename T>
Matcher DoubleClick(const T& p_relatedObj)
{
    return AllOf({notifyFromObject(p_relatedObj), NotifyCode(NM_DBLCLK)});
}

template<typename T>
Matcher NotifyAnyKeyDown(const T& p_relatedObj)
{
    return AllOf({notifyFromObject(p_relatedObj), NotifyCode(LVN_KEYDOWN)});
}

template<typename T>
Matcher NotifyKeyDown(const T& p_relatedObj, WORD p_keyCode)
{
    return AllOf({
               notifyFromObject(p_relatedObj),
               NotifyCode(LVN_KEYDOWN),
               [=](UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam)
                {
                    return (((LPNMLVKEYDOWN)p_lParam)->wVKey == p_keyCode);
                }
            });
}

class MsgKeyDown
{
public:
    MsgKeyDown(WPARAM p_keyCode);
    bool operator()(UINT p_msgId, WPARAM, LPARAM p_lParam) const;
private:
    WPARAM m_keyCode;
};

template<typename T>
class Focused
{
public:
    Focused(const T& p_relatedObj)
        : m_relatedObject(p_relatedObj){}
    bool operator()(UINT, WPARAM, LPARAM) const
    {
        return m_relatedObject.isFocused();
    }
private:
    const T& m_relatedObject;
};

template <typename ControlT>
Matcher MsgKeyDownOnControl(const ControlT& p_control, WPARAM p_keyCode)
{
    return AllOf({Focused<ControlT>(p_control), MsgKeyDown(p_keyCode)});
}

} // namespace MsgMatchers
} // namespace WinApi

#endif
