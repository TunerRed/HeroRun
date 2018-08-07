#include "BlockFloorBase.h"
#include "WorldTag.h"

BlockFloorBase * BlockFloorBase::create(float _floorWidth)
{
	auto block = new BlockFloorBase();
	if (block && block->init(_floorWidth))
	{
		block->autorelease();
		return block;
	}
	else
	{
		delete block;
		return nullptr;
	}
}

bool BlockFloorBase::init(float _floorWidth)
{
	Sprite::init();

	NOTIFICATION->addObserver(this, callfuncO_selector(BlockFloorBase::handleHeroOnDead), "dead", NULL);

	setTag(TAG_FLOOR_UP);
	setTextureRect(Rect(0, 0, _floorWidth, 30));
	setPhysicsBody(PhysicsBody::createBox(getContentSize()));
	getPhysicsBody()->setDynamic(false);
	setColor(Color3B::WHITE);
	getPhysicsBody()->setContactTestBitmask(0);

	scheduleUpdate();

	return true;
}

void BlockFloorBase::update(float dt)
{
	setPosition(getPositionX() - 10, getPositionY());
	if (getPositionX() < -getContentSize().width / 2)
	{
		NOTIFICATION->removeAllObservers(this);
		unscheduleUpdate();
		removeFromParent();
	}
}

void BlockFloorBase::handleHeroOnDead(CCObject * obj)
{
	NOTIFICATION->removeAllObservers(this);
	pause();
}


