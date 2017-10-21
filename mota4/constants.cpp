#include "stdafx.h"

// 地图信息
extern c_map_floor map_floor[50];

// 勇士信息
extern c_hero hero;

extern Http http;

constants::constants()
{
	hge=NULL;
	volume=30;
	bgmvolume=100;
	ScreenLeft=240;
	music=true;
	showdamage=true;
	map_width=13;map_height=13;
	shouldUpload=true;
	wanttosave=0;
	lastload=-1000;
	shouldUpload=false;
}

void constants::init()
{
	canfly=book=moving=opening=flooring=false;
	lefttime=100.0;
	playtime=0.0;
	totaltime=0.0;
	step=0;
	hard=0;
	time_move=time_open=time_animation=time_floor=0;
	msg=MESSAGE_START;
	starttime=0;
	ending=0;
	savetime=0;
	for (int i=0;i<1000;i++) sd[i].hp=0;
}
void constants::loadResources()
{
	ht_map=hge->Texture_Load("Res/map.png");
	ht_icon=hge->Texture_Load("Res/icon.png");
	ht_icon2=hge->Texture_Load("Res/icon2.png");
	ht_hero=hge->Texture_Load("Res/hero.png");
	ht_monster=hge->Texture_Load("Res/monster.png");
	ht_skin=hge->Texture_Load("Res/skin.png");
	ht_npc=hge->Texture_Load("Res/npc.png");
	ht_special=hge->Texture_Load("Res/special.png");
	he_GetItem=hge->Effect_Load("Res/item.ogg");
	he_OpenDoor=hge->Effect_Load("Res/door.ogg");
	he_Attack=hge->Effect_Load("Res/attack.ogg");
	he_Music=hge->Effect_Load("Res/bgm.mp3");
	hgef=new hgeFont("Res/font.fnt");
	//ht_map
	s_ground=new hgeSprite(ht_map,0,0,32,32);
	s_wall=new hgeSprite(ht_map,32,0,32,32);
	s_wall2=new hgeSprite(ht_map,64,0,32,32);
	s_wall3=new hgeSprite(ht_map,96,0,32,32);
	s_wall_hidden=new hgeSprite(ht_map,32,0,32,32);
	s_wall_hidden->SetColor(0xA0FFFFFF);
	s_water=new hgeSprite(ht_map,32,32,32,32);
	s_sky=new hgeSprite(ht_map,0,32,32,32);
	s_lightning=new hgeSprite(ht_npc,0,128,32,32);
	s_barrier=new hgeSprite(ht_special,128,960,32,32);
	s_bg=new hgeSprite(ht_skin,0,0,128,128);
	s_bg->SetColor(0xD0FFFFFF);
	s_bg_font=new hgeSprite(ht_skin,0,0,128,128);
	s_bg_font->SetColor(0x72FFFFFF);
	//ht_door
	s_yellowdoor=new hgeSprite(ht_map,0,64,32,32);
	s_bluedoor=new hgeSprite(ht_map,32,64,32,32);
	s_reddoor=new hgeSprite(ht_map,64,64,32,32);
	s_specialdoor=new hgeSprite(ht_map,96,64,32,32);
	s_downstair=new hgeSprite(ht_map,0,192,32,32);
	s_upstair=new hgeSprite(ht_map,32,192,32,32);
	s_shopleft=new hgeSprite(ht_map,0,224,32,32);
	s_shopright=new hgeSprite(ht_map,64,224,32,32);
	//ht_item
	s_yellowkey=new hgeSprite(ht_icon,0,0,32,32);
	s_bluekey=new hgeSprite(ht_icon,32,0,32,32);
	s_redkey=new hgeSprite(ht_icon,64,0,32,32);
	s_allkey=new hgeSprite(ht_icon,96,0,32,32);
	s_coin=new hgeSprite(ht_icon,0,64,32,32);
	s_fly=new hgeSprite(ht_icon,0,192,32,32);
	s_floor=new hgeSprite(ht_icon,32,160,32,32);
	s_level=new hgeSprite(ht_icon,64,128,32,32);
	s_storey=new hgeSprite(ht_icon,64,96,32,32);
	s_cross=new hgeSprite(ht_icon,96,160,32,32);
	s_stick=new hgeSprite(ht_icon,0,160,32,32);
	s_drink=new hgeSprite(ht_icon,32,192,32,32);
	s_heart=new hgeSprite(ht_icon,64,192,32,32);
	s_expr=new hgeSprite(ht_icon,96,192,32,32);
	s_time=new hgeSprite(ht_icon,32,96,32,32);
	s_step=new hgeSprite(ht_icon,32,128,32,32);
	s_damage=new hgeSprite(ht_icon,64,160,32,32);
	s_enemyinfo=new hgeSprite(ht_icon,32,64,32,32);
	s_bluejewel=new hgeSprite(ht_icon,64,32,32,32);
	s_redjewel=new hgeSprite(ht_icon,96,32,32,32);
	s_greenjewel=new hgeSprite(ht_icon2,0,64,32,32);
	s_redpotion=new hgeSprite(ht_icon,64,64,32,32);
	s_bluepotion=new hgeSprite(ht_icon,96,64,32,32);
	s_yellowpotion=new hgeSprite(ht_icon2,96,96,32,32);
	s_greenpotion=new hgeSprite(ht_icon2,96,128,32,32);
	s_sword1=new hgeSprite(ht_icon,0,32,32,32);
	s_shield1=new hgeSprite(ht_icon,32,32,32,32);
	s_sword2=new hgeSprite(ht_icon,0,96,32,32);
	s_shield2=new hgeSprite(ht_icon,0,128,32,32);
	s_sword3=new hgeSprite(ht_icon,96,96,32,32);
	s_shield3=new hgeSprite(ht_icon,96,128,32,32);
	s_wand=new hgeSprite(ht_special,224,800,32,32);
	s_critical=new hgeSprite(ht_icon2,0,192,32,32);
	s_hint=new hgeSprite(ht_icon2,96,160,32,32);
	//special
	s_sigh=new hgeSprite(ht_special,192,800,32,32);
	s_sighed=new hgeSprite(ht_special,192,832,32,32);
	s_up=new hgeSprite(ht_special,0,928,32,32);
	s_down=new hgeSprite(ht_special,32,928,32,32);
	s_left=new hgeSprite(ht_special,64,928,32,32);
	s_right=new hgeSprite(ht_special,96,928,32,32);
	s_portal=new hgeSprite(ht_special,192,928,32,32);
	s_flower=new hgeSprite(ht_special,224,736,32,32);
	s_box=new hgeSprite(ht_special,192,768,32,32);
	s_boxed=new hgeSprite(ht_special,224,768,32,32);
	s_atk=new hgeSprite(ht_special,224,832,32,32);
	s_def=new hgeSprite(ht_special,224,864,32,32);
	s_life=new hgeSprite(ht_special,224,896,32,32);
	hc_Music=hge->Effect_PlayEx(he_Music,bgmvolume,0,1,true);
}

