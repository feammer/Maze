#pragma once
#include "Universal.h"

//generalDraw.cpp
class generalDraw
{
public:
	generalDraw() {}
	~generalDraw() {}

	void bkDraw(bool plain = false);															// ��������
	void txtDraw(int left, int top, int right, int bottom, 
				LPCTSTR str, bool isHighlighted,bool center = true, bool isTitle = false);		// �ı�����
	bool buttonDraw(int centerX, int centerY, const wchar_t caption[]);							// ��ť����
	int putMessageBox(const wchar_t title[], wchar_t text[], int g_num, int var);				// �Ի������
};

//help.cpp
class Help
{
public:
	Help() {}
	~Help() {}

	void put();
};

//core.cpp
class maze
{
public:
	int life = 8;
	int difficulty = 0;
	bool cheat = false;
	bool towardsLeft = false;
	int pass = 1, all_pass = 1;												// �ؿ����ܹؿ�
	int to_x = 0, to_y = 0, x = 0, y = 0, room[150][150], n = 0, m = 0;						// �յ����ꡢ�������ꡢ�Թ���ͼ������Թ���С
	int tot, reg[2600][2600], feg[2600][2600], flag[3][2600];	// ͼ�ڵ��������ڽӾ��󡢸����ڽӾ�����Թ���������ͼ�ڵ��Ź�ϵ��¼
	bool tip[2600];								// �����Թ���ͼ��ͼ�ڵ��Ƿ��߹���¼

	maze() {}
	~maze() {}

	void guessNum(int chanceLeft);						// ���壨����������
	void game();										// ���˵�����
	void drawRoom(generalDraw* draw);					// �����Թ�����
	bool nextFloor();									// ͨ��һ�ؽ���
	void win(int difficulty);											// ��Ϸʤ������
	bool lost(int difficulty);										// ��Ϸʧ�ܽ���
	void addHeartMsgBox();								// ����ֵ��������
	void manMove(int chances);							// �����ƶ�����
	void cheatMove();									// �����ƶ�����
	void clear();										// ������ݴ���

	int my_line(int x_1, int y_1, int x_2, int y_2);	// ��������
};

//mode.cpp
class mode : public maze
{
public:
	mode() {}
	~mode() {}

	void enterMode();	// ��ģʽ��ϵ�У�����

private:
	void prim();	// Prim ��������Թ�
	void rdmgenerate(int difficulty);	// ������������뽱�����
};
