#include "game.h"		// 游戏头文件

// 全局变量

extern MOUSEMSG m_msg;	// 鼠标消息

// 绘制界面
void Help::put()
{
	generalDraw* draw = new generalDraw();

	while (1)
	{
		// 鼠标消息获取
		while (MouseHit()) m_msg = GetMouseMsg();
		// 绘制背景
		draw->bkDraw();

		draw->txtDraw(titSize, titSize, winWidth - titSize, 2 * titSize, L"帮 助", false);
		setbkmode(TRANSPARENT);
		setfillcolor(bkColorWarmNormal);
		setlinecolor(txtColorWarmDark);
		setlinestyle(PS_SOLID);
		fillroundrect(titSize, 2 * titSize, winWidth - titSize, winHight + titSize, 40, 40);


		//帮助信息显示框


		//返回按钮
		if (draw->buttonDraw(winWidth / 2, winHight - 2 * capSize, L"返回")) break;

		FlushBatchDraw();
	}

	delete draw;
	draw = NULL;
}