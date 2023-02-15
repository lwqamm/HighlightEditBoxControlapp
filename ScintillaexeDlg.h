
// ScintillaexeDlg.h: 头文件
//

#pragma once
#include "ScintillaWnd.h"

// CScintillaexeDlg 对话框
class CScintillaexeDlg : public CDialogEx
{
// 构造
public:
	CScintillaexeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCINTILLAEXE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();

public:
	//ScintillaWnd* m_pScintillaWnd = NULL;
	void GetRange(int start, int end, char* text);
	virtual void OnOK();

	Scintilla     m_SciEdit;
	afx_msg void OnBnClickedButton3();

};
