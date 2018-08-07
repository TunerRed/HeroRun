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

	//����
	for (int i = 0; i < ACTIONS; i++)
	{
		actions[i] = Animation::create();
		actions[i]->setDelayPerUnit(0.1f);
	}
	actions[2]->setDelayPerUnit(0.15f);

	//���ֲ�ͬ״̬�µ���ײ������
	boundingSize[0] = PhysicsBody::createBox(Sprite::create("Run/APimg[0].png")->getContentSize());
	boundingSize[1] = PhysicsBody::createBox(Sprite::create("Run/APimg[7].png")->getContentSize());
	boundingSize[2] = PhysicsBody::createBox(Sprite::create("Run/APimg[17].png")->getContentSize());
	for (int i = 0; i < 3; i++)
	{
		boundingSize[i]->setRotationEnable(false);
		boundingSize[i]->setContactTestBitmask(1);
		boundingSize[i]->retain();
	}
	

	//��ʼ��ͼƬ�ļ����ַ���
	for (int i = 0; i < ACTION_SPRITES; i++)
		sprintf(actionSprites[i],"Run/APimg[%d].png",i);
	//����ͼƬ�����ļ�
	for (int i = 0; i < ACTION_1_FRAMES; i++)
		actions[0]->addSpriteFrameWithFileName(actionSprites[i]);
	for (int i = 0; i < ACTION_2_FRAMES; i++)
		actions[1]->addSpriteFrameWithFileName(actionSprites[ACTION_1_FRAMES + i]);
	for (int i = 0; i < ACTION_3_FRAMES; i++)
		actions[2]->addSpriteFrameWithFileName(actionSprites[ACTION_1_FRAMES + ACTION_2_FRAMES + i]);
	for (int i = 0; i < ACTION_4_FRAMES; i++)
		actions[3]->addSpriteFrameWithFileName(actionSprites[ACTION_1_FRAMES + ACTION_2_FRAMES + ACTION_3_FRAMES + i]);
	/*
	������������
	animate[0] ����
	animate[1] �»�
	animate[2] ��Ծ
	animate[3] ��������
	*/
	for (int i = 0; i < ACTIONS; i++)
	{
		animates[i] = Animate::create(actions[i]);
		animates[i]->setTag(i+ANIMATES_TAG_BASE);
		animates[i]->retain();
	}
	
	//������������
	setPhysicsBody(boundingSize[0]);

	//���ľ��飬������ʾ����Ļ�ϣ����ʵ��л�����
	coreSprite = Sprite::create("Run/APimg[0].png");
	addChild(coreSprite);
	coreSprite->runAction(RepeatForever::create(animates[0]));

	//����������ʼ��ֵ
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
