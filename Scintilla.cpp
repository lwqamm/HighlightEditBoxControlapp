

#include "pch.h"
//#define  DLLEXPORT_EXPORTS


//#include "Scintilla.h"


#include <afx.h>
#include "framework.h"
#include <afxwin.h>

#include <WinSock2.h>
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <stdio.h>
#include <CommCtrl.h>
#include <WinSDKVer.h>


#define  AFX_MFC

#include "Scintilla.h"

#define GWLP_HINSTANCE -6
#define IDC_EDIT1                   10003
#define EDIT1211                    0  
#define  GET_POS_1                  0x0      //获得鼠标矩形返回指针
#define  GET_FONT_1                 0x1      //获得鼠标矩形返回指针

//鼠标Roller_滚轮down 
#define ROLLER_UP                   0x1      //滚轮向上
#define ROLLER_DOWN                 0x2      //滚轮向下

#define SW_SHOW                    ROLLER_UP //显示和隐藏输入焦点符号
#pragma comment(lib,"Comctl32.lib")

/*大写到小写*/
void toUpper(char* src)//大写到小写
{
	char* p = NULL;
	p = src;
	while (*p)//字符串不结束就循环
	{
		if (!p)
		{
			return;
		}
		if (*p >= 'A' && *p <= 'Z') //判断小写字母
			*p += 32; //转大写
		p++; //指针后指，准备处理下一个字母
	}
	return; //返回修改后的字符串首地址
}

typedef 	struct _MyCEDIT
{
	char m_str[MAX_PATH * 2] = { 0 };
	int     m_Strlen = 0;
}MyCEDIT, * PMyCEDIT;

typedef 	struct _HIGHEDIT //高亮关键字
{
	char    m_str[41] = { 0 };
	int     m_Strlen = 0;
}HIGHEDIT, * PHIGHEDIT;    //Highlight

typedef struct _HIGHEDITARR 
{
public:
	_HIGHEDITARR() {}
	~_HIGHEDITARR() { Clear(); }

	//返回数组大小
	int Getlen() { return m_len; };
	//添加数组文本
	int Addm_str(PHIGHEDIT  p)
	{
		if (!p||strlen(p->m_str)<=1||!p->m_Strlen)
		{
			return m_len;
		}
		
		if (p)
		{
			char* t_ch = new char[p->m_Strlen+1];
			memset(t_ch, 0, p->m_Strlen);
			PCHAR p1 = (PCHAR)p->m_str;
			for (size_t i = 0; i < p->m_Strlen; i++)
			{
				if (p1[i] != '\r' || p1[i] != '\n'&& p1[i] != '\0'&& p1[i] != ' ')
				{
					t_ch[strlen(t_ch)] = p1[i];
				}
			}
			memset(p->m_str, 0, p->m_Strlen);
			memcpy(p->m_str, t_ch, strlen(t_ch));
			p->m_Strlen = strlen(t_ch);
		}


	
		for (size_t i = 0; i < m_len; i++)
		{
			if (strcmp(array[i].m_str, p->m_str) == 0)
			{
				return 0;
			}
		}

		toUpper(p->m_str);
		
		if (m_len == 0)
		{
			m_len++;
			array = new HIGHEDIT[m_len];

			if (p == 0|| p->m_Strlen>=41)
			{
				array[m_len - 1].m_Strlen = 0;
				return m_len;
			}
			memcpy(array[m_len - 1].m_str, p->m_str, strlen(p->m_str));
			array[m_len - 1].m_Strlen = p->m_Strlen;
			return m_len;
		}
		m_len++;
		m_array = new HIGHEDIT[m_len];
		for (int i = 0; i < m_len; i++)
		{
			if (i == m_len - 1)
			{
				if (p == 0 || p->m_Strlen >= 41)
				{
					m_array[i].m_Strlen = 0;
					continue;
				}
				memcpy(m_array[i].m_str, p->m_str, strlen(p->m_str));
				m_array[i].m_Strlen = p->m_Strlen;
				continue;
			}
			memcpy(m_array[i].m_str, array[i].m_str, strlen(array[i].m_str));
			m_array[i].m_Strlen = array[i].m_Strlen;
		}
		delete[] array;
		array = m_array;

		m_array = 0;

		return m_len;
	}
	//获取数组文本
	PHIGHEDIT GetEditStr(DWORD dx)
	{
		if (m_len <= 0)
		{
			return 0;
		}

		return &array[dx];
	}
	//清空数组文本
	void Clear()//清空
	{
		if (m_array)
		{
			delete[] m_array;
			m_len = 0;
			m_array = 0;
		}
		if (array)
		{
			delete[] array;
			m_len = 0;
			array = 0;
		}

	}
private:
	PHIGHEDIT m_array = 0;
	PHIGHEDIT array = 0;
	int       m_len = 0;

}HIGHEDITARR, * PHIGHEDITARR,*LPHIGHEDITARR,*NPHIGHEDITARR;

typedef struct _CEditList
{
public:
	_CEditList() {}
	~_CEditList() { Clear(); }

	int Getlen() { return m_len; };

	int Addm_str(PMyCEDIT  p)
	{
		int ret = 0;
		if (p)
		{
			PCHAR p1 = (PCHAR)p->m_str;
			for (size_t i = 0; i < p->m_Strlen; i++)
			{
				if (p1[i] == '\r' || p1[i] == '\n')
				{
					p1[i] = '\0';
					ret++;
				}
			}
		}
		if (ret)
		{
			p->m_Strlen -= ret;
		}

		if (m_len == 0)
		{
			m_len++;
			array = new MyCEDIT[m_len];

			if (p == 0 || p->m_Strlen >= MAX_PATH * 2)
			{
				array[m_len - 1].m_Strlen = 0;
				return m_len;
			}
			memcpy(array[m_len - 1].m_str, p->m_str, strlen(p->m_str));
			array[m_len - 1].m_Strlen = p->m_Strlen;
			return m_len;
		}
		m_len++;
		m_array = new MyCEDIT[m_len];
		for (int i = 0; i < m_len; i++)
		{
			if (i == m_len - 1)
			{
				if (p == 0 || p->m_Strlen >= MAX_PATH * 2)
				{
					m_array[i].m_Strlen = 0;
					continue;
				}
				memcpy(m_array[i].m_str, p->m_str, strlen(p->m_str));
				m_array[i].m_Strlen = p->m_Strlen;
				continue;
			}
			memcpy(m_array[i].m_str, array[i].m_str, strlen(array[i].m_str));
			m_array[i].m_Strlen = array[i].m_Strlen;
		}
		delete[] array;
		array = m_array;

		m_array = 0;

		return m_len;
	}
	PMyCEDIT GetEditStr(DWORD dx)
	{
		if (m_len <= 0)
		{
			return 0;
		}

		return &array[dx];
	}
	void Clear()//清空
	{
		if (m_array)
		{
			delete[] m_array;
			m_len = 0;
			m_array = 0;
		}
		if (array)
		{
			delete[] array;
			m_len = 0;
			array = 0;
		}

	}
private:
	PMyCEDIT m_array = 0;
	PMyCEDIT array = 0;
	int           m_len = 0;

}CEditList, * PCEditList;