void constants::destroy()
{
	hge->Texture_Free(ht_map);
	hge->Texture_Free(ht_icon);
	hge->Texture_Free(ht_hero);
	hge->Texture_Free(ht_skin);
	hge->Texture_Free(ht_monster);
	hge->Texture_Free(ht_npc);
	hge->Texture_Free(ht_special);
	hge->Effect_Free(he_GetItem);
	hge->Effect_Free(he_Music);
	hge->Effect_Free(he_OpenDoor);
	hge->Effect_Free(he_Attack);
	delete hgef;
}

void constants::setMsg(const wchar_t* s[50])
{
	hint.clear();
	for (int i=0;i<50;i++) {
		if (s[i]==NULL) break;
		hint.push_back(wstring(s[i]));
	}

	lasttime=clock();
	nowcnt=0;
	msg=MESSAGE_HINT;
}

void constants::setMsg(const wchar_t* s) 
{
	hint.clear();
	hint.push_back(wstring(s));

	lasttime=clock();
	nowcnt=0;
	msg=MESSAGE_HINT;
}

void constants::goOn(c_hero* hero, c_map_floor* currFloor, float dt)
{
	playtime+=dt;
	totaltime+=dt;
	if(moving) // 移动
	{
		time_move+=dt;
		if(time_move>=0.03)
		{
			time_move-=0.03;
			if(hero->moveComplete())
				moving=false;
		}		
	}
	if(flooring) // 上下楼
	{
		time_floor+=dt;
		if(time_floor>=0.4)
		{
			time_floor=0;
			flooring=false;
		}
	}
	if(opening) // 开门
	{
		time_open+=dt;
		if(time_open>=0.05)
		{
			time_open-=0.05;
			if(map_openingdoor->open())
				opening=false;
		}		
	}

	time_animation+=dt;
	if(time_animation>=0.1) // 四次后又回到自身状态
	{
		time_animation-=0.1;
		currFloor->animation();
	}

	//	if (!ending && lefttime<80 && lefttime>0)
	//		lefttime-=dt;

}
void constants::finishHint()
{
	if (map_npc!=NULL && map_npc->getId()!=0) 
	{
		
		map_npc=NULL;

	}
	else if (ending) {
		upload();
		msg=MESSAGE_WIN;
		lasttime=clock();
	}
	else
		msg=MESSAGE_NONE;
}
void constants::printInfo()
{
	if (lefttime<80)
		hgef->printf(ScreenLeft+16,16,HGETEXT_LEFT,"%.2f",lefttime);
	s_step->Render(ScreenLeft+map_width*32+24, 340);
	hgef->printf(ScreenLeft+map_width*32+68, 340, HGETEXT_LEFT, "%d", step);
	int ptm=playtime;
	s_time->Render(ScreenLeft+map_width*32+24, 298);
	if (ptm>=3600)
		hgef->printf(ScreenLeft+map_width*32+68, 298, HGETEXT_LEFT, "%02d : %02d : %02d", ptm/3600, (ptm/60)%60, ptm%60);
	else hgef->printf(ScreenLeft+map_width*32+68, 298, HGETEXT_LEFT, "%02d : %02d", ptm/60, ptm%60);

	/*
	// 怪物手册
	if (book)
		s_enemyinfo->Render(24, 288);
	// 飞行器
	if (canfly) 
		s_floor->Render(71, 288);
	// 魔杖
	
	if (wand>=0) {
		s_wand->Render(118, 288);
		GfxFont* f=new GfxFont(L"楷体", 14, true);
		f->Print(142, 308, L"%d", wand);
		delete f;
	}
	if (fly>=0) {
		s_fly2->Render(165, 288);
		GfxFont* f=new GfxFont(L"楷体", 14, true);
		f->Print(189, 308, L"%d", fly);
		delete f;
	}
	*/

	GfxFont *f=new GfxFont(L"楷体", 24, true);
	if (hard==1) {
		f->SetColor(0xFF00FF00);
		f->Print(64, 356, L"简单难度");
	}
	else if (hard==2) {
		// f->SetColor(0xFF96CDCD);
		f->SetColor(0xFF1C86EE);
		f->Print(64, 356, L"普通难度");
	}
	else if (hard==3) {
		f->SetColor(0xFFFF0000);
		f->Print(64, 356, L"困难难度");
	}
	delete f;

}

