#include "game.h"		// ��Ϸͷ�ļ�

// ȫ�ֱ���

extern char ch;						// ������Ϣ
extern long long times, start_time;	// ��ʱ��
extern MOUSEMSG m_msg;				// �����Ϣ

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
		clear();									// �������
		prim();										// �����Թ�

		x = 2;
		y = 2;										// ��������
		cheat = false;
		start_time = int(time(NULL));				// ���ó�ʼʱ�� 0 
		srand((unsigned int)time(NULL));
		for (int i = 1; i <= difficulty; i++)
			room[2 + 2 * i + (int)(rand() % 2) * 2][2 + 2 * i + (int)(rand() % 2) * 2] = HEART;
		for (int i = 1; i <= difficulty; i++)
			room[n - 2 * i - (int)(rand() % 2) * 2][n - 2 * i - (int)(rand() % 2) * 2] = MONSTER;
		while (1)
		{
			times = int(time(NULL)) - start_time;	// �ۼ�ʱ��

			ch = '#';								// ������Ϣ���

			if (_kbhit())							// ������Ϣ��ȡ
				ch = _getch();

			while (MouseHit())						// �����Ϣ��ȡ
				m_msg = GetMouseMsg();

			if (m_msg.uMsg == WM_LBUTTONUP)
			{
				if (m_msg.x >= winHight && m_msg.x <= winWidth && m_msg.y >= 4 * capSize && m_msg.y <= 5 * capSize)
				{
					cheat = true;
					m_msg.uMsg = WM_MOUSEMOVE;
				}
			}

			cheat ? cheatMove() : manMove(4);								// �����ƶ�����

			if (lost(difficulty))
				break;

			if (x == to_x && y == to_y)				// �����յ��ж�
				break;

			draw->bkDraw(true);							// ���Ʊ���

			drawRoom(draw);								// ���ƽ���

			if (draw->buttonDraw((winWidth + winHight) / 2, winHight - 2 * capSize, L"��ҳ"))											// �ص���ҳ��ť
			{
				long long t = times;
				wchar_t text[] = L"���Ƿ�Ҫ�ص���ҳ��\n";

				//������ʾ�����µİ�ť
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 2 * capSize, L"��ҳ");
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 4 * capSize, L"��ͣ");

				if (draw->putMessageBox(L"��ҳ", text, 1, MY_YESNO))	// �ص���ҳ�Ի���
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

			if (draw->buttonDraw((winWidth + winHight) / 2, winHight - 4 * capSize, L"��ͣ"))							// ��ͣ��ť
			{
				long long t = times;
				wchar_t text[] = L"����ȷ���������ͣ\n";

				//������ʾ�����µİ�ť
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 2 * capSize, L"��ҳ");
				draw->buttonDraw((winWidth + winHight) / 2, winHight - 4 * capSize, L"��ͣ");

				draw->putMessageBox(L"��ͣ", text, 1, MY_OK);	// ��ͣ�Ի���
				start_time = int(time(NULL)) - t;
				times = t;
			}

			FlushBatchDraw();
		}

		if (life <= 0)
			break;

		if (nextFloor())			// ͨ��һ�ؿ�����
			break;

		if (pass == all_pass)	// ͨ��ȫ�ؿ�����
		{
			win(difficulty);
			break;
		}
		pass++;
		n += 2;
		m += 2;
		if (n > 99 || m > 99)	// ��һ��
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

// Prim ��������Թ�
void mode::prim()
{
	struct Arr						// �߽ṹ��
	{
		int head, tail;
	};

	tot = 0;						// �ڵ������
	std::vector <Arr> ar;			// ���

	for (int i = 1; i <= n; i++)	// ��ʼ��
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
	for (int i = 1; i <= tot - 1; i++)	// Prim �㷨����
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
		reg[ans.head][ans.tail] = 1;	// ���ѡ�߲����

		for (int j = 1; j <= tot; j++)	// �����ѡ��
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
	room[n - 1][m] = END;	// �� Prim �����ʾ���Թ���
}

// ������������뽱�����
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