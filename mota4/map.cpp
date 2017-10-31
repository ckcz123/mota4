#include "stdafx.h"

extern c_map_floor map_floor[50];
extern c_hero hero;
extern constants consts;

void c_map_door::init(int t)
{
	type=t;
	if(t<=4)for(int i=0;i<4;i++) door[i]=new hgeSprite(consts.ht_map,32*(t-1),32*(i+2),32,32);
	if(t==5)for(int i=0;i<4;i++) door[i]=consts.s_shopleft;
	if(t==6)for(int i=0;i<4;i++) door[i]=consts.s_shopright;
	if(t==7)for(int i=0;i<4;i++) door[i]=consts.s_upstair;
	if(t==8)for(int i=0;i<4;i++) door[i]=consts.s_downstair;
	state=0;
}
void c_map_door::show(int i,int j)
{
	if(type!=0 && type<=10)door[state]->Render(j*32+consts.ScreenLeft,i*32);
}
bool c_map_door::open()
{
	state++;
	if(state==3)
	{
		this->init(0);
		return true;
	}
	return false;
}
void c_map_point::init(int _type, int _times)
{
	type=0;
	item=0;
	monster.init(0); monster.setTimes(0);
	npc.init(0); npc.setVisit(0);
	door.init(0);
	special=0;

	if (_type<=9) type=_type;
	else if (_type<=40) item=_type;
	else if (_type<=80) {
		npc.init(_type);
		npc.setVisit(_times);
	}
	else if (_type<=100) door.init(_type-80);
	else if (_type<=200) {
		monster.init(_type-100);
		monster.setTimes(_times);
	}
	else special=_type;
}
int c_map_point::getPoint()
{
	if (special!=0) return special;
	if (monster.getId()!=0) return 100+monster.getId();
	if (door.getType()!=0) return 80+door.getType();
	if (npc.getId()!=0) return npc.getId();
	if (item!=0) return item;
	return type;
}
void c_map_point::show(GfxFont* f,int i,int j)
{
	// type->Render(j*32+consts.ScreenLeft,i*32);
	if (type==5) consts.s_ground->Render(j*32+consts.ScreenLeft,i*32);
	getTypeSprite()->Render(j*32+consts.ScreenLeft,i*32);
	if (getItemSprite()!=NULL)
		getItemSprite()->Render(j*32+consts.ScreenLeft,i*32);
	monster.show(f,i,j);
	door.show(i,j);
	npc.show(i,j);
	if(special!=0) {
		switch (special)
		{
		case 201:
			consts.s_sigh->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 202:
			consts.s_sighed->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 203:
			consts.s_up->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 204:
			consts.s_left->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 205:
			consts.s_right->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 206:
			consts.s_down->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 207:
			consts.s_wall_hidden->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 208:
			consts.s_portal->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 209:
			consts.s_flower->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 210:
			consts.s_box->Render(j*32+consts.ScreenLeft,i*32);
			break;
		case 211:
			consts.s_flower->Render(j*32+consts.ScreenLeft,i*32);
			consts.s_boxed->Render(j*32+consts.ScreenLeft,i*32);
			break;
		default:
			break;
		}

	}
}
hgeSprite* c_map_point::getTypeSprite() {
	if (type==1) return consts.s_wall;
	if (type==2) return consts.s_wall2;
	if (type==3) return consts.s_water;
	if (type==4) return consts.s_sky;
	if (type==5) return consts.s_barrier;
	if (type==6) return consts.s_wall3;
	return consts.s_ground;
}
hgeSprite* c_map_point::getItemSprite() {
	switch (item)
	{
	case 10:return consts.s_lightning;
	case 11:return consts.s_redjewel;break;
	case 12:return consts.s_bluejewel;break;
	case 13:return consts.s_greenjewel;break;
	case 14:return consts.s_enemyinfo;break;
	case 15:return consts.s_yellowkey;break;
	case 16:return consts.s_bluekey;break;
	case 17:return consts.s_redkey;break;
	case 18:return consts.s_redpotion;break;
	case 19:return consts.s_bluepotion;break;
	case 20:return consts.s_yellowpotion;break;
	case 21:return consts.s_greenpotion;break;
	case 22:return consts.s_sword1;break;
	case 23:return consts.s_shield1;break;
	case 24:return consts.s_cross;break;
	case 25:return consts.s_floor;break;
	case 26:return consts.s_sword2;break;
	case 27:return consts.s_shield2;break;
	case 28:return consts.s_sword3;break;
	case 29:return consts.s_shield3;break;
	}
	return NULL;
}
bool c_map_point::isGround()
{
	return type==0 && item==0 && monster.getId()==0 && door.getType()==0 && npc.getId()==0 && special==0;
}
bool c_map_point::canMove(int f)
{
	if(type==0)
	{
		if(door.getType()!=0)
		{
			bool c=false;
			switch(door.getType())
			{
			case 1:c=hero.openYellowDoor();break;
			case 2:c=hero.openBlueDoor();break;
			case 3:c=hero.openRedDoor();break;
			case 7:hero.upstair();break;
			case 8:hero.downstair();break;
			}
			if(c)
			{
				consts.opening=true;
				consts.map_openingdoor=&door;
				if (consts.music)
					consts.hge->Effect_PlayEx(consts.he_OpenDoor,consts.volume);
			}
			return c;
		}
		if(npc.getId()!=0) {
			consts.msg=consts.MESSAGE_NPC;
			return false;
		}
		if(monster.getId()!=0)
		{
			if(hero.canBeat(&monster))
				hero.beat(&monster);
			return false;
		}
		if(special!=0)
		{
			if(special==207) {
				special=0;
				consts.lasttime=clock();
				consts.hge->Effect_PlayEx(consts.he_Attack, consts.volume);
				return false;
			}
			if(special==208) {
				hero.specialMove();
				return false;
			}
			if(special==202) return false;
			if(special==203 && f==0) return false;
			if(special==204 && f==2) return false;
			if(special==205 && f==1) return false;
			if(special==206 && f==3) return false;
		}
		return true;
	}
	else return false;
}
bool c_map_point::openSpecial()
{
	if (door.getType()!=4) return false;
	consts.opening=true;
	consts.map_openingdoor=&door;
	if (consts.music)
		consts.hge->Effect_PlayEx(consts.he_OpenDoor,consts.volume);
	return true;
}
int c_map_point::getItemID()
{
	return item;
}
void c_map_point::toString(char* s)
{
	char ss[200];

	int point=getPoint();
	if (point>40 && point<=80) {
		sprintf_s(ss, " %d %d", point, npc.getVisit());
	}
	else if (point>100 && point<=200) {
		sprintf_s(ss, " %d %d", point, monster.getTimes());
	}
	else sprintf_s(ss, " %d", point);
	strcat_s(s, 20000, ss);
}
void c_map_point::load(FILE* f)
{
	int point=0, t=0;
	fscanf_s(f, "%d", &point);
	if ((point>40 && point<=80) || (point>100 && point<=200))
		fscanf_s(f, "%d", &t);
	init(point, t);
}
void c_map_floor::init(int d,int ch[30][30])
{
	id=d;
	ux=-1;uy=-1;dx=-1;dy=-1;
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].init(ch[i][j]);
			if(ch[i][j]==87)
			{
				ux=j;
				uy=i;
			}
			if(ch[i][j]==88)
			{
				dx=j;
				dy=i;
			}
		}
	}
}
void c_map_floor::show()
{
	GfxFont *f=new GfxFont(L"¿¬Ìå",12,true);
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].show(f,i,j);
		}
	}
	delete f;
}
void c_map_floor::animation()
{	
	for(int i=0;i<consts.map_height;i++)
		for(int j=0;j<consts.map_width;j++)
			info[i][j].animation();
}
void c_map_floor::printMonsterInfo(int i,int j)
{
	info[j][i].printMonsterInfo();
}
bool c_map_floor::canMove(int x,int y,int f)
{
	if (info[y][x].canMove(f)) {
		int spe=info[y][x].getSpecial();
		if (spe!=210 && spe!=211) return true;
		int sx=x+hero.dir[0][f],sy=y+hero.dir[1][f];
		if (sx>=0 && sx<consts.map_width && sy>=0 && sy<consts.map_height &&
			(info[sy][sx].isGround() || info[sy][sx].getSpecial()==209)) return true;
		return false;
	}
	return false;
}
int c_map_floor::getItem(int x,int y)
{
	int c=info[y][x].getItemID();
	if (c!=10)
		info[y][x].distroyItem();
	return c;
}
int c_map_floor::getSpecial(int x, int y)
{
	return info[y][x].getSpecial();
}
void c_map_floor::setSpecial(int x, int y, int _spe)
{
	info[y][x].setSpecial(_spe);
}
void c_map_floor::getDownPosition(int &x,int &y)
{
	x=dx;
	y=dy;
	if (hero.getNowFloor()==21)
		y-=2;
}
void c_map_floor::getUpPosition(int &x,int &y)
{
	x=ux;
	y=uy;
}
bool c_map_floor::hasMonster(int id) {
	for(int i=0;i<consts.map_height;i++)
		for(int j=0;j<consts.map_width;j++)
			if (info[i][j].hasMonster(id))
				return true;
	return false;
}
bool c_map_floor::isLinked()
{
	if (dx<0 || ux<0) return false;
	// (dy, dx) -> (uy, ux)
	bool visited[30][30];
	for (int i=0;i<30;i++) for (int j=0;j<30;j++) visited[i][j]=false;

	int dir[4][2]={{-1,0},{0,-1},{1,0},{0,1}};

	queue<int> q;
	q.push(consts.map_width*dy+dx);
	visited[dy][dx]=true;

	while (!q.empty()) {
		int f=q.front();
		q.pop();

		int cy=f/consts.map_width, cx=f%consts.map_width;
		// bfs
		c_map_point* point=getinfo(cy, cx);
		for (int i=0;i<4;i++) {
			if (point->getSpecial()==203 && i!=0) continue;
			if (point->getSpecial()==204 && i!=1) continue;
			if (point->getSpecial()==205 && i!=3) continue;
			if (point->getSpecial()==206 && i!=2) continue;

			int ny=cy+dir[i][0], nx=cx+dir[i][1];
			if (ny<0 || ny>=consts.map_height || nx<0 || nx>=consts.map_width) continue;
			if (ny==uy && nx==ux) return true;
			c_map_point* np=getinfo(ny, nx);
			bool cango=false;
			if (np->isGround()) cango=true;
			// »¨
			else if (np->getSpecial()==209) cango=true;
			// ¼ýÍ·
			else if (np->getSpecial()==203 && i!=2) cango=true;
			else if (np->getSpecial()==204 && i!=3) cango=true;
			else if (np->getSpecial()==205 && i!=1) cango=true;
			else if (np->getSpecial()==206 && i!=0) cango=true;

			if (cango && !visited[ny][nx]) {
				visited[ny][nx]=true;
				q.push(ny*consts.map_width+nx);
			}
		}
	}

	return false;
}
void c_map_floor::save(FILE* f)
{
	char *ss=toString();
	fprintf_s(f, "%s", ss);
	delete ss;
}
char* c_map_floor::toString()
{
	char* ss=new char[20000];
	sprintf_s(ss, 20000, "%d %d %d %d", dx,dy,ux,uy);
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].toString(ss);
		}
	}
	strcat_s(ss, 20000, " ");
	return ss;
}
void c_map_floor::load(FILE* f)
{
	fscanf_s(f,"%d %d %d %d",&dx,&dy,&ux,&uy);
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].load(f);
		}
	}
}