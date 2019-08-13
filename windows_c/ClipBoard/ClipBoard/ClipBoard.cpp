// ClipBoard.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

int main(int argc, char * argv[])
{
	HGLOBAL hMemory;
	LPTSTR lpMemory;
	char * content = "���а�������";   // ��д������
	int contentSize = strlen(content) + 1;

	if (!OpenClipboard(NULL))    // �򿪼��а壬�򿪺����������޷�����
	{
		puts("���а��ʧ��");
		return 1;
	}

	//HANDLE hMem = GetClipboardData(CF_TEXT); // �ı�
	//if (hMem)
	//{
	//	LPSTR lpStr = (LPSTR)GlobalLock(hMem);
	//	if (lpStr != NULL)
	//	{
	//		puts(lpStr);
	//		GlobalUnlock(hMem);
	//	}
	//}
	//else
	//{
	//	puts("��ȡ���а�����ʧ��");
	//	return 1;
	//}

	HGLOBAL hGlobal = GetClipboardData(CF_HDROP); // �ļ�
	if (hGlobal)
	{
		HDROP hDrop = (HDROP)GlobalLock(hGlobal);
		if (hDrop)
		{
			UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, 0, 0);
			UINT filenameLength;
			for (UINT i = 0; i < fileCount; ++i)
			{
				filenameLength = DragQueryFile(hDrop, i, 0, 0);
				TCHAR lpszFileName[256];
				DragQueryFile(hDrop, i, lpszFileName, filenameLength + 1);
				_tprintf(_T("%s\n"), lpszFileName);
			}
		}
	}
	else
	{
		puts("��ȡ���а�����ʧ��");
		return 1;
	}


	if (!EmptyClipboard())       // ��ռ��а壬д��֮ǰ����������ռ��а�
	{
		puts("��ռ��а�ʧ��");
		CloseClipboard();
		return 1;
	}

	if ((hMemory = GlobalAlloc(GMEM_MOVEABLE, contentSize)) == NULL)    // �Լ��а�����ڴ�
	{
		puts("�ڴ渳ֵ����!!!");
		CloseClipboard();
		return 1;
	}

	if ((lpMemory = (LPTSTR)GlobalLock(hMemory)) == NULL)             // ���ڴ���������
	{
		puts("�����ڴ����!!!");
		CloseClipboard();
		return 1;
	}

	memcpy_s(lpMemory, contentSize, content, contentSize);   // �����ݸ��ƽ����ڴ�����

	GlobalUnlock(hMemory);                   // ����ڴ�����

	if (SetClipboardData(CF_TEXT, hMemory) == NULL)
	{
		puts("���ü��а�����ʧ��!!!");
		CloseClipboard();
		return 1;
	}

	system("pause");
	return 0;
}

