#pragma once
#include "cocos2d.h"
#include "WorldTag.h"

#define EMOJI_KINDS 5
USING_NS_CC;
class Block :public Sprite
{
private:
	int width, heigth;
	float _floorWidth;
	SpriteFrame* frame;
public:
	static Block* create(WorldTag tag);
	virtual bool init(WorldTag tag);
	virtual void update(float dt);
	void handleHeroOnDead(CCObject* obj);
	float getFloorWidth();
	void cleanConllition();
};

