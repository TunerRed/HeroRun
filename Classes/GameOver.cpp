#include "GameOver.h"
#include "HelloWorldScene.h"
#include "GameLayer.h"

Scene* GameOver::createScene(int score)
{
	auto scene = Scene::create();
	auto layer = GameOver::create(score);
	scene->addChild(layer);
	return scene;
}

GameOver* GameOver::create(int score)
{
	auto over = new GameOver();
	if (over && over->init(score))
	{
		return over;
	}
	else
	{
		delete over;
		return nullptr;
	}
}

bool GameOver::init(int score)
{
	if (!LayerColor::initWithColor(Color4B(182,231,118,-30)))
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto rootNode = CSLoader::createNode("gameover.csb");
	addChild(rootNode);
	auto newscoreParticle = rootNode->getChildByName("newScore");
	auto levelRetry = rootNode->getChildByName("level_retry");
	auto levelMain = rootNode->getChildByName("level_main");
	auto newscore = (cocos2d::ui::TextAtlas*)rootNode->getChildByName("score");
	auto bestscore = (cocos2d::ui::TextAtlas*)rootNode->getChildByName("best_score");
	auto level_huaji = rootNode->getChildByName("level_huaji");
	auto level_yinxian = rootNode->getChildByName("level_yinxian");
	auto level_happy = rootNode->getChildByName("level_happy");

	newscoreParticle->setVisible(false);
	level_happy->setVisible(false);
	level_huaji->setVisible(false);
	level_yinxian->setVisible(false);

	auto overListener = EventListenerTouchOneByOne::create();
	overListener->onTouchBegan = [this](Touch *t, Event *e)
	{
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			auto name = e->getCurrentTarget()->getName();
			if (name == "level_retry")
			{
				Director::getInstance()->replaceScene(GameLayer::createScene());
			}
			else if (name == "level_main")
			{
				Director::getInstance()->replaceScene(HelloWorld::createScene());
			}
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(overListener, levelRetry);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(overListener->clone(), levelMain);

	int level = 0;
	int hightest = UserDefault::getInstance()->getIntegerForKey("hightest_score",0);

	log("%s", Value(hightest).asString().c_str());
	newscore->setString(Value(score).asString());
	bestscore->setString(Value(hightest).asString());

	if (hightest == 0)
	{
		level = 1;
	}
	else
	{
		if (score > hightest)
		{
			level = 1;
		}
		else if (score < hightest / 5)
		{
			level = -1;
		}
		else
		{
			level = 0;
		}
	}

	switch (level)
	{
	case 1:
		level_happy->setVisible(true);
		newscoreParticle->setVisible(true);
		UserDefault::getInstance()->setIntegerForKey("hightest_score", score);
		break;
	case 0:
		level_yinxian->setVisible(true);
		break;
	case -1:
		level_huaji->setVisible(true);
		break;
	default:
		break;
	}

	return true;
}
