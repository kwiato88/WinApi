//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#ifndef WINAPI_DIALOGMSGMATCHERS_HPP
#define WINAPI_DIALOGMSGMATCHERS_HPP

#include <Windows.h>
#include <commctrl.h>
#include <functional>

namespace WinApi
{
namespace MsgMatchers
{

class MsgCode
{
public:
    MsgCode(WORD p_expectedMsgCode);
    bool operator()(WORD p_msgCode, WORD p_msgValue) const;
private:
    WORD m_msgCode;
};

MsgCode ButtonClick(WORD p_buttonId);

class MsgCodeAndValue
{
public:
    MsgCodeAndValue(WORD p_expecdedMsgCode, WORD p_expectedMsgValue);
    bool operator()(WORD p_msgCode, WORD p_msgValue) const;
private:
    WORD m_msgCode;
    WORD m_msgValue;
};

template <typename T>
class NotifyFromObject
{
public:
	NotifyFromObject(const T& p_relatedObj)
		: m_relatedObject(p_relatedObj){}
	bool operator()(LPARAM p_lParam) const
	{
		return m_relatedObject.isOwnHandle(Handle(((LPNMHDR)p_lParam)->hwndFrom));
	}
private:
	const T& m_relatedObject;
};

template<typename T>
NotifyFromObject<T> notifyFromObject(const T& p_relatedObject)
{
	return NotifyFromObject<T>(p_relatedObject);
}

template <typename T>
class NotifyFromObjectWithCode
{
public:
    NotifyFromObjectWithCode(const T& p_relatedObj, UINT p_code, std::function<bool(LPARAM)> p_additionalMatcher)
        : m_relatedObject(p_relatedObj), m_code(p_code), m_additionalMatcher(p_additionalMatcher) {}
    bool operator()(LPARAM p_lParam) const
    {
        return m_relatedObject.isOwnHandle(Handle(((LPNMHDR)p_lParam)->hwndFrom))
            && (((LPNMHDR)p_lParam)->code == m_code)
            && m_additionalMatcher(p_lParam);
    }
private:
    const T& m_relatedObject;
    UINT m_code;
    std::function<bool(LPARAM)> m_additionalMatcher;
};

template<typename T>
NotifyFromObjectWithCode<T> DoubleClick(const T& p_relatedObj)
{
    return NotifyFromObjectWithCode<T>(p_relatedObj, NM_DBLCLK, [](LPARAM){return true;});
}

template<typename T>
NotifyFromObjectWithCode<T> KeyDown(const T& p_relatedObj, WORD p_keyCode)
{
    return NotifyFromObjectWithCode<T>(p_relatedObj, LVN_KEYDOWN,
        [=](LPARAM p_lparam)
        {
            return (((LPNMLVKEYDOWN)p_lparam)->wVKey == p_keyCode);
        });
}

template<typename T>
NotifyFromObjectWithCode<T> AnyKeyDown(const T& p_relatedObj)
{
	return NotifyFromObjectWithCode<T>(p_relatedObj, LVN_KEYDOWN, [](LPARAM p_lparam){return true;});
}

} // namespace MsgMatchers
} // namespace WinApi

#endif
