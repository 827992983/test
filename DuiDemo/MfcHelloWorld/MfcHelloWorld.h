
// MfcHelloWorld.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMfcHelloWorldApp: 
// �йش����ʵ�֣������ MfcHelloWorld.cpp
//

class CMfcHelloWorldApp : public CWinApp
{
public:
	CMfcHelloWorldApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMfcHelloWorldApp theApp;