#include "Block.h"

Block* Block::create(WorldTag tag)
{
	auto block = new Block();
	if(block && block->init(tag))
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

bool Block::init(WorldTag tag)
{
	Sprite::init();

	int ifEmojiKind;
	char ifEmojiName[20];
	Sprite* _sprite;
	Animate* bullet_ani;
	Animation* bullet_animation;
	char bullet_name[4][20];

	switch(tag)
	{
	case TAG_FLOOR_UP:
		_floorWidth = CCRANDOM_0_1() * 30 + 200;
		setTextureRect(Rect(0, 0, _floorWidth, 20));
		setColor(Color3B::RED);
		setVisible(false);
		setPhysicsBody(PhysicsBody::createBox(getContentSize()));
		break;
	case TAG_EMOJI:
		ifEmojiKind = (int)(CCRANDOM_0_1() * EMOJI_KINDS);
			switch (ifEmojiKind)
			{
			case 0:
				sprintf(ifEmojiName,"MAIN/huaji.png");
				break;
			case 1:
				sprintf(ifEmojiName, "MAIN/happy.png");
				break;
			case 2:
				sprintf(ifEmojiName, "MAIN/hehe.png");
				break;
			case 3:
				sprintf(ifEmojiName, "MAIN/scary.png");
				break;
			case 4:
				sprintf(ifEmojiName, "MAIN/yinxian.png");
				break;
			default:
				return false;
			}
			_sprite = Sprite::create(ifEmojiName);
			frame = SpriteFrame::create(ifEmojiName,Rect(0,0,_sprite->getContentSize().width, _sprite->getContentSize().height));
			setContentSize(_sprite->getContentSize());
			setPhysicsBody(PhysicsBody::createCircle(getContentSize().height / 2));
			setSpriteFrame(frame);
		break;
	case TAG_NAIL_TOP:
		_sprite = Sprite::create("Run/nailTop.png");
		frame = SpriteFrame::create("Run/nailTop.png", Rect(0, 0, _sprite->getContentSize().width, _sprite->getContentSize().height));
		setContentSize(_sprite->getContentSize());
		setPhysicsBody(PhysicsBody::createBox(getContentSize()));
		setSpriteFrame(frame);
		break;
	case TAG_NAIL_BOTTOM:
		_sprite = Sprite::create("Run/nailBottom.png");
		frame = SpriteFrame::create("Run/nailBottom.png", Rect(0, 0, _sprite->getContentSize().width, _sprite->getContentSize().height));
		setContentSize(_sprite->getContentSize());
		setPhysicsBody(PhysicsBody::createBox(getContentSize()));
		setSpriteFrame(frame);
		break;
	case TAG_WALL:
		setTextureRect(Rect(0, 0, 30, 9 * Director::getInstance()->getVisibleSize().height / 20));
		setColor(Color3B::WHITE);
		setPhysicsBody(PhysicsBody::createBox(getContentSize()));
		break;
	case TAG_BULLET:
		bullet_animation = Animation::create();
		bullet_animation->setDelayPerUnit(0.15f);
		//初始化图片文件名字符串
		for (int i = 0; i < 4; i++)
		{
			sprintf(bullet_name[i], "Run/AWepon[%d].png", i + 6);
			bullet_animation->addSpriteFrameWithFileName(bullet_name[i]);
		}	
		bullet_ani = Animate::create(bullet_animation);
		_sprite = Sprite::create(bullet_name[0]);
		_sprite->runAction(RepeatForever::create(bullet_ani));
		addChild(_sprite);
		setPhysicsBody(PhysicsBody::createCircle(_sprite->getContentSize().width / 2));
		setContentSize(_sprite->getContentSize());
		break;
	default:
		break;
	}
	
	setTag(tag);
	getPhysicsBody()->setDynamic(false);
	getPhysicsBody()->setContactTestBitmask(1);

	NOTIFICATION->addObserver(this, callfuncO_selector(Block::handleHeroOnDead), "dead", NULL);

	scheduleUpdate();

	return true;
}

void Block::update(float dt)
{
	setPosition(getPositionX()-10,getPositionY());
	if (getPositionX() < -getContentSize().width / 2)
	{
		NOTIFICATION->removeAllObservers(this);
		unscheduleUpdate();
		removeFromParent();
	}
}

void Block::handleHeroOnDead(CCObject * obj)
{
	getPhysicsBody()->setContactTestBitmask(0);
	NOTIFICATION->removeAllObservers(this);
	pause();
}

float Block::getFloorWidth()
{
	return _floorWidth;
}

void Block::cleanConllition()
{
	NOTIFICATION->removeAllObservers(this);
	unscheduleUpdate();
	removeFromParent();
}
