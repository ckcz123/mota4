#pragma once

#include "npc.h"
class c_hero
{
public:
	const static int MAX_DAMAGE = 99999999;

	c_hero();//���캯��
	void init();
	int nextX();//��һ���ĺ�����
	int nextY();//��һ����������
	bool canMove(int);//�Ƿ���Գ�ĳ�������ƶ�
	bool moveComplete();//�ƶ��Ƿ����
	bool openRedDoor();//�򿪺�ɫ����
	bool openBlueDoor();//����ɫ����
	bool openYellowDoor();//�򿪻�ɫ����
	void upstair();//��¥
	void downstair();//��¥
	void turn(); // ת��
	void fly(); //¥���Ծ
	void specialMove() {}
	void show();
	void printInfo();//�����ʿ��Ϣ
	bool canBeat(c_monster*);//�ܷ��ܹ���
	int getDamage(c_monster*);//��ܹ�����ͨ�˺�
	int getDamage(int h_atk, int h_def, int h_mdef, int m_hp, int m_atk, int m_def, int m_spe);
	int getCritical(int m_hp, int m_atk, int m_def, int m_spe);
	int getCriticalDamage(int m_hp, int m_atk, int m_def, int m_spe);
	int getDefDamage(int m_hp, int m_atk, int m_def, int m_spe);
	void beat(c_monster*);
	void npc(int select=0);//ѡ��select
	void save(FILE*);//�浵
	char* toString();
	void load(FILE*);//����
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
