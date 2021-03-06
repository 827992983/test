#pragma once
#include "stdafx.h"

class CDuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame测试类"); }
	virtual void Notify(TNotifyUI& msg) 
	{
		if (msg.sType == _T("click"))
		{
			if (msg.pSender->GetName() == _T("btnHello"))
			{
				::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
			}
		}
	}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)//自绘标题栏
	{
		LRESULT lRes = 0;
		if (uMsg == WM_CREATE)
		{
			CControlUI *pWnd = new CButtonUI;
			pWnd->SetName(_T("btnHello")); // 设置控件的名称，这个名称用于标识每一个控件，必须唯一，相当于MFC 里面的控件ID
			pWnd->SetText(_T("Hello World 窗口")); // 设置文字
			pWnd->SetBkColor(0xFFD1EE00); // 设置背景色
			m_PaintManager.Init(m_hWnd);
			m_PaintManager.AttachDialog(pWnd);
			m_PaintManager.AddNotifier(this); // 添加控件等消息响应，这样消息就会传达到duilib 的消息循环，我们可以在Notify 函数里做消息处理
			return lRes;
		}
		// 以下3 个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT 用于屏蔽系统标题栏
		else if (uMsg == WM_NCACTIVATE)
		{
			if (!::IsIconic(m_hWnd))
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}
		else if (uMsg == WM_NCCALCSIZE)
		{
			return 0;
		}
		else if (uMsg == WM_NCPAINT)
		{
			return 0;
		}
		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	virtual LRESULT HandleMessage_backup(UINT uMsg, WPARAM wParam, LPARAM lParam)//按钮鼠标单击事件
	{
		LRESULT lRes = 0;
		if (uMsg == WM_CREATE)
		{
			CControlUI *pWnd = new CButtonUI;
			pWnd->SetName(_T("btnHello")); // 设置控件的名称，这个名称用于标识每一个控件，必须唯一，相当于MFC 里面的控件ID
			pWnd->SetText(_T("Hello World 窗口")); // 设置文字
			pWnd->SetBkColor(0xFF00FF00); // 设置背景色
			m_PaintManager.Init(m_hWnd);
			m_PaintManager.AttachDialog(pWnd);
			m_PaintManager.AddNotifier(this); // 添加控件等消息响应，这样消息就会传达到duilib 的消息循环，我们可以在Notify 函数里做消息处理
			return lRes;
		}
		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
protected:
	CPaintManagerUI m_PaintManager;
};
