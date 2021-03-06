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
			m_PaintManager.Init(m_hWnd);
			CDialogBuilder builder;
			CControlUI* pRoot = builder.Create(_T("XmlUiHelloWorld.xml"), (UINT)0, NULL, &m_PaintManager); // duilib.xml 需要放到exe 目录下
			ASSERT(pRoot && "Failed to parse XML");
			m_PaintManager.AttachDialog(pRoot);
			m_PaintManager.AddNotifier(this);// 添加控件等消息响应，这样消息就会传达到duilib 的消息循环，我们可以在Notify 函数里做消息处理
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
	
protected:
	CPaintManagerUI m_PaintManager;
};
