#include "game.h"	// 游戏头文件

// 全局变量
char ch;						// 键盘消息
long long times, start_time;	// 计时器
MOUSEMSG m_msg;					// 鼠标消息

//maze类函数实现

// 陷阱（猜数）界面
void maze::guessNum(int chanceLeft)
{
	generalDraw* draw = new generalDraw();
	long long t = times;
	srand((unsigned)time(NULL));
	wchar_t txt[] = L"猜数字来逃出陷阱";
	while (1)
		if (!(draw->putMessageBox(L"遇到陷阱 ！", txt, 1, MY_OK)))
			break;
	int actualNum = rand() % 32 + 1;
	int num = 0;
	wchar_t guessNum[3] = { '\0' };
	while (chanceLeft)
	{
		draw->bkDraw();
		wchar_t title[50];
		swprintf_s(title, L"猜数字 剩余次数 %d\0", chanceLeft);
		while (!InputBox(guessNum, 3, title, L"范围为[ 1 , 32 ]"));
		num = _wtoi(guessNum);
		if (chanceLeft == 1) life -= abs(num - actualNum);

		wchar_t result[3][10] = { L"太大了",L"太小了",L"猜对了" };
		if (num > actualNum)
		{
			while (1) if (!(draw->putMessageBox(L"猜测结果", result[0], 1, MY_OK))) break;
		}
		else if (num < actualNum)
		{
			while (1) if (!(draw->putMessageBox(L"猜测结果", result[1], 1, MY_OK))) break;
		}
		else
		{
			while (1) if (!(draw->putMessageBox(L"猜测结果", result[2], 1, MY_OK))) break;
			break;
		}
		chanceLeft--;
	}
	swprintf_s(txt, L"生命值减少 %-2d\0", abs(num - actualNum));
	while (1) if (!(draw->putMessageBox(L"逃出陷阱", txt, 1, MY_OK))) break;
	start_time = int(time(NULL)) - t;
	times = t;
	delete draw;
	draw = NULL;
}

// 主菜单界面
void maze::game()
{
	Help* help = new Help();
	generalDraw* draw = new generalDraw();

	setbkmode(TRANSPARENT);

	BeginBatchDraw();

	while (1)
	{
		if (_kbhit())		// 键盘消息获取
		{
			ch = _getch();	// 键盘消息获取
			if (ch == 27)	// 按 Esc 退出
			{
				break;
			}
		}

		while (MouseHit())	// 鼠标消息获取
			m_msg = GetMouseMsg();

		draw->bkDraw();		// 绘制背景

		draw->txtDraw(0, winHight / 9, winWidth, winHight / 3, L"迷　宫", true, true, true);
		/*
		for (int i = 0; i <= 3; i++)
		{
			settextcolor(RGB(100, 100, 100));
			settextstyle(70, 0, L"微软雅黑");
			outtextxy(235 + i, 85 + i, L"迷　宫");
		}
		settextcolor(WHITE);
		settextstyle(70, 0, L"微软雅黑");
		outtextxy(235 + 3, 85 + 3, L"迷　宫");		// 标题输出
		*/
		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 4, L"简单"))
		{
			mode* Adv = new mode();
			Adv->difficulty = 1;
			Adv->pass = 1;
			Adv->all_pass = 3;
			Adv->n = 15;
			Adv->m = 15;
			times = 0;
			Adv->enterMode();
			delete Adv;
			Adv = NULL;
		}

		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 5, L"普通"))
		{
			mode* Adv = new mode();
			Adv->difficulty = 2;
			Adv->pass = 1;
			Adv->all_pass = 6;
			Adv->n = 25;
			Adv->m = 25;
			times = 0;
			Adv->enterMode();
			delete Adv;
			Adv = NULL;
		}

		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 6, L"困难"))
		{
			mode* Adv = new mode();
			Adv->difficulty = 3;
			Adv->pass = 1;
			Adv->all_pass = 9;
			Adv->n = 45;
			Adv->m = 45;
			times = 0;
			Adv->enterMode();
			delete Adv;
			Adv = NULL;
		}

		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 7, L"帮助"))
			help->put();

		draw->txtDraw(0, winHight * 8 / 9, winWidth, winHight * 8 / 9 + capSize, L"按 Esc 退出游戏", true);

		FlushBatchDraw();
	}

	EndBatchDraw();

	delete draw;
	draw = NULL;
}

