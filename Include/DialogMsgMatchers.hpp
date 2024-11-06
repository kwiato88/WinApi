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

class MsgCode
{
public:
    MsgCode(WORD p_expectedMsgCode);
    bool operator()(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam) const;
private:
    WORD m_msgCode;
};

MsgCode ButtonClick(WORD p_buttonId);

class MsgCodeAndValue
{
public:
    MsgCodeAndValue(WORD p_expecdedMsgCode, WORD p_expectedMsgValue);
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
Matcher AnyKeyDown(const T& p_relatedObj)
{
    return AllOf({notifyFromObject(p_relatedObj), NotifyCode(LVN_KEYDOWN)});
}

template<typename T>
Matcher KeyDown(const T& p_relatedObj, WORD p_keyCode)
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

} // namespace MsgMatchers
} // namespace WinApi

#endif
