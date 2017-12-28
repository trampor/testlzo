
// testlzoDlg.h : 头文件
//

#pragma once
#include "WinApiMD5.h"

// CtestlzoDlg 对话框
class CtestlzoDlg : public CDialogEx
{
// 构造
public:
	CtestlzoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTLZO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	unsigned char* m_psrcbuf, *m_pdestbuf,*m_pworkbuf,*m_prestorbuf;

	CWinApiMD5 winapimd5;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
