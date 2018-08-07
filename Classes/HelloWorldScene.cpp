#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "GameLayer.h"
#include "HelpScene.h"

#define BUTTON_START 10
#define BUTTON_HELP 11
#define BUTTON_QUIT 12
#define BUTTON_DONATE 13

using namespace cocostudio::timeline;


Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	for (int i = 0; i < 5; i++)
		action[i] = RotateBy::create(2, 360);

    auto rootNode = CSLoader::createNode("title.csb");

	auto huaji = rootNode->getChildByName("huaji");
	if (huaji == nullptr)
		return false;
	huaji->setTag(0);
	action[0] = action[0]->reverse();
	huaji->runAction(RepeatForever::create(action[0]));

	auto yinxian = rootNode->getChildByName("yinxian");
	if (yinxian == nullptr)
		return false;
	yinxian->setTag(1);
	yinxian->runAction(RepeatForever::create(action[1]));

	auto scary = rootNode->getChildByName("scary");
	if (scary == nullptr)
		return false;
	scary->setTag(2);
	action[2] = action[2]->reverse();
	scary->runAction(RepeatForever::create(action[2]));

	auto hehe = rootNode->getChildByName("hehe");
	if (hehe == nullptr)
		return false;
	hehe->setTag(3);
	hehe->runAction(RepeatForever::create(action[3]));
	
	auto happy = rootNode->getChildByName("happy");
	if (happy == nullptr)
		return false;
	happy->setTag(4);
	action[4] = action[4]->reverse();
	happy->runAction(RepeatForever::create(action[4]));
	
	auto start = rootNode->getChildByName("start");
	if (start == nullptr)
		return false;
	start->setTag(BUTTON_START);
	auto help = rootNode->getChildByName("help");
	if (help == nullptr)
		return false;
	help->setTag(BUTTON_HELP);
	auto quit = rootNode->getChildByName("quit");
	if (quit == nullptr)
		return false;
	quit->setTag(BUTTON_QUIT);
	auto donate = rootNode->getChildByName("donate");
	if (donate == nullptr)
		return false;
	donate->setTag(BUTTON_DONATE);

    addChild(rootNode);

	auto titleHuajiTouch = EventListenerTouchOneByOne::create();
	titleHuajiTouch->onTouchBegan = [this](Touch* t, Event* e)
	{
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			int cur = e->getCurrentTarget()->getTag();
			e->getCurrentTarget()->stopAction(action[cur]);
			action[cur] = action[cur]->reverse();
			e->getCurrentTarget()->runAction(RepeatForever::create(action[cur]));
		}

		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleHuajiTouch->clone(), huaji);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleHuajiTouch->clone(), hehe);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleHuajiTouch->clone(), yinxian);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleHuajiTouch->clone(), scary);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleHuajiTouch->clone(), happy);

	auto titleOptionsTouch = EventListenerTouchOneByOne::create();
	titleOptionsTouch->onTouchBegan = [this](Touch* t, Event* e)
	{
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			auto end_huaji = Sprite::create("MAIN/huaji.png");
			auto end_back = Sprite::create("MAIN/background.png");
			switch (e->getCurrentTarget()->getTag())
			{
			case BUTTON_START:
				Director::getInstance()->replaceScene(GameLayer::createScene());
				break;
			case BUTTON_HELP:
				Director::getInstance()->replaceScene(TransitionMoveInB::create(0.5f,HelpScene::createScene()));
				break;
			case BUTTON_QUIT:
				end_huaji->setPosition(Director::getInstance()->getVisibleSize() / 2);
				end_huaji->setOpacity(0);
				end_huaji->setScale(3.0f);
				end_back->setPosition(Director::getInstance()->getVisibleSize() / 2);
				end_back->setOpacity(0);
				addChild(end_back);
				addChild(end_huaji);

				runAction(Sequence::create
					(
					Spawn::create
					(
					TargetedAction::create(end_huaji,FadeIn::create(2)),
					TargetedAction::create(end_back, FadeIn::create(2)),
					NULL
					),
					CCCallFunc::create([](){Director::getInstance()->end(); }),
					NULL
					));
				break;
			case BUTTON_DONATE:
				MessageBox("Success", "donate");
				break;
			default:
				break;
			}
		}

		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleOptionsTouch->clone(), start);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleOptionsTouch->clone(), quit);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleOptionsTouch->clone(), help);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(titleOptionsTouch->clone(), donate);

    return true;
}