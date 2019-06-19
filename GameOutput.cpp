#include "GameOutput.h"

CBmp::CBmp()
{
	//��ʼ��ͼƬ����ָ��
	index = 0;
}

CBmp::~CBmp()
{
	//ͼƬ����ָ�벻Ϊ��
	if (index)
		//�ͷ�ͼƬ���ݶ�
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
	//��ȫ���
	if (w < 0 || h < 0)
		return;
	if (index)
		delete[] index;
	//����ͼƬ���ݳ���
	m_len = w * h;
	//����ͼƬ��
	m_w = w;
	//����ͼƬ��
	m_h = h;
	index = new int[m_len];
	int* p = &data;
	//����ͼƬ����
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
	//��ȫ���
	if (w < 1)
		w = 1;
	if (h < 1)
		h = 1;
	//��ʼ��������
	m_W = w;
	//��ʼ��������
	m_H = h;
	//��ʼ���������
	m_S = w * h;
	//��ʼ����ͼ����
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
	//��ȫ���
	if (m_BmpList.find(id) != m_BmpList.end())
		return;
	//������Ҫ���ص�ͼƬ��������
	m_BmpList.insert(std::pair<const char*, CBmp>(id, bmp));
}

void CGameOutput::SetPixel(const char * p)
{
	//����ͼ��
	m_Pixel = p;
	//��ʼ��ͼ�س���
	m_PixelLen = 0;
	//��ȡͼ���ַ�����
	while (p[m_PixelLen++])			/*for (; p[m_PixelLen]; m_PixelLen++)*/
	{
	}
	//��ȡͼ�ظ���
	m_PixelLen /= 2;
}

void CGameOutput::Begin()
{
	//��ʼ����ͼ����
	for (int i = 0; i < m_S; ++i)
	{
		m_Map[i] = 0;
	}
}

void CGameOutput::DrawBmp(const char * id, int x, int y)
{
	//Խ����
	if (y >= m_H || x >= m_W)
		return;
	//����������
	std::map<const char*, CBmp>::iterator it;
	//������ҪͼƬ����
	it = m_BmpList.find(id);
	//��ȫ���
	if (it == m_BmpList.end())
		return;
	//��ʼ��ͼƬ����
	int bmpW = it->second.GetW();
	int bmpH = it->second.GetH();
	int* index = it->second.GetIndex();

	int s = bmpW * bmpH;
	int pos = x + y * m_W;

	for (int i = 0; i < s; ++i)
	{
		//ͼƬ���ݵ��뻭��
		if (x >= 0 && x < m_W && y >= 0 && y < m_H)
			m_Map[pos] = index[i];
		pos += 1;
		x += 1;
		//ͼƬ���ݻ���
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
	//���
	//system("cls");

	//���ƻ���
	for (int i = 0; i < m_S; ++i)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//std::cout << m_Pixel[m_Map[i] * 2] << m_Pixel[m_Map[i] * 2 + 1];
		////����
		//if (i % m_W == m_W - 1)
		//	std::cout << "\n";
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		char cTmp[] = { m_Pixel[m_Map[i] * 2] , m_Pixel[m_Map[i] * 2 + 1] ,0};
		m_Second.SetPos(i % m_W, i / m_W);
		m_Second.Write(cTmp);
		//����
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
	//GetStdHandle:��ȡ��׼����̨��Ļ�����������
	m_houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	//�����µĻ�����
	//CreateConsoleScreenBuffer:��������̨��Ļ������
	m_hback = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,//���´����Ļ��������Զ�����д
		FILE_SHARE_READ | FILE_SHARE_WRITE,//����Ȩ���ǿɶ���д
		NULL,
		CONSOLE_TEXTMODE_BUFFER,//����̨�ı�ģʽ������
		NULL);

	//��Ļ��Ϣ�Ĵ洢
	m_ScreenBuffer = new CHAR_INFO[2 * m_W * m_H];
}

void CSecondaryBuffer::SetCursor(bool sw)
{
	//m_cci.bVisible������ʾ������
	m_cci.bVisible = sw;
	m_cci.dwSize = 1;
	//���ÿ���̨�����Ϣ
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
	DWORD wn;	//���Ƶ��м����
	
	//������̨hback�еľ���λ�û���
	//BOOL WriteConsoleOutputCharacterA(
	//IN HANDLE hConsoleOutput, ����Ŀ���̨
	//IN LPCSTR lpCharacter,    ���Ƶ�����
	//IN DWORD nLength,         �����ַ����ĳ���
	//IN COORD dwWriteCoord,    ���Ƶ�λ��
	//OUT LPDWORD lpNumberOfCharsWritten  ���ƵĴ��ݲ���
	//);
	WriteConsoleOutputCharacterA(m_hback, str, strlen(str), m_pos, &wn);
}

void CSecondaryBuffer::End()
{
	//����̨����������һ���Ը��Ƶ�ǰ̨������
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
	//�Ӻ�̨��������������
	ReadConsoleOutputA(m_hback, m_ScreenBuffer, ds, lt, &rc);
	//������д��ǰ̨������
	WriteConsoleOutputA(m_houtput, m_ScreenBuffer, ds, lt, &rc);
}
