//-----------------------------------------------------------------------------
// File: CreateDevice.cpp
//
// Desc: This is the first tutorial for using Direct3D. In this tutorial, all
//       we are doing is creating a Direct3D device and using it to clear the
//       window.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <d3d9.h>
#pragma warning( disable : 4996 ) // ���ù�ʱ����
#include <strsafe.h>
#pragma warning( default : 4996 )




//-----------------------------------------------------------------------------
// ȫ�ֱ���
//-----------------------------------------------------------------------------
LPDIRECT3D9         g_pD3D = NULL; // ���ڴ���D3D�豸
LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // ���ǵ���Ⱦ�豸




										 //-----------------------------------------------------------------------------
										 // Name: InitD3D()
										 // Desc: ��ʼ�� Direct3D
										 //-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
	// ����D3D�Ķ���������Ҫ����D3DDevice��
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Most parameters are
	// zeroed out. We set Windowed to TRUE, since we want to do D3D in a
	// window, and then set the SwapEffect to "discard", which is the most
	// efficient method of presenting the back buffer to the display.  And 
	// we request a back buffer format that matches the current desktop display 
	// format.
	/*ZeroMemory()��ZeroMemory����0�����һ���ڴ�����
	����Ϊ�˱����Ż��������������Ӱ�죬��ʹ��SecureZeroMemory������
	  ����void ZeroMemory(
		����PVOID Destination,
		  ����SIZE_T Length
			����);
			  ����������
				����Destination :ָ��һ��׼����0�������ڴ�����Ŀ�ʼ��ַ��
				  ����Length :׼����0�������ڴ�����Ĵ�С�����ֽ������㡣
					��������ֵ����
					  ����ZeroMemory ֻ�ǽ�ָ�����ڴ������.
						����ʹ�ýṹǰ����, �����ýṹ�ĳ�Ա��ֵ���в�ȷ����, ��һ���õı��ϰ��
						  */

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;    //����ȫ��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	//����Direct3D �豸. ���������ʹ��Ĭ�ϵ������� (�������
	// ϵͳֻ��һ���Կ�, ���������ж࿨������
	//  ����ҪӲ������� (��������ϣ������Ӳ���豸�������
	// ��������֪���������еĿ��϶�����ʹ�õ�ʱ������
	//��ʹ��������㴦�� ������֧��Ӳ�����㴦����Կ������������ϵĽϴ�����

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// �豸״̬ͨ�����趨������

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: �ͷ�Ԥ�ȳ�ʼ���Ķ���
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: ���Ƴ���
//-----------------------------------------------------------------------------
VOID Render()
{
	if (NULL == g_pd3dDevice)
		return;

	// �����̨������Ϊ��ɫ
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// ������ʼ
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// Rendering of scene objects can happen here

		// ��������
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: ������Ϣ���
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		Render();
		ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: Ӧ�ó�������
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// ע�ᴰ����
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL
	};
	RegisterClassEx(&wc);

	// ����Ӧ�ó��򴰿�
	HWND hWnd = CreateWindow(L"D3D Tutorial", L"D3D Tutorial 01: CreateDevice",
		WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
		NULL, NULL, wc.hInstance, NULL);

	// ��ʼ�� Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// ��ʾ����
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		// ������Ϣѭ��
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(L"D3D Tutorial", wc.hInstance);
	return 0;
}