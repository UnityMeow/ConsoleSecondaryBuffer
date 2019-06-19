#include "GameOutput.h"
int main()
{
	/*======== Meow's GameOutput 基本使用操作示例 ========*/

	//创建画布(画布宽 画布高)
	CGameOutput go(20, 20);
	//设置控制台宽高
	go.SetScreenW_H(20, 20);
	//设置图素
	//SetPixel(0号图素默认为背景图素 填字符串)
	go.SetPixel("  ■◆●▼▲");
	//创建图片
	CBmp bmp1;
	CBmp bmp2;
	//设置图片数据
	//SetBmpData(宽, 高, 设置图片图素)
	bmp1.SetBmpData(2, 3,  3,3,2,2,1,1);
	bmp2.SetBmpData(5, 2,  1,2,3,4,5,5,4,3,2,1);
	//加载图片
	//LoadBmp(图片ID, 设置好数据的图片)
	go.LoadBmp("图1", bmp1);
	go.LoadBmp("图2", bmp2);
	//图片位置
	int bmp1_posX = 3; 
	int bmp1_posY = 2;
	int bmp2_posX = 15; 
	int bmp2_posY = 18;
	//设置二次缓冲
	//SetSecondaryBuffer(false为关闭光标 true为显示光标)
	go.SetSecondaryBuffer(false);
	//主循环
	while(1)
	{
		//初始化画布
		go.Begin();
		//导入图片
		//DrawBmp(图片ID,绘制的位置X,绘制的位置Y)
		go.DrawBmp("图1",bmp1_posX,bmp1_posY);
		go.DrawBmp("图2",bmp2_posX,bmp2_posY);
		//绘制
		go.End();
		//逻辑
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