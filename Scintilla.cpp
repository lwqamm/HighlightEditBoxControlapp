

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
#define EDIT1211                     0  
#define  GET_POS_1                   0x0      //��������η���ָ��
#define  GET_BUFF_TEXT               0x1      //����ı�������ָ��
#define  GET_DRAW_RET                0x2      //����Ƿ����ڻ��ı�
#define  GET_TIPS_TEXT               0x3      //�����ʾ�ı�ָ��
#define  GET_FONT_1                  0x4      //�������ָ��
#define GET_EDIT_LENGTH              0x5      //��ñ༭���������ָ��
//���Roller_����down 
#define ROLLER_UP                   0x1      //��������
#define ROLLER_DOWN                 0x2      //��������

#define SW_SHOW                    ROLLER_UP //��ʾ���������뽹�����
#pragma comment(lib,"Comctl32.lib")
#pragma warning(disable:4996)



typedef class MyClass
{
public:
	MyClass ();
	~MyClass();
//�����߳�
BOOL Createthread(LPVOID p,LPVOID p1);
void join();
private:
	HANDLE m_handle = 0;
}*PMyClass,*LPMyClass,*NPMyClass;

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
	CloseHandle(m_handle);
}
BOOL MyClass::Createthread(LPVOID p, LPVOID p1)
{
	
	m_handle=::CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)p,p1,0,0);
	return (BOOL)m_handle;
}
void MyClass::join()
{
	WaitForSingleObject(m_handle, INFINITE);
}
/*��д��Сд*/
void toUpper(char* src)//��д��Сд
{
	char* p = NULL;
	p = src;
	while (*p)//�ַ�����������ѭ��
	{
		if (!p)
		{
			return;
		}
		if (*p >= 'A' && *p <= 'Z') //�ж�Сд��ĸ
			*p += 32; //ת��д
		p++; //ָ���ָ��׼��������һ����ĸ
	}
	return; //�����޸ĺ���ַ����׵�ַ
}

typedef 	struct _MyCEDIT
{
	char m_str[MAX_PATH * 2] = { 0 };
	int     m_Strlen = 0;
}MyCEDIT, * PMyCEDIT;

typedef 	struct _HIGHEDIT //�����ؼ���
{
	char    m_str[41] = { 0 };
	int     m_Strlen = 0;
	int     m_Max = 41;
}HIGHEDIT, * PHIGHEDIT;    //Highlight

