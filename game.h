#pragma once
#include "Universal.h"

//generalDraw.cpp
class generalDraw
{
public:
	generalDraw() {}
	~generalDraw() {}

	void bkDraw(bool plain = false);															// 背景绘制
	void txtDraw(int left, int top, int right, int bottom, 
				LPCTSTR str, bool isHighlighted,bool center = true, bool isTitle = false);		// 文本绘制
	bool buttonDraw(int centerX, int centerY, const wchar_t caption[]);							// 按钮绘制
	int putMessageBox(const wchar_t title[], wchar_t text[], int g_num, int var);				// 对话框绘制
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
	int pass = 1, all_pass = 1;												// 关卡和总关卡
	int to_x = 0, to_y = 0, x = 0, y = 0, room[150][150], n = 0, m = 0;						// 终点坐标、任务坐标、迷宫地图储存和迷宫大小
	int tot, reg[2600][2600], feg[2600][2600], flag[3][2600];	// 图节点总数、邻接矩阵、辅助邻接矩阵和迷宫中坐标与图节点编号关系记录
	bool tip[2600];								// 辅助迷宫地图、图节点是否走过记录

	maze() {}
	~maze() {}

	void guessNum(int chanceLeft);						// 陷阱（猜数）界面
	void game();										// 主菜单界面
	void drawRoom(generalDraw* draw);					// 绘制迷宫界面
	bool nextFloor();									// 通过一关界面
	void win(int difficulty);											// 游戏胜利界面
	bool lost(int difficulty);										// 游戏失败界面
	void addHeartMsgBox();								// 生命值奖励界面
	void manMove(int chances);							// 人物移动处理
	void cheatMove();									// 作弊移动处理
	void clear();										// 清空数据处理

	int my_line(int x_1, int y_1, int x_2, int y_2);	// 辅助函数
};

//mode.cpp
class mode : public maze
{
public:
	mode() {}
	~mode() {}

	void enterMode();	// 各模式（系列）核心

private:
	void prim();	// Prim 随机生成迷宫
	void rdmgenerate(int difficulty);	// 随机生成陷阱与奖励格点
};
