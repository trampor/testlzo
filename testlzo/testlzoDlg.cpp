
// testlzoDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CtestlzoDlg �Ի���



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


// CtestlzoDlg ��Ϣ�������

BOOL CtestlzoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	int result = lzo_init();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestlzoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestlzoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestlzoDlg::OnBnClickedButton1()
{
	CString filter;
	filter = _T("�ı��ĵ�(*.txt)|*.txt|�����ĵ�����(*.*)|*.*||");

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
