#include "GameOutput.h"

CBmp::CBmp()
{
	//初始化图片数据指针
	index = 0;
}

CBmp::~CBmp()
{
	//图片数据指针不为空
	if (index)
		//释放图片数据堆
		delete[] index;
}

CBmp::CBmp(const CBmp & that)
{
	m_w = that.m_w;
	m_h = that.m_h;
	m_len = that.m_len;

	index = new int[m_len];
	for (int i = 0; i < m_len; i++)
	{
		index[i] = that.index[i];
	}
}

CBmp & CBmp::operator=(const CBmp & that)
{
	m_w = that.m_w;
	m_h = that.m_h;
	m_len = that.m_len;
	if (index)
		delete[] index;
	index = new int[m_len];
	for (int i = 0; i < m_len; i++)
	{
		index[i] = that.index[i];
	}

	return *this;
}

void CBmp::SetBmpData(int w, int h, int data, ...)
{
	//安全检测
	if (w < 0 || h < 0)
		return;
	if (index)
		delete[] index;
	//设置图片数据长度
	m_len = w * h;
	//设置图片宽
	m_w = w;
	//设置图片高
	m_h = h;
	index = new int[m_len];
	int* p = &data;
	//设置图片数据
	for (int i = 0; i < m_len; ++i)
	{
		index[i] = p[i];
	}
}

int CBmp::GetW()
{
	return m_w;
}

int CBmp::GetH()
{
	return m_h;
}

int * CBmp::GetIndex()
{
	return index;
}

CGameOutput::CGameOutput(int w, int h)
{
	//安全检测
	if (w < 1)
		w = 1;
	if (h < 1)
		h = 1;
	//初始化画布宽
	m_W = w;
	//初始化画布高
	m_H = h;
	//初始化画布面积
	m_S = w * h;
	//初始化绘图区域
	m_Map = new char[m_S];
}

CGameOutput::~CGameOutput()
{
	delete[]m_Map;
}

void CGameOutput::SetScreenW_H(int w, int h)
{
	char buf[64];
	w *= 2;
	sprintf(buf,"mode con cols=%d lines=%d",w,h);
	system(buf);
}

void CGameOutput::SetSecondaryBuffer(bool Cursor_sw)
{
	m_Second.Iint(m_W,m_H);
	m_Second.SetCursor(Cursor_sw);
}

void CGameOutput::LoadBmp(const char * id, CBmp bmp)
{
	//安全检测
	if (m_BmpList.find(id) != m_BmpList.end())
		return;
	//将所需要加载的图片存入链表
	m_BmpList.insert(std::pair<const char*, CBmp>(id, bmp));
}

void CGameOutput::SetPixel(const char * p)
{
	//设置图素
	m_Pixel = p;
	//初始化图素长度
	m_PixelLen = 0;
	//获取图素字符长度
	while (p[m_PixelLen++])			/*for (; p[m_PixelLen]; m_PixelLen++)*/
	{
	}
	//获取图素个数
	m_PixelLen /= 2;
}

void CGameOutput::Begin()
{
	//初始化绘图区域
	for (int i = 0; i < m_S; ++i)
	{
		m_Map[i] = 0;
	}
}

void CGameOutput::DrawBmp(const char * id, int x, int y)
{
	//越界检测
	if (y >= m_H || x >= m_W)
		return;
	//创建迭代器
	std::map<const char*, CBmp>::iterator it;
	//查找需要图片数据
	it = m_BmpList.find(id);
	//安全检测
	if (it == m_BmpList.end())
		return;
	//初始化图片数据
	int bmpW = it->second.GetW();
	int bmpH = it->second.GetH();
	int* index = it->second.GetIndex();

	int s = bmpW * bmpH;
	int pos = x + y * m_W;

	for (int i = 0; i < s; ++i)
	{
		//图片数据导入画布
		if (x >= 0 && x < m_W && y >= 0 && y < m_H)
			m_Map[pos] = index[i];
		pos += 1;
		x += 1;
		//图片数据换行
		if (i % bmpW == bmpW - 1)
		{
			y++;
			x -= bmpW;
			pos = x + y * m_W;
		}
	}
}

void CGameOutput::End()
{
	//清空
	//system("cls");

	//绘制画布
	for (int i = 0; i < m_S; ++i)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//std::cout << m_Pixel[m_Map[i] * 2] << m_Pixel[m_Map[i] * 2 + 1];
		////换行
		//if (i % m_W == m_W - 1)
		//	std::cout << "\n";
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		char cTmp[] = { m_Pixel[m_Map[i] * 2] , m_Pixel[m_Map[i] * 2 + 1] ,0};
		m_Second.SetPos(i % m_W, i / m_W);
		m_Second.Write(cTmp);
		//换行
		if (i % m_W == m_W - 1)
		{
			cTmp[0] = '\\';
			cTmp[1] = 'n';
		}
	}
	Sleep(32);
	m_Second.End();
}

int CGameOutput::GetPixelLen()
{
	return m_PixelLen;
}

int CGameOutput::GetClientW()
{
	return m_W;
}

int CGameOutput::GetClientH()
{
	return m_H;
}

void CSecondaryBuffer::Iint(int w, int h)
{
	m_W = w * 2;
	m_H = h;
	//GetStdHandle:获取标准控制台屏幕缓冲区句柄；
	m_houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	//创建新的缓冲区
	//CreateConsoleScreenBuffer:创建控制台屏幕缓冲区
	m_hback = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,//让新创建的缓冲区可以读可以写
		FILE_SHARE_READ | FILE_SHARE_WRITE,//共享权限是可读可写
		NULL,
		CONSOLE_TEXTMODE_BUFFER,//控制台文本模式缓冲区
		NULL);

	//屏幕信息的存储
	m_ScreenBuffer = new CHAR_INFO[2 * m_W * m_H];
}

void CSecondaryBuffer::SetCursor(bool sw)
{
	//m_cci.bVisible光标的显示和隐藏
	m_cci.bVisible = sw;
	m_cci.dwSize = 1;
	//设置控制台光标信息
	SetConsoleCursorInfo(m_hback, &m_cci);
	SetConsoleCursorInfo(m_houtput, &m_cci);
}

void CSecondaryBuffer::SetPos(int x, int y)
{
	m_pos.X = x * 2;
	m_pos.Y = y;
}

void CSecondaryBuffer::Write(char * str)
{
	DWORD wn;	//绘制的中间参数
	
	//往控制台hback中的具体位置绘制
	//BOOL WriteConsoleOutputCharacterA(
	//IN HANDLE hConsoleOutput, 具体的控制台
	//IN LPCSTR lpCharacter,    绘制的内容
	//IN DWORD nLength,         绘制字符串的长度
	//IN COORD dwWriteCoord,    绘制的位置
	//OUT LPDWORD lpNumberOfCharsWritten  绘制的传递参数
	//);
	WriteConsoleOutputCharacterA(m_hback, str, strlen(str), m_pos, &wn);
}

void CSecondaryBuffer::End()
{
	//将后台缓冲区内容一次性复制到前台缓冲区
	COORD ds;
	ds.X = m_W;
	ds.Y = m_H;
	COORD lt;
	lt.X = 0;
	lt.Y = 0;
	SMALL_RECT rc;
	rc.Left = 0;
	rc.Top = 0;
	rc.Right = m_W - 1;
	rc.Bottom = m_H - 1;
	//从后台缓冲区读出数据
	ReadConsoleOutputA(m_hback, m_ScreenBuffer, ds, lt, &rc);
	//将数据写入前台缓冲区
	WriteConsoleOutputA(m_houtput, m_ScreenBuffer, ds, lt, &rc);
}
