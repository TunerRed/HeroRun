#pragma once

#include "cocos2d.h"
#define NOTIFICATION cocos2d::NotificationCenter::getInstance()

enum WorldTag
{
	TAG_HERO,
	TAG_FLOOR,
	TAG_FLOOR_UP,
	TAG_EMOJI,
	TAG_NAIL_TOP,
	TAG_NAIL_BOTTOM,
	TAG_WALL,
	TAG_BULLET,
	TAG_HERO_BULLET
};

enum Gesture
{
	GESTURE_UP,
	GESTURE_DOWN,
	GESTURE_LEFT,
	GESTURE_RIGHT,
	GESTURE_TOUCH
};