typedef struct _HIGHEDITARR 
{
public:
	_HIGHEDITARR() {}
	~_HIGHEDITARR() { Clear(); }

	//���������С
	int Getlen() { return m_len; };
	//��������ı�
	int Addm_str(PHIGHEDIT  p)
	{
		if (!p||strlen(p->m_str)<=1||!p->m_Strlen)
		{
			return m_len;
		}
		
		if (p)
		{
			char* t_ch = new char[p->m_Strlen+1];
			memset(t_ch, 0, p->m_Strlen+1);
			PCHAR p1 = (PCHAR)p->m_str;
			for (size_t i = 0; i < p->m_Strlen; i++)
			{
				if (p1[i] != '\r' || p1[i] != '\n'&& p1[i] != '\0'&& p1[i] != ' ')
				{
					t_ch[strlen(t_ch)] = p1[i];
				}
			}
			memset(p->m_str, 0, p->m_Strlen+1);
			memcpy(p->m_str, t_ch, strlen(t_ch));
			p->m_Strlen = strlen(t_ch);

			delete[] t_ch;
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
	//��ȡ�����ı�
	PHIGHEDIT GetEditStr(DWORD dx)
	{
		if (m_len <= 0)
		{
			return 0;
		}

		return &array[dx];
	}
	//��������ı�
	void Clear()//���
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
	void Clear()//���
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
	PTCHAR         m_pText = 0;        //�ı�������
	DWORD64         stLen = 0;         //�����ı��ֽ���
	DWORD           stHigh = 0;         //���Ƹ߶�
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
	DWORD m_stlen = 0;//��¼�ϴη���ص�ַ����
	//��û�ͼ�¸߶�
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

typedef struct _MYSELPOS //SelPos
{
	DWORD start = 0;       //��ʼ����
	DWORD end = 0;     //����
}MYSELPOS, * PMYSELPOS, * LPMYSELPOS;


typedef class ScintillaWin {
	//IMPLEMENT_DYNAMIC(ScintillaWin, CPropertyPage);
private:
	typedef struct  _MYTIME
	{
		LONGLONG tiem = 0;
		BOOL     ret = FALSE;
	}MYTIME, * PMYTIME, * LPMYTIME, * NPMYTIME;
public:
	//���캯��
	ScintillaWin();
	//�麯��
	~ScintillaWin();
	//���ڹ��̺���
	static LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//�༭����̺���
	static LRESULT CALLBACK  EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//��������
	HWND Create(HWND hwnd, int x, int y, int w, int h, DWORD ID);
	//���㵥������߶�
	int GetLineHeight();
	//���㵥��������
	int GetCharWidth();
	//��ȡ����г���
	int GetMaxLineLength();
	//�����������
	int GetLineCount();
	//��ȡ��һ���ɼ���
	int GetTheVisibleCount();
	//ȡʱ����
	LONGLONG GetTemehou();
	// ���ù��λ��
	void SetCaretPos(POINT pt);
	//�༭�����л�ͼ
	void OnPain(DWORD iStyle = 0, DWORD usid = 0);
	//��ñ༭���߳ߴ�RECT
	BOOL GetClientRect(_Out_ PRECT lpRect);
	//���ñ༭��dc��������
	void* SelectObject(void* p);
	//����ڲ���������ָ��
	void* GetProcP(DWORD dx);
	//����������
	BOOL  SetEditFont(HFONT* font);
	//���������С
	BOOL     SetEditFontSize(DWORD fontSize);
	//������ɫ����
	BOOL  SetEditColor(DWORD  dwStyle, COLORREF clr);
	//��ɫ��䱳��
	BOOL  FillSolidRect(HDC dc, LPCRECT lpRect, COLORREF clr);
	//�ڶ��б༭�ؼ��д�ֱ�����ı� ���أ�������ı�λ��
	int  SetRollTextPos(DWORD  dwStyle, DWORD dx = 1);
	//��Ӹ��������ı�
	BOOL AddEditStr(const char* str);
	//���ñ༭�ؼ���ʾ�ı�
	BOOL SetPromptText(const char* str);
    //���ѡ����ɫ
	BOOL SetSelPosColor(HDC dc, DWORD coten, DWORD h);
	//���ñ༭��ؼ�ѡ�е�λ�� EM_SETSEL
	void SetTextSelPos(DWORD k, DWORD j, DWORD dwStyle='A');
	//�����Զ�����Ϣ
	LRESULT SendMessage_(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	//��ֱ�����ı�����
	void SetRollTextPos_1(DWORD  dwStyle, DWORD dx = 1);
public:
	//��ȡ�ı�
	char* GetEditText();
	//�����ı�
	BOOL SetEditText(const char* str);
	//��ñ༭�򴰿ھ��
	HWND  GetWindowHwnd();
	//��ñ༭��ص�����
	LONG_PTR  GetEditProc();
	void  SetEditProc(LONG_PTR p);
	//����ı�
	BOOL ClearEditText();
	//��ȡ���ı�
	BOOL GetLineText(IN PVOID uot, DWORD stlen, DWORD* dx = 0);
	//�鿴�༭�������Ƿ��޸�
	BOOL  GetTextModify();
	//���ñ༭�������޸�
	BOOL  SetTextModify(BOOL ret = TRUE);
	//ʹ�õ�ǰ����ʹ�õ�ǰ���ʻ���һ�����β�ʹ�õ�ǰ���ʽ�����䡣
	BOOL Rectangle(_In_ HDC hdc, _In_ int left, _In_ int top, _In_ int right, _In_ int bottom,DWORD cColor);
	//ʹ�õ�ǰ����ʹ�õ�ǰ���ʻ���һ�����β�ʹ�õ�ǰ���ʽ�����䡣
	BOOL Rectangle(_In_ HDC hdc, LPCRECT lpRect, DWORD cColor);
public:
	//���ı�
	void  OnDrawText(HDC dc, PMyCEDIT sp, DWORD dx);
	//���к�
	void  OnDrawLineNum(HDC dc, PMyCEDIT sp, DWORD dx);
	//�����ı�
	void OnDraw(DWORD iStyle, DWORD usid);
	//��ȡѡ��λ��
	void GetSelPos();
private:
	//����������
	void CalcLineCharDim();
	//�����ı�
	void  OnDraw();

	//����Ƿ��Ѿ�����
	BOOL GetDrawText(PDRAWTEXT p);
	//��ȡѡ������
	int GetSelPosText(PMYSELPOS pos, PINT pt);
	//��ȡ����RECT
	void GetStrRect(HDC dc,CRect* p ,const char* str, DWORD len);

private:
	HDC m_fdc = NULL;                                              //�к�dc
	DWORD m_BackgRoundColor = RGB(0xFF, 0xFF, 0xFF);               //�༭�򱳾���ɫ
	DWORD m_FontColor = 0;                                         //�༭��������ɫ
	DWORD m_HighlightColor = RGB(0x0, 0x0, 0xFF); ;                //���������ɫ
	HFONT* m_Font = NULL;                                          //����ָ��
	DWORD  m_DigitalColor = 128;                                   //������ɫ
	DWORD  m_NoteColor = RGB(0x20, 0x90, 0x20);                    //ע����ɫ
	DWORD  m_LineBackgColor = RGB(0xC8, 0xC8, 0xC8);               //�кű�����ɫ
	DWORD  m_LineColor = RGB(0x20, 0x90, 0x20);                    //�к���ɫ
	DWORD  m_SelPosColor= RGB(0x00, 0x78, 0xd7);                    //ѡ���ı���ɫ
	DRAWTEXT  m_Draw;                                               //����Ƿ����
	BOOL  m_DrawRet=FALSE;                                               //����Ƿ����
	BOOL  m_DrawLock=FALSE;                                               //����Ƿ����

	HDC    m_cacheDC_1 = 0;                                         //�кű���DC
	DWORD    m_FontSize = 26;                                       //�кſ��
	HBITMAP m_HEditBmp = 0;                                         //����BMPλͼ
	HIGHEDITARR     m_Highlight;                                    //�����ı�����
	LPTSTR m_AppStr[142] = {
	"r9","rbx","int","__asm","enum","__multiple_inheritance","template","auto","__except","__single_inheritance","this","__based",       //10
	"explicit","__virtual_inheritance","thread","bool","extern","mutable","throw","break","false","naked","true",       //11
	"case","__fastcall","namespace","try","catch","__finally","new","__try","__cdecl","float","operator","typedef",     //12
	"char","for","private","typeid","class","friend","protected","typename","const","goto","public","union",            //12
	"const_cast","if","register","unsigned","continue","inline", "reinterpret_cast","using","__declspec",               //10
	"__inline","return","uuid","default","short","__uuidof","delete","__int8","signed","virtual","dllexport",     //12
	"__int16","sizeof","void","dllimport","__int32","static","volatile","do","__int64","static_cast","wmain",           //11
	"double","__leave","__stdcall","while","dynamic_cast","long","struct","xalloc","else","main","switch","interface",  //12
	"persistent","_persistent","transient","depend","ondemand","cset","useindex","indexdef","while",        //10
	"wchar_t","cset","indexdef","dword","hfont","lpcstr", //12
	"hwnd","hinstance","long_ptr","point","bitmap","tchar","dword64","hmodule","hcurcor","LRESULT lresult","nullptr",   //12
	"false","uint","long","uint64","long64","longlong","ulonglong","ulong64","ulong","#define","#ifdef",         //12
	"#include","push","pushad","popad","mov","eax"	,"ebx"	,"ecx"	,"edx","esp"	,"ebp"	,"esi"	,"edi","call","int"		//14	
																													//1
	};


private:
	//��������
	HWND         m_hWnd = 0;               //��������
	HWND           m_fWnd = 0;              //������
	DWORD          m_Child = IDC_EDIT1;     //�Ӵ���ID
	HINSTANCE      m_hModule = 0;        //ʵ�����
	LONG_PTR       m_OEDITProc = 0;  //ԭ���Ĵ��ڹ���
	CEditList      m_EDitStr;        //�ı�ÿһ������
	MYTIME          m_POs = { 0 };   //�������λ��
private:
	//	Line/character dimensions ����/�ַ��ߴ�
	int m_nLineHeight = -1, //��������߶�
		m_nCharWidth = -1;  //����������

	//	Amount of lines/characters that completely fits the client area
	//��ȫ�ʺϹ�����������/�ַ���
	int            m_nMaxLineLength = -1;    //����г���
	int            m_nIdealCharPos = -1;     //���λ��
	PTCHAR         m_pTextBuffer = 0;        //�ı�������
	int            m_s_max=0;                  //��¼�ϴ��ַ�����
	TCHAR          m_PromptText[101];               //������ʾ�ı�
	DWORD64        m_maxTextLength = 0;            //�ı�����ָ��
	MYSELPOS       m_SelPos = { 0 };                       //ѡ��λ��
}*PScintillaWin, * LPScintillaWin;


typedef struct _WinArr
{
	HWND hwnd = 0;      //���ھ��
	HWND fhwnd = 0;     //�����ھ��
	HWND fzhwnd = 0;    //
	PScintillaWin Windows = 0; //�������ַ
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
	//��������С
	int GetSize()
	{
		return m_len;
	}
	//�������
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
	//�����ֵ
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
	//������ֵ
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

typedef struct _MYOPAIN
{
	PWinArr Windows = 0; //�������ַ
	DWORD         dx = 0;      //�������
	PCEditList    p = 0;       //�����ַ���
	HDC           dc = 0;      //���ı�dc
	HDC           fdc = 0;      //���к�dc
	DWORD         m_Max = 0;   //��������
	PDWORD        m_Count = 0; //��������
	PDWORD        m_ret = 0;   //�ж��߳��Ƿ񷵻�
}MYOPAIN,*PMYOPAIN,*LPMYOPAIN,*NPMYOPAIN;

void EditThreadProc(PMYOPAIN p1)
{
	if (!p1->Windows||!p1->dc || !p1->fdc || !p1->m_Max || !p1->m_Count || !p1->m_ret)
	{
		*(PDWORD)p1->m_ret = 1;//�ɹ���ֵ;
		return;
	}
	MYOPAIN pptrq;
	memcpy(&pptrq,p1,sizeof(MYOPAIN));
	PMYOPAIN p = &pptrq;
	int m_max = 0;
	if (p->m_Max - p->dx*30 >=30)
	{
		m_max = 30;
	}
	else
	{
		m_max = p->m_Max - p->dx * 30;
	}


	PMyCEDIT pp=0;
	char t_ch1[30] = { 0 };

	*(PDWORD)p1->m_ret = 1;//�ɹ���ֵ;

	for (int i =p->dx*30; i < p->dx * 30+m_max; i++)
	{
		if (i>30)
		{
			i = i;
		}
		pp= p->p->GetEditStr(i);
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
		p->Windows->Windows->OnDrawLineNum(p->fdc, &t_editp, i);

			if (*(DWORD64*)p->Windows->Windows->GetProcP(GET_EDIT_LENGTH)==0)//GET_EDIT_LENGTH
			{
				MyCEDIT pp1;
				char* ppt110 = (char*)p->Windows->Windows->GetProcP(GET_TIPS_TEXT);//m_pTextBuffer
				PCHAR ppt111 = (char*)p->Windows->Windows->GetProcP(GET_BUFF_TEXT);
				if (ppt110)
				{
					sprintf(pp1.m_str, ppt110, strlen(ppt110));
					pp1.m_Strlen = strlen(ppt110);
					//p->Windows->Windows->OnDrawText(p->dc, &pp1, 0);
					DWORD opcor=::SetTextColor(p->dc, RGB(0xF3, 0x05, 0x05));
					HWND hwnd = p->Windows->Windows->GetWindowHwnd();
					if (!hwnd)
					{
						*(DWORD*)(p->m_Count) = *p->m_Count + 1;
						return;
					}
					CRect rct;
					::GetClientRect(hwnd, rct);
					SIZE t_ww = { 0 };
					GetTextExtentPoint32(p->dc, pp1.m_str, pp1.m_Strlen, &t_ww);
					int x = (rct.Width() / 2) - (t_ww.cx/2);
					if (x<0)
					{
						x = 0;
					}
					::TextOut(p->dc,x,0,pp1.m_str,pp1.m_Strlen);
					opcor = ::SetTextColor(p->dc, opcor);
				}
			}
			else
			{
				p->Windows->Windows->OnDrawText(p->dc, pp, i);
			}
			
		*(DWORD*)(p->m_Count) = *p->m_Count + 1;

	}
	return;
}
//���캯��
ScintillaWin::ScintillaWin()
{
	//��ʼ�������ı�����
	//m_Highlight
		for (size_t i = 0; i < sizeof(m_AppStr)/ sizeof(LPTSTR); i++)
		{
			if (!m_AppStr[i])
			{
				return;
			}
	/*		if (i==14)
			{
				i = i;
			}*/
			HIGHEDIT pp;
			memset(&pp,0, sizeof(HIGHEDIT));
			pp.m_Strlen = strlen(m_AppStr[i]);
			memcpy(pp.m_str, m_AppStr[i], pp.m_Strlen);
			m_Highlight.Addm_str(&pp);
		}

		SetPromptText("�������ı�! , ���������ʾ�Զ���ʧ!");

		 AddEditStr("int call push ptr eax ebx ecx edx ebp esp esi edi rax rbx rcx rdx rbp rsp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15 mov rax rcx dword qword test int3");
		 AddEditStr("pop mov sub pus lea xor int test je ja and jmp ror js call cmp jns popad cmove ret retn add set jae jne bt btr jbe syscall cmo lock imu inc retn dec out ins jo arp lod dec fim div rcr neg mul xch sbb db or shr r8");
		 AddEditStr("raxd rbxd rdxd rcxd rbpd rspd rsid rdid r8d rbxd r9d r10d r11d r12d r13d r14d r15d rip word byte ds fs ss gs es cs dr0 dr1 dr2 dr3 dr4 dr5 dr6 dr7 ymm0 ymm1 ymm2 ymm3 ymm4 ymm5 ymm6 ymm7 ymm8 ymm9 ymm10 ymm11 ymm12 ymm13 ymm14 ymm15");
		 AddEditStr("xmm0 xmm1 xmm2 xmm3 xmm4 xmm5 xmm6 xmm7 xmm8 xmm9 xmm10 xmm11 xmm12 xmm13 xmm14 xmm15 mm0 mm1 mm2 mm3 mm4 mm5 mm6 mm7 move movdqu xmmword");
		 //�����߼�����

	//ԭ�����ӣ�https ://blog.csdn.net/softn/article/details/51718347
}

//�麯��
ScintillaWin::~ScintillaWin()
{
	m_EDitStr.Clear();
	::DeleteObject(m_Font);//ɾ������
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
//�����Զ�����Ϣ
LRESULT ScintillaWin::SendMessage_(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(m_hWnd,msg,wParam,lParam);
}
//���ñ༭��ؼ�ѡ�е�λ�� EM_SETSEL
void ScintillaWin::SetTextSelPos(DWORD k, DWORD j, DWORD dwStyle)
{
	if (dwStyle== 'A')
	{
		::SendMessage(m_hWnd, EM_SETSEL, 0,-1);
		return;
	}
	::SendMessage(m_hWnd, EM_SETSEL, k, j);
	return;
}
//��ȡ����RECT
void ScintillaWin::GetStrRect(HDC dc,CRect* p, const char* str,DWORD len)
{
	SIZE t_hh = { 0 };
	memset(p, 0, sizeof(CRect));
	if (len<=40&&len>=0)
	{
		GetTextExtentPoint32(dc, str, len, &t_hh);
		
		p->bottom = t_hh.cy;
		p->right = t_hh.cx;
	}


	return;
}
//��ȡѡ������
int ScintillaWin::GetSelPosText(PMYSELPOS pos,PINT pt)
{
	DWORD start = 0; PMyCEDIT p; int a = 0, b = 0;
	for (size_t i = 0; i < m_EDitStr.Getlen(); i++)
	{
		p = m_EDitStr.GetEditStr(i);
		if ((m_SelPos.start>= start)&& m_SelPos.start <= (start + p->m_Strlen) )
		{
			*(PDWORD)&pos->start = i;
			a = 1;
			*(PINT)pt = start;
		}
		if ((m_SelPos.end >= start) && m_SelPos.end <= (start + p->m_Strlen))
		{
			*(PDWORD)&pos->end = i;
			b = 1;
		}
		start += p->m_Strlen+2;
		if (a==1 && b==1)
		{
			if (pos->end== pos->start)
			{
				*(PDWORD)&pos->end = pos->end + 1;
				a = 1;
				return a;
			}
			return abs((long)(pos->end - pos->start))+1;
		}
	}
	return 0;
}
//ʹ�õ�ǰ����ʹ�õ�ǰ���ʻ���һ�����β�ʹ�õ�ǰ���ʽ�����䡣
BOOL ScintillaWin::Rectangle(_In_ HDC dc, _In_ int left, _In_ int top, _In_ int right, _In_ int bottom, DWORD cColor)
{
	ASSERT(dc != NULL);
	HPEN  Pen_ = CreatePen(PS_SOLID, 1, cColor);
	HPEN  oPen_ = (HPEN)::SelectObject(dc,Pen_);
	HBRUSH resut = CreateSolidBrush(cColor);
	HBRUSH oresut = (HBRUSH)::SelectObject(dc, resut);
	BOOL ret= ::Rectangle(dc, left, top, right, bottom);
	::SelectObject(dc, oPen_);
	::SelectObject(dc, oresut);
	DeleteObject(Pen_);
	DeleteObject(oresut);

	return ret;
}
//ʹ�õ�ǰ����ʹ�õ�ǰ���ʻ���һ�����β�ʹ�õ�ǰ���ʽ�����䡣
BOOL ScintillaWin::Rectangle(_In_ HDC dc, LPCRECT lpRect, DWORD cColor)
{
	ASSERT(hdc != NULL); 
	
	ASSERT(dc != NULL);
	HPEN  Pen_ = CreatePen(PS_SOLID, 1, cColor);
	HPEN  oPen_ = (HPEN)::SelectObject(dc, Pen_);
	HBRUSH resut = CreateSolidBrush(cColor);
	HBRUSH oresut = (HBRUSH)::SelectObject(dc, resut);
	BOOL ret = ::Rectangle(dc, lpRect->left, lpRect->top,lpRect->right, lpRect->bottom);
	::SelectObject(dc, oPen_);
	::SelectObject(dc, oresut);
	DeleteObject(Pen_);
	DeleteObject(oresut);
	
	return ret;
}
//���ѡ����ɫ
BOOL ScintillaWin::SetSelPosColor(HDC dc ,DWORD coten,DWORD h)
{
	if ((m_SelPos.end==0&& m_SelPos.start == 0 )||m_nLineHeight==0)
	{
		return FALSE;
	}
	
	CRect rtc;



	DWORD start = 0;
	DWORD stlen = m_SelPos.end - m_SelPos.start;
		if (m_SelPos.start== 0&& m_SelPos.end== strlen(m_pTextBuffer)||(m_SelPos.start == -1&& m_SelPos.end ==-1))
		{
			for (size_t i = 0; i < coten; i++)
			{
				rtc.left = 0;
				rtc.top = i * m_nLineHeight;
				rtc.bottom = rtc.top + m_nLineHeight;
				SIZE t_hh = { 0 };
				GetTextExtentPoint32(dc, m_EDitStr.GetEditStr(i)->m_str, m_EDitStr.GetEditStr(i)->m_Strlen, &t_hh);
				rtc.right = rtc.left + t_hh.cx + 3;
				Rectangle(dc, &rtc, m_SelPosColor);
			}
			ReleaseDC(m_hWnd, dc);
			return FALSE;
		}
		//HDC pdc = GetDC(m_hWnd);
		PMyCEDIT p = 0;
		CRect    rtc1;
		start = 0;
		MYSELPOS pos;
		int aln = GetSelPosText(&pos,(int*)&start);
		for (size_t i =pos.start; i < pos.start+aln; i++)
		{
			p = m_EDitStr.GetEditStr(i);
			if (i== pos.start)
			{
				if (aln==1)
				{
					GetStrRect(dc, &rtc, p->m_str, m_SelPos.start - start);
					GetStrRect(dc, &rtc1, p->m_str+ (m_SelPos.start - start), m_SelPos.end - m_SelPos.start);
					rtc1.left += rtc.right;
					rtc1.top = i * m_nLineHeight;
					rtc1.bottom += rtc1.top;
					rtc1.right += rtc1.left;

					Rectangle(dc, &rtc1, m_SelPosColor);
					return TRUE;
				}
				else  if (i!= pos.start &&i != (pos.start + aln) - 1)
				{
					GetStrRect(dc, &rtc, p->m_str, p->m_Strlen);
					rtc.left = 0;
					rtc.top = i * m_nLineHeight;
					rtc.bottom += rtc.top;
					rtc.right += rtc.left;
					Rectangle(dc, &rtc, m_SelPosColor);
					start += p->m_Strlen + 2;
					return TRUE;
				}
				else
				{
					GetStrRect(dc, &rtc, p->m_str, m_SelPos.start - start);
					GetStrRect(dc, &rtc1, p->m_str + (m_SelPos.start - start),p->m_Strlen- (m_SelPos.start - start));
					rtc1.left = +rtc.right;
					rtc1.top = i * m_nLineHeight;
					rtc1.bottom += rtc1.top;
					rtc1.right += rtc1.left;

					Rectangle(dc, &rtc1, m_SelPosColor);
					start += p->m_Strlen + 2;
				}

				continue;
			}
			else if (i == (pos.start+aln) - 1)
			{
				GetStrRect(dc, &rtc1, p->m_str , m_SelPos.end -start);
				rtc1.left = 0;
				rtc1.top = i * m_nLineHeight;
				rtc1.bottom += rtc1.top;
				rtc1.right += rtc1.left;

				Rectangle(dc, &rtc1, m_SelPosColor);
				start += p->m_Strlen + 2;
				return TRUE;
			}
			else
			{
				GetStrRect(dc, &rtc, p->m_str, p->m_Strlen);
				rtc.left = 0;
				rtc.top = i * rtc1.Height();
				rtc.bottom += rtc.top;
				rtc.right += rtc.left;
				Rectangle(dc, &rtc, m_SelPosColor);
				start += p->m_Strlen + 2;
				continue;
			}
		
	

		}

	//ReleaseDC(m_hWnd, dc);
	return TRUE;
}

//��ȡѡ��λ��
void ScintillaWin::GetSelPos()
{
	::SendMessage(m_hWnd, EM_GETSEL,(WPARAM)&m_SelPos.start,(LPARAM)&m_SelPos.end);
	if (m_SelPos.start== m_SelPos.end)
	{
		memset(&m_SelPos, 0, sizeof(MYSELPOS));
	}
	if (m_SelPos.start==-1|| m_SelPos.end == -1)
	{
		m_SelPos.start = -1;
		m_SelPos.end = -1;
	}
}
//���ñ༭�ؼ���ʾ�ı�
BOOL ScintillaWin::SetPromptText(const char* str)
{
	if (!str)
	{
		return FALSE;
	}
	int	stln = strlen(str);
	if (stln >= 100)
	{
		stln = 100;
	}
	memcpy(m_PromptText, str, stln);
	return TRUE;
}
//��Ӹ��������ı�
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
		OnDraw();
		return TRUE;
	}
	return FALSE;
}
//�ڶ��б༭�ؼ��д�ֱ�����ı� ���أ�������ı�λ��
int  ScintillaWin::SetRollTextPos(DWORD  dwStyle, DWORD dx )
{
	switch (dwStyle)
	{
	case SB_LINEDOWN:
	{
		SetRollTextPos_1(SB_LINEDOWN, 3);
		break;
	}
	case SB_LINEUP:
	{
		SetRollTextPos_1(SB_LINEUP, 3);
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

//��ֱ�����ı�����
void ScintillaWin::SetRollTextPos_1(DWORD  dwStyle, DWORD dx)
{
	for (size_t i = 0; i < dx; i++)
	{
		::SendMessage(m_hWnd, EM_SCROLL, dwStyle, 0);
	}
}

//��ȡ�ı�
char* ScintillaWin::GetEditText()
{
	return m_pTextBuffer;
}

//�����ı�
BOOL ScintillaWin::SetEditText(const char* str)
{
	BOOL ret = SetWindowText(m_hWnd, str);
	//SendMessage(m_fWnd, WM_COMMAND, IDC_EDIT1, EN_CHANGE);
	OnPain();

	return TRUE;
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

//����ı�
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

//��ȡ���ı�
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
//�鿴�༭�������Ƿ��޸�
BOOL  ScintillaWin::GetTextModify()
{
	return ::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0);
}
//���ñ༭�������޸�
BOOL  ScintillaWin::SetTextModify(BOOL ret)
{
	return ::SendMessage(m_hWnd, EM_SETMODIFY, ret, 0);
}
//����Ƿ��Ѿ�����
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

	LONGLONG atrrr = GetTemehou();//��ȡ��ǰʱ���־
	
	DWORD iStyle = 0;

	if (m_POs.tiem == 0)
	{iStyle = SW_SHOW;m_POs.tiem = atrrr;}else
	{
		if (atrrr- m_POs.tiem<100){return;}
		if (m_POs.ret == TRUE)
		{iStyle = SW_SHOW ;}else{iStyle = SW_HIDE;}
	}
	OnDraw(0, 0);
	int nLineHeight = GetLineHeight();int rle = GetTheVisibleCount();
	CRect rct;
	::GetClientRect(m_fWnd, rct);
	HDC pdc = GetDC(m_hWnd);
	switch (iStyle)
	{
	case SW_SHOW://��ʾ
	{
		HDC cacheDC = ::CreateCompatibleDC(m_fdc);//�ڴ�༭��dc
		HBITMAP HEditBmp = CreateCompatibleBitmap(m_fdc, 1, nLineHeight-2); //�ڴ�༭��λͼ
		HBITMAP pOHEditmap = (HBITMAP)::SelectObject(cacheDC, HEditBmp);        //�ڴ滭�༭��λͼ

		::BitBlt(pdc, pt.x, pt.y, 1, nLineHeight-2, cacheDC, 0, 0, SRCCOPY);
		::SelectObject(cacheDC, pOHEditmap);
		DeleteObject(HEditBmp);
		::DeleteDC(cacheDC);
		m_POs.tiem = GetTemehou();
		m_POs.ret = FALSE;
		break;
	}
	case SW_HIDE: //����
	{
		::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
		m_POs.tiem = GetTemehou();
		m_POs.ret = TRUE;
		break;
	}
	}
	ReleaseDC(m_hWnd, pdc);
	return;
}

void ScintillaWin::OnPain(DWORD iStyle, DWORD usid)
{

	//��ȡ������dc
	if (m_fdc == NULL)
	{
		m_fdc = GetDC(m_fWnd);

	}
	DWORD64 s_max = GetWindowTextLength(m_hWnd);
	m_maxTextLength = s_max;


		if (s_max == 0)
		{
			if (m_s_max == 1)
			{
				return;
			}
			CRect rct;
			::GetClientRect(m_fWnd, &rct);
			rct.left = 0;
			rct.right = rct.left + 26;
			FillSolidRect(m_fdc, rct, m_LineBackgColor);
			OnDraw();
			SetTextModify(TRUE);
			m_s_max = s_max + 1;
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
	
	if (iStyle == WM_LBUTTONUP)
	{
		GetSelPos();
		OnDraw();
		SetTextModify(TRUE);
		return;
	}
	int conet = GetLineCount();
	if (!t_hch.AddTextBuff(s_max + 6) || !t_hch.AddTextstr(m_hWnd, conet, m_pTextBuffer, strlen(m_pTextBuffer)))
	{
		return;
	}

	m_s_max = s_max + 1;

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


	POINT po1;
	::GetCaretPos(&po1);

	SetCaretPos(po1);
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
		int conet = rct.Height() / nLineHeight;//����༭��������ɶ�����
		int xconet = GetLineCount();               //��ȡ�༭�������ֶ�����
		if (xconet > conet)
		{
			switch (iStyle)
			{
			case ROLLER_UP://��������
			{
				if (rle == 0)
				{
					return;
				}
				rle = SetRollTextPos(SB_LINEUP, 1);      //���Ϲ���һ�С�
				::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
				return;
			}
			case ROLLER_DOWN://��������
			{

				rle = SetRollTextPos(SB_LINEDOWN, 1);    //���¹���һ�С�
				::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
				return;
			}
			break;
			}
		}

	}
	// ֱ�����豸DC�ϻ���	
	rle = GetTheVisibleCount();
	::BitBlt(m_fdc, 0, 0, rct.Width(), rct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);
}
void ScintillaWin::OnDraw()
{
	while (m_DrawRet)
	{
		Sleep(10);
	}
	m_DrawRet = TRUE;

	CRect rcClient;
	GetClientRect(&rcClient);//��ȡ�༭��RECT
	CRect frct;
	::GetClientRect(m_fWnd, &frct);//��ȡ�к�RECT
	int nLineCount = GetLineCount();
	if (nLineCount >= 0 && nLineCount <= 99)//С��100��
	{
		m_FontSize = m_nCharWidth * 2 + 1;
	}
	else if (nLineCount >= 100 && nLineCount <= 999)//С��1000��
	{
		m_FontSize = m_nCharWidth * 3 + 1;
	}
	else if (nLineCount >= 1000 && nLineCount <= 9999)//С��10000��
	{
		m_FontSize = m_nCharWidth * 4 + 1;
	}
	else if (nLineCount >= 10000 && nLineCount <= 99999)//С��10000��
	{
		m_FontSize = m_nCharWidth * 5 + 1;
	}
	else
	{
		m_DrawRet = FALSE;
		return;  //����100000�˳�
	}

	rcClient.left = frct.left + m_FontSize;
	rcClient.right = frct.left + frct.right;
	MoveWindow(m_hWnd, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), TRUE);
	GetClientRect(&rcClient);//��ȡ�༭��RECT
	int nLineHeight = GetLineHeight();



	HDC pdc = GetDC(m_hWnd); //�༭��dc 

	int t_h = nLineCount * nLineHeight;
	if (t_h > frct.Height())
	{

		rcClient.bottom = rcClient.top + t_h + 34;
		frct.bottom += frct.top + t_h + 34;
	}


	HDC cacheDC = ::CreateCompatibleDC(pdc);//�ڴ�༭��dc
	HBITMAP HEditBmp = CreateCompatibleBitmap(pdc, rcClient.Width(), rcClient.Height()); //�ڴ�༭��λͼ
	HBITMAP pOHEditmap = (HBITMAP)::SelectObject(cacheDC, HEditBmp);        //�ڴ滭�༭��λͼ


	m_cacheDC_1 = ::CreateCompatibleDC(m_fdc);//�ڴ��к�dc
	m_HEditBmp = CreateCompatibleBitmap(m_fdc, frct.Width(), frct.Height()); //�ڴ��к�λͼ
	HBITMAP pOHEditmap_1 = (HBITMAP)::SelectObject(m_cacheDC_1, m_HEditBmp);        //�ڴ��к�λͼ

	FillSolidRect(cacheDC, rcClient, m_BackgRoundColor);//���༭�򱳾�ɫ
	FillSolidRect(m_cacheDC_1, frct, m_LineBackgColor);//����кű���ɫ
	if (m_Font)
	{
		::SelectObject(cacheDC, m_Font);
		::SelectObject(m_cacheDC_1, m_Font);
	}
	

	SetSelPosColor(cacheDC, nLineCount, nLineHeight);

	MYOPAIN pp;
	pp.dc = cacheDC;
	pp.fdc = m_cacheDC_1;
	pp.m_Count =(PDWORD)&nLineCount;
	pp.m_Max = nLineCount;
	pp.p = &m_EDitStr;
	pp.Windows = aman.GetVae(m_hWnd);

	int n_max = nLineCount % 30;
	
	if (nLineCount <= 30)
	{
		n_max = 1;
	}
	else
	{
		if (n_max != 0)
		{
			n_max = nLineCount / 30 + 1;
		}
		else
		{
			n_max = nLineCount / 30;
		}
	}
	DWORD ret = FALSE;
	pp.m_ret = &ret;
	nLineCount = 0;

	PMyClass hadle = new MyClass[n_max];
	//memset(hadle, 0, sizeof(thread) * n_max);

	for (size_t i = 0; i < n_max; i++)
	{
		ret = FALSE;
		pp.dx = i;
		hadle[i].Createthread(EditThreadProc, &pp);
		while (ret == FALSE)
		{
			//Sleep(1);
			nLineCount = nLineCount;
		}
	}
	for (size_t i = 0; i < n_max; i++)
	{
		hadle[i].join();
	}

		

	

	//MAXIMUM_WAIT_OBJECTS
	int rle = GetTheVisibleCount();
	
	delete[] hadle;//ɾ���߳̾��

	CRect rtc1, rtc2;
	GetWindowRect(m_fWnd, &rtc1);
	GetWindowRect(m_hWnd, &rtc2);
	int ax= abs(rtc2.left - rtc1.left) ;
	::BitBlt(m_cacheDC_1, ax, 0, frct.Width(), frct.Height(), cacheDC, 0, 0, SRCCOPY);

	::BitBlt(m_fdc, 0, 0, frct.Width(), frct.Height(), m_cacheDC_1, 0, rle * nLineHeight, SRCCOPY);

	::SelectObject(cacheDC, pOHEditmap);
	DeleteObject(HEditBmp);
	DeleteDC(cacheDC);


	ReleaseDC(m_hWnd, pdc);
	m_DrawRet = FALSE;
}

//���ı����� ,��ɫϵͳ
void ScintillaWin::OnDrawText(HDC dc, PMyCEDIT sp, DWORD dx)
{
	if (!sp->m_str)
	{
		return;
	}
	while (m_DrawLock)//
	{
		Sleep(1);
	}
	m_DrawLock = TRUE;

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
			if ((stlen - star) < ph->m_Strlen)
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
		//������
		if (sp->m_str[star] == ',')
		{
			::SetTextColor(dc, m_HighlightColor);
			::TextOut(dc, t_ww.cx, dx * m_nLineHeight, sp->m_str + star, 1);
			star += 1;
			aend = 0;
			continue;
		}
		//m_DigitalColor;
		if (sp->m_str[star] >= '0' && sp->m_str[star] <= '9'|| (p[star] >= 'A' && p[star] <= 'F') || (p[star] >= 'a' && p[star] <= 'f'))
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
							if (!((p[j] >= '0' && p[j] <= '9') || ((p[j] >= 'A' && p[j] <= 'F') || (p[j] >= 'a' && p[j] <= 'f'))))
							{
								break;
							}
						}
						aend = j;
						ret = TRUE;
						break;
					}
					if (!((p[j] >= '0' && p[j] <= '9') || ((p[j] >= 'A' && p[j] <= 'F') || (p[j] >= 'a' && p[j] <= 'f'))))
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
									if (!(p[j] >= 'A' && p[j] <= 'F'))
									{
										if (!(p[j] >= 'a' && p[j] <= 'f'))
										{
											break;//123654
										}
									}
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
							if (!(p[j] >= 'A' && p[j] <= 'F'))
							{
								if (!(p[j] >= 'a' && p[j] <= 'f'))
								{
									break;//123654
								}
							}
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
	
	m_DrawLock = FALSE;
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
		return &m_POs; //����ѡ��Χ����
	}
	case GET_BUFF_TEXT:
	{
		return &m_pTextBuffer;
	}
	case GET_DRAW_RET:
	{
		return &m_DrawRet;
	}
	case GET_TIPS_TEXT:
	{
		return &m_PromptText;//GET_EDIT_LENGTH
	}
	case GET_FONT_1:
	{
		return &m_Font;
	}
	case GET_EDIT_LENGTH:
	{
		return &m_maxTextLength;
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
		m_hWnd,  //����������Ŀؼ����
		WM_SETFONT,  //��Ϣ������Ϣ���ͣ�
		(WPARAM)*font,  //������
		NULL  //����ֵ����
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
	::DeleteObject(m_Font);//ɾ������
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
	strcpy(logFont.lfFaceName, "΢���ź�");

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

	DWORD dw=::SetBkColor(dc, clr);
	BOOL ret= ::ExtTextOut(dc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
	::SetBkColor(dc, dw);
	return	ret;
}
BOOL g_mct = 0;

 BOOL Mybool=FALSE;

LRESULT CALLBACK ScintillaWin::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{

	case WM_LBUTTONDOWN:
	{
		PWinArr p1 = aman.GetVae(hWnd);
		BOOL* bol = (BOOL*)p1->Windows->GetProcP(GET_DRAW_RET);
		//p1->Windows->OnPain(2,0);
		if (*bol)
		{
			return FALSE;
		}
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
			BOOL* bol = (BOOL*)p1->Windows->GetProcP(GET_DRAW_RET);
			//p1->Windows->OnPain(2,0);
			if (*bol)
			{
				return FALSE;
			}
			p1->Windows->OnPain(WM_LBUTTONUP,0);
			g_mct = FALSE;
		}
		
		break;
	}
	case SET_EDIT_TEXT_SEL://ȫѡ�ı�
	{
		PWinArr p1 = aman.GetVae(hWnd);
		p1->Windows->SetTextSelPos(0, 0, 'A');
		p1->Windows->OnPain(WM_LBUTTONUP, 0);
		return FALSE;
	}
	case WM_KEYDOWN:
	{
		if (wParam == 'A' && ::GetKeyState(VK_CONTROL) < 0)
		{
			PWinArr p1 = aman.GetVae(hWnd);
			BOOL* bol = (BOOL*)p1->Windows->GetProcP(GET_DRAW_RET);
			//p1->Windows->OnPain(2,0);
			if (*bol)
			{
				return FALSE;
			}
			p1->Windows->SetTextSelPos(0, 0, 'A');
			p1->Windows->OnPain(WM_LBUTTONUP, 0);
			return FALSE;
		}
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
			BOOL* bol = (BOOL*)p1->Windows->GetProcP(GET_DRAW_RET);
			//p1->Windows->OnPain(2,0);
			if (*bol)
			{
				return FALSE;
			}


			p1->Windows->OnPain();
			g_mct = FALSE;
		}
		
		break;
	}
	//case WM_MOUSEMOVE:
	//{
	//	// return FALSE;
	//}
	case WM_MOUSEWHEEL:
	{
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		PWinArr p1 = aman.GetVae(hWnd);
		if (p1)
		{
			PWinArr p1 = aman.GetVae(hWnd);
			BOOL* bol = (BOOL*)p1->Windows->GetProcP(GET_DRAW_RET);
			//p1->Windows->OnPain(2,0);
			if (*bol)
			{
				return FALSE;
			}
			if (delta > 0)
			{
				p1->Windows->OnPain(ROLLER_UP, delta);//���������Ϲ���
			}
			else
			{
				p1->Windows->OnPain(ROLLER_DOWN, delta);//���������¹���
			}
			return TRUE;
		}
		
	}

	case  WM_PAINT:
	{
		if (Mybool)
		{
			break;
		}
			PWinArr p1 = aman.GetVae(hWnd);

            if (p1&& ::GetFocus()== hWnd)
            {
				//::GetFocus()
				BOOL* bol=(BOOL*)p1->Windows->GetProcP(GET_DRAW_RET);
            	//p1->Windows->OnPain(2,0);
				if (*bol)
				{
					return FALSE;
				}
				POINT po1;
				::GetCaretPos(&po1);
				
					p1->Windows->SetCaretPos(po1);
					return FALSE;
            }

		p1->Windows->OnPain(0, 0);//���������¹���
		return FALSE;
	}

	default:
		break;
	}

	//����һ��Ҫע�⣬����ֻ����������Ҫ�������Ϣ�����ǲ���Ҫ�������Ϣ�Ÿ�ԭ���Ļص���������
	PWinArr p1 = aman.GetVae(hWnd);

	ULONG_PTR  Onproc = p1->Windows->GetEditProc();
	return CallWindowProc((WNDPROC)Onproc, hWnd, msg, wParam, lParam);
}
//ԭ�����ӣ�https ://blog.csdn.net/hbllll1/article/details/115676312

LRESULT ScintillaWin::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
	{
		DWORD  usg = LOWORD(wParam); //: �ؼ�ID
		if (usg == IDC_EDIT1)
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE://���༭������ı�
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
		p1->Windows->OnDraw(ROLLER_UP,120);

		break;;
	}
	case WM_DESTROY:
	{
		//�������ƺ���
		PWinArr p = aman.GetVae(hwnd);
		if (p)
		{
			::DeleteObject(p->Windows->GetProcP(GET_FONT_1));//ɾ������������
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
	HBRUSH hBrush = CreateSolidBrush(RGB(0xc8, 0xc8, 0xc8));//��ͼ��������ɫ
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
	// �������б༭�ؼ�
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
	strcpy(logFont.lfFaceName, "΢���ź�");

	hFont = CreateFontIndirect(&logFont);

	//ԭ�����ӣ�https ://blog.csdn.net/softn/article/details/51718347

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
	p->Windows->SetPromptText("�������ı���");
	//HFONT pp = (HFONT)p->Windows->GetProcP(GET_FONT_1);
	   //*(HFONT*)pp = hFont;
   //MSG msg;
   //while (GetMessageW(&msg, NULL, 0, 0)) {

   //	TranslateMessage(&msg);
   //	DispatchMessageW(&msg);
   //}


	return m_hWnd;
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

	// �������б༭�ؼ�
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

	//���ÿؼ�����
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	return p->Windows->SetEditFont(font);
}

BOOL Scintilla::SetEditFontSize(DWORD fontSize)//SetEditFontSize
{
	//���ÿؼ������С
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	return p->Windows->SetEditFontSize(fontSize);
}

//������ɫ����
BOOL Scintilla::SetEditColor(DWORD dwStyle, COLORREF clr)
{
	//���ÿؼ�����
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	return p->Windows->SetEditColor(dwStyle, clr);
}

//��Ӹ��������ı�
BOOL Scintilla::AddEditStr(const char* str)
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	return p->Windows->AddEditStr(str);
}

// ���±༭�򴰿�
void Scintilla::UpdateEditWindow()
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return ;
	}

	return p->Windows->OnDraw(0, 0);
}

//���ñ༭�ؼ���ʾ�ı�
BOOL Scintilla::SetPromptText(const char* str)
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}

	return p->Windows->SetPromptText(str);
}

LRESULT Scintilla::SendMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	PWinArr p = aman.GetVae(m_hWnd);
	if (!p)
	{
		return FALSE;
	}
	
	return p->Windows->SendMessage_(msg,  wParam, lParam);

}