void constants::upload()
{
	if (!shouldUpload && ending==0) return;
	currentmax=0;
	thread t1(&constants::doUpload, this);
	t1.detach();
}
void constants::uploadAll(int delay)
{
	if (!shouldUpload) return;
	thread t1(&constants::doUploadAll, this, delay);
	t1.detach();
}

void constants::doUpload()
{
	char url[200];
	// 开始时间、难度、当前层数、生命、攻击、防御、魔防、金钱、黄钥匙、蓝钥匙、游戏时间、总时间、步数、结局
	sprintf_s(url, "/service/mota/mota4.php?action=upload&starttime=%ld&hard=%d&floor=%d&hp=%d&atk=%d&def=%d&mdef=%d&money=%d&yellow=%d&blue=%d&playtime=%.3f&totaltime=%.3f&step=%d&ending=%d",
		starttime, hard, hero.getNowFloor(), hero.getHP(), hero.getAtk(), hero.getDef(), hero.getMDef(), hero.getMoney(), hero.yellow(), hero.blue(), playtime, totaltime, step, ending);

	char s[50000];

	char* ss=toString();
	strcpy_s(s, ss);
	delete ss;
	for (int i=0;i<map_floornum/2;i++) {
		ss=map_floor[i].toString();
		strcat_s(s, ss);
		delete ss;
	}
	ss=hero.toString();
	strcat_s(s, ss);
	delete ss;
	for (int i=map_floornum/2;i<map_floornum;i++) {
		ss=map_floor[i].toString();
		strcat_s(s, ss);
		delete ss;
	}

	char sd[100000]="data=";
	char* encoded=http.base64_urlencode(s);
	strcat_s(sd, encoded);
	delete encoded;

	char* output=http.get(http.server, http.port, url, sd);

	if (output!=NULL) {
		string text(output);
		stringstream stream;
		stream << text;

		string s1;
		stream >> s1 >> currentmax;
		if (currentmax<=0) currentmax=-1;
		else {
			size_t outsize;
			mbstowcs_s(&outsize, rank, 20, s1.c_str(), 20);
		}
		delete output;
	}
	else
		currentmax=-1;
}

