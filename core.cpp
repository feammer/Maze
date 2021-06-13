#include "game.h"	// ��Ϸͷ�ļ�

// ȫ�ֱ���
char ch;						// ������Ϣ
long long times, start_time;	// ��ʱ��
MOUSEMSG m_msg;					// �����Ϣ

//maze�ຯ��ʵ��

// ���壨����������
void maze::guessNum(int chanceLeft)
{
	generalDraw* draw = new generalDraw();
	long long t = times;
	srand((unsigned)time(NULL));
	wchar_t txt[] = L"���������ӳ�����";
	while (1)
		if (!(draw->putMessageBox(L"�������� ��", txt, 1, MY_OK)))
			break;
	int actualNum = rand() % 32 + 1;
	int num = 0;
	wchar_t guessNum[3] = { '\0' };
	while (chanceLeft)
	{
		draw->bkDraw();
		wchar_t title[50];
		swprintf_s(title, L"������ ʣ����� %d\0", chanceLeft);
		while (!InputBox(guessNum, 3, title, L"��ΧΪ[ 1 , 32 ]"));
		num = _wtoi(guessNum);
		if (chanceLeft == 1) life -= abs(num - actualNum);

		wchar_t result[3][10] = { L"̫����",L"̫С��",L"�¶���" };
		if (num > actualNum)
		{
			while (1) if (!(draw->putMessageBox(L"�²���", result[0], 1, MY_OK))) break;
		}
		else if (num < actualNum)
		{
			while (1) if (!(draw->putMessageBox(L"�²���", result[1], 1, MY_OK))) break;
		}
		else
		{
			while (1) if (!(draw->putMessageBox(L"�²���", result[2], 1, MY_OK))) break;
			break;
		}
		chanceLeft--;
	}
	swprintf_s(txt, L"����ֵ���� %-2d\0", abs(num - actualNum));
	while (1) if (!(draw->putMessageBox(L"�ӳ�����", txt, 1, MY_OK))) break;
	start_time = int(time(NULL)) - t;
	times = t;
	delete draw;
	draw = NULL;
}

// ���˵�����
void maze::game()
{
	Help* help = new Help();
	generalDraw* draw = new generalDraw();

	setbkmode(TRANSPARENT);

	BeginBatchDraw();

	while (1)
	{
		if (_kbhit())		// ������Ϣ��ȡ
		{
			ch = _getch();	// ������Ϣ��ȡ
			if (ch == 27)	// �� Esc �˳�
			{
				break;
			}
		}

		while (MouseHit())	// �����Ϣ��ȡ
			m_msg = GetMouseMsg();

		draw->bkDraw();		// ���Ʊ���

		draw->txtDraw(0, winHight / 9, winWidth, winHight / 3, L"�ԡ���", true, true, true);
		/*
		for (int i = 0; i <= 3; i++)
		{
			settextcolor(RGB(100, 100, 100));
			settextstyle(70, 0, L"΢���ź�");
			outtextxy(235 + i, 85 + i, L"�ԡ���");
		}
		settextcolor(WHITE);
		settextstyle(70, 0, L"΢���ź�");
		outtextxy(235 + 3, 85 + 3, L"�ԡ���");		// �������
		*/
		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 4, L"��"))
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

		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 5, L"��ͨ"))
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

		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 6, L"����"))
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

		if (draw->buttonDraw(winWidth / 2, winHight / 9 * 7, L"����"))
			help->put();

		draw->txtDraw(0, winHight * 8 / 9, winWidth, winHight * 8 / 9 + capSize, L"�� Esc �˳���Ϸ", true);

		FlushBatchDraw();
	}

	EndBatchDraw();

	delete draw;
	draw = NULL;
}

