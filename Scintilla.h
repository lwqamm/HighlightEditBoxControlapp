#pragma once

// 符号视为是被导出的。


//#ifdef DLLEXPORT_EXPORTS
//#define DLLEXPORT_API   _declspec(dllexport)
//#else
//#define DLLEXPORT_API   _declspec(dllimport)
//#endif

#define  AFX_MFC
// 此类是从 dllExport.dll 导出的
#define SET_EDITBACKCOLOR              0x0                    //编辑框背景颜色
#define SET_EDITFONTCOLOR              0x1                    //编辑框字体颜色
#define SET_EDITHIGHLICOLOR            0x2                    //字体高亮颜色
#define SET_EDITDIGITALCOLOR           0x3                    //数字颜色
#define SET_EDITNOTECOLOR              0x4                    //注释颜色
#define SET_EDITLINEBACKGCOLOR         0x5                    //行号背景颜色
#define SET_EDITLINECOLOR              0x6                    //行号颜色
 class  Scintilla
{

public:
	 HWND Create(HWND hwnd,int x,int y,int w,int h, DWORD ID);
#ifdef AFX_MFC
	 CString  GetEditText();
	 
#else
	 BOOL GetEditText(OUT char* p);
#endif
	 //设置编辑框文本
	 BOOL     SetEditText(IN  const char* str);
	 //清空编辑框文本
	 BOOL     ClearEditText();
	 BOOL     SetEditFont(HFONT* font);

	 //设置字体大小
	 BOOL     SetEditFontSize(DWORD fontSize);
	 //设置颜色参数
	 BOOL     SetEditColor(DWORD  dwStyle, COLORREF clr);
	 //
	 /*
	 功能：添加高亮过滤文本
	 参数：过滤文本 不用区分大小写 以空格分开成多个单词
	 样式        "push bool FALSE int"
	 返回值：是否添加成功
	 */
	 BOOL AddEditStr(const char* str);
	 
private:
	HWND m_hWnd = 0;     //操作句柄

};

// HWND Createwin(HWND hwnd, int x, int y, int w, int h, DWORD ID);
	// TODO: 在此添加您的方法。
//extern "C" DLLEXPORT_API Scintilla * GetCat();

