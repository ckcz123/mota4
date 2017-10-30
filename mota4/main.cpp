#include "stdafx.h"

// 地图信息
c_map_floor map_floor[50];

// 勇士信息
c_hero hero;

// 常量信息
constants consts;

// HTTP
Http http;

void loadsave()
{
	if (consts.lastload>=consts.wanttosave-5 && consts.lastload<=consts.wanttosave+5)
		return;

	FILE *savefile;
	constants tmpcon;
	c_hero tmphero;
	c_map_floor tmpfloor;
	for (int i=consts.wanttosave-5;i<=consts.wanttosave+5;i++) {
		if (i<0||i>=1000) continue;
		char s[100]="";
		sprintf_s(s,"Save/save%d.dat",i);
		int err=fopen_s(&savefile,s,"r");
		if (err!=0)
			consts.sd[i].hp=-1;
		else {
			tmpcon.load(savefile);
			for (int x=0;x<tmpcon.nowcnt/2;x++) tmpfloor.load(savefile);
			tmphero.load(savefile);
			consts.sd[i].init(tmphero.getHP(), tmphero.getAtk(), tmphero.getDef(), tmphero.getNowFloor(), tmpcon.hard);
			fclose(savefile);
		}
	}
	consts.lasttime=clock();
	consts.lastload=consts.wanttosave;
}

