
// DuiLibInMfcDlg.h : ͷ�ļ�
//

#pragma once
#include "DuiLibControlDemo.h"

// CDuiLibInMfcDlg �Ի���
class CDuiLibInMfcDlg : public CDialogEx
{
// ����
public:
	CDuiLibInMfcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUILIBINMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CDuiFrameWnd m_duiFrame;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