typedef struct _DRAWTEXT
{
public:
	PTCHAR         m_pText = 0;        //文本缓冲区
	DWORD64         stLen = 0;         //绘制文本字节数
	DWORD           stHigh = 0;         //绘制高度
	_DRAWTEXT() {}
	~_DRAWTEXT() { DeleteTextBuff(); }
public:

	BOOL  AddTextstr(HWND hwnd, int conet, PTCHAR p, DWORD stlen)
	{
		if (!m_pText || stlen > m_stlen)
		{
			return FALSE;
		}
		memcpy(m_pText, p, stlen);
		stLen = stlen;
		stHigh = GetDcRect(hwnd, conet);
		return TRUE;
	}
	BOOL  AddTextBuff(DWORD stlen)
	{
		if (m_pText)
		{
			if (m_stlen == stlen && stlen > 0)
			{
				memset(m_pText, 0, stlen);
				return TRUE;
			}
			free(m_pText);
			m_pText = 0;
		}
		if (stlen <= 0)
		{
			return FALSE;
		}
		m_pText = (PTCHAR)malloc(stlen);
		if (m_pText)
		{
			memset(m_pText, 0, stlen);
			m_stlen = stlen;
			return TRUE;
		}
		return FALSE;
	}
	BOOL  DeleteTextBuff()
	{
		if (m_stlen > 0 && m_pText)
		{
			memset(m_pText, 0, m_stlen);
			delete[] m_pText;
			m_pText = 0;
			stLen = 0;
			stHigh = 0;
			return TRUE;
		}
		return FALSE;
	}
private:
	DWORD m_stlen = 0;//记录上次分配地地址长度
	//获得画图新高度
	int GetDcRect(HWND hwnd, int conet)
	{
		CRect rct;
		::GetClientRect(hwnd, &rct);
		SIZE t_ww = { 0 };
		HDC dc = GetDC(hwnd);
		GetTextExtentPoint32(dc, "X", 1, &t_ww);
		ReleaseDC(hwnd, dc);
		conet = t_ww.cy * conet;
		if (conet > rct.Height())
		{
			conet = conet - rct.Height();
			rct.bottom += conet;
		}
		return rct.bottom += conet;
	}
}DRAWTEXT, * PDRAWTEXT, * LPDRAWTEXT;

typedef class ScintillaWin {
	//IMPLEMENT_DYNAMIC(ScintillaWin, CPropertyPage);
private:
	typedef struct  _MYTIME
	{
		LONGLONG tiem = 0;
		BOOL     ret = FALSE;
	}MYTIME, * PMYTIME, * LPMYTIME, * NPMYTIME;
public:
	//构造函数
	ScintillaWin();
	//虚函数
	~ScintillaWin();
	//窗口过程函数
	static LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//编辑框过程函数
	static LRESULT CALLBACK  EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//创建窗口
	HWND Create(HWND hwnd, int x, int y, int w, int h, DWORD ID);
	//计算单个字体高度
	int GetLineHeight();
	//计算单个字体宽度
	int GetCharWidth();
	//获取最大行长度
	int GetMaxLineLength();
	//计算最大行数
	int GetLineCount();
	//获取第一个可见行
	int GetTheVisibleCount();
	//取时间间隔
	LONGLONG GetTemehou();
	// 设置光标位置
	void SetCaretPos(POINT pt);
	//编辑框所有画图
	void OnPain(DWORD iStyle = 0, DWORD usid = 0);
	//获得编辑框宽高尺寸RECT
	BOOL GetClientRect(_Out_ PRECT lpRect);
	//设置编辑框dc操作对象
	void* SelectObject(void* p);
	//获得内部操作变量指针
	void* GetProcP(DWORD dx);
	//设置新字体
	BOOL  SetEditFont(HFONT* font);
	//设置字体大小
	BOOL     SetEditFontSize(DWORD fontSize);
	//设置颜色参数
	BOOL  SetEditColor(DWORD  dwStyle, COLORREF clr);
	//纯色填充背景
	BOOL  FillSolidRect(HDC dc, LPCRECT lpRect, COLORREF clr);
	//在多行编辑控件中垂直滚动文本 返回：最后顶行文本位置
	int  SetRollTextPos(DWORD  dwStyle, DWORD dx = 1);
	//添加高亮过滤文本
	BOOL AddEditStr(const char* str);
private:
	//垂直滚动文本过程
	void SetRollTextPos_1(DWORD  dwStyle, DWORD dx = 1);
public:
	//读取文本
	char* GetEditText();
	//设置文本
	BOOL SetEditText(const char* str);
	//获得编辑框窗口句柄
	HWND  GetWindowHwnd();
	//获得编辑框回调函数
	LONG_PTR  GetEditProc();
	void  SetEditProc(LONG_PTR p);
	//清空文本
	BOOL ClearEditText();
	//获取行文本
	BOOL GetLineText(IN PVOID uot, DWORD stlen, DWORD* dx = 0);
	//查看编辑框内容是否修改
	BOOL  GetTextModify();
	//设置编辑框内容修改
	BOOL  SetTextModify(BOOL ret = TRUE);
private:
	//更新字体宽高
	void CalcLineCharDim();
	//画新文本
	void  OnDraw();
	//画旧文本
	void OnDraw(DWORD iStyle, DWORD usid);
	void  OnDrawText(HDC dc, PMyCEDIT sp, DWORD dx);
	//画行号
	void  OnDrawLineNum(HDC dc, PMyCEDIT sp, DWORD dx);
	//检测是否已经画出
	BOOL GetDrawText(PDRAWTEXT p);

private:
	HDC m_fdc = NULL;                                              //行号dc
	DWORD m_BackgRoundColor = RGB(0xFF, 0xFF, 0xFF);               //编辑框背景颜色
	DWORD m_FontColor = 0;                                         //编辑框字体颜色
	DWORD m_HighlightColor = RGB(0x0, 0x0, 0xFF); ;                //字体高亮颜色
	HFONT* m_Font = NULL;                                          //字体指针
	DWORD  m_DigitalColor = 128;                                   //数字颜色
	DWORD  m_NoteColor = RGB(0x20, 0x90, 0x20);                    //注释颜色
	DWORD  m_LineBackgColor = RGB(0xC8, 0xC8, 0xC8);               //行号背景颜色
	DWORD  m_LineColor = RGB(0x20, 0x90, 0x20);                    //行号颜色
	DRAWTEXT  m_Draw;                                               //检测是否绘制

	HDC    m_cacheDC_1 = 0;                                         //行号备份DC
	DWORD    m_FontSize = 26;                                       //行号宽度
	HBITMAP m_HEditBmp = 0;                                         //备份BMP位图
	HIGHEDITARR     m_Highlight;                                    //高亮文本数组
	LPTSTR m_AppStr[147] = {
"__asm","enum","__multiple_inheritance","template","auto","__except","__single_inheritance","this","__based",       //10
"explicit","__virtual_inheritance","thread","bool","extern","mutable","throw","break","false","naked","true",       //11
"case","__fastcall","namespace","try","catch","__finally","new","__try","__cdecl","float","operator","typedef",     //12
"char","for","private","typeid","class","friend","protected","typename","const","goto","public","union",            //12
"const_cast","if","register","unsigned","continue","inline", "reinterpret_cast","using","__declspec",               //10
"__inline","return","uuid","default","int","short","__uuidof","delete","__int8","signed","virtual","dllexport",     //12
"__int16","sizeof","void","dllimport","__int32","static","volatile","do","__int64","static_cast","wmain",           //11
"double","__leave","__stdcall","while","dynamic_cast","long","struct","xalloc","else","main","switch","interface",  //12
"persistent","_persistent","transient","depend","ondemand","transient","cset","useindex","indexdef","while",        //10
"wchar_t","volatile","void","ondemand","transient","cset","useindex","indexdef","virtual","dword","hfont","lpcstr", //12
"hwnd","hinstance","long_ptr","point","bitmap","tchar","dword64","hmodule","hcurcor","LRESULT lresult","nullptr",   //12
"false","true","uint","long","uint64","long64","longlong","ulonglong","ulong64","ulong","#define","#ifdef",         //12
"#include","push","pushad","popad","mov","eax"	,"ebx"	,"ecx"	,"edx","esp,"	,"ebp"	,"esi"	,"edi","call"		//14																			   //1
	};
private:
	//操作窗口
	HWND         m_hWnd = 0;               //操作窗口
	HWND           m_fWnd = 0;              //父窗口
	DWORD          m_Child = IDC_EDIT1;     //子窗口ID
	HINSTANCE      m_hModule = 0;        //实例句柄
	LONG_PTR       m_OEDITProc = 0;  //原来的窗口过程
	CEditList      m_EDitStr;        //文本每一行数据
	MYTIME          m_POs = { 0 };   //光标插入符位置
private:
	//	Line/character dimensions 线条/字符尺寸
	int m_nLineHeight = -1, //单个字体高度
		m_nCharWidth = -1;  //单个字体宽度

	//	Amount of lines/characters that completely fits the client area
	//完全适合工作区的行数/字符数
	int            m_nMaxLineLength = -1;    //最大行长度
	int            m_nIdealCharPos = -1;     //光标位置
	PTCHAR         m_pTextBuffer = 0;        //文本缓冲区


}*PScintillaWin, * LPScintillaWin;


