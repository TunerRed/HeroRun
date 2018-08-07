#pragma once

#define SPRITE_SIZE 7
#include "cocos2d.h"
USING_NS_CC;

class HelpScene : public cocos2d::Layer
{
public:
	int current_sprite;
	LabelTTF *left, *right,*quit;
	Sprite* help[SPRITE_SIZE];
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelpScene);
};

