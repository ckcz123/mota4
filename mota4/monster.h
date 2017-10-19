#pragma once
#include <hge.h>
#include <hgefont.h>
#include <hgesprite.h>
#include "gfxfont.h"
using namespace std;

class c_monster
{
public:
	void init(int);//��ʼ��
	void show(GfxFont*,int,int);//��Ⱦ
	void printInfo();//�����Ϣ
	void changeState();//���ﶯ��
	int getId() {return id;}
	int getHp() {return hp;}
	bool beAttacked(int hero_atk) {hp-=special==2?1:hero_atk-def;return hp<=0;}
	int getAtk();
	int getDef();
	int getMoney() {
		int m=money;
		for (int i=0;i<times;i++) m/=2;
		return m;
	}
	int getTimes() {return times;}
	void setTimes(int x) {times=x;}
	int getSpecial() {return special;}
private:
	wchar_t name[30];
	int id,hp,atk,def,money,special;
	int times;
	int state,position;
	hgeSprite *monster[4];
};