typedef struct _WinArr
{
	HWND hwnd = 0;      //窗口句柄
	HWND fhwnd = 0;     //父窗口句柄
	HWND fzhwnd = 0;    //
	PScintillaWin Windows = 0; //操作类基址
}WinArr, * PWinArr, * LPWinArr;

typedef struct _MYHWNDARR {
public:
	PWinArr array = 0;
	int AddInt(PWinArr vae)
	{
		if (m_len == 0)
		{
			m_len++;
			array = new WinArr[m_len];
			array[m_len - 1] = *vae;
			return m_len;
		}
		m_len++;
		m_array = new WinArr[m_len];
		for (int i = 0; i < m_len; i++)
		{
			if (i == m_len - 1)
			{
				m_array[i] = *vae;
				continue;
			}
			m_array[i] = array[i];
		}
		delete[] array;
		array = m_array;
		m_array = 0;
		return m_len;
	}
	//获得数组大小
	int GetSize()
	{
		return m_len;
	}
	//清空数组
	void Clear()
	{
		if (m_array)
		{
			delete[] m_array;
			delete[] m_array;
		}
		if (array)
		{
			delete[] array;
			array = 0;
		}
		m_len = 0;
	}
	//获得数值
	PWinArr GetVae(HWND hwnd)
	{
		if (!hwnd)
		{
			return 0;
		}
		for (size_t i = 0; i < m_len; i++)
		{

			if (array[i].hwnd == hwnd || array[i].fzhwnd == hwnd || array[i].fhwnd == hwnd)
			{
				return &array[i];
			}


		}
		return 0;
	}
	//更新数值
	BOOL SetVae(PWinArr p)
	{
		if (!p)
		{
			return FALSE;
		}

		for (size_t i = 0; i < m_len; i++)
		{
			if (array[i].fhwnd == p->fhwnd)
			{
				array[i].hwnd = p->hwnd;
				array[i].fzhwnd = p->fzhwnd;
				return TRUE;
			}
		}
		return FALSE;
	}

public:
	_MYHWNDARR() {}
	~_MYHWNDARR() { Clear(); }
private:
	PWinArr m_array = 0;
	int  m_len = 0;
}MYHWNDARR, * PMYHWNDARR;
MYHWNDARR aman;

//构造函数
ScintillaWin::ScintillaWin()
{
	//初始化高亮文本数组
	//m_Highlight
		for (size_t i = 0; i < sizeof(m_AppStr)/ sizeof(LPTSTR); i++)
		{
			HIGHEDIT pp;
			memset(&pp,0, sizeof(HIGHEDIT));
			pp.m_Strlen = strlen(m_AppStr[i]);
			memcpy(pp.m_str, m_AppStr[i], pp.m_Strlen);
			m_Highlight.Addm_str(&pp);
		}
	//创建逻辑字体

	//原文链接：https ://blog.csdn.net/softn/article/details/51718347
}

//虚函数
ScintillaWin::~ScintillaWin()
{
	m_EDitStr.Clear();
	::DeleteObject(m_Font);//删除字体
	if (m_pTextBuffer)
	{
		free(m_pTextBuffer);
		m_pTextBuffer = 0;
	}
	if (m_cacheDC_1)
	{
		::DeleteObject(m_cacheDC_1);
		::DeleteObject(m_HEditBmp);
		m_cacheDC_1 = 0;
		m_HEditBmp = 0;
	}
	m_Highlight.Clear();
}

