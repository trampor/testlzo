
// testlzoDlg.h : ͷ�ļ�
//

#pragma once
#include "WinApiMD5.h"

// CtestlzoDlg �Ի���
class CtestlzoDlg : public CDialogEx
{
// ����
public:
	CtestlzoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTLZO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	unsigned char* m_psrcbuf, *m_pdestbuf,*m_pworkbuf,*m_prestorbuf;

	CWinApiMD5 winapimd5;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CString m_sFileName;
	UINT m_nCompressedSize;
	UINT m_nrestoredSize;
	UINT m_nFileSize;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
