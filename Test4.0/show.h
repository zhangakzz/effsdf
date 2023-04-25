#include <iostream>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib>
using namespace std;

//窗口大小
const int HEIGHT = 750;
const int WIDTH = 1200;

//摄像机坐标
POINT CameraPos;

//游戏数据
const int Player_speed = 10;//人物速度
const int Bullet_speed = 30;//子弹速度
const int Bullet_number = 1;//拥有子弹数量，目前不可变
const int Bullet_range = 400;//子弹攻击范围
const int xhz_number = 10;//敌人ikun数量
const int Xhz_speed = 10;//小黑子速度
const int constHP = 3;//初始血量
bool hit = false;//判断是否命中
int mark = 0;//得分
TCHAR MARK[20];
TCHAR hp[10];
struct ikun//小黑子结构体
{
	//位置
	int x;
	int y;
	//方向
	float dir_x;
	float dir_y;
};

// 载入PNG图并去透明部分
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{

	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度
	int picture_height = picture->getheight(); //获取picture的高度
	int graphWidth = getwidth();       //获取绘图区的宽度
	int graphHeight = getheight();     //获取绘图区的高度
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

//绘制子弹
void drawbullet(int real_bullet_x, int real_bullet_y, int camera_x, int camera_y, IMAGE* bullet)
{
	int bullet_x = real_bullet_x - camera_x - 15;
	int bullet_y = real_bullet_y - camera_y - 15;
	if (bullet_x >= 0 && bullet_x <= WIDTH - 30 && bullet_y >= 0 && bullet_y <= HEIGHT - 30)//这里30是直接带入子弹的大小了
	{
		drawAlpha(bullet, bullet_x, bullet_y);
	}
}
//绘制小黑子
void drawxhz(int real_xhz_x, int real_xhz_y, int camera_x, int camera_y, IMAGE* xhz)
{
	int xhz_x = real_xhz_x - camera_x - 15;
	int xhz_y = real_xhz_y - camera_y - 15;
	if (xhz_x >= 0 && xhz_x <= WIDTH - 60 && xhz_y >= 0 && xhz_y <= HEIGHT - 60)//这里30是直接带入子弹的大小了
	{
		drawAlpha(xhz, xhz_x, xhz_y);
	}
}