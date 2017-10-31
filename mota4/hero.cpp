#include "stdafx.h"

extern c_map_floor map_floor[50];
extern constants consts;

c_hero::c_hero()
{
	dir[0][0]=dir[0][3]=dir[1][1]=dir[1][2]=0;
	dir[0][1]=dir[1][3]=-1;
	dir[0][2]=dir[1][0]=1;
};
void c_hero::init()
{
	hp=1000;
	atk=10;
	def=10;
	mdef=0;
	money=0;
	yellowkey=0;
	bluekey=0;
	redkey=0;
	x=6;
	y=12;
	face=3;
	move=0;
	now_floor=0;
	fly_floor=0;
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			sprites[i][j]=new hgeSprite(consts.ht_hero, 32*i, 33*j, 32, 33);
}
int c_hero::nextX()
{
	return x+dir[0][face];
}
int c_hero::nextY()
{
	return y+dir[1][face];
}
bool c_hero::canMove(int f)
{
	face=f;
	int special=map_floor[now_floor].getSpecial(x,y);
	if (special==203 && f!=3) return false;
	if (special==204 && f!=1) return false;
	if (special==205 && f!=2) return false;
	if (special==206 && f!=0) return false;
	int nx=nextX(), ny=nextY();
	if (nx<0 || nx>=consts.map_width || ny<0 || ny>=consts.map_height)
		return false;
	return map_floor[now_floor].canMove(nx,ny,f);
}
void c_hero::show()
{
	sprites[move][face]->Render(x*32+8*move*dir[0][face]+consts.ScreenLeft, y*32+8*move*dir[1][face]);
}
bool c_hero::nearStair(int t)
{
	if (t==0) return nearDownStair() || nearUpStair();
	if (map_floor[now_floor].getinfo(y,x)->nearStair(t)) return true;
	if (x>0 && map_floor[now_floor].getinfo(y,x-1)->nearStair(t)) return true;
	if (y>0 && map_floor[now_floor].getinfo(y-1,x)->nearStair(t)) return true;
	if (x<consts.map_width-1 && map_floor[now_floor].getinfo(y,x+1)->nearStair(t)) return true;
	if (y<consts.map_height-1 && map_floor[now_floor].getinfo(y+1,x)->nearStair(t)) return true;
	return false;
}
bool c_hero::moveComplete()
{
	bool c=false;
	if(move==3)
	{
		c=true;
		x+=dir[0][face];
		y+=dir[1][face];
	}
	if (c)
	{
		int item=map_floor[now_floor].getItem(x,y);
		if(item>=11 && consts.music)
			consts.hge->Effect_PlayEx(consts.he_GetItem,consts.volume);
		int level=(now_floor+4)/5;
		switch(item)
		{
		case 11:atk+=1+level;
			break;
		case 12:def+=1+level;
			break;
		case 13: mdef+=2+3*level;
			break;
		case 14:
			consts.book=true;
			consts.setMsg(L"获得怪物手册。\n你可以将鼠标放置在怪物上查看怪物\n的基本信息。\n注意倒数第二行括号外为当前伤害，\n括号内为+1防减伤；最后一行为临界\n值和临界减伤。");
			consts.upload();
			break;
		case 15:yellowkey++;
			break;
		case 16:bluekey++;
			break;
		case 17:redkey++;
			break;
		case 18:hp+=100;
			break;
		case 19:hp+=250;
			break;
		case 20:hp+=500;
			break;
		case 21:hp+=800;
			break;
		case 22:
			atk+=10;
			consts.setMsg(L"获得铁剑，攻击+10。");
			break;
		case 23:
			def+=10;
			consts.setMsg(L"获得铁盾，防御+10。");
			break;
		case 25:
			consts.canfly=true;
			consts.setMsg(L"获得楼层飞行器。\n\n[G]键使用。");
			break;
		case 26:
			atk+=20;
			consts.setMsg(L"获得银剑，攻击+20。");
			break;
		case 27:
			def+=20;
			consts.setMsg(L"获得银盾，防御+20。");
			break;
		case 28:
			atk+=40;
			consts.setMsg(L"获得神圣剑，攻击+40。");
			break;
		case 29:
			def+=40;
			consts.setMsg(L"获得神圣盾，防御+40。");
			break;
		}
		int special=map_floor[now_floor].getSpecial(x,y);
		if (special!=0) {
			if (special==201) {
				map_floor[now_floor].setSpecial(x,y,202);
			}
			if (special==210 || special==211) {
				map_floor[now_floor].setSpecial(x,y,special==210?0:209);
				if (map_floor[now_floor].getSpecial(x+dir[0][face],y+dir[1][face])==0) {
					map_floor[now_floor].setSpecial(x+dir[0][face],y+dir[1][face],210);
				}
				else if (map_floor[now_floor].getSpecial(x+dir[0][face],y+dir[1][face])==209) {
					map_floor[now_floor].setSpecial(x+dir[0][face],y+dir[1][face],211);
				}
			}
		}
		consts.step++;
		if (now_floor==20 && x==6 && y==8 && map_floor[now_floor].hasMonster() && map_floor[now_floor].getinfo(9,6)->isGround()) {
			if (consts.music)
				consts.hge->Effect_PlayEx(consts.he_OpenDoor,consts.volume);
			consts.upload();
			map_floor[now_floor].getinfo(9,6)->init(84);
		}
	}
	move++;
	move%=4;
	return c;
}
bool c_hero::openRedDoor()
{
	if(redkey>0)
	{
		redkey--;
		return true;
	}
	return false;
}
bool c_hero::openBlueDoor()
{
	if(bluekey>0)
	{
		bluekey--;
		return true;
	}
	return false;
}
bool c_hero::openYellowDoor()
{
	if(yellowkey>0)
	{
		yellowkey--;
		return true;
	}
	return false;
}
void c_hero::upstair()
{
	now_floor++;
	setFlyFloor();
	map_floor[now_floor].getDownPosition(x,y);
	consts.flooring=true;
	consts.step++;
	if (now_floor>=consts.map_floornum)
		consts.map_floornum=now_floor+1;
}
void c_hero::downstair()
{
	now_floor--;
	setFlyFloor();
	map_floor[now_floor].getUpPosition(x,y);
	consts.flooring=true;
	consts.step++;
}
void c_hero::turn()
{
	if (face==0) face=2;
	else if (face==2) face=3;
	else if (face==3) face=1;
	else face=0;
}
void c_hero::fly()
{
	if(now_floor<=fly_floor && fly_floor!=0)map_floor[fly_floor].getDownPosition(x,y);
	else map_floor[fly_floor].getUpPosition(x,y);
	now_floor=fly_floor;
	consts.flooring=true;
}
void c_hero::printInfo()
{
	int py=16;
	consts.s_storey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",getDisplayFloor());
	py+=32;
	consts.s_heart->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",hp);
	py+=32;
	consts.s_sword1->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",atk);
	py+=32;
	consts.s_shield1->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",def);
	py+=32;
	consts.s_cross->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",mdef);
	py+=32;
	consts.s_coin->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",money);
	py+=32;
	consts.s_yellowkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",yellowkey);
	py+=32;
	consts.s_bluekey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",bluekey);
	py+=32;
	consts.s_redkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",redkey);

}
bool c_hero::canBeat(c_monster* monster) // 判断能否打败
{
	return hp>getDamage(monster);
}
int c_hero::getDamage(c_monster* monster) // 打败怪物，返回hp
{
	return getDamage(atk,def,mdef,monster->getHp(),monster->getAtk(),monster->getDef(),monster->getSpecial());
}
int c_hero::getDamage(int hero_atk, int hero_def, int hero_mdef,
					  int mon_hp, int mon_atk, int mon_def, int mon_special)
{
	// 魔攻
	if (mon_special==2) hero_def=0;
	// 坚固
	if (mon_special==3 && mon_def<hero_atk-1) mon_def=hero_atk-1;
	// 模仿
	if (mon_special==10)
	{
		mon_atk=hero_atk; mon_def=hero_def;
	}
	if (hero_atk<=mon_def) return MAX_DAMAGE;

	int per_damage=mon_atk-hero_def;
	if (per_damage<0) per_damage=0;
	// 2连击 & 3连击

	if (mon_special==4) per_damage*=2;
	if (mon_special==5) per_damage*=3;
	if (mon_special==6) per_damage*=4;
	// 反击
	if (mon_special==8) per_damage+=(int)(0.1*hero_atk);

	// 先攻
	int damage=mon_special==1?per_damage:0;
	// 破甲
	if (mon_special==7) damage=(int)(0.9*hero_def);
	// 净化
	if (mon_special==9) damage=3*hero_mdef;

	int ans=damage+(mon_hp-1)/(hero_atk-mon_def)*per_damage;
	ans -= hero_mdef;

	if (ans<=0) ans=0;
	if (consts.hard==1) ans=(int)(ans*0.9);
	if (consts.hard==2) ans=(int)(ans*0.96);
	return ans;
}
int c_hero::getCritical(int m_hp, int m_atk, int m_def, int m_spe)
{
	if (m_spe==3 || m_spe==10) return -1;
	int last=getDamage(atk, def, mdef, m_hp, m_atk, m_def, m_spe);
	if (last==0) return 0;
	for (int i = atk+1; i <= m_hp+m_def; i++)
	{
		int damage = getDamage(i, def, mdef, m_hp, m_atk, m_def, m_spe);
		if (damage < last)
			return i-atk;
		last = damage;
	}
	return 0;
}
int c_hero::getCriticalDamage(int m_hp, int m_atk, int m_def, int m_spe)
{
	int c=getCritical(m_hp,m_atk,m_def,m_spe);
	if (c<=0) return 0;
	int last=getDamage(atk, def, mdef, m_hp, m_atk, m_def, m_spe);
	if (last==MAX_DAMAGE) return 0;
	return last-getDamage(atk+c, def, mdef, m_hp, m_atk, m_def, m_spe);
}
int c_hero::getDefDamage(int m_hp, int m_atk, int m_def, int m_spe)
{
	return getDamage(atk, def, mdef, m_hp, m_atk, m_def, m_spe)-
		getDamage(atk, def+1, mdef, m_hp, m_atk, m_def, m_spe);
}
void c_hero::beat(c_monster* monster)
{
	int d=getDamage(monster), id=monster->getId();
	hp-=d;
	if (consts.music)
		consts.hge->Effect_PlayEx(consts.he_Attack, consts.volume);

	money+=monster->getMoney();

	monster->init(0);

	// 打完，触发一些事件

	if (now_floor==3 && id==13 && !map_floor[now_floor].getinfo(4,9)->hasMonster() && !map_floor[now_floor].getinfo(4,11)->hasMonster())
	{
		map_floor[now_floor].getinfo(3,10)->openSpecial();
	}
	if (now_floor==3 && id==14 && !map_floor[now_floor].getinfo(9,9)->hasMonster() && !map_floor[now_floor].getinfo(9,11)->hasMonster())
	{
		map_floor[now_floor].getinfo(8,10)->openSpecial();
	}
	if (now_floor==4 && id==14 && !map_floor[now_floor].getinfo(7,5)->hasMonster() && !map_floor[now_floor].getinfo(7,7)->hasMonster())
	{
		map_floor[now_floor].getinfo(6,6)->openSpecial();
	}
	if (now_floor==5 && id==15)
	{
		map_floor[now_floor].getinfo(3,6)->openSpecial();
		consts.upload();
	}
	if (now_floor==8 && id==18 && !map_floor[now_floor].getinfo(7,9)->hasMonster() && !map_floor[now_floor].getinfo(7,11)->hasMonster())
	{
		map_floor[now_floor].getinfo(8,10)->openSpecial();
	}
	if (now_floor==9 && id==19 && !map_floor[now_floor].getinfo(1,3)->hasMonster() && !map_floor[now_floor].getinfo(3,3)->hasMonster())
	{
		map_floor[now_floor].getinfo(2,2)->openSpecial();
	}
	if (now_floor==10 && id==8)
	{
		map_floor[now_floor].getinfo(3,6)->openSpecial();
		consts.upload();
	}
	if (now_floor==12 && id==22 && !map_floor[now_floor].getinfo(8,9)->hasMonster() && !map_floor[now_floor].getinfo(8,11)->hasMonster())
	{
		map_floor[now_floor].getinfo(7,10)->openSpecial();
	}
	if (now_floor==12 && id==7 && !map_floor[now_floor].getinfo(6,9)->hasMonster() && !map_floor[now_floor].getinfo(6,11)->hasMonster())
	{
		map_floor[now_floor].getinfo(5,10)->openSpecial();
	}
	if (now_floor==12 && id==16 && !map_floor[now_floor].getinfo(4,9)->hasMonster() && !map_floor[now_floor].getinfo(4,11)->hasMonster())
	{
		map_floor[now_floor].getinfo(3,10)->openSpecial();
	}
	if (now_floor==13 && (id==25 || id==27) && !map_floor[now_floor].getinfo(1,7)->hasMonster() && !map_floor[now_floor].getinfo(3,7)->hasMonster())
	{
		map_floor[now_floor].getinfo(2,8)->openSpecial();
	}
	if (now_floor==14 && id==27 && !map_floor[now_floor].getinfo(4,9)->hasMonster() && !map_floor[now_floor].getinfo(6,9)->hasMonster())
	{
		map_floor[now_floor].getinfo(5,8)->openSpecial();
	}
	if (now_floor==15 && id==26)
	{
		map_floor[now_floor].getinfo(9,6)->openSpecial();
		consts.upload();
	}
	if (now_floor==17 && id==40 && !map_floor[now_floor].getinfo(7,5)->hasMonster() && !map_floor[now_floor].getinfo(7,7)->hasMonster())
	{
		map_floor[now_floor].getinfo(8,6)->openSpecial();
	}

	consts.lasttime=clock();
}
void c_hero::shop(int select)
{
	int npcid=consts.map_npc->getId();
	int npctime=consts.map_npc->getVisit();
	if (npcid==41) {
		int need=20+2*npctime, hpadd=100+5*npctime;
		if (money<need) return;
		money-=need;
		if (select==1) hp+=hpadd;
		if (select==2) atk+=2;
		if (select==3) def+=2;
		if (select==4) mdef+=5;
		consts.map_npc->visitNpc();
		return;
	}
	if (npcid==42) {
		int need=40+4*npctime, hpadd=200+10*npctime;
		if (money<need) return;
		money-=need;
		if (select==1) hp+=hpadd;
		if (select==2) atk+=4;
		if (select==3) def+=4;
		if (select==4) mdef+=10;
		consts.map_npc->visitNpc();
		return;
	}


}
void c_hero::npc(int select)
{
	c_map_npc* map_npc=map_floor[now_floor].getinfo(nextY(),nextX())->getNpc();
	if (map_npc->getId()==0) return;
	int npcid=map_npc->getId();
	int npctime=map_npc->getVisit();
	switch (npcid)
	{
	case 41:
		{
			int need=20+2*npctime, hpadd=100+5*npctime;
			if (money<need) break;
			money-=need;
			if (select==1) hp+=hpadd;
			if (select==2) atk+=2;
			if (select==3) def+=2;
			if (select==4) mdef+=5;
			map_npc->visitNpc();
			break;
		}
	case 42:
		{
			int need=40+4*npctime, hpadd=200+10*npctime;
			if (money<need) break;
			money-=need;
			if (select==1) hp+=hpadd;
			if (select==2) atk+=4;
			if (select==3) def+=4;
			if (select==4) mdef+=10;
			map_npc->visitNpc();
			break;
		}
	case 43:
		{
			const wchar_t* ss[50]={
				L"游戏提示\t本塔快捷键说明：\n\nS/L: 存/读档\nK: 读取自动存档\nP: 查看当前MAX\nZ: 转向\nR: 重新开始\n-: 开启/关闭地图显伤\nM: 音乐开关",
				L"游戏提示\t本塔共四个区，每5层一个小Boss。\n\n困难难度下作者测试数据：\n一区：97\n二区：78\n三区：83\n\n亲民游戏，绝不卡血；希望各位血海\n大佬狠狠蹂躏之！",
				L"游戏提示\t本塔由艾之葵使用C++制作，如有问\n题请于发布帖下进行反馈，小艾在此\n感谢大家的支持！\n\n祝大家游戏愉快~"
			};
			consts.setMsg(ss);
			break;
		}
	case 44:
		{
			const wchar_t* ss[50]={
				L"游戏提示\t本塔共两个商店，分别在3F和12F。\n商店出售生命值的大小会随着购买次\n数而依次递增。\n\n本商店按[U]键可快捷使用。",
				L"游戏提示\t另外，困难难度下全塔可以不用购买\n任何一次生命通关，你能做到吗？"
			};
			consts.setMsg(ss);
			break;
		}
	case 45:
		consts.setMsg(L"游戏提示\t注意蓝钥匙的转换，这是本区域的难\n点所在。");
		break;
	case 46:
		consts.setMsg(L"游戏提示\t本区域怪物伤害较高，请尽快开启12\n楼的商店。\n\n该商店按[I]键可快捷使用。");
		break;
	case 47:
		consts.setMsg(L"游戏提示\t13-15F存在三条完全不同的路线，它\n们的差距不大且都能最终打掉本区域\n的Boss，但有一条相对略优一筹，你\n能找到它吗？");
		break;
	case 48:
		consts.setMsg(L"游戏提示\t最后一个区了，加油！");
		break;
	case 49:
		consts.setMsg(L"游戏提示\t下面的神圣剑和神圣盾只能拿一个。");
		break;
	default:
		consts.setMsg(L"勇士\t这是啥？");
		break;
	}
	consts.lasttime=clock();
}
void c_hero::save(FILE* f)
{
	char* ss=toString();
	fprintf_s(f,"%s",ss);
	delete ss;
}
char* c_hero::toString()
{
	char* ss=new char[2000];
	sprintf_s(ss, 2000, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",
		hp/77,hp%77,atk/46,atk%46,def/51,def%51,mdef/19,mdef%19,money/22,money%22,yellowkey,bluekey,redkey,x,y,face,now_floor);
	return ss;
}
void c_hero::load(FILE* f)
{
	int h1,h2,a1,a2,d1,d2,md1,md2,m1,m2;
	fscanf_s(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&h1,&h2,&a1,&a2,&d1,&d2,&md1,&md2,&m1,&m2,&yellowkey,&bluekey,&redkey,&x,&y,&face,&now_floor);
	hp=77*h1+h2; atk=46*a1+a2; def=51*d1+d2; mdef=19*md1+md2; money=22*m1+m2;
}

void c_hero::setFlyFloor(int delta)
{
	if (delta==0) 
		fly_floor=now_floor;
	else
	{
		int tmpfloor=fly_floor+delta;
		if (tmpfloor>=0) fly_floor=tmpfloor;
	}
}