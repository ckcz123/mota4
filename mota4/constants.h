#pragma once
#include "map.h"
#include <string>
#include <vector>

struct savedata
{
	int hp, atk, def, now_floor, hard;
	void init(int _hp, int _atk, int _def, int _nowf, int _hard) {hp=_hp;atk=_atk;def=_def;now_floor=_nowf;hard=_hard;}
};

struct record
{
	int hp, atk, def, money, yellow, blue;
	void init() {hp=atk=def=money=yellow=blue=0;}
};

class constants
{
public:
	static const int MESSAGE_NONE = 0;
	static const int MESSAGE_RESTART = 2;
	static const int MESSAGE_SAVE = 3;
	static const int MESSAGE_LOAD = 4;
	static const int MESSAGE_WIN = 5;
	static const int MESSAGE_FLYING = 7;
	static const int MESSAGE_NPC = 8;
	static const int MESSAGE_HINT = 9;
	static const int MESSAGE_START = 10;
	static const int MESSAGE_CHOOSE_HARD = 11;
	static const int MESSAGE_TEXT = 12;
	static const int MESSAGE_RANK = 13;

	constants();
	void init();
	void loadResources();
	void destroy();
	void setMsg(const wchar_t*[50]);
	void setMsg(const wchar_t*);
	bool isFree() {return !moving && !opening && !flooring && !ending && msg==MESSAGE_NONE && clock()-lasttime>150;}
	wchar_t* getHardText(int h) {return h==1?L"简单":h==2?L"普通":h==3?L"困难":L"";}
	void goOn(c_hero*, c_map_floor*, float);
	void save(FILE*);
	char* toString();
	void load(FILE*);
	void finishHint();
	void printInfo();
	void upload();
	void uploadAll(int delay=0);
	void doUpload();
	void doUploadAll(int delay=0);
	void getRank();
	void doGetRank();

	int msg;
	vector<wstring> hint;
	int nowcnt;
	long lasttime;
	long starttime;

	float playtime, lefttime, totaltime;
	int step;

	// 难度，1简单2普通3困难
	int hard;

	bool canfly,book;

	bool moving,opening,flooring,music,showdamage,shouldUpload;
	int map_floornum,map_height,map_width,volume,bgmvolume,ScreenLeft;

	savedata sd[1000];
	int wanttosave;
	int lastload;

	// 结局：1 NE 2GE 3TE
	int ending;

	// 排名信息
	wchar_t rank[20];
	int currentmax;
	record rd[8][5];
	int tmp[50];

	// 正在打开的门
	c_map_door *map_openingdoor;

	// 正在对话的npc
	c_map_npc *map_npc;

	// HGE
	HGE *hge;

	//各种纹理
	HTEXTURE ht_map,ht_icon,ht_icon2,ht_hero,ht_monster,ht_skin,ht_npc,ht_special;

	//各种音效
	HEFFECT he_GetItem,he_OpenDoor,he_Attack, he_Music;
	HCHANNEL hc_Music;

	//各种图块
	hgeSprite *s_ground,*s_wall,*s_wall2,*s_wall3,*s_wall_hidden,*s_water,*s_sky,*s_lightning,*s_barrier,*s_bg,*s_bg_font;
	hgeSprite *s_redjewel,*s_bluejewel,*s_greenjewel,*s_redkey,*s_bluekey,*s_yellowkey,*s_allkey,*s_critical;
	hgeSprite *s_redpotion,*s_bluepotion,*s_yellowpotion, *s_greenpotion, *s_coin,*s_fly,*s_cross,*s_floor,*s_stick,*s_drink,*s_wand;
	hgeSprite *s_sword1,*s_shield1,*s_sword2,*s_shield2,*s_sword3,*s_shield3;
	hgeSprite *s_enemyinfo, *s_level, *s_storey, *s_heart, *s_expr, *s_damage, *s_time, *s_step;
	hgeSprite *s_reddoor,*s_bluedoor,*s_yellowdoor,*s_upstair,*s_downstair,*s_specialdoor;
	hgeSprite *s_shopleft,*s_shopright;

	hgeSprite *s_portal, *s_sigh, *s_sighed, *s_up, *s_left, *s_right, *s_down, *s_flower, *s_box, *s_boxed;
	hgeSprite *s_atk, *s_def, *s_life;



	// 文字
	hgeFont *hgef;

	// 其他常量
	float time_move,time_open,time_animation,time_floor;

};