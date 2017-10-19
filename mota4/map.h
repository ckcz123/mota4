#pragma once
#include "hero.h"

class c_map_door
{
public:
	void init(int);//初始化
	void show(int,int);//渲染
	bool open();//开某个普通门
	int getType() {return type;} //门的类型
private:
	int type;
	bool opened;
	int state;
	hgeSprite *door[4];
};

class c_map_point
{
public:
	void init(int,int times=0);
	int getPoint();
	void show(GfxFont*,int,int);//渲染
	void animation() {monster.changeState();npc.changeState();}//npc状态改变
	void printMonsterInfo() {monster.printInfo();} //输出怪物信息
	bool canMove(int);//判断该点是否可移动
	bool openSpecial();
	int getItemID();//获取该点的物品编号
	int getSpecial() {return special;}
	void setSpecial(int _spe) {special=_spe;}
	void distroyItem() {item=NULL;} //获取该点物品
	c_map_door* getDoor() {return &door;}
	c_map_npc* getNpc() {return &npc;}
	c_monster* getMonster() {return &monster;}
	bool nearStair(int t) {return door.getType()==t;}
	bool hasMonster(int id=0) {return id==0?monster.getId()!=0:monster.getId()==id;}
	bool isGround();
	hgeSprite* getTypeSprite();
	hgeSprite* getItemSprite();
	void toString(char*s);
	void load(FILE*);//读档
private:
	int type;//地面类型（地 OR 墙）
	int item;//道具类型
	c_monster monster;//敌人类型
	c_map_door door;//门类型
	c_map_npc npc;// npc类型
	int special; // 特殊层特殊按钮
};

class c_map_floor
{
public:
	void init(int,int[30][30]);//初始化
	void show();//渲染
	void animation();//状态改变
	void printMonsterInfo(int,int);//输出怪物信息
	bool canMove(int,int,int);//判断该地图的某点是否可移动
	int getItem(int,int);//获取地图某点的物品
	int getSpecial(int,int);
	void setSpecial(int,int,int);
	void getDownPosition(int&,int&);//找到地图的下楼楼梯
	void getUpPosition(int&,int&);//找到地图的上楼楼梯
	void save(FILE*);//存档
	char* toString();
	void load(FILE*);//读档
	bool hasMonster(int id=0);
	bool isLinked(); //该楼从下楼楼梯到上楼楼梯是否可达
	c_map_point* getinfo(int x, int y) {return &info[x][y];}
private:
	int id;//楼层编号
	int dx,dy,ux,uy;//楼梯坐标
	c_map_point info[30][30];//每点信息
};