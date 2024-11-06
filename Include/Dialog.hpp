//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#ifndef WINAPI_GRID_DIALOG_HPP
#define WINAPI_GRID_DIALOG_HPP

#include <windows.h>
#include <list>
#include <utility>
#include <functional>
#include "WinApiTypes.hpp"

namespace WinApi
{

class Dialog
{
public:
    typedef std::function<bool(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam)> MsgMatcher;
    typedef std::function<void()> MsgHandler;
	
	static const int RESULT_OK = 2;
	static const int RESULT_CANCEL = 1;

    Dialog(InstanceHandle p_hInstance, Handle p_parent, ResourceId p_resourceId, const std::string& p_title = "");
    virtual ~Dialog() {}
	Dialog(const Dialog&) = delete;
	Dialog& operator=(const Dialog&) = delete;

	/**
	* @throws LastErrorException
	*/
    int show();

protected:
    void registerHandler(MsgMatcher p_matcher, MsgHandler p_handler);

	void setTitle(const std::string& p_title);
    void close(int p_returnCode);
	void redraw();
	Handle getItem(ResourceId p_itemId) const;
	static Handle getFocus();

    InstanceHandle m_hInstance;
	Handle m_parent;
	Handle m_self;

private:
    typedef std::list<std::pair<MsgMatcher, MsgHandler>> HandlersContainer;

    static INT_PTR CALLBACK DialogFun(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    BOOL handleDialogMsg(UINT p_msgCode, WPARAM p_wParam, LPARAM p_lParam);
    BOOL dispatchMessage(UINT p_msgId, WPARAM p_wParam, LPARAM p_lParam);
	BOOL handleContextMenuMsg(LPARAM p_lParam);
	int adjustPosX(int p_x) const;
	int adjustPosY(int p_y) const;
    virtual void onInit() = 0;
	virtual bool showContextMenu(int p_xPos, int p_yPos);
    virtual void onClose();

	const std::string m_defaultTitle;
	const ResourceId m_resourceId;
    HandlersContainer m_msgHandlers;
};

} // namespace WinApi

#endif // WINAPI_GRID_DIALOG_HPP
