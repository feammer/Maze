#include "game.h"		// ��Ϸͷ�ļ�

// ȫ�ֱ���

extern MOUSEMSG m_msg;	// �����Ϣ

// ���ƽ���
void Help::put()
{
	generalDraw* draw = new generalDraw();

	while (1)
	{
		// �����Ϣ��ȡ
		while (MouseHit()) m_msg = GetMouseMsg();
		// ���Ʊ���
		draw->bkDraw();

		draw->txtDraw(titSize, titSize, winWidth - titSize, 2 * titSize, L"�� ��", false);
		setbkmode(TRANSPARENT);
		setfillcolor(bkColorWarmNormal);
		setlinecolor(txtColorWarmDark);
		setlinestyle(PS_SOLID);
		fillroundrect(titSize, 2 * titSize, winWidth - titSize, winHight + titSize, 40, 40);


		//������Ϣ��ʾ��


		//���ذ�ť
		if (draw->buttonDraw(winWidth / 2, winHight - 2 * capSize, L"����")) break;

		FlushBatchDraw();
	}

	delete draw;
	draw = NULL;
}