// �����Թ�����
void maze::drawRoom(generalDraw* draw)
{
	//��Ϸ�����������
	wchar_t tim[25], pas1[50], pas2[50], lifeValue[50];	// ��ʱ����ǰ�ؿ����ܹؿ�
	swprintf_s(pas1, L"�� %d ��\0", pass);
	swprintf_s(pas2, L"�� %d ��\0", all_pass);
	swprintf_s(tim, L"ʹ��ʱ�� %lld ��\0", times);
	swprintf_s(lifeValue, L"ʣ������ֵ %-2d\0", life);
	draw->txtDraw(winHight, capSize, winWidth, 2 * capSize, pas1, true);
	draw->txtDraw(winHight, 2 * capSize, winWidth, 3 * capSize, pas2, true);
	draw->txtDraw(winHight, 3 * capSize, winWidth, 4 * capSize, tim, true);
	draw->txtDraw(winHight, 4 * capSize, winWidth, 5 * capSize, lifeValue, true);
	//�Թ����⻭�����
	IMAGE roomFramework(winHight, winHight);
	SetWorkingImage(&roomFramework);
	int blockWidth = roomFramework.getwidth() / 9;
	IMAGE you, youleft, road, wall, heart;
	loadimage(&you, _T(".\\asset\\you.png"), blockWidth, blockWidth, true);
	loadimage(&youleft, _T(".\\asset\\youleft.png"), blockWidth, blockWidth, true);
	loadimage(&road, _T(".\\asset\\road.png"), blockWidth, blockWidth, true);
	loadimage(&wall, _T(".\\asset\\wall.png"), blockWidth, blockWidth, true);
	loadimage(&heart, _T(".\\asset\\heart.png"), blockWidth, blockWidth, true);
	for (int i = x - 4; i <= x + 4; i++)	// �����Թ�
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
				//�������λ�þ���
				x = i; y = j;
			}
		}
	}
	SetWorkingImage();
	putimage(0, 0, &roomFramework);
}