// 绘制迷宫界面
void maze::drawRoom(generalDraw* draw)
{
	//游戏界面文字输出
	wchar_t tim[25], pas1[50], pas2[50], lifeValue[50];	// 计时、当前关卡和总关卡
	swprintf_s(pas1, L"第 %d 层\0", pass);
	swprintf_s(pas2, L"共 %d 层\0", all_pass);
	swprintf_s(tim, L"使用时间 %lld 秒\0", times);
	swprintf_s(lifeValue, L"剩余生命值 %-2d\0", life);
	draw->txtDraw(winHight, capSize, winWidth, 2 * capSize, pas1, true);
	draw->txtDraw(winHight, 2 * capSize, winWidth, 3 * capSize, pas2, true);
	draw->txtDraw(winHight, 3 * capSize, winWidth, 4 * capSize, tim, true);
	draw->txtDraw(winHight, 4 * capSize, winWidth, 5 * capSize, lifeValue, true);
	//迷宫主题画面绘制
	IMAGE roomFramework(winHight, winHight);
	SetWorkingImage(&roomFramework);
	int blockWidth = roomFramework.getwidth() / 9;
	IMAGE you, youleft, road, wall, heart;
	loadimage(&you, _T(".\\asset\\you.png"), blockWidth, blockWidth, true);
	loadimage(&youleft, _T(".\\asset\\youleft.png"), blockWidth, blockWidth, true);
	loadimage(&road, _T(".\\asset\\road.png"), blockWidth, blockWidth, true);
	loadimage(&wall, _T(".\\asset\\wall.png"), blockWidth, blockWidth, true);
	loadimage(&heart, _T(".\\asset\\heart.png"), blockWidth, blockWidth, true);
	for (int i = x - 4; i <= x + 4; i++)	// 绘制迷宫
	{
		for (int j = y - 4; j <= y + 4; j++)
		{
			if (i < 1 || j < 1) {
				continue;
			}
			if (room[i][j] == WALL) putimage((j - 1 - y + 5) * blockWidth, (i - 1 - x + 5) * blockWidth, &wall);
			else if (room[i][j] == ROAD || room[i][j] == MONSTER) putimage((j - 1 - y + 5) * blockWidth, (i - 1 - x + 5) * blockWidth, &road);
			else if (room[i][j] == HEART) putimage((j - 1 - y + 5) * blockWidth, (i - 1 - x + 5) * blockWidth, &heart);
			else if (room[i][j] == END)
			{
				setfillcolor(BLUE);
				solidrectangle((j - 1 - y + 5) * blockWidth, (i - 1 - x + 5) * blockWidth, (j - y + 5) * blockWidth, (i - x + 5) * blockWidth);
				to_x = i;
				to_y = j;
			}
			else if (room[i][j] == YOU)
			{
				if (!towardsLeft)
					putimage((j - 1 - y + 5) * blockWidth, (i - 1 - x + 5) * blockWidth, &you);
				else
					putimage((j - 1 - y + 5) * blockWidth, (i - 1 - x + 5) * blockWidth, &youleft);
				//保持玩家位置居中
				x = i; y = j;
			}
		}
	}
	SetWorkingImage();
	putimage(0, 0, &roomFramework);
}

