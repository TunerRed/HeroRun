#include "GameLayer.h"
#include "GameOver.h"
#include "HeroBullet.h"

#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;

GameLayer::~GameLayer()
{
	CC_SAFE_DELETE(_fsmHero);
}

Scene* GameLayer::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -900));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = GameLayer::create();
	scene->addChild(layer);

	return scene;
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	isVoiceOn = true;
	_score = 0;
	framesLimit = 0;
	
	auto background = Sprite::create("MAIN/background.png");
	background->setPosition(visibleSize / 2);
	addChild(background);

	labelScore = LabelAtlas::create(".","Run/numbers.png",25,28,'.');
	labelScore->setPosition(0, 9 * visibleSize.height / 10);
	labelScore->setScale(2.0f);
	labelScore->setString("0");
	bulletSprite = Sprite::create("Run/AWepon[6].png");
	bullets = LabelTTF::create("0","Arial",40);
	bulletSprite->setAnchorPoint(Point(1,1));
	bullets->setAnchorPoint(Point(0, 1));
	bulletSprite->setPosition(visibleSize.width / 4, visibleSize.height);
	bullets->setPosition(visibleSize.width / 4, bulletSprite->getPositionY() - bulletSprite->getContentSize().height / 4);

	//批量渲染地板
	batchNode = SpriteBatchNode::create("Run/ground.png");
	grounds[0][0] = Sprite::create("Run/ground.png");

	//设置障碍物的八种y初始坐标
	block_height[0] = grounds[0][0]->getContentSize().height;
	block_height[1] = (visibleSize.height - 3*block_height[0]) / 4 + block_height[0];
	block_height[2] = (visibleSize.height - block_height[0]) / 2;
	block_height[3] = block_height[2] + block_height[0];
	block_height[4] = block_height[2] + block_height[1];
	block_height[5] = block_height[2] * 2;

	//设置障碍物第一次的出现时间
	for (int i = 0; i < 5; i++)
	{
	block_current_time[i] = 0;
	block_time_limit[i] = CCRANDOM_0_1()*150;
	}
	block_time_limit[0] += 100;
	block_time_limit[1] += 150;
	block_time_limit[2] += 100;
	block_time_limit[3] += 500;
	block_time_limit[4] += 200;

	//物理边框
	Node *edge1 = Node::create();
	edge1->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize/*, PHYSICSBODY_MATERIAL_DEFAULT, 3*/));
	edge1->setContentSize(visibleSize);
	edge1->setPosition(visibleSize.width / 2, visibleSize.height / 2 + grounds[0][0]->getContentSize().height);
	addChild(edge1);
	Node *edge2 = Node::create();
	edge2->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize/*, PHYSICSBODY_MATERIAL_DEFAULT, 3*/));
	edge2->setContentSize(visibleSize);
	edge2->setPosition(visibleSize.width / 2, visibleSize.height + grounds[0][0]->getContentSize().height / 2);
	addChild(edge2);


	//加入移动的地板
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < GROUND_COUNT; i++)
		{
			grounds[j][i] = Sprite::create("Run/ground.png");
			grounds[j][i]->setScaleX((visibleSize.width / (GROUND_COUNT - 1)) / grounds[j][i]->getContentSize().width);
			grounds[j][i]->setContentSize(Size(grounds[j][i]->getContentSize().width * grounds[j][i]->getScaleX(), grounds[j][i]->getContentSize().height));
			grounds[j][i]->setAnchorPoint(Point(0, 1));
			grounds[j][i]->setPosition(i * visibleSize.width / (GROUND_COUNT - 1), j * ((visibleSize.height - grounds[j][i]->getContentSize().height) / 2) + grounds[j][i]->getContentSize().height);
			batchNode->addChild(grounds[j][i]);
	}

	hero = Hero::create();
	hero->setScale(2);
	hero->setPosition(visibleSize / 4);
	addChild(hero);
	_fsmHero = FSMHero::create(hero);

	floorUp = Sprite::create();
	floorUp->setTextureRect(Rect(0,0,visibleSize.width,grounds[0][0]->getContentSize().height));
	floorUp->setVisible(false);
	floorUp->setTag(TAG_FLOOR);
	floorUp->setPhysicsBody(PhysicsBody::createBox(floorUp->getContentSize()));
	floorUp->getPhysicsBody()->setDynamic(false);
	floorUp->getPhysicsBody()->setContactTestBitmask(1);
	floorUp->setAnchorPoint(Point(0,0));
	floorUp->setPosition(0,0);
	floorUp->retain();
	floorBottom = Sprite::create();
	floorBottom->setTextureRect(Rect(0, 0, visibleSize.width, grounds[0][0]->getContentSize().height));
	floorBottom->setVisible(false);
	floorBottom->setTag(TAG_FLOOR);
	floorBottom->setPhysicsBody(PhysicsBody::createBox(floorBottom->getContentSize()));
	floorBottom->getPhysicsBody()->setDynamic(false);
	floorBottom->getPhysicsBody()->setContactTestBitmask(1);
	floorBottom->setAnchorPoint(Point(0, 0.5f));
	floorBottom->setPosition(0, visibleSize.height / 2);
	floorBottom->retain();

	addChild(bullets);
	addChild(bulletSprite);

	addChild(batchNode);
	_pause = Sprite::create("MAIN/pause.png");
	_pause->setPosition(visibleSize / 2);
	_pause->retain();
	pauseBtn = Sprite::create("MAIN/pauseBtn.png");
	pauseBtn->setAnchorPoint(Point(0,0));
	pauseBtn->setPosition(9 * visibleSize.width / 10, 17 * visibleSize.height / 20);
	pauseBtn->setTag(BUTTON_PAUSE);
	addChild(pauseBtn);
	voiceBtn = Sprite::create("MAIN/voiceOn.png");
	voiceBtn->setPosition(9 * visibleSize.width / 10, 7 * visibleSize.height / 10);
	voiceBtn->setAnchorPoint(Point(0, 0));
	voiceBtn->setTag(BUTTON_MUSIC);
	addChild(voiceBtn);
	voiceBtnRect = Rect(0, 0, voiceBtn->getContentSize().width, voiceBtn->getContentSize().width);
	voiceBtnOn = SpriteFrame::create("MAIN/voiceOn.png",voiceBtnRect);
	voiceBtnOff = SpriteFrame::create("MAIN/voiceOff.png", voiceBtnRect);
	voiceBtnOn->retain();
	voiceBtnOff->retain();

	eventdispatch();
	handleHero();
	scheduleUpdate();
	addChild(labelScore);
	return true;
}

