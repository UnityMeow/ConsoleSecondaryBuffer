#include "GameOutput.h"
int main()
{
	/*======== Meow's GameOutput ����ʹ�ò���ʾ�� ========*/

	//��������(������ ������)
	CGameOutput go(20, 20);
	//���ÿ���̨���
	go.SetScreenW_H(20, 20);
	//����ͼ��
	//SetPixel(0��ͼ��Ĭ��Ϊ����ͼ�� ���ַ���)
	go.SetPixel("  �����񨋡�");
	//����ͼƬ
	CBmp bmp1;
	CBmp bmp2;
	//����ͼƬ����
	//SetBmpData(��, ��, ����ͼƬͼ��)
	bmp1.SetBmpData(2, 3,  3,3,2,2,1,1);
	bmp2.SetBmpData(5, 2,  1,2,3,4,5,5,4,3,2,1);
	//����ͼƬ
	//LoadBmp(ͼƬID, ���ú����ݵ�ͼƬ)
	go.LoadBmp("ͼ1", bmp1);
	go.LoadBmp("ͼ2", bmp2);
	//ͼƬλ��
	int bmp1_posX = 3; 
	int bmp1_posY = 2;
	int bmp2_posX = 15; 
	int bmp2_posY = 18;
	//���ö��λ���
	//SetSecondaryBuffer(falseΪ�رչ�� trueΪ��ʾ���)
	go.SetSecondaryBuffer(false);
	//��ѭ��
	while(1)
	{
		//��ʼ������
		go.Begin();
		//����ͼƬ
		//DrawBmp(ͼƬID,���Ƶ�λ��X,���Ƶ�λ��Y)
		go.DrawBmp("ͼ1",bmp1_posX,bmp1_posY);
		go.DrawBmp("ͼ2",bmp2_posX,bmp2_posY);
		//����
		go.End();
		//�߼�
		if (GetAsyncKeyState('A') & 0x8000)
		{
			bmp1_posX--;
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			bmp1_posX++;
		}
	}
	return 0;
}