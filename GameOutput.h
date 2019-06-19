#pragma once
#include <iostream>
#include <map>
#include <windows.h>

//ͼƬ��
class CBmp
{
	int m_w;	//ͼƬ��
	int m_h;	//ͼƬ��
	int* index;	//ͼƬ����
	int m_len;	//���ݳ���
public:
	CBmp();
	~CBmp();
	CBmp(const CBmp& that);
	CBmp& operator =(const CBmp& that);

	void SetBmpData(int w, int h, int data, ...);	//����ͼƬ���ͼƬ����
	int GetW();			//��ȡͼƬ��
	int GetH();			//��ȡͼƬ��
	int* GetIndex();	//��ȡͼƬ����
};

//���λ�����
class CSecondaryBuffer
{
	int m_W;
	int m_H;
	HANDLE m_houtput;	//��ȡ����̨��Ļ������
	HANDLE m_hback;		//�����¿���̨��Ļ������
	CONSOLE_CURSOR_INFO m_cci;	//�����Ϣ
	PCHAR_INFO m_ScreenBuffer;	//��Ļ��Ϣ����
	COORD m_pos;	//����λ��
public:
	//�������λ���
	void Iint(int w, int h);
	//���ù����Ϣ
	void SetCursor(bool sw);
	//���û���λ��
	void SetPos(int x, int y);
	//���ƺ���
	void Write(char* str);
	//��������
	void End();
};

//������
class CGameOutput
{
	const char* m_Pixel;	//ͼ��
	int m_PixelLen;			//ͼ�س���
	//ͼƬ����
	std::map<const char*, CBmp> m_BmpList;
	CSecondaryBuffer m_Second;
	//��ͼ����
	char* m_Map;
	int m_W;
	int m_H;
	int m_S;
public:
	CGameOutput(int w, int h);	//��ʼ����ͼ������
	~CGameOutput();				//�ͷ�

	void SetSecondaryBuffer(bool Cursor_sw);//���ö��λ���
	void SetScreenW_H(int w, int h);		//���ÿ���̨���
	void LoadBmp(const char* id, CBmp bmp);	//����ͼƬ
	void SetPixel(const char* p);			//����ͼ��
	

	void Begin();	//��ʼ������
	void DrawBmp(const char* id, int x, int y);	//ͼƬ���ݵ��뻭��
	void End();		//���ƻ���

	int GetPixelLen();	//��ȡͼ�س���
	int GetClientW();	//��ȡ������
	int GetClientH();	//��ȡ������
};