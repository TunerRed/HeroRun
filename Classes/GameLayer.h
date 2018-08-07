#pragma once

#define TOUCH_DISTANCE 70
#define NOTIFICATION NotificationCenter::getInstance()
#define GROUND_COUNT 5
#define SPEED 2.0f
#define BUTTON_PAUSE 100
#define BUTTON_MUSIC 101

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "WorldTag.h"
#include "Hero.h"
#include "FSMHero.h"

#include "Block.h"
#include "BlockFloorBase.h"

USING_NS_CC;
using namespace CocosDenshion;

class GameLayer : public cocos2d::Layer
{
public:
	~GameLayer();
	int block_current_time[6], block_time_limit[6];
	float block_height[6];
	std::vector<Block*> blockList[4];
	LabelAtlas* labelScore;
	LabelTTF* bullets;
	int _score;
	int framesLimit;
	FSMHero* _fsmHero;
	bool isVoiceOn;
	Hero* hero;
	SpriteBatchNode* batchNode;
	Sprite* grounds[3][GROUND_COUNT];
	Sprite *floorUp, *floorBottom, *bulletSprite;
	Sprite *_pause, *pauseBtn, *voiceBtn;
	SpriteFrame *voiceBtnOn, *voiceBtnOff;
	Rect voiceBtnRect;
	Size visibleSize;
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameLayer);
	void eventdispatch();
	void blockdispatch();
	void gamePause();
	void gameUnpause();
	void handleHero();
	void handleHeroOnLand(CCObject *obj);
	void handleHeroOnDead(CCObject *obj);
	void handleHeroOnLandUp(CCObject *obj);
	void handleHeroOnBullet(CCObject *obj);
	void handleHeroOnHitBullet(CCObject *obj);
	virtual void update(float dt);
	int getScore()
	{
		return _score;
	}
};

