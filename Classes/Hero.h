#pragma once

#include "cocos2d.h"  
#include "WorldTag.h"

#define ANIMATES_TAG_BASE 42
#define ACTIONS 4
#define ACTION_1_FRAMES 5
#define ACTION_2_FRAMES 3
#define ACTION_3_FRAMES 7
#define ACTION_4_FRAMES 3
#define ACTION_SPRITES ACTION_1_FRAMES+ACTION_2_FRAMES+ACTION_3_FRAMES+ACTION_4_FRAMES

USING_NS_CC;

enum State
{
	STATE_HERO_RUN,
	STATE_HERO_RUN_UP,
	STATE_HERO_JUMP,
	STATE_HERO_FALL,
	STATE_HERO_DIE
};



class Hero : public Node
{
private:
	State _state;
	Sprite *coreSprite;
	char actionSprites[ACTION_SPRITES][20];
	Size visibleSize;
	PhysicsBody *boundingSize[3];
	Animation* actions[ACTIONS];
	Animate *animates[ACTIONS];
	ActionInterval* _adjust;
	bool _isAdjusting;
	float floorHeight;
	int bullets;
public:
	//创建create方法，自动调用init和返回Hero类  
	virtual bool init();
	CREATE_FUNC(Hero);
	void setState(State state);
	State getState()
	{
		return _state;
	}
	void stopCurrentAction()
	{
		coreSprite->stopAllActions();
	}
	void over();
	void jump();
	void transferUp(Size visibleSize, float floorHeight);
	void transferDown(Size visibleSize, float floorHeight);
	void fall();
	void hit();
	bool adjust();
	void addBullet();
	int getBulletCounts();
};