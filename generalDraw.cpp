#include "game.h"

extern char ch;
extern MOUSEMSG m_msg;

void generalDraw::bkDraw(bool plain)
{
	if (plain)
	{
		setfillcolor(RGB(71, 66, 74));
		solidrectangle(0, 0, winWidth, winHight);
	}
	else
	{
		float R = 30.0f;
		float G = 31.0f;
		float B = 34.0f;
		setfillcolor(RGB(R, G, B));
		solidrectangle(0, 0, winWidth, winHight);
		for (int x = -100, y = -100; x < winWidth / 4; x++, y++)
		{
			R += (71 - 30) * 4.0f / winWidth;
			G += (66 - 31) * 4.0f / winWidth;
			B += (74 - 34) * 4.0f / winWidth;
			setfillcolor(RGB(R, G, B));
			solidellipse(x, y, winWidth - x, winHight - y);
		}
	}
}

void generalDraw::txtDraw(int left, int top, int right, int bottom, LPCTSTR str, bool isHighlighted, bool center, bool isTitle)
{
	RECT r = { left, top, right, bottom };
	setbkmode(TRANSPARENT);
	LOGFONT f;
	gettextstyle(&f);
	isTitle ? f.lfHeight = titSize : f.lfHeight = capSize;
	//isTitle ? f.lfWidth  = titSize : f.lfWidth  = capSize;
	wcscpy_s(f.lfFaceName, capFont);
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	isHighlighted ? settextcolor(txtColorWarmLight) : settextcolor(txtColorWarmDark);
	if (center) drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else		drawtext(str, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

// 放置按钮
bool generalDraw::buttonDraw(int centerX, int centerY, const wchar_t caption[])
{
	static int x, y;																// 鼠标坐标
	setfillcolor(bkColorWarmNormal);
	setlinecolor(txtColorWarmDark);
	fillroundrect(centerX - (int)(capSize * (wcslen(caption) / 2 + 0.5)), centerY - capSize / 2,
		centerX + (int)(capSize * (wcslen(caption) / 2 + 0.5)), centerY + capSize / 2, 12, 12);					// 绘制边框

	x = m_msg.x;
	y = m_msg.y;																	// 获取坐标

	if (x > centerX - (int)(capSize * (wcslen(caption) / 2 + 0.5))
		&& x < centerX + (int)(capSize * (wcslen(caption) / 2 + 0.5))
		&& y > centerY - capSize / 2 && y < centerY + capSize) // 获得焦点显示
	{
		setfillcolor(bkColorWarmHighlighted);
		fillroundrect(centerX - (int)(capSize * (wcslen(caption) / 2 + 0.5)), centerY - capSize / 2,
			centerX + (int)(capSize * (wcslen(caption) / 2 + 0.5)), centerY + capSize / 2, 12, 12);
		if (m_msg.uMsg == WM_LBUTTONUP)
		{
			m_msg.uMsg = WM_MOUSEMOVE;

			return 1;
		}
	}
	txtDraw(centerX - (int)(capSize * (wcslen(caption) / 2 + 0.5)), centerY - capSize / 2,
		centerX + (int)(capSize * (wcslen(caption) / 2 + 0.5)), centerY + capSize / 2, caption, false);
	return 0;
}

// 绘制对话框
int generalDraw::putMessageBox(const wchar_t title[], wchar_t text[], int g_num, int var)
{
	setbkmode(TRANSPARENT);

	while (1)
	{
		if (_kbhit())								// 键盘消息清空
			ch = _getch();

		while (MouseHit())
			m_msg = GetMouseMsg();					// 鼠标消息获取

		setfillcolor(txtColorWarmDark);
		setlinecolor(txtColorWarmLight);
		fillroundrect(winWidth / 4, winHight / 4, winWidth * 3 / 4, winHight * 3 / 4, 36, 36); // 绘制边框
		int borderSize = txtSize / 2;
		LOGFONT f;
		gettextstyle(&f);
		f.lfHeight = titSize;
		//f.lfWidth = titSize;
		wcscpy_s(f.lfFaceName, titFont);
		f.lfQuality = ANTIALIASED_QUALITY;
		settextstyle(&f);
		settextcolor(txtColorWarmLight);
		RECT r = { winWidth / 4 + borderSize, winHight / 4 + borderSize, winWidth * 3 / 4 - borderSize, winHight / 4 + borderSize + titSize };
		drawtext(title, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		f.lfHeight = txtSize;
		wcscpy_s(f.lfFaceName, txtFont);
		f.lfQuality = ANTIALIASED_QUALITY;
		settextstyle(&f);
		settextcolor(txtColorWarmLight);
		r = { winWidth / 4 + borderSize, winHight / 4 + 2 * borderSize + titSize, winWidth * 3 / 4 - borderSize, winHight / 4 + 2 * borderSize + titSize + txtSize };
		drawtext(text, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		if (var == MY_OK)							// 按钮放置
		{
			if (buttonDraw(winWidth * 3 / 4 - 2 * capSize - borderSize, winHight * 3 / 4 - capSize / 2 - borderSize, L"确定"))
				return 0;
		}
		else if (var == MY_YESNO)
		{
			if (buttonDraw(winWidth * 3 / 4 - 3 * capSize - borderSize, winHight * 3 / 4 - capSize / 2 - borderSize, L"是"))
				return 1;

			if (buttonDraw(winWidth * 3 / 4 - capSize - borderSize, winHight * 3 / 4 - capSize / 2 - borderSize, L"否"))
				return 0;
		}
		FlushBatchDraw();
	}
	return 0;
}