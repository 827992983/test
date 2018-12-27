// HelloWorld.cpp : ����Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "CNotifyPumpDemo.h"


DUI_BEGIN_MESSAGE_MAP(CPage1, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CPage1::CPage1()
{
	m_pPaintManager = NULL;
}

void CPage1::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	m_pPaintManager = pPaintMgr;
}

void CPage1::OnClick(TNotifyUI& msg)
{
	MessageBox(NULL, _T("OnClick"), _T("Cpage1"), MB_OK);
}

DUI_BEGIN_MESSAGE_MAP(CDuiFrameWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

void CDuiFrameWnd::OnClick(TNotifyUI& msg)
{
	MessageBox(NULL, _T("CDuiFrameWnd"), _T("CDuiFrameWnd"), MB_OK);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,
	int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CDuiFrameWnd* pFrame = new CDuiFrameWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("CNotifyPumpDemo ����"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 500, 400);
	pFrame->CenterWindow();
	pFrame->ShowWindow();  // ʹ��ShowWindow���ͽ��룺CPaintManagerUI::MessageLoop()��Ϣѭ����Ҳ������ ::ShowWindow(*pFrame, SW_SHOW);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
