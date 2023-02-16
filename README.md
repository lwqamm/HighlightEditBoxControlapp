
![Image]([https://raw.githubusercontent.com/Gladysid/Images-blog/master/IE-box-pic.png](https://github.com/lwqamm/HighlightEditBoxControlapp/blob/main/image.png))

重绘EDIT控件接管大部分功能

哈哈自己写了自己用的 那个开源插件X86可以高亮X64就不行了！

后来才有了这个

引用也简单就几个函数

      //设置编辑框文本
      
 BOOL     SetEditText(IN  const char* str);
 
	 //清空编辑框文本
	 
	 BOOL     ClearEditText();
	 
	 BOOL     SetEditFont(HFONT* font);

	 //设置字体大小
	 
	 BOOL     SetEditFontSize(DWORD fontSize);
   {
	 //设置颜色参数;
	 
    SET_EDITBACKCOLOR           0x0                    //编辑框背景颜色
    
 SET_EDITFONTCOLOR              0x1                    //编辑框字体颜色
 
 SET_EDITHIGHLICOLOR            0x2                    //字体高亮颜色
 
 SET_EDITDIGITALCOLOR           0x3                    //数字颜色
 
 SET_EDITNOTECOLOR              0x4                    //注释颜色
 
 SET_EDITLINEBACKGCOLOR         0x5                    //行号背景颜色
 
 SET_EDITLINECOLOR              0x6                    //行号颜色
 
	 BOOL     SetEditColor(DWORD  dwStyle, COLORREF clr);
	 
   }
	 //
	 
	 /*
	 
	 功能：添加高亮过滤文本
	 
	 参数：过滤文本 不用区分大小写 以空格分开成多个单词
	 
	 样式        "push bool FALSE int"
	 
	 返回值：是否添加成功
	 
	 */
	 
	 BOOL AddEditStr(const char* str);
	 
