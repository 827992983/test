#include "stdafx.h"
#include "XmlUiHelloWorld.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,
	int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());// ������Դ��Ĭ��·�����˴�����Ϊ��exe ��ͬһĿ¼��
	CDuiFrameWnd duiFrame;
	duiFrame.Create(NULL, _T("DUIWnd����"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();
	return 0;
}