//添加高亮过滤文本
BOOL ScintillaWin::AddEditStr(const char* str)
{
	if (!str|| (DWORD64)str<=0)
	{
		return FALSE;
	}
	DWORD stlen = strlen(str);
	PCHAR p = (PCHAR)str;

	HIGHEDIT pp = {0};
	int ret = m_Highlight.Getlen();
	for (size_t i = 0; i < stlen; i++)
	{
		if (p[i] != ' ' && p[i] != '\0' && p[i] != '\r' && p[i] != '\n')
		{
			pp.m_str[strlen(pp.m_str)] = p[i];
			//continue;
		}
		if (p[i]==' '||i== stlen-1)
		{
			pp.m_Strlen = strlen(pp.m_str);
			m_Highlight.Addm_str(&pp);
			memset(&pp, 0, sizeof(HIGHEDIT));
		}

	}
	if (m_Highlight.Getlen()>ret)
	{
		return TRUE;
	}
	return FALSE;
}
//在多行编辑控件中垂直滚动文本 返回：最后顶行文本位置
int  ScintillaWin::SetRollTextPos(DWORD  dwStyle, DWORD dx )
{
	switch (dwStyle)
	{
	case SB_LINEDOWN:
	{
		SetRollTextPos_1(SB_LINEDOWN, dx);
		break;
	}
	case SB_LINEUP:
	{
		SetRollTextPos_1(SB_LINEUP, dx);
		break;
	}
	case SB_PAGEDOWN:
	{
		SetRollTextPos_1(SB_PAGEDOWN, dx);
		break;
	}
	case SB_PAGEUP:
	{
		SetRollTextPos_1(SB_PAGEUP, dx);
		break;
	}
	break;
	}
	return GetTheVisibleCount();
}

//垂直滚动文本过程
void ScintillaWin::SetRollTextPos_1(DWORD  dwStyle, DWORD dx)
{
	for (size_t i = 0; i < dx; i++)
	{
		::SendMessage(m_hWnd, EM_SCROLL, dwStyle, 0);
	}
}

//读取文本
char* ScintillaWin::GetEditText()
{
	return m_pTextBuffer;
}

//设置文本
BOOL ScintillaWin::SetEditText(const char* str)
{
	BOOL ret = SetWindowText(m_hWnd, str);
	SendMessage(m_fWnd, WM_COMMAND, IDC_EDIT1, EN_CHANGE);
	return ret;
}

HWND  ScintillaWin::GetWindowHwnd()
{
	return m_fWnd;
}

LONG_PTR  ScintillaWin::GetEditProc()
{
	return m_OEDITProc;
}
void  ScintillaWin::SetEditProc(LONG_PTR p)
{
	m_OEDITProc = p;
}

//清空文本
BOOL ScintillaWin::ClearEditText()
{
	BOOL ret = SetWindowText(m_hWnd, "");

	if (m_pTextBuffer)
	{
		m_EDitStr.Clear();
		memset(m_pTextBuffer, 0, strlen(m_pTextBuffer));
	}
	m_Draw.DeleteTextBuff();

	return ret;
}

//获取行文本
BOOL ScintillaWin::GetLineText(IN PVOID uot, DWORD stlen, DWORD* dx)
{
	DWORD ltMAX = stlen;
	memset(uot, 0, stlen);
	TCHAR  ch[10] = { 0 };
	_itoa(ltMAX, ch, 10);
	memcpy(uot, &ch, sizeof(ch));
	BOOL ret = ::SendMessage(m_hWnd, EM_GETLINE, *dx, (LPARAM)uot);
	if (!ret)
	{
		memset(uot, 0, stlen);
		return FALSE;
	}
	*dx = ret;
	return ret;
}
//查看编辑框内容是否修改
BOOL  ScintillaWin::GetTextModify()
{
	return ::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0);
}
//设置编辑框内容修改
BOOL  ScintillaWin::SetTextModify(BOOL ret)
{
	return ::SendMessage(m_hWnd, EM_SETMODIFY, ret, 0);
}
//检测是否已经画出
BOOL ScintillaWin::GetDrawText(PDRAWTEXT p)
{
	if (!p || !m_pTextBuffer || !m_Draw.m_pText || !m_Draw.stHigh || !m_Draw.stLen) { return FALSE; }

	if (p->stLen != m_Draw.stLen || p->stHigh != m_Draw.stHigh || strcmp(p->m_pText, m_Draw.m_pText) != 0)
	{
		return FALSE;
	}
	return TRUE;
}

int ScintillaWin::GetLineCount()
{
	int n_Count = ::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0);

	m_EDitStr.Clear();

	MyCEDIT t_cmp = { 0 };
	DWORD stlen = MAX_PATH * 2;
	DWORD dx = 0;
	for (size_t i = 0; i < n_Count; i++)
	{
		dx = i;
		GetLineText(t_cmp.m_str, stlen, &dx);
		t_cmp.m_Strlen = dx;
		m_EDitStr.Addm_str(&t_cmp);

	}

	return n_Count;
}

void* ScintillaWin::SelectObject(void* p)
{
	HDC pdc = GetDC(m_hWnd);
	if (!pdc)
	{
		return 0;
	}
	p = ::SelectObject(pdc, p);
	ReleaseDC(m_hWnd, pdc);
	return p;
}

BOOL ScintillaWin::GetClientRect(_Out_ PRECT lpRect)
{
	return ::GetClientRect(m_hWnd, (PRECT)lpRect);
}

void ScintillaWin::SetCaretPos(POINT pt)
{
	//DWORD startCount = 0;

	LONGLONG atrrr = GetTemehou();//获取当前时间标志
	
	DWORD iStyle = 0;
	if (m_POs.tiem == 0)
	{iStyle = SW_SHOW;m_POs.tiem = atrrr;}else
	{
		if (atrrr- m_POs.tiem<100){return;}
		if (m_POs.ret == TRUE)
		{iStyle = SW_SHOW ;}else{iStyle = SW_HIDE;}
	}
	int nLineHeight = GetLineHeight();int rle = GetTheVisibleCount();
	CRect rct;
	::GetClientRect(m_fWnd, rct);
	switch (iStyle)
	{
	case SW_SHOW://显示
	{
		HDC cacheDC = ::CreateCompatibleDC(m_fdc);//内存编辑框dc
		HBITMAP HEditBmp = CreateCompatibleBitmap(m_fdc, 1, nLineHeight-2); //内存编辑框位图
		HBITMAP pOHEditmap = (HBITMAP)::SelectObject(cacheDC, HEditBmp);        //内存画编辑框位图
		CRect rtc1, rtc2;
		GetWindowRect(m_fWnd, &rtc1);
		GetWindowRect(m_hWnd, &rtc2);
		pt.x += abs(rtc2.left - rtc1.left)+1;
		::BitBlt(m_fdc, pt.x, pt.y, 2, nLineHeight, cacheDC, 0, 0, SRCCOPY);
		::SelectObject(cacheDC, pOHEditmap);
		DeleteObject(HEditBmp);
		::DeleteDC(cacheDC);
		m_POs.tiem = GetTemehou();
		m_POs.ret = FALSE;
		break;
	}
	case SW_HIDE: //隐藏
	{
		::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
		m_POs.tiem = GetTemehou();
		m_POs.ret = TRUE;
		break;
	}
	}
	
	return;
}

