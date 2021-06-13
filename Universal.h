#pragma once
//����ͷ�ļ�
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
//��ͼͷ�ļ�
#include <conio.h>
#include <graphics.h>

#pragma comment (lib, "winmm.lib")

using namespace std;

#define MY_OK 0
#define MY_YESNO 1
#define INF 2147483647
enum BLOCK { WALL = 1, END, YOU, ROAD, HEART, MONSTER };

//��������
const int winWidth = 1080;
const int winHight = 720;

const int titSize = 64;
const int capSize = 48;
const int txtSize = 32;

const wchar_t titFont[] = L"����";
const wchar_t capFont[] = L"΢���ź�";
const wchar_t txtFont[] = L"����";

const COLORREF txtColorWarmDark = RGB(53, 48, 44);
const COLORREF txtColorWarmLight = RGB(248, 236, 216);
const COLORREF txtColorColdDark = RGB(40, 44, 48);
const COLORREF txtColorColdLight = RGB(174, 192, 211);

const COLORREF bkColorWarmNormal = RGB(248, 226, 206);
const COLORREF bkColorWarmHighlighted = RGB(248, 236, 216);
const COLORREF bkColorColdNormal = RGB(145, 160, 176);
const COLORREF bkColorColdHighlighted = RGB(174, 192, 211);