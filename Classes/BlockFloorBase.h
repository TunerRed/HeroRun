#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BlockFloorBase :public Sprite
{
public:
	static BlockFloorBase* create( float _floorWidth);
	virtual bool init(float _floorWidth);
	virtual void update(float dt);
	void handleHeroOnDead(CCObject* obj);
};

