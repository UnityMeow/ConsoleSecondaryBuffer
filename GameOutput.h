#pragma once
#include <iostream>
#include <map>
#include <windows.h>

//图片类
class CBmp
{
	int m_w;	//图片宽
	int m_h;	//图片高
	int* index;	//图片数据
	int m_len;	//数据长度
public:
	CBmp();
	~CBmp();
	CBmp(const CBmp& that);
	CBmp& operator =(const CBmp& that);

	void SetBmpData(int w, int h, int data, ...);	//设置图片宽高图片数据
	int GetW();			//获取图片宽
	int GetH();			//获取图片高
	int* GetIndex();	//获取图片数据
};

//二次缓冲类
class CSecondaryBuffer
{
	int m_W;
	int m_H;
	HANDLE m_houtput;	//获取控制台屏幕缓冲区
	HANDLE m_hback;		//创建新控制台屏幕缓冲区
	CONSOLE_CURSOR_INFO m_cci;	//光标信息
	PCHAR_INFO m_ScreenBuffer;	//屏幕信息储存
	COORD m_pos;	//绘制位置
public:
	//创建二次缓冲
	void Iint(int w, int h);
	//设置光标信息
	void SetCursor(bool sw);
	//设置绘制位置
	void SetPos(int x, int y);
	//绘制函数
	void Write(char* str);
	//结束绘制
	void End();
};

//绘制类
class CGameOutput
{
	const char* m_Pixel;	//图素
	int m_PixelLen;			//图素长度
	//图片管理
	std::map<const char*, CBmp> m_BmpList;
	CSecondaryBuffer m_Second;
	//绘图区域
	char* m_Map;
	int m_W;
	int m_H;
	int m_S;
public:
	CGameOutput(int w, int h);	//初始化绘图区域宽高
	~CGameOutput();				//释放

	void SetSecondaryBuffer(bool Cursor_sw);//设置二次缓冲
	void SetScreenW_H(int w, int h);		//设置控制台宽高
	void LoadBmp(const char* id, CBmp bmp);	//加载图片
	void SetPixel(const char* p);			//设置图素
	

	void Begin();	//初始化画布
	void DrawBmp(const char* id, int x, int y);	//图片数据导入画布
	void End();		//绘制画布

	int GetPixelLen();	//获取图素长度
	int GetClientW();	//获取画布宽
	int GetClientH();	//获取画布高
};