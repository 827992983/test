#pragma once
#include "stdafx.h"

class CDuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame������"); }
	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			if (msg.pSender->GetName() == _T("btnHello"))
			{
				::MessageBox(NULL, _T("���ǰ�ť"), _T("����˰�ť"), NULL);
			}
		}
	}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)//�Ի������
	{
		LRESULT lRes = 0;
		if (uMsg == WM_CREATE)
		{
			m_PaintManager.Init(m_hWnd);
			CDialogBuilder builder;
			CControlUI* pRoot = builder.Create(_T("XmlUiHelloWorld.xml"), (UINT)0, NULL, &m_PaintManager); // duilib.xml ��Ҫ�ŵ�exe Ŀ¼��
			ASSERT(pRoot && "Failed to parse XML");
			m_PaintManager.AttachDialog(pRoot);
			m_PaintManager.AddNotifier(this);// ��ӿؼ�����Ϣ��Ӧ��������Ϣ�ͻᴫ�ﵽduilib ����Ϣѭ�������ǿ�����Notify ����������Ϣ����
			return lRes;
		}
		// ����3 ����ϢWM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT ��������ϵͳ������
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
