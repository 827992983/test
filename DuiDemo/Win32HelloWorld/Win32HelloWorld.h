#pragma once

#pragma once

#include <windows.h>
#include <atltypes.h>
#include <tchar.h>

//��ԴID
#define ID_BUTTON_DRAW      1000
#define ID_BUTTON_SWEEP     1001

// ע�ᴰ����
ATOM AppRegisterClass(HINSTANCE hInstance);
// ��ʼ������
BOOL InitInstance(HINSTANCE, int);
// ��Ϣ������(�ֽд��ڹ���)
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
// (��ɫ����)��ť�¼�
void OnButtonWhite();
// (��ɫ����)��ť�¼�
void OnButtonGray();
// �����¼�
void OnDraw(HDC hdc);

