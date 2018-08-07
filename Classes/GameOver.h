#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;

USING_NS_CC;

class GameOver:public LayerColor
{
public:
	static Scene* createScene(int score);
	virtual bool init(int score);
	static GameOver* create(int score);
	//void eventdispatch();
};