void constants::doUploadAll(int delay)
{
	if (delay>0)
		Sleep(delay);
	char url[200];
	// 开始时间、难度、当前层数、生命、攻击、防御、魔防、金钱、黄钥匙、蓝钥匙、游戏时间、总时间、步数、结局
	sprintf_s(url, "/service/mota/mota4.php?action=upload&starttime=%ld&hard=%d&floor=%d&hp=%d&atk=%d&def=%d&mdef=%d&money=%d&yellow=%d&blue=%d&playtime=%.3f&totaltime=%.3f&step=%d&ending=%d",
		starttime, hard, hero.getNowFloor(), hero.getHP(), hero.getAtk(), hero.getDef(), hero.getMDef(), hero.getMoney(), hero.yellow(), hero.blue(), playtime, totaltime, step, ending);

	char s[50000];

	char* ss=toString();
	strcpy_s(s, ss);
	delete ss;
	for (int i=0;i<map_floornum/2;i++) {
		ss=map_floor[i].toString();
		strcat_s(s, ss);
		delete ss;
	}
	ss=hero.toString();
	strcat_s(s, ss);
	delete ss;
	for (int i=map_floornum/2;i<map_floornum;i++) {
		ss=map_floor[i].toString();
		strcat_s(s, ss);
		delete ss;
	}

	char sd[100000]="data=";
	char* encoded=http.base64_urlencode(s);
	strcat_s(sd, encoded);
	delete encoded;

	char* output=http.get(http.server, http.port, url, sd);

	if (output!=NULL)
		delete output;

}

void constants::getRank()
{
	msg=MESSAGE_RANK;
	currentmax=0;
	for (int x=0;x<3;x++)
		for (int i=0; i<10; i++)
			rd[x][i].init();
	nowcnt=0;
	thread t2(&constants::doGetRank, this);
	t2.detach();
}

void constants::doGetRank()
{
	char* output=http.get(http.server, http.port, "/service/mota/mota4.php?action=top", NULL);
	if (output!=NULL) {

		string text(output);
		stringstream stream;
		stream << text;

		// 首先存放8个数据
		for (int i=0;i<8;i++)
			stream >> tmp[i];

		for (int x=0;x<3;x++) {
			stream >> currentmax;
			for (int i=0; i<currentmax; i++) {
				stream >> rd[x][i].hp >> rd[x][i].atk >> rd[x][i].def >> rd[x][i].mdef;
			}
		}
		delete output;
		if (currentmax==0) currentmax=1;
	}
	else
		currentmax=-1;
}

void constants::save(FILE* f) 
{
	char *ss=toString();
	fprintf_s(f, "%s", ss);
	delete ss;
}
char* constants::toString()
{
	char* ss=new char[2000];
	sprintf_s(ss, 2000, "%d %d %d %d %d %.2f %.2f %.2f %ld ", map_floornum, canfly?1:0, book?1:0, step, hard, playtime, lefttime, totaltime, starttime);
	return ss;
}

void constants::load(FILE* f)
{
	int _fly, _book;
	float total;
	fscanf_s(f, "%d %d %d %d %d %f %f %f %ld", &nowcnt, &_fly, &_book, &step, &hard, &playtime, &lefttime, &total, &starttime);
	if (totaltime<total)
		totaltime=total;
	canfly=_fly==1;
	book=_book==1;
	moving=opening=flooring=false;
}