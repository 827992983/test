
// DuiLibInMfc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDuiLibInMfcApp: 
// �йش����ʵ�֣������ DuiLibInMfc.cpp
//

class CDuiLibInMfcApp : public CWinApp
{
public:
	CDuiLibInMfcApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDuiLibInMfcApp theApp;