void ScintillaWin::OnPain(DWORD iStyle, DWORD usid)
{
	//获取父窗口dc
	if (m_fdc == NULL)
	{
		m_fdc = GetDC(m_fWnd);

	}
	DWORD64 s_max = GetWindowTextLength(m_hWnd);

	if (s_max == 0)
	{
		CRect rct;
		::GetClientRect(m_fWnd, &rct);
		rct.left = 0;
		rct.right = rct.left + 26;
		FillSolidRect(m_fdc, rct, m_LineBackgColor);
		OnDraw();
		SetTextModify(TRUE);
		return;
	}
	if (m_pTextBuffer)
	{
		free(m_pTextBuffer);
		m_pTextBuffer = 0;

	}

	m_pTextBuffer = (PTCHAR)malloc(s_max + 6);

	if (!m_pTextBuffer)
	{
		return;
	}


	memset(m_pTextBuffer, 0, s_max + +6);
	GetWindowText(m_hWnd, m_pTextBuffer, s_max + 1);

	DRAWTEXT t_hch;
	int conet = GetLineCount();
	if (!t_hch.AddTextBuff(s_max + 6) || !t_hch.AddTextstr(m_hWnd, conet, m_pTextBuffer, strlen(m_pTextBuffer)))
	{
		return;
	}

	//CRect rct;
	//::GetClientRect(m_fWnd, &rct);
	//FillSolidRect(m_fdc, rct, m_LineBackgColor);
	if (!GetDrawText(&t_hch))
	{
		OnDraw();
		SetTextModify(TRUE);
	}
	else
	{
		OnDraw(iStyle, usid);
		SetTextModify(FALSE);
	}
	m_Draw.AddTextBuff(s_max + 6);
	m_Draw.AddTextstr(m_hWnd, conet, m_pTextBuffer, strlen(m_pTextBuffer));

	return;

}
void ScintillaWin::OnDraw(DWORD iStyle, DWORD usid)
{
	CRect rct;
	::GetClientRect(m_fWnd, rct);

	int rle = GetTheVisibleCount();
	int nLineHeight = GetLineHeight();
	if (iStyle > 0)
	{
		int conet = rct.Height() / nLineHeight;//计算编辑框最大容纳多少行
		int xconet = GetLineCount();               //获取编辑框内容又多少行
		if (xconet > conet)
		{
			switch (iStyle)
			{
			case ROLLER_UP://滚轮向上
			{
				if (rle == 0)
				{
					return;
				}
				rle = SetRollTextPos(SB_LINEUP, 1);      //向上滚动一行。
				::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
				return;
			}
			case ROLLER_DOWN://滚轮向下
			{

				rle = SetRollTextPos(SB_LINEDOWN, 1);    //向下滚动一行。
				::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
				return;
			}
			break;
			}
		}

	}
	// 直接在设备DC上绘制	
	rle = GetTheVisibleCount();
	::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
}
void ScintillaWin::OnDraw()
{
	CRect rcClient;
	GetClientRect(&rcClient);//获取编辑框RECT
	CRect frct;
	::GetClientRect(m_fWnd, &frct);//获取行号RECT
	int nLineCount = GetLineCount();
	if (nLineCount >= 0 && nLineCount <= 99)//小于100行
	{
		m_FontSize = m_nCharWidth * 2 + 1;
	}
	else if (nLineCount >= 100 && nLineCount <= 999)//小于1000行
	{
		m_FontSize = m_nCharWidth * 3 + 1;
	}
	else if (nLineCount >= 1000 && nLineCount <= 9999)//小于10000行
	{
		m_FontSize = m_nCharWidth * 4 + 1;
	}
	else if (nLineCount >= 10000 && nLineCount <= 99999)//小于10000行
	{
		m_FontSize = m_nCharWidth * 5 + 1;
	}
	else
	{
		return;  //大于100000退出
	}

	rcClient.left = frct.left + m_FontSize;
	rcClient.right = frct.left + frct.right;
	MoveWindow(m_hWnd, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), TRUE);
	GetClientRect(&rcClient);//获取编辑框RECT
	int nLineHeight = GetLineHeight();

	HDC pdc = GetDC(m_hWnd); //编辑框dc 

	int t_h = nLineCount * nLineHeight;
	if (t_h > frct.Height())
	{

		rcClient.bottom = rcClient.top + t_h + 34;
		frct.bottom += frct.top + t_h + 34;
	}


	HDC cacheDC = ::CreateCompatibleDC(pdc);//内存编辑框dc
	HBITMAP HEditBmp = CreateCompatibleBitmap(pdc, rcClient.Width(), rcClient.Height()); //内存编辑框位图
	HBITMAP pOHEditmap = (HBITMAP)::SelectObject(cacheDC, HEditBmp);        //内存画编辑框位图


	m_cacheDC_1 = ::CreateCompatibleDC(m_fdc);//内存行号dc
	m_HEditBmp = CreateCompatibleBitmap(m_fdc, frct.Width(), frct.Height()); //内存行号位图
	HBITMAP pOHEditmap_1 = (HBITMAP)::SelectObject(m_cacheDC_1, m_HEditBmp);        //内存行号位图

	FillSolidRect(cacheDC, rcClient, m_BackgRoundColor);//填充编辑框背景色
	FillSolidRect(m_cacheDC_1, frct, m_LineBackgColor);//填充行号背景色

	if (m_Font)
	{
		::SelectObject(cacheDC, m_Font);
		::SelectObject(m_cacheDC_1, m_Font);
	}
	char t_ch1[30] = { 0 };

	for (size_t i = 0; i < nLineCount; i++)
	{
		PMyCEDIT pp = m_EDitStr.GetEditStr(i);


		_itoa(i + 1, t_ch1, 10);
		MyCEDIT t_editp;
		if (i + 1 <= 9)
		{
			t_editp.m_str[0] = '0';
			memcpy(t_editp.m_str + 1, t_ch1, strlen(t_ch1));
			t_editp.m_Strlen = 2;
		}
		else
		{
			memcpy(t_editp.m_str, t_ch1, strlen(t_ch1));
			t_editp.m_Strlen = strlen(t_ch1);
		}
		OnDrawLineNum(m_cacheDC_1, &t_editp, i);//画行号
		OnDrawText(cacheDC, pp, i);//画内容
	}


	// 直接在设备DC上绘制


	int rle = GetTheVisibleCount();
	CRect rtc1, rtc2;
	GetWindowRect(m_fWnd, &rtc1);
	GetWindowRect(m_hWnd, &rtc2);
	int ax= abs(rtc2.left - rtc1.left) ;
	::BitBlt(m_cacheDC_1, ax+4, 0, frct.Width(), frct.Height(), cacheDC, 0, 0, SRCCOPY);


	::BitBlt(m_fdc, 0, 0, frct.Width(), frct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);

	::SelectObject(cacheDC, pOHEditmap);
	DeleteObject(HEditBmp);
	DeleteDC(cacheDC);


	ReleaseDC(m_hWnd, pdc);
}

