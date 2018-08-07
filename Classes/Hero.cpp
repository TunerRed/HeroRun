#include "Hero.h"
#include "Block.h"
#include "HelloWorldScene.h"
#include "GameOver.h"


bool Hero::init()
{
	if (!Node::init())
		return false;;

	visibleSize = Director::getInstance()->getVisibleSize();
	floorHeight = Sprite::create("Run/ground.png")->getContentSize().height;
	bullets = 0;

	setTag(TAG_HERO);

	//动画
	for (int i = 0; i < ACTIONS; i++)
	{
		actions[i] = Animation::create();
		actions[i]->setDelayPerUnit(0.1f);
	}
	actions[2]->setDelayPerUnit(0.15f);

	//三种不同状态下的碰撞检测盒子
	boundingSize[0] = PhysicsBody::createBox(Sprite::create("Run/APimg[0].png")->getContentSize());
	boundingSize[1] = PhysicsBody::createBox(Sprite::create("Run/APimg[7].png")->getContentSize());
	boundingSize[2] = PhysicsBody::createBox(Sprite::create("Run/APimg[17].png")->getContentSize());
	for (int i = 0; i < 3; i++)
	{
		boundingSize[i]->setRotationEnable(false);
		boundingSize[i]->setContactTestBitmask(1);
		boundingSize[i]->retain();
	}
	

	//初始化图片文件名字符串
	for (int i = 0; i < ACTION_SPRITES; i++)
		sprintf(actionSprites[i],"Run/APimg[%d].png",i);
	//加载图片序列文件
	for (int i = 0; i < ACTION_1_FRAMES; i++)
		actions[0]->addSpriteFrameWithFileName(actionSprites[i]);
	for (int i = 0; i < ACTION_2_FRAMES; i++)
		actions[1]->addSpriteFrameWithFileName(actionSprites[ACTION_1_FRAMES + i]);
	for (int i = 0; i < ACTION_3_FRAMES; i++)
		actions[2]->addSpriteFrameWithFileName(actionSprites[ACTION_1_FRAMES + ACTION_2_FRAMES + i]);
	for (int i = 0; i < ACTION_4_FRAMES; i++)
		actions[3]->addSpriteFrameWithFileName(actionSprites[ACTION_1_FRAMES + ACTION_2_FRAMES + ACTION_3_FRAMES + i]);
	/*
	产生动画动作
	animate[0] 奔跑
	animate[1] 下滑
	animate[2] 跳跃
	animate[3] 倒地死亡
	*/
	for (int i = 0; i < ACTIONS; i++)
	{
		animates[i] = Animate::create(actions[i]);
		animates[i]->setTag(i+ANIMATES_TAG_BASE);
		animates[i]->retain();
	}
	
	//物理世界设置
	setPhysicsBody(boundingSize[0]);

	//核心精灵，用于显示在屏幕上，并适当切换动作
	coreSprite = Sprite::create("Run/APimg[0].png");
	addChild(coreSprite);
	coreSprite->runAction(RepeatForever::create(animates[0]));

	//设置其他初始化值
	_state = STATE_HERO_RUN;

	return true;
}

void Hero::setState(State state)
{	
	switch (_state)
	{
	case STATE_HERO_RUN:
	case STATE_HERO_RUN_UP:
		stopCurrentAction();
		removeComponent(boundingSize[0]);
		break;
	case STATE_HERO_JUMP:
		removeComponent(boundingSize[0]);
		break;
	case STATE_HERO_FALL:
		removeComponent(boundingSize[1]);
		break;
	case STATE_HERO_DIE:
		return;
	default:
		break;
	}

	_state = state;

	switch (_state)
	{
	case STATE_HERO_RUN:
	case STATE_HERO_RUN_UP:
		setPhysicsBody(boundingSize[0]);
		coreSprite->runAction(RepeatForever::create(animates[0]));
		break;
	case STATE_HERO_JUMP:
		setPhysicsBody(boundingSize[0]);
		break;
	case STATE_HERO_FALL:
		setPhysicsBody(boundingSize[1]);
		break;
	case STATE_HERO_DIE:
		setPhysicsBody(boundingSize[2]);
		break;
	default:
		break;
	}

}

void Hero::transferUp(Size _visibleSize, float _floorHeight)
{
	visibleSize = _visibleSize;
	floorHeight = _floorHeight;
	
	coreSprite->runAction(Sequence::create
		(
		Blink::create(0.2, 3),
		CallFunc::create([this](){setPositionY(getPositionY() + (visibleSize.height - 3 * floorHeight) / 2); }),
		Blink::create(0.3, 4),
		NULL
		));
}

void Hero::transferDown(Size _visibleSize, float _floorHeight)
{
	visibleSize = _visibleSize;
	floorHeight = _floorHeight;
	coreSprite->runAction(Sequence::create
		(
		Blink::create(0.2, 3),
		CallFunc::create([this](){setPositionY(getPositionY() - (visibleSize.height - 3 * floorHeight) / 2); }),
		Blink::create(0.3, 4),
		NULL
		));
}

void Hero::jump()
{
	getPhysicsBody()->setVelocity(Vec2(0, 600));
	coreSprite->runAction(animates[2]);
}

void Hero::fall()
{
	removeComponent(boundingSize[0]);
	setPhysicsBody(boundingSize[1]);
	coreSprite->runAction(Sequence::create
		(
		animates[1],
		DelayTime::create(1),
		CallFunc::create([this]()
	{
		setState(STATE_HERO_RUN);
	}),
		NULL
		));
}

bool Hero::adjust()
{
	return true;
}

void Hero::hit()
{
	if (bullets == 0)
		return;
	bullets--;
	NOTIFICATION->postNotification("hitbullet");
	NOTIFICATION->postNotification("updatebullet");
}

void Hero::addBullet()
{
	if(bullets < 3)
		bullets++;
}

int Hero::getBulletCounts()
{
	return bullets;
}

void Hero::over()
{
	stopCurrentAction();
	coreSprite->runAction(Sequence::create
		(
			animates[3],
			CallFunc::create([this]() 
	{
		setState(STATE_HERO_DIE);
	}),
			NULL
		));
}
