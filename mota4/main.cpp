#include "stdafx.h"

// ��ͼ��Ϣ
c_map_floor map_floor[50];

// ��ʿ��Ϣ
c_hero hero;

// ������Ϣ
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
	consts.setMsg(L"�浵�ɹ���");
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
	//consts.setMsg(L"�浵�ɹ���");
}
void autoload()
{
	if (consts.hard!=0)
		consts.uploadAll();
	FILE *loadfile;
	int err=fopen_s(&loadfile,"Save/autosave.dat","r");
	if (err!=0)
	{
		consts.setMsg(L"�Զ��浵��ȡʧ�ܣ�");
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
	consts.setMsg(L"�Զ��浵��ȡ�ɹ���");
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
	consts.setMsg(L"�����ɹ���");
}
void showMessage(const wchar_t *_s) // ��ʾ��ʾ
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
	GfxFont *f=new GfxFont(L"����", 22);

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
	GfxFont *f=new GfxFont(L"����", 22);
	f->Print(16+consts.ScreenLeft+8, consts.map_height*32-350+8, L"%s", s);
	delete f;
}
void init()
{
	// ��ʼ��
	consts.init();
	hero.init();
	// �����ͼ
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
		// 3F�̵�
		consts.map_npc=map_floor[3].getinfo(1,10)->getNpc();
		if (consts.map_npc->getVisit()==0) {
			consts.map_npc=NULL;
		}
		else consts.msg=consts.MESSAGE_SHOP;
	}
	if(consts.hge->Input_GetKeyState(HGEK_I) && consts.isFree()) {
		// 3F�̵�
		consts.map_npc=map_floor[12].getinfo(1,10)->getNpc();
		if (consts.map_npc->getVisit()==0) {
			consts.map_npc=NULL;
		}
		else consts.msg=consts.MESSAGE_SHOP;
	}
	/*
	if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_C) && consts.fly>0 && clock()-consts.lasttime>250) {
		if (!hero.canCenterFly()) {
			consts.setMsg(L"�޷�ʹ�����ĶԳƷ�������Ŀ�����\n��Ϊ�յء�");
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
			if (consts.lefttime<80.) consts.setMsg(L"����������ʧЧ�ˣ�");
			// else if (!hero.nearStair()) consts.setMsg(L"ֻ����¥���Բ���ʹ�ô�������");
			else {
				hero.setFlyFloor();
				consts.msg=consts.MESSAGE_FLYING;
			}
		}
	}
	if(consts.hge->Input_GetKeyState(HGEK_M) && consts.isFree()) {
		consts.music=!consts.music;
		consts.setMsg(consts.music?L"�����ѿ���":L"�����ѹر�");
		if (consts.music) consts.hge->Channel_SetVolume(consts.hc_Music, consts.bgmvolume);
		else consts.hge->Channel_SetVolume(consts.hc_Music, 0);
	}
	if(consts.hge->Input_GetKeyState(HGEK_MINUS) && consts.isFree()) {
		consts.showdamage=!consts.showdamage;
		consts.setMsg(consts.showdamage?L"�����ѿ���":L"�����ѹر�");
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
			consts.setMsg(L"��ʿ\t���ͣ���һ���ܴ����������ģ�");
			consts.nowcnt=0;
		}
	}

	// ��ʾ��Ϣ
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

	// �浵
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

	// ����
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

	// ʤ��
	if (consts.msg==consts.MESSAGE_WIN && clock()-consts.lasttime>300) {
		if (consts.hge->Input_GetKeyState(HGEK_P))
			consts.getRank();
		else if (consts.hge->Input_GetKeyState(HGEK_ENTER)) init();
	}
	// ���¿�ʼ
	if (consts.msg==consts.MESSAGE_RESTART) {
		if (consts.hge->Input_GetKeyState(HGEK_ENTER))
			init();
		else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE))
			consts.msg=consts.MESSAGE_NONE;
	}

	// ���ڷ���
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

		// ���ܹ�����
		if (consts.lefttime<80) {
			// �̵�
			if (npcid==45||npcid==50) {
				consts.setMsg(L"̰��֮��\t����Ҫ�����㻹�����������ɣ�");
			}
			else consts.setMsg(L"�ǻ�֮Ӱ\t���ӹ�ϲ��ɹ��ˣ����쵹�ˣ�����\n�ɣ������������꣬���ù������ˡ�");
		}

		// �̵�
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
		// ħ������
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
		// �ԳƷ�����
		else if (npcid==51) {
			if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
				hero.npc();
			}
			else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
				consts.msg=consts.MESSAGE_NONE;
			}
		}
		// Ѫ����
		else if (npcid==52 || npcid==53) {
			if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
				hero.npc();
			}
			else if (consts.hge->Input_GetKeyState(HGEK_ESCAPE)) {
				consts.msg=consts.MESSAGE_NONE;
			}
		}
		// 17¥����
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

	// �Զ��浵
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
		GfxFont *f=new GfxFont(L"����", 25);
		float len=f->GetTextSize(L"[1] ��ʼ��Ϸ").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height, L"[1] ��ʼ��Ϸ");
		f->Print(left, height+32, L"[2] ��ȡ��Ϸ");
		f->Print(left, height+64, L"[3] �˳���Ϸ");
		delete f;
		f=new GfxFont(L"����", 40);
		//f->SetColor(0xFFFF0000);
		f->SetColor(0xFF1C86EE);
		len=f->GetTextSize(L"20�������").cx;
		f->Print(16*consts.map_width+consts.ScreenLeft-len/2, height*0.4, L"20�������");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_CHOOSE_HARD) {
		GfxFont *f=new GfxFont(L"����", 25);
		float len=f->GetTextSize(L"[2] ��ͨ������4%%��").cx;
		float left=16*consts.map_width+consts.ScreenLeft-len/2;
		float height=consts.map_height*32*0.6;
		f->Print(left, height-36, L"��ѡ���Ѷȣ�");
		f->SetColor(0xFF00FF00);
		f->Print(left, height, L"[1] �򵥣�����10%%��");
		//f->SetColor(0xFF96CDCD);
		f->SetColor(0xFF1C86EE);
		f->Print(left, height+32, L"[2] ��ͨ������4%%��");
		f->SetColor(0xFFFF0000);
		f->Print(left, height+64, L"[3] ���ѣ�����ģʽ��");
		delete f;
		consts.hge->Gfx_EndScene();
		return false;
	}
	if (consts.msg==consts.MESSAGE_TEXT) {
		wchar_t* msg[50]={
			L"���Ͻӡ������������飩\n��û����������������˿�����ȥ��һ�棩\n��https://tieba.baidu.com/p/5356252626��",
			L"����λ��ʿ�ܲ�������Ŭ��֮�£�����֮��\n������ˮ����ȡ�ߣ�������Ҳ���׵�����\n\nս���Ϲ����ǵ���������Ҳ��ʧ�ˣ��۹���\n��ʿ�ǳ嵽���Ԫ����ǰ�����׽���رա�",
			L"Ȼ������Ȼ��Ҷ�۹����ⳡս����ȡ������\n�յ�ʤ������ȴ�������ޱȾ޴�Ĵ��ۡ�\n�����Ǹ߶�ս���������������ء�",
			L"��һ���棬����ô���˴���������֮����ȴ\nֻ��һ������֮���յػ�ʤ��Ҳ����һ����\n��˵������ʿ�Ǹ���ս��������ȱʧ��",
			L"Ϊ�˶�����ʿ�ǵ���ս���������������\nԭ���������ż��������޽���һ��20��ߵ�\n��������ս���з����һЩ�������Ԫ�Ĺ�\n��������У��Թ������ս���ҡ�",
			L"���ң�ֻ����ȫͨ�������20������������\nʿ�����ܱ����ϲ��õ������Ļ��ᡣ\n\n���㣬��Ϊһ����Ȼƽ������ͨȴ���ػ���\n־����ʿ����Ȼ��Ȼ��ʼ���Լ�����ս��"
		};

		GfxFont *f=new GfxFont(L"����", 24);
		f->SetKerningHeight(8);
		f->Render(consts.ScreenLeft+8, 32*consts.map_height*0.3, msg[consts.nowcnt]);
		delete f;

		f=new GfxFont(L"����", 24, true);
		if (consts.hard==1) {
			f->SetColor(0xFF00FF00);
			f->Print(64, 356, L"���Ѷ�");
		}
		else if (consts.hard==2) {
			// f->SetColor(0xFF96CDCD);
			f->SetColor(0xFF1C86EE);
			f->Print(64, 356, L"��ͨ�Ѷ�");
		}
		else if (consts.hard==3) {
			f->SetColor(0xFFFF0000);
			f->Print(64, 356, L"�����Ѷ�");
		}
		delete f;
		consts.hge->Gfx_EndScene();

		return false;
	}
	if (consts.msg==consts.MESSAGE_LOAD && consts.hard==0) {
		wchar_t ss[200];
		savedata* sd=&consts.sd[consts.wanttosave];
		if (sd->hp<0)
			wsprintf(ss,L"��ȡ�浵 %d\n(��Ч�Ĵ浵)\n\n[��][��][��][��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��", consts.wanttosave+1);
		else
			wsprintf(ss,L"��ȡ�浵 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[��][��][��][��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��",
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

	float mx,my; // ���λ��
	consts.hge->Input_GetMousePos(&mx,&my);
	int x=(mx-consts.ScreenLeft)/32,y=my/32;
	if (consts.book && x>=0 && x<consts.map_width)map_floor[hero.getNowFloor()].printMonsterInfo(x,y);

	switch (consts.msg)
	{
	case consts.MESSAGE_RESTART:
		showMessage(L"�������¿�ʼ��\n\n[ENTER] ȷ��\n[ESC] ȡ��");
		break;
	case consts.MESSAGE_WIN:
		{
			wchar_t ss[200];
			wsprintf(ss, L"��ϲͨ�أ����ķ����� %d��\n", hero.getHP());

			// uploading...
			if (consts.currentmax==0) {
				wcscat_s(ss, L"�����ϴ��ɼ�... ���Ժ�\n");
			}
			// error
			else if (consts.currentmax<0) {
				wcscat_s(ss, L"�ɼ��ϴ�ʧ�ܣ������������á�\n");
			}
			else {
				wchar_t tmp[200];
				wsprintf(tmp, L"��ǰ�Ѷ�������%s����ǰMAX %d��\n", consts.rank, consts.currentmax);
				wcscat_s(ss, tmp);
			}
			wcscat_s(ss, L"��P���ɲ鿴���Ѷ�MAX��¼��Ϣ����\n��ӭ��ͼ���������½�����ҫ��\n\n[ENTER] ���¿�ʼ");
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
				wsprintf(ss,L"�浵���ļ� %d\n(�հ�)\n\n[��][��][��][��] ���Ĵ浵λ��\n[ENTER] ȷ�ϴ浵\n[ESC] ȡ��", consts.wanttosave+1);
			else
				wsprintf(ss,L"�浵���ļ� %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[��][��][��][��] ���Ĵ浵λ��\n[ENTER] ȷ�ϴ浵\n[ESC] ȡ��",
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
				wsprintf(ss,L"��ȡ�浵 %d\n(��Ч�Ĵ浵)\n\n[��][��][��][��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��", consts.wanttosave+1);
			else
				wsprintf(ss,L"��ȡ�浵 %d\n(%s/F%d/HP%d/A%d/D%d)\n\n[��][��][��][��] ���Ķ���λ��\n[ENTER] ȷ�϶���\n[ESC] ȡ��",
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

			wsprintf(ss, L"��Ҫ��ȡ�Զ��浵��\n��ǰʱ�䣺%s\n�浵ʱ�䣺%s\n\n[ENTER] ȷ�϶�ȡ\n[ESC]ȡ��", tmp, tmp2);
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_FLYING:
		{
			wchar_t ss[100];
			wsprintf(ss,L"��Ҫ���� %d ¥\n\n[��] [��] ����¥��\n[ENTER] ȷ�Ϸ���\n[ESC] ȡ��",hero.getFlyFloor());
			showMessage(ss);
			break;
		}
	case consts.MESSAGE_RANK:
		{
			wchar_t ss[1200];
			if (consts.currentmax<0) {
				wsprintf(ss, L"��ȡMAX��¼ʧ�ܣ������������ӣ�\n���������½��з�����\n\n[ENTER] ȷ��");
				showMessage(ss);
			}
			else if (consts.currentmax==0) {
				wsprintf(ss, L"������ȡMAX��¼�����Ժ�...\n�������Ҫ�����ӡ�\n\n[ENTER] ȡ��");
				showMessage(ss);
			}
			else {

				if (consts.nowcnt==0) {
					wsprintf(ss, L"��ǰ����%d�˴γ��ԣ�%d�˴�ͨ�ء�\n\n", consts.tmp[0], consts.tmp[1]);
					wchar_t tmp[100];
					for (int i=1;i<=3;i++) {
						wsprintf(tmp, L"%s��%d�˴γ��ԣ�%d�˴�ͨ�ء�\n", consts.getHardText(i), consts.tmp[2*i], consts.tmp[2*i+1]);
						wcscat_s(ss, tmp);
						record* r=&consts.rd[i-1][0];
						wsprintf(tmp, L"MAX: [%d, %d, %d, %d]\n", r->hp, r->atk, r->def, r->mdef);
						wcscat_s(ss, tmp);
					}
					wcscat_s(ss, L"\n��ע�����˴Ρ����õ������ֲ����\n���㡣ÿ���õ��ֲᣬ�˴�+1��\n");
					wcscat_s(ss, L"\n[��][��] ��ϸ����   [ENTER] ȷ��");
					showMax(ss);
				}

				else {
					wsprintf(ss, L"%s�Ѷ���MAX���ݣ�\n", consts.getHardText(consts.nowcnt));

					for (int i=0; i<10; i++) {
						record* r=&consts.rd[consts.nowcnt-1][i];
						wchar_t tmp[100];
						wsprintf(tmp, L"TOP%d: [%d, %d, %d, %d]\n", i+1, r->hp, r->atk, r->def, r->mdef);
						wcscat_s(ss, tmp);
					}

					wcscat_s(ss, L"\n[��][��] �����Ѷ�   [ENTER] ȷ��");
					showMax(ss);
				}
			}

			break;
		}
	default:
		break;
	}
	// �̵�
	if (consts.msg==consts.MESSAGE_SHOP) {
		int id=consts.map_npc->getId(), times=consts.map_npc->getVisit();
		// 6¥�̵�
		if (id==41) {
			int need=20+2*times, hpadd=100+5*times;
			wchar_t s[200];
			wsprintf(s, L"̰��֮��\t�¸ҵ���ʿ��������%d��ҾͿ��ԣ�\n\n[1] ����+%d\n[2] ����+2\n[3] ����+2\n[4] ħ��+5\n[ESC] �뿪", need, hpadd);
			showMessage(s);
		}
		if (id==42) {
			int need=40+4*times, hpadd=200+10*times;
			wchar_t s[200];
			wsprintf(s, L"̰��֮��\t�¸ҵ���ʿ��������%d��ҾͿ��ԣ�\n\n[1] ����+%d\n[2] ����+4\n[3] ����+4\n[4] ħ��+10\n[ESC] �뿪", need, hpadd);
			showMessage(s);
		}
	}
	if (consts.msg==consts.MESSAGE_NPC) {
		c_map_npc* npc=map_floor[hero.getNowFloor()].getinfo(hero.nextY(),hero.nextX())->getNpc();
		int id=npc->getId(), times=npc->getVisit();

		// �̵�
		if (id==41) {
			int need=20+2*times, hpadd=100+5*times;
			wchar_t s[200];
			wsprintf(s, L"̰��֮��\t�¸ҵ���ʿ��������%d��ҾͿ��ԣ�\n\n[1] ����+%d\n[2] ����+2\n[3] ����+2\n[4] ħ��+5\n[ESC] �뿪", need, hpadd);
			showMessage(s);
		}
		if (id==42) {
			int need=40+4*times, hpadd=200+10*times;
			wchar_t s[200];
			wsprintf(s, L"̰��֮��\t�¸ҵ���ʿ��������%d��ҾͿ��ԣ�\n\n[1] ����+%d\n[2] ����+4\n[3] ����+4\n[4] ħ��+10\n[ESC] �뿪", need, hpadd);
			showMessage(s);
		}
		// 7¥����
		if (id==46) {
			if (times>0) {
				int need=25*(1+times);
				wchar_t s[200];
				wsprintf(s, L"�ǻ�֮Ӱ\t%d���һ�Σ�������ħ��ʹ�ô�����\n\n[ENTER] ��Ҫ\n[ESC] �뿪", need);
				showMessage(s);
			}
		}
		// 12¥�̵�
		if (id==50) {
			int need=50+4*times;
			wchar_t s[200];
			wsprintf(s, L"̰��֮��\t�¸ҵ���ʿ��������%d��ҾͿ��ԣ�\n\n[1] ����+4\n[2] ����+6\n[ESC] �뿪", need);
			showMessage(s);
		}
		// 14¥����
		if (id==51) {
			int need=50*(times+1);
			wchar_t s[200];
			wsprintf(s, L"�ǻ�֮Ӱ\t%d���һ�����ĶԳƷ�������Ҫ��\n[C]������ʹ�á�\n\n[ENTER] ��Ҫ\n[ESC] �뿪", need);
			showMessage(s);
		}
		// 8¥Ѫ����
		if (id==52) {
			showMessage(L"�ǻ�֮Ӱ\t70��һ�ȡ500����ֵ��Ҫ��\n\n[ENTER] ��Ҫ\n[ESC] �뿪");
		}
		// 13¥Ѫ����
		if (id==53) {
			showMessage(L"�ǻ�֮Ӱ\t180��һ�ȡ2000����ֵ��Ҫ��\n\n[ENTER] ��Ҫ\n[ESC] �뿪");
		}
		// 17¥����
		if (id==54) {
			if (times>0) {
				if (times==1) {
					showMessage(L"����\t��Ҫ��������1����\n�Ѷȣ�����\n���ؽ���������+1500��\n\n[ENTER] ȷ�Ͻ���\n[ESC] �뿪");
				}
				else if (times==2) {
					showMessage(L"����\tҪ��������2����\n�Ѷȣ������\n���ؽ�����ħ��+1���ԳƷ�+2��\n\n[ENTER] ȷ�Ͻ���\n[ESC] �뿪");
				}
				else if (times==3) {
					showMessage(L"����\tҪ��������3����\n�Ѷȣ�������\n���ؽ���������+10��\n\n[ENTER] ȷ�Ͻ���\n[ESC] �뿪");
				}
			}
		}
	}
	consts.hge->Gfx_EndScene();
	return false;
}
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{

	//hge��ʼ��
	consts.hge=hgeCreate(HGE_VERSION);
	consts.hge->System_SetState(HGE_FRAMEFUNC,frameFunc);
	consts.hge->System_SetState(HGE_RENDERFUNC,renderFunc);
	consts.hge->System_SetState(HGE_USESOUND,true);
	consts.hge->System_SetState(HGE_TITLE,"20������� By ��֮��");
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
