#pragma once

#include "cocos2d.h"
#include "WorldTag.h"
#include "Hero.h"

USING_NS_CC;

class FSMHero:public Node
{
private:
	Hero* hero;
	Size visibleSize;
	float floorHeight;
public:
	static FSMHero* create(Hero* hero);
	bool init(Hero* hero);
	float getFloorHeight()
	{
		return floorHeight;
	}
	bool handleMessage(Gesture gesture);
	bool handleUp();
	bool handleDown();
	bool handleLeft();
	bool handleRight();
	bool handleTouch();
};

