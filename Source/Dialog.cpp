#include "Dialog.hpp"

namespace WinApi
{

Handle Dialog::getFocus()
{
	return Handle(GetFocus());
}

Dialog::Dialog(InstanceHandle p_hInstance, Handle p_parent, ResourceId p_resourceId)
    : m_hInstance(p_hInstance), m_parent(p_parent), m_self(0), m_resourceId(p_resourceId)
{
}

BOOL CALLBACK Dialog::DialogFun(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    Dialog *l_dlg=reinterpret_cast<Dialog*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (!l_dlg)
    {
        if (Msg == WM_INITDIALOG)
        {
            l_dlg = reinterpret_cast<Dialog*>(lParam);
            l_dlg->m_self = hwnd;
            l_dlg->onInit();
            SetWindowLongPtr(hwnd,GWLP_USERDATA,lParam);
        }
        else
        {
            return 0; //let system deal with message
        }
    }
    //forward message to member function handler
    return l_dlg->handleDialogMsg(Msg, wParam, lParam);
}

BOOL Dialog::handleDialogMsg(UINT p_msgCode, WPARAM p_wParam, LPARAM p_lParam)
{
    if(p_msgCode == WM_COMMAND)
        return dispatchCommandMsg(p_wParam);
    else if(p_msgCode == WM_NOTIFY)
        return dispatchNotifyMsg(p_lParam);
    return FALSE;
}

BOOL Dialog::dispatchCommandMsg(WPARAM p_wParam)
{
    for(const auto& handler : m_commandMsgHandlers)
    {
        if(handler.first(LOWORD(p_wParam), HIWORD(p_wParam)))
        {
            handler.second();
            return TRUE;
        }
    }
    return FALSE;
}

void Dialog::registerHandler(CommandMsgMatcher p_matcher, MsgHandler p_handler)
{
    m_commandMsgHandlers.push_back(std::make_pair(p_matcher, p_handler));
}

void Dialog::registerHandler(NotifyMsgMatcher p_matcher, MsgHandler p_handler)
{
    m_notifyMsgHandlers.push_back(std::make_pair(p_matcher, p_handler));
}

BOOL Dialog::dispatchNotifyMsg(LPARAM p_lParam)
{
    for(const auto& handler : m_notifyMsgHandlers)
    {
        if(handler.first(p_lParam))
        {
            handler.second();
            return TRUE;
        }
    }
    return FALSE;
}

int Dialog::show()
{
    return DialogBoxParam(
        m_hInstance,                     // app instance: HISTANCE
        MAKEINTRESOURCE( m_resourceId ), // dialog template id
        m_parent,                        // parent: HWND
        DialogFun,                       // dialog func
        reinterpret_cast<LPARAM>(this)
    );
}

void Dialog::close(int p_returnCode)
{
    EndDialog(m_self, p_returnCode);
}

Handle Dialog::getItem(ResourceId p_itemId)
{
    return Handle(GetDlgItem(m_self, p_itemId));
}

} // namespace WinApi