//画文本内容 ,着色系统
void ScintillaWin::OnDrawText(HDC dc, PMyCEDIT sp, DWORD dx)
{
	if (!sp->m_str)
	{
		return;
	}

	SetBkMode(dc, 0);
	BOOL ret = FALSE;
	int star = 0;
	int aend = 0;
	DWORD stlen = sp->m_Strlen;

	int s_end = 0;

	char t_ch1[MAX_PATH * 2] = { 0 };
	memset(t_ch1, 0, MAX_PATH * 2);
	memcpy(t_ch1, sp->m_str, stlen);
	toUpper(t_ch1);
	PCHAR p = (PCHAR)t_ch1, p1 = 0;
	PHIGHEDIT ph = 0;
	while (star < stlen)
	{
		for (size_t j = 0; j < m_Highlight.Getlen(); j++)
		{
			ph = m_Highlight.GetEditStr(j);
			if (stlen - star < ph->m_Strlen)
			{
				continue;
			}


			p1 = (PCHAR)ph->m_str;

			for (size_t i = 0; i < ph->m_Strlen; i++)
			{
				if (p[star + i] != p1[i])
				{
					s_end = 0;
					break;
				}
				s_end++;

			}

			if (s_end == ph->m_Strlen)
			{
				if (star == 0)
				{

					if (p[ph->m_Strlen] == ' ' || star + ph->m_Strlen == stlen || p[ph->m_Strlen] == ',')
					{
						s_end = 0;
						aend = j;
						ret = TRUE;
						break;
					}
				}
				else if (star + ph->m_Strlen == stlen)
				{
					if (p[star - 1] == ' ' || p[star - 1] == ',')
					{
						s_end = 0;
						aend = j;
						ret = TRUE;
						break;
					}
				}
				else if ((p[star - 1] == ' '|| p[star - 1] == ',') && (p[star + ph->m_Strlen] == ' ' || p[star + ph->m_Strlen] == ','))
				{
					s_end = 0;
					aend = j;
					ret = TRUE;
					break;
				}

			}
			s_end = 0;
		}

		if (ret)
		{
			ret = FALSE;
			SIZE t_ww = { 0 };
			GetTextExtentPoint32(dc, sp->m_str, star, &t_ww);
			::SetTextColor(dc, m_HighlightColor);
			::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, m_Highlight.GetEditStr(aend)->m_Strlen);
			star += m_Highlight.GetEditStr(aend)->m_Strlen;
			aend = 0;
			continue;
		}
		if (sp->m_str[star] == ' ')
		{
			star++;
			continue;
		}
		SIZE t_ww = { 0 };
		GetTextExtentPoint32(dc, sp->m_str, star, &t_ww);

		::SetTextColor(dc, m_FontColor);
		if (sp->m_str[star] < 0)
		{
			::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, 2);
			star += 2;
			continue;
		}
		//画，号
		if (sp->m_str[star] == ',')
		{
			::SetTextColor(dc, m_HighlightColor);
			::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, 1);
			star += 1;
			aend = 0;
			continue;
		}
		//m_DigitalColor;
		if (sp->m_str[star] >= '0' && sp->m_str[star] <= '9')
		{
			if (star == 0)
			{
				if (star == stlen - 1)
				{
					::SetTextColor(dc, m_DigitalColor);
					::TextOut(dc, t_ww.cx, dx * m_nLineHeight, p + star, 1);
					star += 1;
					continue;
				}
				ret = FALSE;
				for (size_t j = star + 1; j < stlen - (star); j++)
				{
					if (p[star + 1] == 'x' || p[star + 1] == 'X')
					{
						if (j == star + 1)
						{
							continue;
						}
						if (!((p[j] >= '0' && p[j] <= '9') || ((p[j] >= 'A' && p[j] <= 'F') || (p[j] >= 'a' && p[j] <= 'f'))) && p[j]
							!= ' ')
						{
							break;
						}
						if (p[j] == ' ' || j == stlen - 1)
						{
							if (p[j] != ' ')
							{
								if (!((p[j] >= '0' && p[j] <= '9') || ((p[j] >= 'A' && p[j] <= 'F') || (p[j] >= 'a' && p[j] <= 'f'))))
								{
									break;
								}
							}
							aend = j;
							ret = TRUE;
							break;
						}
						continue;
					}
					if (p[j] == ' ' || j == stlen - 1)
					{
						if (p[j] != ' ')
						{
							if (!(p[j] >= '0' && p[j] <= '9'))
							{
								break;
							}
						}
						aend = j;
						ret = TRUE;
						break;
					}
					if (!(p[j] >= '0' && p[j] <= '9'))
					{
						break;
					}

				}
				if (ret)
				{
					ret = FALSE;
					::SetTextColor(dc, m_DigitalColor);
					::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, aend - star + 1);
					star += aend - star + 1;
					continue;
				}

				//::SetTextColor(dc, m_FontColor);
			}
			else
			{
				if (p[star - 1] == ' ')
				{
					if (star == stlen - 1)
					{
						::SetTextColor(dc, m_DigitalColor);
						::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, 1);
						star += 1;
						continue;
					}
					ret = FALSE;

					for (int j = star + 1; j < stlen; j++)
					{
						if (p[star + 1] == 'x' || p[star + 1] == 'X')
						{
							if (p[j] == 'x' || p[j] == 'X')
							{
								continue;
							}
							if (p[j] == ' ' || j == stlen - 1 )
							{
								if (p[j] != ' ')
								{
									if (!(p[j] >= '0' && p[j] <= '9'))
									{
										if (!(p[j] >= 'A' && p[j] <= 'F'))
										{
											if (!(p[j] >= 'a' && p[j] <= 'f'))
											{
												break;//123654
											}
										}

									}
								}
								aend = j;
								ret = TRUE;
								break;
							}
							if (!(p[j] >= '0' && p[j] <= '9'))
							{
								if (!(p[j] >= 'A' && p[j] <= 'F'))
								{
									if (!(p[j] >= 'a' && p[j] <= 'f'))
									{
										break;//123654
									}
								}

							}

							continue;
						}

						if (p[j] == ' ' || j == stlen - 1 )
						{
							if (p[j] != ' ')
							{
								if (!(p[j] >= '0' && p[j] <= '9'))
								{
									break;
								}
							}

							//          s         8
									   //0 1 2 3 4 5 6 7
							aend = j;  //0 0 8 8 8 8 8 8
							ret = TRUE;
							break;
						}
						if (!(p[j] >= '0' && p[j] <= '9'))
						{
							break;
						}

					}
					if (ret)
					{
						ret = FALSE;
						::SetTextColor(dc, m_DigitalColor);
						::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, aend - star + 1);
						star += aend - star + 1;
						continue;
					}
				}

			}
		}
		if (star < stlen - 1)
		{
			if (p[star] == '/' && p[star + 1] == '/')
			{
				::SetTextColor(dc, m_NoteColor);
				::TextOut(dc, t_ww.cx, dx * m_nLineHeight, p + star, stlen - star);
				star += stlen - star;
				break;
			}
		}

		::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, 1);
		star++;
	}
	ReleaseDC(m_hWnd, dc);
	//
}