// ͨ��һ�ؽ���
bool maze::nextFloor()
{
	generalDraw* draw = new generalDraw();

	wchar_t title[50];

	swprintf_s(title, L"ʹ��ʱ�� %lld ��\0", times);
	mciSendString(L"close powerup", NULL, 0, NULL);
	mciSendString(L"open .\\asset\\powerup.mp3 alias powerup", NULL, 0, NULL);
	mciSendString(L"play powerup", NULL, 0, NULL);
	while (1)
	{
		if (_kbhit())				// ������Ϣ��ȡ
			ch = _getch();

		while (MouseHit())
			m_msg = GetMouseMsg();	// �����Ϣ��ȡ

		draw->bkDraw();				// ���Ʊ���

		draw->txtDraw(0, winHight / 3, winWidth, winHight / 3 + titSize, title, true);

		if (draw->buttonDraw(winWidth / 2, winHight * 5 / 9, L" ��һ�� "))	// ��һ�ذ�ť
		{
			mciSendString(L"close powerup", NULL, 0, NULL);
			break;
		}
		if (draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"�ص���ҳ"))	// �ص���ҳ��ť
		{
			wchar_t text[] = L"��ȷ����Ҫ�ص���ҳ��\n";
			draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"�ص���ҳ");
			if (draw->putMessageBox(L"�ص���ҳ", text, 1, MY_YESNO))		// �ص���ҳ�Ի���
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

// ��Ϸʤ������
void maze::win(int difficulty)
{
	generalDraw* draw = new generalDraw();
	mciSendString(L"close goal", NULL, 0, NULL);
	mciSendString(L"open .\\asset\\goal.mp3 alias goal", NULL, 0, NULL);
	mciSendString(L"play goal", NULL, 0, NULL);
	while (1)
	{
		if (_kbhit())				// ������Ϣ��ȡ
			ch = _getch();

		while (MouseHit())			// �����Ϣ��ȡ
			m_msg = GetMouseMsg();

		draw->bkDraw();				// ���Ʊ���

		draw->txtDraw(0, winHight / 3, winWidth, winHight / 3 + titSize, L"��Ϸʤ��", true);

		if (draw->buttonDraw(winWidth / 2, winHight * 5 / 9, L"�ص���ҳ"))		// �ص���ҳ��ť
		{
			if (difficulty == 1)
				mciSendString(L"close castle", NULL, 0, NULL);
			else if (difficulty == 2)
				mciSendString(L"close dungeon", NULL, 0, NULL);
			else if (difficulty == 3)
				mciSendString(L"close field", NULL, 0, NULL);
			break;
		}
		if (draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"�˳���Ϸ"))		// �˳���Ϸ��ť
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

// ��Ϸʧ�ܽ���
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
			if (_kbhit())				// ������Ϣ��ȡ
				ch = _getch();

			while (MouseHit())			// �����Ϣ��ȡ
				m_msg = GetMouseMsg();

			draw->bkDraw();				// ���Ʊ���

			draw->txtDraw(0, winHight / 3, winWidth, winHight / 3 + titSize, L"��Ϸʧ��", true);

			if (draw->buttonDraw(winWidth / 2, winHight * 5 / 9, L"�ص���ҳ"))		// �ص���ҳ��ť
			{
				if (difficulty == 1)
					mciSendString(L"close castle", NULL, 0, NULL);
				else if (difficulty == 2)
					mciSendString(L"close dungeon", NULL, 0, NULL);
				else if (difficulty == 3)
					mciSendString(L"close field", NULL, 0, NULL);
				return 1;
			}
			if (draw->buttonDraw(winWidth / 2, winHight * 7 / 9, L"�˳���Ϸ"))		// �˳���Ϸ��ť
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

// ����ֵ��������
void maze::addHeartMsgBox()
{
	generalDraw* draw = new generalDraw();
	long long t = times;
	mciSendString(L"close coin", NULL, 0, NULL);
	mciSendString(L"open .\\asset\\coin.mp3 alias coin", NULL, 0, NULL);
	mciSendString(L"play coin", NULL, 0, NULL);
	while (1)
		if (!(draw->putMessageBox(L"����", (wchar_t*)L"����ֵ���� 4", 1, MY_OK)))
			break;
	life += 4;
	start_time = int(time(NULL)) - t;
	times = t;
	delete draw;
	draw = NULL;
}

// �����ƶ�����
void maze::manMove(int chances)
{
	if (ch == 'a') towardsLeft = true;
	if (ch == 'd') towardsLeft = false;
	if (ch == 'w' && room[x - 1][y] != WALL)		// ���ƴ���
	{
		if (room[x - 1][y] == HEART) addHeartMsgBox();
		if (room[x - 1][y] == MONSTER) guessNum(chances);
		room[x - 1][y] = YOU;
		room[x][y] = ROAD;
		x--;
		ch = '#';									// ��Ϊ����ֵ����ֹ��������ƶ�
	}
	else if (ch == 's' && room[x + 1][y] != WALL)	// ���ƴ���
	{
		if (room[x + 1][y] == HEART) addHeartMsgBox();
		if (room[x + 1][y] == MONSTER) guessNum(chances);
		room[x + 1][y] = YOU;
		room[x][y] = ROAD;
		y++;
		ch = '#';
	}
	else if (ch == 'a' && room[x][y - 1] != WALL)	// ���ƴ���
	{
		if (room[x][y - 1] == HEART) addHeartMsgBox();
		if (room[x][y - 1] == MONSTER) guessNum(chances);
		room[x][y - 1] = YOU;
		room[x][y] = ROAD;
		y--;
		ch = '#';
	}
	else if (ch == 'd' && room[x][y + 1] != WALL)	// ���ƴ���
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
	if (ch == 'w')		// ���ƴ���
	{
		room[x - 1][y] = YOU;
		room[x][y] = ROAD;
		x--;
		ch = '#';									// ��Ϊ����ֵ����ֹ��������ƶ�
	}
	else if (ch == 's')	// ���ƴ���
	{
		room[x + 1][y] = YOU;
		room[x][y] = ROAD;
		y++;
		ch = '#';
	}
	else if (ch == 'a')	// ���ƴ���
	{
		room[x][y - 1] = YOU;
		room[x][y] = ROAD;
		y--;
		ch = '#';
	}
	else if (ch == 'd')	// ���ƴ���
	{
		room[x][y + 1] = YOU;
		room[x][y] = ROAD;
		y++;
		ch = '#';
	}
}

// ������ݴ���
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

// ��������
int maze::my_line(int x_1, int y_1, int x_2, int y_2)
{
	if (x_1 == x_2)		// ��ͨ����
		room[x_1][(y_1 + y_2) / 2] = ROAD;
	else if (y_1 == y_2)
		room[(x_1 + x_2) / 2][y_1] = ROAD;
	return 0;
}