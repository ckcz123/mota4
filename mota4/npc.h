#pragma once
#include "monster.h"

class c_map_npc
{
public:
	void init(int);//��ʼ��
	void show(int,int);//��Ⱦ
	int getId();//����npc��id
	void changeState();//�ı�״̬
	int getVisit() {return visit;}
	void setVisit(int t) {visit=t;}
	void visitNpc() {visit++;}
private:
	int id;
	int state;
	int visit;
	hgeSprite *npc[4];
};