void ScintillaWin::OnDrawLineNum(HDC dc, PMyCEDIT p, DWORD dx)
{
	if (!p || !dc)
	{
		return;
	}
	::SetBkMode(dc, 0);
	::SetTextColor(dc, m_LineColor);
	::TextOut(dc, 0, dx * m_nLineHeight, p->m_str, p->m_Strlen);
	return;
}

void ScintillaWin::CalcLineCharDim()
{
	HDC pdc = GetDC(m_hWnd);
	HFONT pOldFont = (HFONT)::SelectObject(pdc, (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
	SIZE szCharExt = { 0 };// 
	::GetTextExtentPoint32(pdc, TEXT("X"), strlen(TEXT("X")), &szCharExt);
	m_nLineHeight = szCharExt.cy;
	if (m_nLineHeight < 1)
		m_nLineHeight = 1;
	m_nCharWidth = szCharExt.cx;

	CRect rct;
	GetClientRect(&rct);
	//w = rct.Width();
	//h = rct.Height();

	::SelectObject(pdc, pOldFont);
	ReleaseDC(m_hWnd, pdc);
}

void* ScintillaWin::GetProcP(DWORD dx)
{
	switch (dx)
	{
	case GET_POS_1:
	{
		return &m_POs; //返回选择范围坐标
	}
	case GET_FONT_1:
	{
		return &m_Font;
	}

	default:
		break;
	}
	

	return nullptr;
}
BOOL ScintillaWin::SetEditFont(HFONT* font)
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	BOOL ret = ::SendMessage(
		m_hWnd,  //欲设置字体的控件句柄
		WM_SETFONT,  //消息名（消息类型）
		(WPARAM)*font,  //字体句柄
		NULL  //传空值即可
	);
	*(HFONT*)&m_Font = *font;
	if (ret)
	{
		int eroo = GetLastError();
		CalcLineCharDim();
	}

	return ret;
}
BOOL ScintillaWin::SetEditFontSize(DWORD fontSize)
{

	if (fontSize < 13 || fontSize == m_nLineHeight)
	{
		return FALSE;
	}
	::DeleteObject(m_Font);//删除字体
	LOGFONT logFont;
	HFONT hFont;
	logFont.lfHeight = fontSize;
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_REGULAR;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = GB2312_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont.lfFaceName, "微软雅黑");

	hFont = CreateFontIndirect(&logFont);

	return  SetEditFont(&hFont);
}
BOOL ScintillaWin::SetEditColor(DWORD  dwStyle, COLORREF clr)
{
	switch (dwStyle)
	{
	case SET_EDITBACKCOLOR:
	{
		if (clr < 0 || clr == m_BackgRoundColor)
		{
			return FALSE;
		}
		m_BackgRoundColor = clr;
		break;
	}
	case SET_EDITFONTCOLOR:
	{
		if (clr < 0 || clr == m_FontColor)
		{
			return FALSE;
		}
		m_FontColor = clr;
		break;
	}
	case SET_EDITHIGHLICOLOR:
	{
		if (clr < 0 || clr == m_HighlightColor)
		{
			return FALSE;
		}
		m_HighlightColor = clr;
		break;
	}
	case SET_EDITDIGITALCOLOR:
	{
		if (clr < 0 || clr == m_DigitalColor)
		{
			return FALSE;
		}
		m_DigitalColor = clr;
		break;
	}
	case SET_EDITNOTECOLOR:
	{
		if (clr < 0 || clr == m_NoteColor)
		{
			return FALSE;
		}
		m_NoteColor = clr;
		break;
	}
	case SET_EDITLINEBACKGCOLOR:
	{
		if (clr < 0 || clr == m_LineBackgColor)
		{
			return FALSE;
		}
		m_LineBackgColor = clr;
		break;
	}
	case SET_EDITLINECOLOR:
	{
		if (clr < 0 || clr == m_LineColor)
		{
			return FALSE;
		}
		m_LineColor = clr;
		break;
	}
	break;
	}
	return TRUE;
}
BOOL ScintillaWin::FillSolidRect(HDC dc, LPCRECT lpRect, COLORREF clr)
{
	ENSURE_VALID(this);
	ENSURE(dc != NULL);
	ENSURE(lpRect);

	::SetBkColor(dc, clr);
	return	::ExtTextOut(dc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
}
BOOL g_mct = 0;

LRESULT CALLBACK ScintillaWin::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{

	case WM_LBUTTONDOWN:
	{

		if (g_mct == 0)
		{
			g_mct = TRUE;
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		if (g_mct)
		{
			PWinArr p1 = aman.GetVae(hWnd);
			p1->Windows->OnPain();
			g_mct = FALSE;
		}

		break;
	}
	case WM_KEYDOWN:
	{
		if (g_mct == 0)
		{
			g_mct = TRUE;
		}
		break;
	}
	case WM_KEYUP:
	{
		if (g_mct)
		{
			PWinArr p1 = aman.GetVae(hWnd);
			p1->Windows->OnPain();
			g_mct = FALSE;
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		PWinArr p1 = aman.GetVae(hWnd);
		if (p1)
		{
			if (delta > 0)
			{
				p1->Windows->OnPain(ROLLER_UP, delta);//鼠标滚轮向上滚动
			}
			else
			{
				p1->Windows->OnPain(ROLLER_DOWN, delta);//鼠标滚轮向下滚动
			}
			return TRUE;
		}

	}

	case  WM_PAINT:
	{

			PWinArr p1 = aman.GetVae(hWnd);

            if (p1)
            {
            	//p1->Windows->OnPain();
				POINT po1;
				::GetCaretPos(&po1);
					
					p1->Windows->SetCaretPos(po1);
		
            }
        
		return FALSE;
		break;
	}

	default:
		break;
	}

	//这里一定要注意，我们只处理我们想要处理的消息，我们不需要处理的消息放给原来的回调函数处理
	PWinArr p1 = aman.GetVae(hWnd);

	ULONG_PTR  Onproc = p1->Windows->GetEditProc();
	return CallWindowProc((WNDPROC)Onproc, hWnd, msg, wParam, lParam);
}
//原文链接：https ://blog.csdn.net/hbllll1/article/details/115676312

LRESULT ScintillaWin::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
	{
		DWORD  usg = LOWORD(wParam); //: 控件ID
		if (usg == IDC_EDIT1)
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE://当编辑框字体改变
			{
				PWinArr p1 = aman.GetVae(hwnd);
				p1->Windows->OnPain();
				return FALSE;
			}

			case EN_VSCROLL:
			{
				PWinArr p1 = aman.GetVae(hwnd);
				if (p1)
				{
					p1->Windows->OnPain();
					return TRUE;
				}

				break;
			}

			default:
				break;
			}
		}


		break;
	}

	case  WM_PAINT:
	{

		PWinArr p1 = aman.GetVae(hwnd);
		p1->Windows->OnPain();

		break;;
	}
	case WM_DESTROY:
	{
		//请做好善后工作
		PWinArr p = aman.GetVae(hwnd);
		if (p)
		{
			::DeleteObject(p->Windows->GetProcP(GET_FONT_1));//删除创建的字体
		}
		::PostQuitMessage(0);
		return 0;
	}

	default:
		break;
	}



	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

