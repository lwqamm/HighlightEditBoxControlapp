
// ScintillaexeDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Scintillaexe.h"
#include "ScintillaexeDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hwnd = 0;
// CScintillaexeDlg 对话框



CScintillaexeDlg::CScintillaexeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCINTILLAEXE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScintillaexeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CScintillaexeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CScintillaexeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CScintillaexeDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, &CScintillaexeDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

//
//typedef int (*fun_ptr)();
//fun_ptr RegisterClassExAS = NULL;
// CScintillaexeDlg 消息处理程序
HMODULE g_Scintilla = 0;
BOOL CScintillaexeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	 g_hwnd=m_SciEdit.Create(m_hWnd, 49, 191, 469, 140, 10098);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScintillaexeDlg::OnPaint()
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
HCURSOR CScintillaexeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HMODULE  g_dll1 = 0;

void CScintillaexeDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString str;
	
	SetDlgItemText(IDC_EDIT1, m_SciEdit.GetEditText());

}


void CScintillaexeDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_EDIT1, str);



	m_SciEdit.AddEditStr( str);


}

void CScintillaexeDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SciEdit.ClearEditText();
}

void CScintillaexeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, g_Scintilla, 0, 0);
	//::SendMessage(g_hwnd, WM_CLOSE, 0, 0);
	//Sleep(10);
	//FreeLibrary(g_Scintilla);
	CDialogEx::OnClose();
}

void CScintillaexeDlg::GetRange(int start, int end, char* text)
{
	//TEXTRANGE tr;
	//tr.chrg.cpMin = start;
	//tr.chrg.cpMax = end;
	//tr.lpstrText = (text);
	//m_pScintillaWnd->SendMessage(EM_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
}


void CScintillaexeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}




