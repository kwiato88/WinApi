//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include <boost/lexical_cast.hpp>
#include <windowsx.h>
#include "Dialog.hpp"
#include "WinApiLastErrorException.hpp"

namespace WinApi
{

Handle Dialog::getFocus()
{
	return Handle(GetFocus());
}

Dialog::Dialog(InstanceHandle p_hInstance, Handle p_parent, ResourceId p_resourceId, const std::string& p_title)
    : m_hInstance(p_hInstance), m_parent(p_parent), m_self(0), m_resourceId(p_resourceId), m_defaultTitle(p_title)
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
			if (!l_dlg->m_defaultTitle.empty())
				l_dlg->setTitle(l_dlg->m_defaultTitle);
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
    if(p_msgCode == WM_NOTIFY)
        return dispatchNotifyMsg(p_lParam);
	if (p_msgCode == WM_CONTEXTMENU)
	{
		showContextMenu(adjustPosX(GET_X_LPARAM(p_lParam)), adjustPosY(GET_Y_LPARAM(p_lParam)));
		return TRUE;
	}
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

int Dialog::adjustPosX(int p_x)
{
	if (p_x != -1)
		return p_x;
	RECT rec;
	if (GetWindowRect(m_self, &rec) == 0)
		return 0;
	return rec.left;
}

int Dialog::adjustPosY(int p_y)
{
	if (p_y != -1)
		return p_y;
	RECT rec;
	if (GetWindowRect(m_self, &rec) == 0)
		return 0;
	return rec.top;
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
    auto exitCode = DialogBoxParam(
        m_hInstance,                     // app instance: HISTANCE
        MAKEINTRESOURCE( m_resourceId ), // dialog template id
        m_parent,                        // parent: HWND
        DialogFun,                       // dialog func
        reinterpret_cast<LPARAM>(this)
    );
	if (exitCode == -1 || exitCode == 0)
		throw LastErrorException("Failed to show dialog. Exit code: " + boost::lexical_cast<std::string>(exitCode));
	return exitCode;
}

void Dialog::close(int p_returnCode)
{
    EndDialog(m_self, p_returnCode);
}

void Dialog::redraw()
{
	InvalidateRect(m_self, NULL, true);
	UpdateWindow(m_self);
}

void Dialog::setTitle(const std::string& p_title)
{
	TCHAR title[_MAX_PATH] = TEXT("");
	mbstowcs(title, p_title.c_str(), _MAX_PATH - 1);
	SetWindowText(m_self, title);
}

Handle Dialog::getItem(ResourceId p_itemId)
{
    return Handle(GetDlgItem(m_self, p_itemId));
}

void Dialog::showContextMenu(int, int)
{
}

} // namespace WinApi