// 通过一关界面
bool maze::nextFloor()
{
	generalDraw* draw = new generalDraw();

	wchar_t title[50];

	swprintf_s(title, L"使用时间 %lld 秒\0", times);
	mciSendString(L"close powerup", NULL, 0, NULL);
	mciSendString(L"open .\\asset\\powerup.mp3 alias powerup", NULL, 0, NULL);
	mciSendString(L"play powerup", NULL, 0, NULL);
	while (1)
	{
		if (_kbhit())				// 键盘消息获取
			ch = _getch();

		while (MouseHit())
			m_msg = GetMouseMsg();	// 鼠标消息获取

		draw->bkDraw();				// 绘制背景

		draw->txtDraw(0, winHight / 3, winWidth, winHight / 3 + titSize, title, true);

		if (draw->buttonDraw(winWidth / 2, winHight * 5 / 9, L" 下一层 "))	// 下一关按钮
		{
			mciSendString(L"close powerup", NULL, 0, NULL);
			break;
		}
		if (draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"回到主页"))	// 回到主页按钮
		{
			wchar_t text[] = L"你确定你要回到主页吗？\n";
			draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"回到主页");
			if (draw->putMessageBox(L"回到主页", text, 1, MY_YESNO))		// 回到主页对话框
			{
				mciSendString(L"close powerup", NULL, 0, NULL);
				delete draw;
				draw = NULL;
				return 1;
			}
		}

		FlushBatchDraw();
		Sleep(5);
	}

	times = 0;
	delete draw;
	draw = NULL;
	return 0;
}

// 游戏胜利界面
void maze::win(int difficulty)
{
	generalDraw* draw = new generalDraw();
	mciSendString(L"close goal", NULL, 0, NULL);
	mciSendString(L"open .\\asset\\goal.mp3 alias goal", NULL, 0, NULL);
	mciSendString(L"play goal", NULL, 0, NULL);
	while (1)
	{
		if (_kbhit())				// 键盘消息获取
			ch = _getch();

		while (MouseHit())			// 鼠标消息获取
			m_msg = GetMouseMsg();

		draw->bkDraw();				// 绘制背景

		draw->txtDraw(0, winHight / 3, winWidth, winHight / 3 + titSize, L"游戏胜利", true);

		if (draw->buttonDraw(winWidth / 2, winHight * 5 / 9, L"回到主页"))		// 回到主页按钮
		{
			if (difficulty == 1)
				mciSendString(L"close castle", NULL, 0, NULL);
			else if (difficulty == 2)
				mciSendString(L"close dungeon", NULL, 0, NULL);
			else if (difficulty == 3)
				mciSendString(L"close field", NULL, 0, NULL);
			break;
		}
		if (draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"退出游戏"))		// 退出游戏按钮
		{
			if (difficulty == 1)
				mciSendString(L"close castle", NULL, 0, NULL);
			else if (difficulty == 2)
				mciSendString(L"close dungeon", NULL, 0, NULL);
			else if (difficulty == 3)
				mciSendString(L"close field", NULL, 0, NULL);
			delete draw;
			draw = NULL;
			EndBatchDraw();
			closegraph();
			std::exit(0);
		}

		FlushBatchDraw();
		Sleep(5);
	}

	delete draw;
	draw = NULL;
}

// 游戏失败界面
bool maze::lost(int difficulty)
{
	if (life <= 0)
	{
		mciSendString(L"close death", NULL, 0, NULL);
		mciSendString(L"open .\\asset\\death.mp3 alias death", NULL, 0, NULL);
		mciSendString(L"play death", NULL, 0, NULL);
		generalDraw* draw = new generalDraw();

		while (1)
		{
			if (_kbhit())				// 键盘消息获取
				ch = _getch();

			while (MouseHit())			// 鼠标消息获取
				m_msg = GetMouseMsg();

			draw->bkDraw();				// 绘制背景

			draw->txtDraw(0, winHight / 3, winWidth, winHight / 3 + titSize, L"游戏失败", true);

			if (draw->buttonDraw(winWidth / 2, winHight * 5 / 9, L"回到主页"))		// 回到主页按钮
			{
				if (difficulty == 1)
					mciSendString(L"close castle", NULL, 0, NULL);
				else if (difficulty == 2)
					mciSendString(L"close dungeon", NULL, 0, NULL);
				else if (difficulty == 3)
					mciSendString(L"close field", NULL, 0, NULL);
				return 1;
			}
			if (draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"退出游戏"))		// 退出游戏按钮
			{
				if (difficulty == 1)
					mciSendString(L"close castle", NULL, 0, NULL);
				else if (difficulty == 2)
					mciSendString(L"close dungeon", NULL, 0, NULL);
				else if (difficulty == 3)
					mciSendString(L"close field", NULL, 0, NULL);
				delete draw;
				draw = NULL;
				EndBatchDraw();
				closegraph();
				std::exit(0);
			}

			FlushBatchDraw();
			Sleep(5);
		}

		delete draw;
		draw = NULL;
	}
	else
		return 0;
}

// 生命值奖励界面
void maze::addHeartMsgBox()
{
	generalDraw* draw = new generalDraw();
	long long t = times;
	mciSendString(L"close coin", NULL, 0, NULL);
	mciSendString(L"open .\\asset\\coin.mp3 alias coin", NULL, 0, NULL);
	mciSendString(L"play coin", NULL, 0, NULL);
	while (1)
		if (!(draw->putMessageBox(L"奖励", (wchar_t*)L"生命值增加 4", 1, MY_OK)))
			break;
	life += 4;
	start_time = int(time(NULL)) - t;
	times = t;
	delete draw;
	draw = NULL;
}

// 人物移动处理
void maze::manMove(int chances)
{
	if (ch == 'a') towardsLeft = true;
	if (ch == 'd') towardsLeft = false;
	if (ch == 'w' && room[x - 1][y] != WALL)		// 上移处理
	{
		if (room[x - 1][y] == HEART) addHeartMsgBox();
		if (room[x - 1][y] == MONSTER) guessNum(chances);
		room[x - 1][y] = YOU;
		room[x][y] = ROAD;
		x--;
		ch = '#';									// 赋为其他值，防止人物持续移动
	}
	else if (ch == 's' && room[x + 1][y] != WALL)	// 下移处理
	{
		if (room[x + 1][y] == HEART) addHeartMsgBox();
		if (room[x + 1][y] == MONSTER) guessNum(chances);
		room[x + 1][y] = YOU;
		room[x][y] = ROAD;
		y++;
		ch = '#';
	}
	else if (ch == 'a' && room[x][y - 1] != WALL)	// 左移处理
	{
		if (room[x][y - 1] == HEART) addHeartMsgBox();
		if (room[x][y - 1] == MONSTER) guessNum(chances);
		room[x][y - 1] = YOU;
		room[x][y] = ROAD;
		y--;
		ch = '#';
	}
	else if (ch == 'd' && room[x][y + 1] != WALL)	// 右移处理
	{
		if (room[x][y + 1] == HEART) addHeartMsgBox();
		if (room[x][y + 1] == MONSTER) guessNum(chances);
		room[x][y + 1] = YOU;
		room[x][y] = ROAD;
		y++;
		ch = '#';
	}
}

void maze::cheatMove()
{
	life = 999;
	if (ch == 'a') towardsLeft = true;
	if (ch == 'd') towardsLeft = false;
	if (ch == 'w')		// 上移处理
	{
		room[x - 1][y] = YOU;
		room[x][y] = ROAD;
		x--;
		ch = '#';									// 赋为其他值，防止人物持续移动
	}
	else if (ch == 's')	// 下移处理
	{
		room[x + 1][y] = YOU;
		room[x][y] = ROAD;
		y++;
		ch = '#';
	}
	else if (ch == 'a')	// 左移处理
	{
		room[x][y - 1] = YOU;
		room[x][y] = ROAD;
		y--;
		ch = '#';
	}
	else if (ch == 'd')	// 右移处理
	{
		room[x][y + 1] = YOU;
		room[x][y] = ROAD;
		y++;
		ch = '#';
	}
}

// 清空数据处理
void maze::clear()
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			room[i][j] = 0;
		}
	}
	for (int i = 1; i <= (n + 1) / 2 * (m + 1) / 2; i++)
	{
		for (int j = 1; j <= (n + 1) / 2 * (m + 1) / 2; j++)
		{
			reg[i][j] = 0;
			feg[i][j] = 0;
		}
	}
	for (int i = 1; i <= (n + 1) / 2 * (m + 1) / 2; i++)
	{
		for (int j = 0; j < 3; j++)
			flag[j][i] = 0;
		tip[i] = 0;
	}
}

// 辅助函数
int maze::my_line(int x_1, int y_1, int x_2, int y_2)
{
	if (x_1 == x_2)		// 连通处理
		room[x_1][(y_1 + y_2) / 2] = ROAD;
	else if (y_1 == y_2)
		room[(x_1 + x_2) / 2][y_1] = ROAD;
	return 0;
}