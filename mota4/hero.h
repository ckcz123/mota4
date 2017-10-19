#pragma once

#include "npc.h"
class c_hero
{
public:
	const static int MAX_DAMAGE = 99999999;

	c_hero();//构造函数
	void init();
	int nextX();//下一步的横坐标
	int nextY();//下一步的纵坐标
	bool canMove(int);//是否可以朝某个方向移动
	bool moveComplete();//移动是否完成
	bool openRedDoor();//打开红色的门
	bool openBlueDoor();//打开蓝色的门
	bool openYellowDoor();//打开黄色的门
	void upstair();//上楼
	void downstair();//下楼
	void turn(); // 转向
	void fly(); //楼层飞跃
	void specialMove() {}
	void show();
	void printInfo();//输出勇士信息
	bool canBeat(c_monster*);//能否打败怪物
	int getDamage(c_monster*);//打败怪物普通伤害
	int getDamage(int h_atk, int h_def, int h_mdef, int m_hp, int m_atk, int m_def, int m_spe);
	int getCritical(int m_hp, int m_atk, int m_def, int m_spe);
	int getCriticalDamage(int m_hp, int m_atk, int m_def, int m_spe);
	int getDefDamage(int m_hp, int m_atk, int m_def, int m_spe);
	void beat(c_monster*);
	void npc(int select=0);//选择select
	void save(FILE*);//存档
	char* toString();
	void load(FILE*);//读档
	bool nearStair(int t=0);
	bool nearDownStair() {return nearStair(8);}
	bool nearUpStair() {return nearStair(7);}
	void getYellowKey() {yellowkey++;}
	void getBlueKey() {bluekey++;}
	void getRedKey() {redkey++;}
	int getX() {return x;}
	int getY() {return y;}
	int getHP() {return hp;}
	int getAtk() {return atk;}
	int getDef() {return def;}
	int getMDef() {return mdef;}
	int getMoney() {return money;}
	int yellow() {return yellowkey;}
	int blue() {return bluekey;}
	int getNowFloor() {return now_floor;}
	int getDisplayFloor(int f=-1) {return f<0?getDisplayFloor(now_floor):(f>20?f-20:f);}
	void setFlyFloor(int delta=0);
	int getFlyFloor() {return fly_floor;}
	int dir[2][4];
private:
	int hp,atk,def,mdef,money,redkey,bluekey,yellowkey;
	int x,y,move,face,now_floor,fly_floor;
	hgeSprite* sprites[4][4];
};