void save(int id)
{
	FILE *savefile;
	char s[100]="";
	sprintf_s(s,"Save/save%d.dat",id);
	fopen_s(&savefile,s,"w");
	consts.save(savefile);
	for(int i=0;i<consts.map_floornum/2;i++)map_floor[i].save(savefile);
	hero.save(savefile);
	for (int i=consts.map_floornum/2;i<consts.map_floornum;i++)map_floor[i].save(savefile);
	fclose(savefile);
	consts.uploadAll();
	consts.lastload=-1000;
	consts.setMsg(L"存档成功！");
}
void autosave()
{
	FILE *savefile;
	fopen_s(&savefile,"Save/autosave.dat","w");
	consts.save(savefile);
	for(int i=0;i<consts.map_floornum/2;i++)map_floor[i].save(savefile);
	hero.save(savefile);
	for (int i=consts.map_floornum/2;i<consts.map_floornum;i++)map_floor[i].save(savefile);
	fclose(savefile);
	//consts.setMsg(L"存档成功！");
}
void autoload()
{
	if (consts.hard!=0)
		consts.uploadAll();
	FILE *loadfile;
	int err=fopen_s(&loadfile,"Save/autosave.dat","r");
	if (err!=0)
	{
		consts.setMsg(L"自动存档读取失败！");
		return;
	}
	consts.load(loadfile);
	for(int i=0;i<consts.nowcnt/2;i++)map_floor[i].load(loadfile);
	hero.load(loadfile);
	for(int i=consts.nowcnt/2;i<consts.nowcnt;i++)map_floor[i].load(loadfile);
	if (consts.map_floornum<consts.nowcnt)
		consts.map_floornum=consts.nowcnt;
	consts.nowcnt=0;
	fclose(loadfile);
	consts.uploadAll(1500);
	consts.lastload=-1000;
	consts.setMsg(L"自动存档读取成功！");
}
void load(int id)
{
	if (consts.hard!=0)
		consts.uploadAll();
	FILE *loadfile;
	char s[100]="";
	sprintf_s(s,"Save/save%d.dat",id);
	fopen_s(&loadfile,s,"r");
	consts.load(loadfile);
	for(int i=0;i<consts.nowcnt/2;i++)map_floor[i].load(loadfile);
	hero.load(loadfile);
	for(int i=consts.nowcnt/2;i<consts.nowcnt;i++)map_floor[i].load(loadfile);
	if (consts.map_floornum<consts.nowcnt)
		consts.map_floornum=consts.nowcnt;
	consts.nowcnt=0;
	fclose(loadfile);
	consts.uploadAll(1500);
	consts.lastload=-1000;
	consts.setMsg(L"读档成功！");
}
void showMessage(const wchar_t *_s) // 显示提示
{

	wchar_t s[500];
	wcscpy_s(s, _s);

	// cal lines
	int l=1;
	for (wchar_t* p=s; *p!=L'\0';p++) {
		if (*p==L'\n') l++;
	}
	if (l<6) l=6;

	int left=16+consts.ScreenLeft, right=consts.map_width*32+consts.ScreenLeft-16,
		top=consts.map_height*32-12-28*l, bottom=consts.map_height*32-8;
	consts.s_bg->RenderStretch(left, top, right, bottom);
	GfxFont *f=new GfxFont(L"楷体", 22);

	// get name
	wchar_t* pos=s;
	while (*pos!=L'\t' && *pos!=L'\0') *pos++;
	if (*pos==L'\t') {
		*pos=L'\0';
		consts.s_bg->RenderStretch(left, top-40, left+25+f->GetTextSize(s).cx, top-2);
		f->Print(left+12, top-33, L"%s", s);
		pos++;
	}
	else pos=s;

	f->Print(left+12, top+12, L"%s", pos);

	delete f;
}
void showMax(const wchar_t *s)
{
	consts.s_bg->RenderStretch(16+consts.ScreenLeft, consts.map_height*32-350, consts.map_width*32+consts.ScreenLeft-16, consts.map_height*32-8);
	GfxFont *f=new GfxFont(L"楷体", 22);
	f->Print(16+consts.ScreenLeft+8, consts.map_height*32-350+8, L"%s", s);
	delete f;
}
void init()
{
	// 初始化
	consts.init();
	hero.init();
	// 读入地图
	FILE *map_infile;
	fopen_s(&map_infile,"Res/map.dat","r");
	fscanf_s(map_infile, "%d", &consts.map_floornum);
	for(int i=0;i<consts.map_floornum;i++)
	{
		int ch[30][30];
		for(int j=0;j<consts.map_height;j++)for(int k=0;k<consts.map_width;k++)fscanf_s(map_infile,"%d",&ch[j][k]);
		map_floor[i].init(i,ch);
	}
	fclose(map_infile);
}
bool frameFunc()
{
	float dt=consts.hge->Timer_GetDelta();

	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_DOWN) && hero.canMove(0))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_LEFT) && hero.canMove(1))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_RIGHT) && hero.canMove(2))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_UP) && hero.canMove(3))consts.moving=true;
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_Z) && clock()-consts.lasttime>200) {
		hero.turn();
		consts.lasttime=clock();
	}
	if(consts.hge->Input_GetKeyState(HGEK_R) && consts.isFree()) consts.msg=consts.MESSAGE_RESTART;
	if(consts.hge->Input_GetKeyState(HGEK_S) && consts.isFree()) {
		loadsave();
		consts.msg=consts.MESSAGE_SAVE;
	}
	if(consts.hge->Input_GetKeyState(HGEK_L) && consts.isFree()) {
		loadsave();
		consts.msg=consts.MESSAGE_LOAD;
	}
	if(consts.hge->Input_GetKeyState(HGEK_K) && consts.isFree()) {
		consts.msg=consts.MESSAGE_AUTOLOAD;
	}
	if(consts.hge->Input_GetKeyState(HGEK_U) && consts.isFree()) {
		// 3F商店
		consts.map_npc=map_floor[3].getinfo(1,10)->getNpc();
		if (consts.map_npc->getVisit()==0) {
			consts.map_npc=NULL;
		}
		else consts.msg=consts.MESSAGE_SHOP;
	}
	if(consts.hge->Input_GetKeyState(HGEK_I) && consts.isFree()) {
		// 3F商店
		consts.map_npc=map_floor[12].getinfo(1,10)->getNpc();
		if (consts.map_npc->getVisit()==0) {
			consts.map_npc=NULL;
		}
		else consts.msg=consts.MESSAGE_SHOP;
	}
	/*
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_C) && consts.fly>0 && clock()-consts.lasttime>250) {
		if (!hero.canCenterFly()) {
			consts.setMsg(L"无法使用中心对称飞行器：目标落点\n不为空地。");
		}
		else {
			hero.centerFly();
			consts.fly--;
			consts.lasttime=clock();
		}
	}
	*/
	if (consts.isFree() && consts.hge->Input_GetKeyState(HGEK_P)) consts.getRank();
	if(consts.hge->Input_GetKeyState(HGEK_G) && consts.isFree()) {
		if (consts.canfly) { 
			if (consts.lefttime<80.) consts.setMsg(L"传送器好像失效了！");
			// else if (!hero.nearStair()) consts.setMsg(L"只有在楼梯旁才能使用传送器！");
			else {
				hero.setFlyFloor();
				consts.msg=consts.MESSAGE_FLYING;
			}
		}
	}
	if(consts.hge->Input_GetKeyState(HGEK_M) && consts.isFree()) {
		consts.music=!consts.music;
		consts.setMsg(consts.music?L"音乐已开启":L"音乐已关闭");
		if (consts.music) consts.hge->Channel_SetVolume(consts.hc_Music, consts.bgmvolume);
		else consts.hge->Channel_SetVolume(consts.hc_Music, 0);
	}
	if(consts.hge->Input_GetKeyState(HGEK_MINUS) && consts.isFree()) {
		consts.showdamage=!consts.showdamage;
		consts.setMsg(consts.showdamage?L"显伤已开启":L"显伤已关闭");
	}

	if (consts.msg==consts.MESSAGE_START) {
		if (consts.hge->Input_GetKeyState(HGEK_1)) {
			consts.lasttime=clock();
			consts.msg=consts.MESSAGE_CHOOSE_HARD;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_2)) {
			loadsave();
			consts.msg=consts.MESSAGE_LOAD;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_3)) {
			return true;
		}
	}
	if (consts.msg==consts.MESSAGE_CHOOSE_HARD) {
		if (consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
			consts.hard=1;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
			consts.starttime=time(NULL);
			//consts.msg=consts.MESSAGE_NONE;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
			consts.hard=2;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
			consts.starttime=time(NULL);
			//consts.msg=consts.MESSAGE_NONE;
		}
		else if (consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
			consts.hard=3;
			consts.msg=consts.MESSAGE_TEXT;
			consts.nowcnt=0;
			consts.starttime=time(NULL);
			//consts.msg=consts.MESSAGE_NONE;

		}
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt==6) {
			consts.setMsg(L"勇士\t加油，我一定能闯过这座塔的！");
			consts.nowcnt=0;
		}
	}

	// 提示消息
	if (consts.msg==consts.MESSAGE_HINT)
	{
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt>=consts.hint.size()) {
			consts.finishHint();
		}
	}

	// 存档
	if(consts.msg==consts.MESSAGE_SAVE)
	{
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>200) {
			consts.wanttosave++;
			if (consts.wanttosave>=1000) consts.wanttosave=999;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_RIGHT) && clock()-consts.lasttime>200) {
			consts.wanttosave+=10;
			if (consts.wanttosave>=1000) consts.wanttosave=999;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>200) {
			consts.wanttosave--;
			if (consts.wanttosave<0) consts.wanttosave=0;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_LEFT) && clock()-consts.lasttime>200) {
			consts.wanttosave-=10;
			if (consts.wanttosave<0) consts.wanttosave=0;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_ESCAPE))
			consts.msg=consts.MESSAGE_NONE;
		else if(consts.hge->Input_GetKeyState(HGEK_ENTER))
			save(consts.wanttosave);
	}

	// 读档
	if(consts.msg==consts.MESSAGE_LOAD)
	{
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>200) {
			consts.wanttosave++;
			if (consts.wanttosave>=1000) consts.wanttosave=999;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_RIGHT) && clock()-consts.lasttime>200) {
			consts.wanttosave+=10;
			if (consts.wanttosave>=1000) consts.wanttosave=999;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>200) {
			consts.wanttosave--;
			if (consts.wanttosave<0) consts.wanttosave=0;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_LEFT) && clock()-consts.lasttime>200) {
			consts.wanttosave-=10;
			if (consts.wanttosave<0) consts.wanttosave=0;
			loadsave();
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
			if (consts.hard==0) consts.msg=consts.MESSAGE_START;
			else consts.msg=consts.MESSAGE_NONE;
		}
		else if(consts.hge->Input_GetKeyState(HGEK_ENTER)) {
			if (consts.sd[consts.wanttosave].hp>=0)
				load(consts.wanttosave);
		}
	}
	if (consts.msg==consts.MESSAGE_AUTOLOAD)
	{
		if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
			autoload();
		}
		else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
			consts.msg=consts.MESSAGE_NONE;
		}
	}

	// 胜利
	if (consts.msg==consts.MESSAGE_WIN && clock()-consts.lasttime>300) {
		if (consts.hge->Input_GetKeyState(HGEK_P))
			consts.getRank();
		else if (consts.hge->Input_GetKeyState(HGEK_ENTER)) init();
	}
	// 重新开始
	if (consts.msg==consts.MESSAGE_RESTART) {
		if (consts.hge->Input_GetKeyState(HGEK_ENTER))
			init();
		else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE))
			consts.msg=consts.MESSAGE_NONE;
	}

	// 正在飞行
	if(consts.msg==consts.MESSAGE_FLYING)
	{
		if(consts.hge->Input_GetKeyState(HGEK_UP) && clock()-consts.lasttime>100)
		{
			if (hero.getFlyFloor()<hero.getNowFloor()+(hero.nearUpStair()?1:0)
				|| map_floor[hero.getFlyFloor()].isLinked())
				hero.setFlyFloor(1);
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && clock()-consts.lasttime>100)
		{
			hero.setFlyFloor(-1);
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_ESCAPE))
		{
			consts.msg=consts.MESSAGE_NONE;
		}
		if(consts.hge->Input_GetKeyState(HGEK_ENTER))
		{
			hero.fly();
			consts.msg=consts.MESSAGE_NONE;
		}
	}

	if (consts.msg==consts.MESSAGE_SHOP) {
		if(consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
			hero.shop(1); 
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
			hero.shop(2); 
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
			hero.shop(3); 
			consts.lasttime=clock();
		}
		else if(consts.hge->Input_GetKeyState(HGEK_4) && clock()-consts.lasttime>200) {
			hero.shop(4); 
			consts.lasttime=clock();
		}
		else if (consts.hge->Input_GetKeyState(HGEK_ENTER) || consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
			consts.map_npc=NULL;
			consts.msg=consts.MESSAGE_NONE;
		}
	}

	if(consts.msg==consts.MESSAGE_NPC)
	{
		c_map_npc* map_npc=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpc();
		int npcid=map_npc->getId(), npctimes=map_npc->getVisit();

		// 逃跑过程中
		if (consts.lefttime<80) {
			// 商店
			if (npcid==45||npcid==50) {
				consts.setMsg(L"贪婪之神\t塔都要倒了你还想来买东西不成？");
			}
			else consts.setMsg(L"徘徊之影\t孩子恭喜你成功了！塔快倒了，快跑\n吧，我们已是亡魂，不用管我们了。");
		}

		// 商店
		else if (npcid==41 || npcid==42) {
			if(consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
				hero.npc(1);
			}
			else if(consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
				hero.npc(2);
			}
			else if(consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
				hero.npc(3);
			}
			else if(consts.hge->Input_GetKeyState(HGEK_4) && clock()-consts.lasttime>200) {
				hero.npc(4);
			}
			else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
				consts.msg=consts.MESSAGE_NONE;
			}
		}
		// 魔杖商人
		else if (npcid==46) {
			if (npctimes==0) {
				hero.npc();
			}
			else {
				if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
					hero.npc();
				}
				else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
					consts.msg=consts.MESSAGE_NONE;
				}
			}
		}
		// 对称飞商人
		else if (npcid==51) {
			if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
				hero.npc();
			}
			else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
				consts.msg=consts.MESSAGE_NONE;
			}
		}
		// 血商人
		else if (npcid==52 || npcid==53) {
			if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
				hero.npc();
			}
			else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
				consts.msg=consts.MESSAGE_NONE;
			}
		}
		// 17楼仙子
		else if (npcid==54) {
			if (npctimes==0 || npctimes==4) {
				hero.npc();
			}
			else {
				if (consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>150) {
					hero.npc();
				}
				else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
					consts.msg=consts.MESSAGE_NONE;
				}
			}
		}
		else {
			hero.npc();
		}
	}

	if (consts.msg==consts.MESSAGE_RANK) {
		if(consts.hge->Input_GetKeyState(HGEK_DOWN) && consts.currentmax>0 && clock()-consts.lasttime>180)
		{
			consts.nowcnt++; if (consts.nowcnt>3) consts.nowcnt=3;
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_UP) && consts.currentmax>0 && clock()-consts.lasttime>180)
		{
			consts.nowcnt--; if (consts.nowcnt<0) consts.nowcnt=0;
			consts.lasttime=clock();
		}
		if(consts.hge->Input_GetKeyState(HGEK_ENTER))
		{
			if (consts.ending)
				init();
			else 
				consts.msg=consts.MESSAGE_NONE;
			consts.nowcnt=0;
		}
	}

	if ((consts.msg==consts.MESSAGE_FLYING || consts.msg==consts.MESSAGE_NPC) && consts.hge->Input_GetKeyState(HGEK_ENTER))
		consts.msg=consts.MESSAGE_NONE;

	// 自动存档
	if (consts.isFree() && time(NULL)-consts.savetime>30) {
		autosave();
		consts.savetime=time(NULL);
	}

	consts.goOn(&hero, &map_floor[hero.getNowFloor()], dt);
	return false;
}
bool renderFunc()
{
	consts.hge->Gfx_BeginScene();
	consts.hge->Gfx_Clear(0);

	if (consts.msg==consts.MESSAGE_START) {
		GfxFont *f=new GfxFont(L"楷体", 25);
		float len=f->GetTextSize(L"[1] 开始游戏").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height, L"[1] 开始游戏");
		f->Print(left, height+32, L"[2] 读取游戏");
		f->Print(left, height+64, L"[3] 退出游戏");
		delete f;
		f=new GfxFont(L"楷体", 40);
		//f->SetColor(0xFFFF0000);
		f->SetColor(0xFF1C86EE);
		len=f->GetTextSize(L"20层的试炼").cx;
		f->Print(16*consts.map_width+consts.ScreenLeft-len/2, height*0.4, L"20层的试炼");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_CHOOSE_HARD) {
		GfxFont *f=new GfxFont(L"楷体", 25);
		float len=f->GetTextSize(L"[2] 普通（减伤4%%）").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height-36, L"请选择难度：");
		f->SetColor(0xFF00FF00);
		f->Print(left, height, L"[1] 简单（减伤10%%）");
		//f->SetColor(0xFF96CDCD);
		f->SetColor(0xFF1C86EE);
		f->Print(left, height+32, L"[2] 普通（减伤4%%）");
		f->SetColor(0xFFFF0000);
		f->Print(left, height+64, L"[3] 困难（正常模式）");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		wchar_t* msg[50]={
			L"（上接《重生塔》剧情）\n（没玩过《重生塔》的人可以先去玩一玩）\n（https://tieba.baidu.com/p/5356252626）",
			L"在那位勇士奋不顾死的努力之下，重生之塔\n塔顶的水晶被取走，这座塔也彻底倒塌。\n\n战场上怪物们的重生能力也消失了，帝国的\n勇士们冲到异次元大门前，彻底将其关闭。",
			L"然而，虽然青叶帝国在这场战斗中取得了最\n终的胜利，但却付出了无比巨大的代价。\n尤其是高端战力，更是伤亡惨重。",
			L"另一方面，有那么多人闯入了重生之塔，却\n只有一个人险之又险地获胜，也从另一个方\n面说明了武士们个人战斗能力的缺失。",
			L"为了锻炼武士们的作战能力，国王下令，在\n原重生塔的遗迹上重新修建了一座20层高的\n塔，并将战斗中俘获的一些来自异次元的怪\n物放入塔中，以供大家挑战自我。",
			L"并且，只有完全通过了这个20层试炼塔的武\n士，才能被承认并得到晋升的机会。\n\n而你，作为一名虽然平凡而普通却又胸怀大\n志的武士，毅然决然开始了自己的挑战。"
		};

		GfxFont *f=new GfxFont(L"楷体", 24);
		f->SetKerningHeight(8);
		f->Render(consts.ScreenLeft+8, 32*consts.map_height*0.3, msg[consts.nowcnt]);
		delete f;

		f=new GfxFont(L"楷体", 24, true);
		if (consts.hard==1) {
			f->SetColor(0xFF00FF00);
			f->Print(64, 356, L"简单难度");
		}
		else if (consts.hard==2) {
			// f->SetColor(0xFF96CDCD);
			f->SetColor(0xFF1C86EE);
			f->Print(64, 356, L"普通难度");
		}
		else if (consts.hard==3) {
			f->SetColor(0xFFFF0000);
			f->Print(64, 356, L"困难难度");
		}
		delete f;
		consts.hge->Gfx_EndScene();

		return false;
	}
	if (consts.msg==consts.MESSAGE_LOAD && consts.hard==0) {
		wchar_t ss[200];
		savedata* sd=&consts.sd[consts.wanttosave];
		if (sd->hp<0)
			wsprintf(ss,L"读取存档 %d\n(无效的存档)\n\n[↑][↓][←][→] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消", consts.wanttosave+1);
		else
			wsprintf(ss,L"读取存档 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[↑][↓][←][→] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消",
			consts.wanttosave+1, consts.getHardText(sd->hard), hero.getDisplayFloor(sd->now_floor), sd->hp,
			sd->atk, sd->def);
		showMessage(ss);

		consts.hge->Gfx_EndScene();

		return false;
	}

	map_floor[hero.getNowFloor()].show();
	hero.show();
	hero.printInfo();
	consts.printInfo();

	float mx,my; // 鼠标位置
	consts.hge->Input_GetMousePos(&mx,&my);
	int x=(mx-consts.ScreenLeft)/32,y=my/32;
	if (consts.book && x>=0 && x<consts.map_width)map_floor[hero.getNowFloor()].printMonsterInfo(x,y);

	switch (consts.msg)
	{
	case consts.MESSAGE_RESTART:
		showMessage(L"你想重新开始吗？\n\n[ENTER] 确认\n[ESC] 取消");
		break;
	case consts.MESSAGE_WIN:
		{
			wchar_t ss[200];
			wsprintf(ss, L"恭喜通关！您的分数是 %d。\n", hero.getHP());

			// uploading...
			if (consts.currentmax==0) {
				wcscat_s(ss, L"正在上传成绩... 请稍后\n");
			}
			// error
			else if (consts.currentmax<0) {
				wcscat_s(ss, L"成绩上传失败，请检查网络设置。\n");
			}
			else {
				wchar_t tmp[200];
				wsprintf(tmp, L"当前难度下排名%s，当前MAX %d。\n", consts.rank, consts.currentmax);
				wcscat_s(ss, tmp);
			}
			wcscat_s(ss, L"（P键可查看各难度MAX记录信息。）\n欢迎截图到发布帖下进行炫耀！\n\n[ENTER] 重新开始");
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_HINT:
		showMessage(consts.hint.at(consts.nowcnt).c_str());
		break;
	case consts.MESSAGE_SAVE:
		{
			wchar_t ss[200];
			savedata* sd=&consts.sd[consts.wanttosave];
			if (sd->hp<0)
				wsprintf(ss,L"存档到文件 %d\n(空白)\n\n[↑][↓][←][→] 更改存档位置\n[ENTER] 确认存档\n[ESC] 取消", consts.wanttosave+1);
			else
				wsprintf(ss,L"存档到文件 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[↑][↓][←][→] 更改存档位置\n[ENTER] 确认存档\n[ESC] 取消",
				consts.wanttosave+1, consts.getHardText(sd->hard), hero.getDisplayFloor(sd->now_floor), sd->hp,
				sd->atk, sd->def);
			showMessage(ss);
			break;
			break;
		}
	case consts.MESSAGE_LOAD:
		{
			wchar_t ss[200];
			savedata* sd=&consts.sd[consts.wanttosave];
			if (sd->hp<0)
				wsprintf(ss,L"读取存档 %d\n(无效的存档)\n\n[↑][↓][←][→] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消", consts.wanttosave+1);
			else
				wsprintf(ss,L"读取存档 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[↑][↓][←][→] 更改读档位置\n[ENTER] 确认读档\n[ESC] 取消",
				consts.wanttosave+1, consts.getHardText(sd->hard), hero.getDisplayFloor(sd->now_floor), sd->hp,
				sd->atk, sd->def);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_AUTOLOAD:
		{
			wchar_t ss[200], tmp[200], tmp2[200];
			char timestr[200];
			time_t st=consts.savetime;
			tm t;
			localtime_s(&t, &st);
			strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &t);
			size_t outsize;
			mbstowcs_s(&outsize, tmp2, 200, timestr, 200);

			st=time(NULL);
			localtime_s(&t, &st);
			strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &t);
			mbstowcs_s(&outsize, tmp, 200, timestr, 200);

			wsprintf(ss, L"你要读取自动存档吗？\n当前时间：%s\n存档时间：%s\n\n[ENTER] 确认读取\n[ESC]取消", tmp, tmp2);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_FLYING:
		{
			wchar_t ss[100];
			wsprintf(ss,L"我要飞往 %d 楼\n\n[↑] [↓] 更改楼号\n[ENTER] 确认飞行\n[ESC] 取消",hero.getFlyFloor());
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_RANK:
		{
			wchar_t ss[1200];
			if (consts.currentmax<0) {
				wsprintf(ss, L"拉取MAX记录失败，请检查网络连接，\n或在贴吧下进行反馈。\n\n[ENTER] 确定");
				showMessage(ss);
			}
			else if (consts.currentmax==0) {
				wsprintf(ss, L"正在拉取MAX记录，请稍后...\n这可能需要几秒钟。\n\n[ENTER] 取消");
				showMessage(ss);
			}
			else {

				if (consts.nowcnt==0) {
					wsprintf(ss, L"当前已有%d人次尝试，%d人次通关。\n\n", consts.tmp[0], consts.tmp[1]);
					wchar_t tmp[100];
					for (int i=1;i<=3;i++) {
						wsprintf(tmp, L"%s：%d人次尝试，%d人次通关。\n", consts.getHardText(i), consts.tmp[2*i], consts.tmp[2*i+1]);
						wcscat_s(ss, tmp);
						record* r=&consts.rd[i-1][0];
						wsprintf(tmp, L"MAX: [%d, %d, %d, %d]\n", r->hp, r->atk, r->def, r->mdef);
						wcscat_s(ss, tmp);
					}
					wcscat_s(ss, L"\n备注：“人次”以拿到怪物手册进行\n计算。每次拿到手册，人次+1。\n");
					wcscat_s(ss, L"\n[↑][↓] 详细数据   [ENTER] 确定");
					showMax(ss);
				}

				else {
					wsprintf(ss, L"%s难度下MAX数据：\n", consts.getHardText(consts.nowcnt));

					for (int i=0; i<10; i++) {
						record* r=&consts.rd[consts.nowcnt-1][i];
						wchar_t tmp[100];
						wsprintf(tmp, L"TOP%d: [%d, %d, %d, %d]\n", i+1, r->hp, r->atk, r->def, r->mdef);
						wcscat_s(ss, tmp);
					}

					wcscat_s(ss, L"\n[↑][↓] 其他难度   [ENTER] 确定");
					showMax(ss);
				}
			}

			break;
		}
	default:
		break;
	}
	// 商店
	if (consts.msg==consts.MESSAGE_SHOP) {
		int id=consts.map_npc->getId(), times=consts.map_npc->getVisit();
		// 6楼商店
		if (id==41) {
			int need=20+2*times, hpadd=100+5*times;
			wchar_t s[200];
			wsprintf(s, L"贪婪之神\t勇敢的武士啊，给我%d金币就可以：\n\n[1] 生命+%d\n[2] 攻击+2\n[3] 防御+2\n[4] 魔防+5\n[ESC] 离开", need, hpadd);
			showMessage(s);
		}
		if (id==42) {
			int need=40+4*times, hpadd=200+10*times;
			wchar_t s[200];
			wsprintf(s, L"贪婪之神\t勇敢的武士啊，给我%d金币就可以：\n\n[1] 生命+%d\n[2] 攻击+4\n[3] 防御+4\n[4] 魔防+10\n[ESC] 离开", need, hpadd);
			showMessage(s);
		}
	}
	if (consts.msg==consts.MESSAGE_NPC) {
		c_map_npc* npc=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpc();
		int id=npc->getId(), times=npc->getVisit();

		// 商店
		if (id==41) {
			int need=20+2*times, hpadd=100+5*times;
			wchar_t s[200];
			wsprintf(s, L"贪婪之神\t勇敢的武士啊，给我%d金币就可以：\n\n[1] 生命+%d\n[2] 攻击+2\n[3] 防御+2\n[4] 魔防+5\n[ESC] 离开", need, hpadd);
			showMessage(s);
		}
		if (id==42) {
			int need=40+4*times, hpadd=200+10*times;
			wchar_t s[200];
			wsprintf(s, L"贪婪之神\t勇敢的武士啊，给我%d金币就可以：\n\n[1] 生命+%d\n[2] 攻击+4\n[3] 防御+4\n[4] 魔防+10\n[ESC] 离开", need, hpadd);
			showMessage(s);
		}
		// 7楼奸商
		if (id==46) {
			if (times>0) {
				int need=25*(1+times);
				wchar_t s[200];
				wsprintf(s, L"徘徊之影\t%d金币一次，可增加魔杖使用次数。\n\n[ENTER] 我要\n[ESC] 离开", need);
				showMessage(s);
			}
		}
		// 12楼商店
		if (id==50) {
			int need=50+4*times;
			wchar_t s[200];
			wsprintf(s, L"贪婪之神\t勇敢的武士啊，给我%d金币就可以：\n\n[1] 攻击+4\n[2] 防御+6\n[ESC] 离开", need);
			showMessage(s);
		}
		// 14楼奸商
		if (id==51) {
			int need=50*(times+1);
			wchar_t s[200];
			wsprintf(s, L"徘徊之影\t%d金币一个中心对称飞行器，要吗？\n[C]键可以使用。\n\n[ENTER] 我要\n[ESC] 离开", need);
			showMessage(s);
		}
		// 8楼血商人
		if (id==52) {
			showMessage(L"徘徊之影\t70金币换取500生命值，要吗？\n\n[ENTER] 我要\n[ESC] 离开");
		}
		// 13楼血商人
		if (id==53) {
			showMessage(L"徘徊之影\t180金币换取2000生命值，要吗？\n\n[ENTER] 我要\n[ESC] 离开");
		}
		// 17楼仙子
		if (id==54) {
			if (times>0) {
				if (times==1) {
					showMessage(L"仙子\t你要进入试炼1层吗？\n难度：★★★\n过关奖励：生命+1500。\n\n[ENTER] 确认进入\n[ESC] 离开");
				}
				else if (times==2) {
					showMessage(L"仙子\t要进入试炼2层吗？\n难度：★★★★\n过关奖励：魔杖+1，对称飞+2。\n\n[ENTER] 确认进入\n[ESC] 离开");
				}
				else if (times==3) {
					showMessage(L"仙子\t要进入试炼3层吗？\n难度：★★★★★\n过关奖励：攻防+10。\n\n[ENTER] 确认进入\n[ESC] 离开");
				}
			}
		}
	}
	consts.hge->Gfx_EndScene();
	return false;
}
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{

	//hge初始化
	consts.hge=hgeCreate(HGE_VERSION);
	consts.hge->System_SetState(HGE_FRAMEFUNC,frameFunc);
	consts.hge->System_SetState(HGE_RENDERFUNC,renderFunc);
	consts.hge->System_SetState(HGE_USESOUND,true);
	consts.hge->System_SetState(HGE_TITLE,"20层的试炼 By 艾之葵");
	consts.hge->System_SetState(HGE_WINDOWED,true);
	consts.hge->System_SetState(HGE_HIDEMOUSE,false);
	consts.hge->System_SetState(HGE_SCREENHEIGHT,32*consts.map_height);
	consts.hge->System_SetState(HGE_SCREENWIDTH,32*consts.map_width+2*consts.ScreenLeft);
	if(consts.hge->System_Initiate())
	{
		consts.loadResources();
		init();
		consts.hge->System_Start();
		consts.destroy();
	}
	consts.hge->System_Shutdown();
	consts.hge->Release();
	return 0;
}
