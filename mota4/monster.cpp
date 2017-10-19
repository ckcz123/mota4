#include "stdafx.h"

extern c_hero hero;
extern constants consts;
void c_monster::init(int t)
{
	id=t;
	state=0;
	times=0;
	special=0;

	// 1 - 先攻
	// 2 - 魔攻
	// 3 - 坚固
	// 4 - 特殊

	switch(t)
	{
	case 1:wcscpy_s(name,L"绿色史莱姆");position=0;hp=130;atk=14;def=4;money=1;break;
	case 2:wcscpy_s(name,L"红色史莱姆");position=1;hp=156;atk=21;def=5;money=1;break;
	case 3:wcscpy_s(name,L"青头怪");position=2;hp=189;atk=27;def=6;money=3;break;
	case 4:wcscpy_s(name,L"史莱姆王");position=3;hp=543;atk=286;def=179;money=30;break;
	case 5:wcscpy_s(name,L"小蝙蝠");position=4;hp=162;atk=25;def=5;money=2;break;
	case 6:wcscpy_s(name,L"大蝙蝠");position=5;hp=170;atk=90;def=40;money=12;special=1;break;
	case 7:wcscpy_s(name,L"红蝙蝠");position=6;hp=700;atk=335;def=280;money=35;break;
	case 8:wcscpy_s(name,L"冥灵魔王");position=7;hp=800;atk=550;def=90;money=25;break;
	case 9:wcscpy_s(name,L"初级法师");position=8;hp=150;atk=50;def=0;money=4;special=2;break;
	case 10:wcscpy_s(name,L"高级法师");position=9;hp=230;atk=120;def=50;money=16;special=2;break;
	case 11:wcscpy_s(name,L"初级巫师");position=10;hp=400;atk=200;def=0;money=38;special=2;break;
	case 12:wcscpy_s(name,L"高级巫师");position=11;hp=700;atk=350;def=0;money=40;special=2;break;
	case 13:wcscpy_s(name,L"骷髅人");position=12;hp=223;atk=44;def=3;money=5;break;
	case 14:wcscpy_s(name,L"骷髅士兵");position=13;hp=343;atk=58;def=10;money=8;break;
	case 15:wcscpy_s(name,L"骷髅队长");position=14;hp=600;atk=108;def=20;money=13;break;
	case 16:wcscpy_s(name,L"冥队长");position=15;hp=538;atk=376;def=98;money=32;break;
	case 17:wcscpy_s(name,L"兽面人");position=16;hp=250;atk=100;def=35;money=14;break;
	case 18:wcscpy_s(name,L"兽面武士");position=17;hp=600;atk=175;def=66;money=19;break;
	case 19:wcscpy_s(name,L"石头人");position=18;hp=27;atk=54;def=0;money=8;special=3;break;
	case 20:wcscpy_s(name,L"影子战士");position=19;hp=333;atk=140;def=60;money=20;break;
	case 21:wcscpy_s(name,L"初级卫兵");position=20;hp=444;atk=85;def=24;money=10;break;
	case 22:wcscpy_s(name,L"中级卫兵");position=21;hp=600;atk=136;def=65;money=18;break;
	case 23:wcscpy_s(name,L"高级卫兵");position=22;hp=500;atk=308;def=119;money=28;break;
	case 24:wcscpy_s(name,L"双手剑士");position=23;hp=454;atk=584;def=0;money=28;break;
	case 25:wcscpy_s(name,L"冥战士");position=24;hp=649;atk=409;def=112;money=34;break;
	case 26:wcscpy_s(name,L"骑士队长");position=25;hp=1356;atk=594;def=140;money=38;break;
	case 27:wcscpy_s(name,L"金骑士");position=26;hp=847;atk=451;def=233;money=46;break;
	case 28:wcscpy_s(name,L"灵武士");position=27;hp=988;atk=673;def=235;money=50;break;
	case 30:wcscpy_s(name,L"白衣武士");position=29;hp=735;atk=652;def=100;money=42;break;
	case 32:wcscpy_s(name,L"铁面人");position=34;hp=748;atk=401;def=354;money=45;break;
	case 33:wcscpy_s(name,L"近卫骑士");position=39;hp=950;atk=500;def=350;money=48;break;
	case 34:wcscpy_s(name,L"黑衣魔王");position=36;hp=2017;atk=1021;def=243;money=58;break;
	case 51:wcscpy_s(name,L"绿头怪");position=0;hp=21;atk=0;def=0;money=0;special=4;break;
	case 52:wcscpy_s(name,L"红头怪");position=1;hp=201;atk=0;def=0;money=0;special=4;break;
	case 53:wcscpy_s(name,L"青头怪");position=2;hp=401;atk=0;def=0;money=0;special=4;break;
	case 54:wcscpy_s(name,L"怪王");position=3;hp=601;atk=0;def=0;money=0;special=4;break;
	default:position=-1;
	}
	if (position!=-1)
		for(int i=0;i<4;i++)
			monster[i]=new hgeSprite(consts.ht_monster,32*i,32*position,32,32);
}
int c_monster::getAtk() {
	return special==4?hero.getDef()+1:atk;
}
int c_monster::getDef() {
	return special==3||special==4?hero.getAtk()-1:def;
}
void c_monster::show(GfxFont* f,int i,int j)
{
	if(id!=0)
	{
		monster[state]->Render(j*32+consts.ScreenLeft,i*32);
		if (consts.book && consts.showdamage && f!=NULL) {
			int damage=hero.getDamage(this);
			int hp=hero.getHP();
			if (damage>=hp) {
				f->SetColor(0xFFFF0000);
			}
			else if (damage<=hp/10) {
				f->SetColor(0xFF00FF00);
			}
			else if (damage<=hp/5) {
				f->SetColor(0xFF66CD00);
			}
			else if (damage<=hp/2) {
				f->SetColor(0xFF96CDCD);
			}
			else if (damage<=hp*3/4) {
				f->SetColor(0xFFEE7942);
			}
			else {
				f->SetColor(0xFFEE3B3B);
			}
			wchar_t ss[100];
			if (damage>=c_hero::MAX_DAMAGE)
				wcscpy_s(ss, L"???");
			else if (damage<100000)
				wsprintf(ss, L"%d", damage);
			else
				wsprintf(ss, L"%.1fW", damage/10000.0);
			SIZE size=f->GetTextSize(ss);
			int left=j*32+consts.ScreenLeft, top=i*32+20;
			consts.s_bg_font->RenderStretch(left-1, top-1, left+size.cx+1, top+size.cy+1);
			f->Render(left, top, ss);
		}
	}
}
void c_monster::printInfo()
{
	if(id!=0)
	{
		int py=16;
		GfxFont *f=new GfxFont(L"楷体",24);
		wchar_t s0[100]=L"";
		if (special==1) wcscpy_s(s0, L"（先攻）");
		else if (special==2) wcscpy_s(s0, L"（魔攻）");
		else if (special==3) wcscpy_s(s0, L"（坚固）");
		else if (special==4) wcscpy_s(s0, L"（特殊）");
		f->Print(consts.ScreenLeft+consts.map_width*32+16,py,L"%s%s",name,s0);
		delete f;
		py+=32;
		consts.s_heart->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getHp());
		py+=32;
		consts.s_sword1->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getAtk());
		py+=32;
		consts.s_shield1->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getDef());
		py+=32;
		consts.s_coin->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getMoney());
		py+=32;
		consts.s_damage->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		int damage=hero.getDamage(this), defDamage=hero.getDefDamage(getHp(), getAtk(), getDef(), getSpecial());
		int critical=hero.getCritical(getHp(), getAtk(), getDef(), getSpecial());
		int criticalDamage=hero.getCriticalDamage(getHp(), getAtk(), getDef(), getSpecial());
		if (damage==c_hero::MAX_DAMAGE)
			consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"???  (%d)",defDamage);
		else
			consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d  (%d)",damage, defDamage);
		py+=32;
		if (critical>0)
		{
			consts.s_critical->Render(consts.ScreenLeft+consts.map_width*32+16,py);
			if (criticalDamage>0)
				consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d  (%d)",critical,criticalDamage);
			else
				consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",critical);
		}

	}
}
void c_monster::changeState()
{
	state++;
	state%=4;
}