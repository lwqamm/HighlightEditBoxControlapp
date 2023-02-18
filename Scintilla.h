#pragma once

// ������Ϊ�Ǳ������ġ�


//#ifdef DLLEXPORT_EXPORTS
//#define DLLEXPORT_API   _declspec(dllexport)
//#else
//#define DLLEXPORT_API   _declspec(dllimport)
//#endif

#define  AFX_MFC
// �����Ǵ� dllExport.dll ������
#define SET_EDITBACKCOLOR              0x0                    //�༭�򱳾���ɫ
#define SET_EDITFONTCOLOR              0x1                    //�༭��������ɫ
#define SET_EDITHIGHLICOLOR            0x2                    //���������ɫ
#define SET_EDITDIGITALCOLOR           0x3                    //������ɫ
#define SET_EDITNOTECOLOR              0x4                    //ע����ɫ
#define SET_EDITLINEBACKGCOLOR         0x5                    //�кű�����ɫ
#define SET_EDITLINECOLOR              0x6                    //�к���ɫ
#define SET_EDIT_TEXT_SEL              WM_USER+100            //ȫ��ѡ���ı�
 class  Scintilla
{

public:
	 HWND Create(HWND hwnd,int x,int y,int w,int h, DWORD ID);
#ifdef AFX_MFC
	 CString  GetEditText();
	 
#else
	 BOOL GetEditText(OUT char* p);
#endif
	 //���ñ༭���ı�
	 BOOL     SetEditText(IN  const char* str);
	 //��ձ༭���ı�
	 BOOL     ClearEditText();
	 BOOL     SetEditFont(HFONT* font);

	 //���������С
	 BOOL     SetEditFontSize(DWORD fontSize);
	 //������ɫ����
	 BOOL     SetEditColor(DWORD  dwStyle, COLORREF clr);
	 //
	 /*
	 ���ܣ���Ӹ��������ı�
	 �����������ı� �������ִ�Сд �Կո�ֿ��ɶ������
	 ��ʽ        "push bool FALSE int"
	 ����ֵ���Ƿ���ӳɹ�
	 */
	 BOOL AddEditStr(const char* str);
	 //���±༭�򴰿�
	 void UpdateEditWindow();
	 //���ñ༭�ؼ���ʾ�ı�
	 BOOL SetPromptText(const char* str);
	 LRESULT SendMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	 
private:
	HWND m_hWnd = 0;     //�������

};

// HWND Createwin(HWND hwnd, int x, int y, int w, int h, DWORD ID);
	// TODO: �ڴ�������ķ�����
//extern "C" DLLEXPORT_API Scintilla * GetCat();

 extern  BOOL Mybool;//������������ ��Ȼ���ò����ͷ�
