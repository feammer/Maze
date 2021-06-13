#include "game.h"		// 游戏头文件

// 全局变量

extern char ch;						// 键盘消息
extern long long times, start_time;	// 计时器
extern MOUSEMSG m_msg;				// 鼠标消息

void mode::enterMode()
{
	generalDraw* draw = new generalDraw();
	life = 8;
	if (difficulty == 1)
	{
		mciSendString(L"open .\\asset\\castle.mp3 alias castle", NULL, 0, NULL);
		mciSendString(L"play castle repeat", NULL, 0, NULL);
	}
	else if (difficulty == 2)
	{
		mciSendString(L"open .\\asset\\dungeon.mp3 alias dungeon", NULL, 0, NULL);
		mciSendString(L"play dungeon repeat", NULL, 0, NULL);
	}
	else if (difficulty == 3)
	{
		mciSendString(L"open .\\asset\\field.mp3 alias field", NULL, 0, NULL);
		mciSendString(L"play field repeat", NULL, 0, NULL);
	}
	while (1)
	{
		clear();									// 清空数据
		prim();										// 生成迷宫

		x = 2;
		y = 2;										// 重置坐标
		cheat = false;
		start_time = int(time(NULL));				// 设置初始时间 0 
		srand((unsigned int)time(NULL));
		for (int i = 1; i <= difficulty; i++)
			room[2 + 2 * i + (int)(rand() % 2) * 2][2 + 2 * i + (int)(rand() % 2) * 2] = HEART;
		for (int i = 1; i <= difficulty; i++)
			room[n - 2 * i - (int)(rand() % 2) * 2][n - 2 * i - (int)(rand() % 2) * 2] = MONSTER;
		while (1)
		{
			times = int(time(NULL)) - start_time;	// 累加时间

			ch = '#';								// 键盘消息清空

			if (_kbhit())							// 键盘消息获取
				ch = _getch();

			while (MouseHit())						// 鼠标消息获取
				m_msg = GetMouseMsg();

			if (m_msg.uMsg == WM_LBUTTONUP)
			{
				if (m_msg.x >= winHight && m_msg.x <= winWidth && m_msg.y >= 4 * capSize && m_msg.y <= 5 * capSize)
				{
					cheat = true;
					m_msg.uMsg = WM_MOUSEMOVE;
				}
			}

			cheat ? cheatMove() : manMove(4);								// 人物移动处理

			if (lost(difficulty))
				break;

			if (x == to_x && y == to_y)				// 到达终点判断
				break;

			draw->bkDraw(true);							// 绘制背景

			drawRoom(draw);								// 绘制界面

			if (draw->buttonDraw((winWidth + winHight) / 2, winHight - 2 * capSize, L"主页"))											// 回到主页按钮
			{
				long long t = times;
				wchar_t text[] = L"你是否要回到主页？\n";

				//高亮显示被按下的按钮
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 2 * capSize, L"主页");
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 4 * capSize, L"暂停");

				if (draw->putMessageBox(L"主页", text, 1, MY_YESNO))	// 回到主页对话框
				{
					if (difficulty == 1)
						mciSendString(L"close castle", NULL, 0, NULL);
					else if (difficulty == 2)
						mciSendString(L"close dungeon", NULL, 0, NULL);
					else if (difficulty == 3)
						mciSendString(L"close field", NULL, 0, NULL);
					delete draw;
					draw = NULL;
					return;
				}
				start_time = int(time(NULL)) - t;
				times = t;
			}

			if (draw->buttonDraw((winWidth + winHight) / 2, winHight - 4 * capSize, L"暂停"))							// 暂停按钮
			{
				long long t = times;
				wchar_t text[] = L"按“确定”解除暂停\n";

				//高亮显示被按下的按钮
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 2 * capSize, L"主页");
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 4 * capSize, L"暂停");

				draw->putMessageBox(L"暂停", text, 1, MY_OK);	// 暂停对话框
				start_time = int(time(NULL)) - t;
				times = t;
			}

			FlushBatchDraw();
		}

		if (life <= 0)
			break;

		if (nextFloor())			// 通过一关卡界面
			break;

		if (pass == all_pass)	// 通过全关卡界面
		{
			win(difficulty);
			break;
		}
		pass++;
		n += 2;
		m += 2;
		if (n > 99 || m > 99)	// 下一关
		{
			n = 99;
			m = 99;
		}
	}
	if (difficulty == 1)
		mciSendString(L"close castle", NULL, 0, NULL);
	else if (difficulty == 2)
		mciSendString(L"close dungeon", NULL, 0, NULL);
	else if (difficulty == 3)
		mciSendString(L"close field", NULL, 0, NULL);

	delete draw;
	draw = NULL;
}

// Prim 随机生成迷宫
void mode::prim()
{
	struct Arr						// 边结构体
	{
		int head, tail;
	};

	tot = 0;						// 节点数清空
	std::vector <Arr> ar;			// 存边

	for (int i = 1; i <= n; i++)	// 初始化
	{
		for (int j = 1; j <= m; j++)
		{
			if (i % 2 == 0 && j % 2 == 0 && i != n && j != m)
			{
				room[i][j] = ROAD;
				flag[0][++tot] = tot;
				flag[1][tot] = i;
				flag[2][tot] = j;
			}
			else
			{
				room[i][j] = WALL;
			}
		}
	}
	for (int i = 1; i <= tot; i++)
	{
		for (int j = 1; j <= tot; j++)
		{
			reg[i][j] = INF;
			feg[i][j] = INF;
		}
	}
	for (int i = 1; i <= tot; i++)
	{
		tip[i] = 0;
		if (i % ((n - 1) / 2) != 0)
		{
			feg[i][i + 1] = 1;
			feg[i + 1][i] = 1;
		}
		if (i <= tot - (n - 1) / 2)
		{
			feg[i][i + (n - 1) / 2] = 1;
			feg[i + (n - 1) / 2][i] = 1;
		}
	}
	for (int i = 1; i <= tot; i++)
	{
		if (feg[1][i] == 1)
		{
			Arr afo;
			afo.head = i;
			afo.tail = 1;
			ar.push_back(afo);
		}
	}

	tip[1] = 1;
	for (int i = 1; i <= tot - 1; i++)	// Prim 算法核心
	{
		std::random_shuffle(ar.begin(), ar.end());
		Arr ans;
		while (1) {
			ans = ar.back();
			if (tip[ans.head] && tip[ans.tail])
			{
				ar.pop_back();
			}
			else {
				break;
			}
		}
		ar.pop_back();
		tip[ans.head] = 1;
		tip[ans.tail] = 1;
		reg[ans.tail][ans.head] = 1;
		reg[ans.head][ans.tail] = 1;	// 随机选边并标记

		for (int j = 1; j <= tot; j++)	// 加入侯选边
		{
			if (feg[ans.head][j] == 1 && !tip[j])
			{
				Arr afo;
				afo.head = j;
				afo.tail = ans.head;
				ar.push_back(afo);
			}
		}
	}

	for (int i = 1; i <= tot; i++)
	{
		for (int j = 1; j <= tot; j++)
		{
			if (reg[i][j] == 1)
			{
				my_line(flag[1][i], flag[2][i], flag[1][j], flag[2][j]);
			}
		}
	}
	room[2][2] = YOU;
	room[n - 1][m] = END;	// 将 Prim 结果显示到迷宫中
}

// 随机生成陷阱与奖励格点
void mode::rdmgenerate(int difficulty)
{
	srand((unsigned int)time(NULL));
	static int heart = 4;
	static int monster = to_x - 2;
	room[monster - rand() % 2 * 2][monster - rand() % 2] = MONSTER;
	room[monster - 4 - rand() % 2 * 2][monster - 4 - rand() % 2] = MONSTER;
	room[heart + rand() % 2 * 2][heart + rand() % 2] = HEART;
	room[heart + 4 + rand() % 2 * 2][heart + 4 + rand() % 2 * 2] = HEART;
}