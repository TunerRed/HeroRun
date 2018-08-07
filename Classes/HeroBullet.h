#pragma once

#include "cocos2d.h"  
#include "WorldTag.h"
USING_NS_CC;
class HeroBullet :public Sprite
{
public:
	CREATE_FUNC(HeroBullet);
	virtual bool init();
	virtual void update(float dt);
	void handleHeroOnDead(CCObject * obj);
	void cleanConllition();
};