void GameLayer::gamePause()
{
	//暂停画面以及对应的处理
	//cocos2d-x导演暂停期间物理世界物体仍然缓慢受重力影响
	hero->getPhysicsBody()->setEnabled(false);
	Director::getInstance()->pause();
	addChild(_pause);
}

void GameLayer::gameUnpause()
{
	//暂停画面以及对应的处理
	Director::getInstance()->resume();
	hero->getPhysicsBody()->setEnabled(true);
	removeChild(_pause);
}

void GameLayer::handleHero()
{
	NOTIFICATION->addObserver(this, callfuncO_selector(GameLayer::handleHeroOnLand), "land", NULL);
	NOTIFICATION->addObserver(this, callfuncO_selector(GameLayer::handleHeroOnDead), "dead", NULL);
	NOTIFICATION->addObserver(this, callfuncO_selector(GameLayer::handleHeroOnLandUp), "landUp", NULL);
	NOTIFICATION->addObserver(this, callfuncO_selector(GameLayer::handleHeroOnBullet), "updatebullet", NULL);
	NOTIFICATION->addObserver(this, callfuncO_selector(GameLayer::handleHeroOnHitBullet), "hitbullet", NULL);
}

void GameLayer::handleHeroOnLand(CCObject *obj)
{
	hero->getPhysicsBody()->setVelocity(Vec2(0,0));
	hero->setState(STATE_HERO_RUN);
	if (getChildByTag(TAG_FLOOR) != NULL)
	{
		removeChild(floorUp);
		removeChild(floorBottom);
	}
}

