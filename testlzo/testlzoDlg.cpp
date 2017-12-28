
// testlzoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testlzo.h"
#include "testlzoDlg.h"
#include "afxdialogex.h"

#include "lzo/lzo1x.h"
#include "lzo/lzo1y.h"

#include "MD5.h"
#include "WinApiMD5.h"

#include "quicklz/quicklz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestlzoDlg 对话框



CtestlzoDlg::CtestlzoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestlzoDlg::IDD, pParent)
	, m_sFileName(_T(""))
	, m_nCompressedSize(0)
	, m_nrestoredSize(0)
	, m_nFileSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_psrcbuf = NULL;
	m_pdestbuf = NULL;
	m_prestorbuf = NULL;
}

void CtestlzoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sFileName);
	DDX_Text(pDX, IDC_EDIT5, m_nCompressedSize);
	DDX_Text(pDX, IDC_EDIT6, m_nrestoredSize);
	DDX_Text(pDX, IDC_EDIT2, m_nFileSize);
}

BEGIN_MESSAGE_MAP(CtestlzoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestlzoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CtestlzoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CtestlzoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CtestlzoDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CtestlzoDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CtestlzoDlg 消息处理程序

BOOL CtestlzoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	int result = lzo_init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestlzoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestlzoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestlzoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestlzoDlg::OnBnClickedButton1()
{
	CString filter;
	filter = _T("文本文档(*.txt)|*.txt|所有文档类型(*.*)|*.*||");

	CFileDialog fd(true, _T(".txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (fd.DoModal() != IDOK)
		return;

	SetDlgItemText(IDC_EDIT1,fd.GetPathName());
}


void CtestlzoDlg::OnBnClickedButton2()
{
	UpdateData();

	if (m_psrcbuf != NULL)
	{
		delete m_psrcbuf;
		m_psrcbuf = NULL;
	}
	if (m_pdestbuf != NULL)
	{
		delete m_pdestbuf;
		m_pdestbuf = NULL;
	}
	if (m_prestorbuf != NULL)
	{
		delete m_prestorbuf;
		m_prestorbuf = NULL;
	}
	if (m_pworkbuf != NULL)
	{
		delete m_pworkbuf;
		m_pworkbuf = NULL;
	}

	CFile destfile;
	if (destfile.Open(m_sFileName, CFile::modeRead))
	{
		LONGLONG filesize = destfile.GetLength();
		m_nFileSize = filesize;
		SetDlgItemInt(IDC_EDIT2, filesize);

		m_psrcbuf = new unsigned char[filesize];
		m_pdestbuf = new unsigned char[filesize + filesize/16 + 64+3];
		m_prestorbuf = new unsigned char[filesize];
		m_pworkbuf = new unsigned char[1000000];

		DWORD start = GetTickCount();
		int readnum = destfile.Read(m_psrcbuf, filesize);
		DWORD end = GetTickCount();
		SetDlgItemInt(IDC_EDIT7, end - start);
		MD5 md5;
		SetDlgItemText(IDC_EDIT9, md5.digestMemory(m_psrcbuf, filesize));
		DWORD end2 = GetTickCount();
		SetDlgItemInt(IDC_EDIT11, end2 - end);

		SetDlgItemText(IDC_EDIT14, winapimd5.GetMD5(m_psrcbuf, filesize));
		DWORD end3 = GetTickCount();
		SetDlgItemInt(IDC_EDIT15, end3- end2);

		if (readnum == filesize)
		{
			start = GetTickCount();
			int result = lzo1x_1_compress(m_psrcbuf, filesize, m_pdestbuf,(unsigned long*) &m_nCompressedSize, m_pworkbuf);
			end = GetTickCount();
			if (result == LZO_E_OK)
			{
				SetDlgItemInt(IDC_EDIT3, end-start);
				SetDlgItemInt(IDC_EDIT5, m_nCompressedSize);

				MD5 md5;
				SetDlgItemText(IDC_EDIT8, md5.digestMemory(m_pdestbuf, m_nCompressedSize));
				end2 = GetTickCount();
				SetDlgItemInt(IDC_EDIT12, end2 - end);
				SetDlgItemText(IDC_EDIT16, winapimd5.GetMD5(m_pdestbuf, m_nCompressedSize));
				end3 = GetTickCount();
				SetDlgItemInt(IDC_EDIT17, end3 - end2);
			}
		}

		destfile.Close();
	}
}


void CtestlzoDlg::OnBnClickedButton3()
{
	DWORD start = GetTickCount();
	int result = lzo1x_decompress(m_pdestbuf, m_nCompressedSize, m_prestorbuf, (unsigned long*)&m_nrestoredSize, m_pworkbuf);
	DWORD end = GetTickCount();
	if (result == LZO_E_OK)
	{
		SetDlgItemInt(IDC_EDIT4, end - start);
		SetDlgItemInt(IDC_EDIT6, m_nrestoredSize);
		MD5 md5;
		SetDlgItemText(IDC_EDIT10, md5.digestMemory(m_prestorbuf, m_nrestoredSize));
		DWORD end2 = GetTickCount();
		SetDlgItemInt(IDC_EDIT13, end2 - end);
		SetDlgItemText(IDC_EDIT18, winapimd5.GetMD5(m_prestorbuf, m_nrestoredSize));
		DWORD end3 = GetTickCount();
		SetDlgItemInt(IDC_EDIT19, end3 - end2);
	}
}

void CtestlzoDlg::OnBnClickedButton4()
{
	UpdateData();

	if (m_psrcbuf != NULL)
	{
		delete m_psrcbuf;
		m_psrcbuf = NULL;
	}
	if (m_pdestbuf != NULL)
	{
		delete m_pdestbuf;
		m_pdestbuf = NULL;
	}
	if (m_prestorbuf != NULL)
	{
		delete m_prestorbuf;
		m_prestorbuf = NULL;
	}
	if (m_pworkbuf != NULL)
	{
		delete m_pworkbuf;
		m_pworkbuf = NULL;
	}

	CFile destfile;
	if (destfile.Open(m_sFileName, CFile::modeRead))
	{
		LONGLONG filesize = destfile.GetLength();
		m_nFileSize = filesize;
		SetDlgItemInt(IDC_EDIT2, filesize);

		m_psrcbuf = new unsigned char[filesize];
		m_pdestbuf = new unsigned char[filesize + 400];
		m_prestorbuf = new unsigned char[filesize];
		m_pworkbuf = new unsigned char[1000000];

		DWORD start = GetTickCount();
		int readnum = destfile.Read(m_psrcbuf, filesize);
		DWORD end = GetTickCount();
		SetDlgItemInt(IDC_EDIT7, end - start);
		MD5 md5;
		SetDlgItemText(IDC_EDIT9, md5.digestMemory(m_psrcbuf, filesize));
		DWORD end2 = GetTickCount();
		SetDlgItemInt(IDC_EDIT11, end2 - end);

		SetDlgItemText(IDC_EDIT14, winapimd5.GetMD5(m_psrcbuf, filesize));
		DWORD end3 = GetTickCount();
		SetDlgItemInt(IDC_EDIT15, end3 - end2);

		if (readnum == filesize)
		{
			qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
			start = GetTickCount();
			m_nCompressedSize = qlz_compress(m_psrcbuf, (char*)m_pdestbuf, filesize, state_compress);
			end = GetTickCount();
			if (m_nCompressedSize > 0)
			{
				SetDlgItemInt(IDC_EDIT20, end - start);
				SetDlgItemInt(IDC_EDIT21, m_nCompressedSize);

				MD5 md5;
				SetDlgItemText(IDC_EDIT8, md5.digestMemory(m_pdestbuf, m_nCompressedSize));
				end2 = GetTickCount();
				SetDlgItemInt(IDC_EDIT12, end2 - end);
				SetDlgItemText(IDC_EDIT16, winapimd5.GetMD5(m_pdestbuf, m_nCompressedSize));
				end3 = GetTickCount();
				SetDlgItemInt(IDC_EDIT17, end3 - end2);
			}
		}

		destfile.Close();
	}
}


void CtestlzoDlg::OnBnClickedButton5()
{
	qlz_state_decompress *state_decompress = (qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));
	DWORD start = GetTickCount();
	m_nrestoredSize = qlz_decompress((char*)m_pdestbuf, m_prestorbuf, state_decompress);
	DWORD end = GetTickCount();
	if (m_nrestoredSize > 0)
	{
		SetDlgItemInt(IDC_EDIT22, end - start);
		SetDlgItemInt(IDC_EDIT23, m_nrestoredSize);
		MD5 md5;
		SetDlgItemText(IDC_EDIT10, md5.digestMemory(m_prestorbuf, m_nrestoredSize));
		DWORD end2 = GetTickCount();
		SetDlgItemInt(IDC_EDIT13, end2 - end);
		SetDlgItemText(IDC_EDIT18, winapimd5.GetMD5(m_prestorbuf, m_nrestoredSize));
		DWORD end3 = GetTickCount();
		SetDlgItemInt(IDC_EDIT19, end3 - end2);
	}
}