HWND ScintillaWin::Create(HWND hwnd, int x, int y, int w, int h, DWORD ID)
{
	if (!hwnd || !w || !h || !ID || x < 0 || y < 0)
	{
		return FALSE;
	}
	HINSTANCE hModule = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	if (!hModule)
	{
		return FALSE;
	}
	m_hModule = hModule;
	WNDCLASSEXA wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)ScintillaWin::WndProc;
	wc.cbWndExtra = 0;
	HBRUSH hBrush = CreateSolidBrush(RGB(0xc8, 0xc8, 0xc8));//绘图背景字体色
	wc.hbrBackground = hBrush;
	wc.hCursor = (HICON)LoadImageW(hModule, L"myIcon", IMAGE_ICON, 24, 24, LR_DEFAULTCOLOR);
	wc.hIcon = (HICON)LoadImageW(hModule, L"myIcon", IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	wc.hInstance = hModule;
	wc.lpszClassName = "Scintilla11q";
	wc.lpszMenuName = NULL;
	if (!RegisterClassEx(&wc))
	{
		//int eroo = GetLastError();
		return FALSE;
	}

	m_fWnd = CreateWindow(wc.lpszClassName, NULL, WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER /*WS_POPUP*/,
		x, y, w, h, hwnd, (HMENU)ID, hModule, 0);
	if (!m_fWnd)
	{
		int eroo = GetLastError();
		return FALSE;
	}
	// 创建多行编辑控件
	m_hWnd = CreateWindowEx(0, TEXT("edit"), TEXT(""),
		WS_VSCROLL | WS_CHILD | WS_VISIBLE | ES_WANTRETURN | ES_MULTILINE,
		m_FontSize, 0, w - m_FontSize, h, m_fWnd, (HMENU)IDC_EDIT1, m_hModule, 0);//ES_WANTRETURN
	if (!m_hWnd)
	{
		return FALSE;
	}
	LOGFONT logFont;
	HFONT hFont;
	logFont.lfHeight = 22;
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_REGULAR;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = GB2312_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont.lfFaceName, "微软雅黑");

	hFont = CreateFontIndirect(&logFont);

	//原文链接：https ://blog.csdn.net/softn/article/details/51718347

	WinArr t_cmp = { 0 };
	t_cmp.hwnd = m_hWnd;
	t_cmp.fhwnd = hwnd;
	t_cmp.fzhwnd = m_fWnd;
	aman.SetVae(&t_cmp);


	UINT_PTR	m_OEDI = SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)EditProc);
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		::SendMessage(m_fWnd, WM_CLOSE, 0, 0);
		return FALSE;
	}
	p->Windows->SetEditProc(m_OEDI);
	ShowWindow(m_fWnd, SW_SHOW);
	::UpdateWindow(m_fWnd);
	p->Windows->SetEditFont(&hFont);
	//HFONT pp = (HFONT)p->Windows->GetProcP(GET_FONT_1);
	   //*(HFONT*)pp = hFont;
   //MSG msg;
   //while (GetMessageW(&msg, NULL, 0, 0)) {

   //	TranslateMessage(&msg);
   //	DispatchMessageW(&msg);
   //}


	return m_fWnd;
}

int ScintillaWin::GetLineHeight()
{
	if (m_nLineHeight == -1)
		CalcLineCharDim();
	return m_nLineHeight;
}

int ScintillaWin::GetCharWidth()
{
	if (m_nCharWidth == -1)
		CalcLineCharDim();
	return m_nCharWidth;
}

int ScintillaWin::GetTheVisibleCount()
{
	return ::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, NULL, NULL);
}

LONGLONG ScintillaWin::GetTemehou()
{
	LARGE_INTEGER   temm;
	QueryPerformanceCounter(&temm);
	double pcFreq = (double)temm.QuadPart / 100000.0;

	return LONGLONG(pcFreq);
}

int ScintillaWin::GetMaxLineLength()
{
	if (m_nMaxLineLength == -1)
	{
		m_nMaxLineLength = 0;
		int nLineCount = GetLineCount();

	}
	return m_nMaxLineLength;
}



HWND Scintilla::Create(HWND hwnd, int x, int y, int w, int h, DWORD ID)
{
	if (m_hWnd)
	{
		return 0;
	}
	PScintillaWin p = new ScintillaWin;
	WinArr   t_cmp = { 0 };
	t_cmp.Windows = p;
	t_cmp.fhwnd = hwnd;
	aman.AddInt(&t_cmp);
	HWND hw = p->Create(hwnd, x, y, w, h, ID);

	// 创建多行编辑控件
	m_hWnd = hw;

	return  hw;
}


#ifdef AFX_MFC
CString Scintilla::GetEditText()
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return "";
	}

	return p->Windows->GetEditText();
}

#else
BOOL Scintilla::GetEditText(OUT char* ptu)
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	int n_max = strlen(p->Windows->GetEditText());
	ptu = new char(n_max + 1);
	memset(ptu, 0, n_max + 1);
	memcpy(ptu, p->Windows->GetEditText(), n_max);
	return GetWindowText(p->Windows->GetWindowHwnd(), ptu, n_max);
}
#endif

BOOL Scintilla::SetEditText(IN const char* str)
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}


	return p->Windows->SetEditText(str);
}

BOOL Scintilla::ClearEditText()
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}


	BOOL ret = p->Windows->ClearEditText();

	p->Windows->OnPain();
	return ret;
}

BOOL Scintilla::SetEditFont(HFONT* font)
{

	//设置控件字体
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	return p->Windows->SetEditFont(font);
}

BOOL Scintilla::SetEditFontSize(DWORD fontSize)
{
	return 0;
}

//设置颜色参数
BOOL Scintilla::SetEditColor(DWORD dwStyle, COLORREF clr)
{
	//设置控件字体
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	return p->Windows->SetEditColor(dwStyle, clr);
}

//添加高亮过滤文本
BOOL Scintilla::AddEditStr(const char* str)
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	return p->Windows->AddEditStr(str);
}