void GameLayer::handleHeroOnLandUp(CCObject *obj)
{
	hero->setState(STATE_HERO_RUN_UP);
}

void GameLayer::handleHeroOnBullet(CCObject * obj)
{
	bullets->setString(Value(hero->getBulletCounts()).asString());
}

void GameLayer::handleHeroOnHitBullet(CCObject * obj)
{
	//发出一颗子弹
	auto _bullet = HeroBullet::create();
	_bullet->setPosition(8 * hero->getPositionX() / 5, hero->getPositionY());
	addChild(_bullet);
}

void GameLayer::handleHeroOnDead(CCObject *obj)
{
	unscheduleUpdate();
	NOTIFICATION->removeAllObservers(this);
	hero->over();
	runAction(Sequence::create
		(
		DelayTime::create(2.0f),
		CallFunc::create([this](){Director::getInstance()->replaceScene(GameOver::createScene(getScore())); }),
		NULL
		));
}

void GameLayer::eventdispatch()
{
	auto btnListener = EventListenerTouchOneByOne::create();
	btnListener->onTouchBegan = [this](Touch *t, Event *e){return true; };
	btnListener->onTouchEnded = [this](Touch *t, Event *e)
	{
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			switch (e->getCurrentTarget()->getTag())
			{
			case BUTTON_PAUSE:
				if (Director::getInstance()->isPaused())
					gameUnpause();
				else
					gamePause();
				break;
			case BUTTON_MUSIC:
				if (isVoiceOn)
				{
					SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
					SimpleAudioEngine::getInstance()->setEffectsVolume(0);
					voiceBtn->setSpriteFrame(voiceBtnOff);
					isVoiceOn = false;
				}
				else
				{
					SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
					SimpleAudioEngine::getInstance()->setEffectsVolume(1);
					voiceBtn->setSpriteFrame(voiceBtnOn);
					isVoiceOn = true;
				}
				break;
			default:
				break;
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(btnListener, pauseBtn);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(btnListener->clone(), voiceBtn);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [this](Touch *t, Event *e){if (Director::getInstance()->isPaused())return false; return true; };
	touchListener->onTouchEnded = [this](Touch *t, Event *e)
	{
		Point start = t->getStartLocation();
		Point end = t->getLocation();
		if (abs(end.x - start.x) > abs(end.y - start.y))
		{
			if (end.x - start.x > TOUCH_DISTANCE)
			{
				_fsmHero->handleMessage(GESTURE_RIGHT);
			}
			else if (end.x - start.x < -TOUCH_DISTANCE)
			{
				_fsmHero->handleMessage(GESTURE_LEFT);
			}
			else if (end.x < 8 * visibleSize.width / 10 || end.y < 7 * visibleSize.height / 10)
			{
				if (_fsmHero->handleMessage(GESTURE_TOUCH))
				{
					addChild(floorUp);
					addChild(floorBottom);
				}
			}
		}
		else
		{
			if (end.y - start.y > TOUCH_DISTANCE)
			{
				float _py = hero->getPositionY();
				_fsmHero->handleMessage(GESTURE_UP);
			}
			else if (end.y - start.y < -TOUCH_DISTANCE)
			{
				_fsmHero->handleMessage(GESTURE_DOWN);
			}
			else if (end.x < 8 * visibleSize.width / 10 || end.y < 7 * visibleSize.height / 10)
			{
				if(_fsmHero->handleMessage(GESTURE_TOUCH))
				{
					addChild(floorUp);
					addChild(floorBottom);
				}
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

}

void GameLayer::blockdispatch()
{
	/*
	0-TAG_FLOOR_UP
	1-TAG_WALL
	2-TAG_EMOJI
	3-TAG_NAIL_TOP
	4-TAG_NAIL_BOTTOM
	5-TAG_BULLET
	*/


	for (int i = 0; i < 6; i++)
		block_current_time[i]++;
	int block_positionY = 0;
	if (block_current_time[0] >= block_time_limit[0])
	{
		block_positionY = CCRANDOM_0_1() * 2;
		auto block = Block::create(TAG_FLOOR_UP);
		auto block_base = BlockFloorBase::create(block->getFloorWidth());
		block->setPosition(visibleSize.width + block->getContentSize().width,block_height[1 + block_positionY*3]);
		block_base->setPosition(block->getPositionX(), 
			block->getPositionY() - block_base->getContentSize().height / 2 - block->getContentSize().height / 2);
		addChild(block);
		addChild(block_base);
		block_current_time[0] = 0;
		block_time_limit[0] = CCRANDOM_0_1() * 100 + 200;
	}
	if (block_current_time[1] >= block_time_limit[1])
	{
		block_positionY = CCRANDOM_0_1() * 2;
		auto block = Block::create(TAG_WALL);
		block->setPosition(visibleSize.width + block->getContentSize().width, block_height[1 + block_positionY * 3]);
		addChild(block);
		block_current_time[1] = 0;
		block_time_limit[1] = CCRANDOM_0_1() * 150 + 100;
	}
	if (block_current_time[2] >= block_time_limit[2])
	{
		block_positionY = CCRANDOM_0_1() * 4;
		auto block = Block::create(TAG_EMOJI);
		block->setPosition(visibleSize.width + block->getContentSize().width, block_height[block_positionY +  block_positionY / 2] + (1 - block_positionY % 2) * block->getContentSize().height / 2);
		addChild(block);
		block_current_time[2] = 0;
		block_time_limit[2] = CCRANDOM_0_1() * 100 + 100;
	}
	if (block_current_time[3] >= block_time_limit[3])
	{
		block_positionY = CCRANDOM_0_1() * 2;
		auto block = Block::create(TAG_NAIL_TOP);
		block->setPosition(visibleSize.width + block->getContentSize().width, block_height[2 + block_positionY * 3] - block->getContentSize().height / 2);
		addChild(block);
		block_current_time[3] = 0;
		block_time_limit[3] = CCRANDOM_0_1() * 150 + 200;
	}
	if (block_current_time[4] >= block_time_limit[4])
	{
		block_positionY = CCRANDOM_0_1() * 2;
		auto block = Block::create(TAG_NAIL_BOTTOM);
		block->setPosition(visibleSize.width + block->getContentSize().width, block_height[0 + block_positionY * 3] + block->getContentSize().height / 2);
		addChild(block);
		block_current_time[4] = 0;
		block_time_limit[4] = CCRANDOM_0_1() * 150 + 200;
	}
	if (block_current_time[5] >= block_time_limit[5])
	{
		block_positionY = CCRANDOM_0_1() * 2;
		auto block = Block::create(TAG_BULLET);
		block->setPosition(visibleSize.width + block->getContentSize().width, block_height[0 + block_positionY * 3] + block->getContentSize().height);
		addChild(block);
		block_current_time[5] = 0;
		block_time_limit[5] = CCRANDOM_0_1() * 300 + 600;
	}

}

void GameLayer::update(float dt)
{
	framesLimit++;
	if (framesLimit == 3)
	{
		framesLimit = 0;
		_score++;
		labelScore->setString(Value(_score).asString());
	}

	for (int j = 0; j < 3; j++)
		for (int i = 0; i < GROUND_COUNT; i++)
		{
			grounds[j][i]->setPositionX(grounds[j][i]->getPositionX()-10);
			if (grounds[j][i]->getPositionX() <= -grounds[j][i]->getContentSize().width)
				grounds[j][i]->setPosition(Point(Director::getInstance()->getVisibleSize().width, grounds[j][i]->getPositionY()));
		}

	blockdispatch();
}
