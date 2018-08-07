#include "HelpScene.h"


#include "HelloWorldScene.h"

Scene* HelpScene::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}

bool HelpScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	char help_name[15];

	for (int i = 0; i < SPRITE_SIZE; i++)
	{
		sprintf(help_name,"Help/help%d.png",i);
		help[i] = Sprite::create(help_name);
		help[i]->setAnchorPoint(Point(0,0));
		help[i]->setVisible(false);
		addChild(help[i]);
	}
	help[0]->setVisible(true);
	current_sprite = 0;

	left = LabelTTF::create("<<", "Arial", 60);
	left->setName("<<");
	left->setAnchorPoint(Point(0, 0.5f));
	left->setPosition(visibleSize.width / 30,visibleSize.height / 2);
	left->setVisible(false);
	addChild(left);
	right = LabelTTF::create(">>", "Arial", 60);
	right->setName(">>");
	right->setAnchorPoint(Point(1, 0.5f));
	right->setPosition(29 * visibleSize.width / 30, visibleSize.height / 2);
	addChild(right);
	quit = LabelTTF::create("X", "Arial", 50);
	quit->setName("X");
	quit->setAnchorPoint(Point(1, 1));
	quit->setPosition(29 * visibleSize.width / 30, 29 * visibleSize.height / 30);
	addChild(quit);

	auto helpListener = EventListenerTouchOneByOne::create();
	helpListener->onTouchBegan = [this](Touch *t,Event *e)
	{
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			if (e->getCurrentTarget()->getName() == "<<")
			{
				if (current_sprite == 1)
					left->setVisible(false);
				else if (current_sprite == SPRITE_SIZE - 1)
					right->setVisible(true);
				if (current_sprite != 0)
				{
					help[current_sprite]->setVisible(false);
					help[--current_sprite]->setVisible(true);
				}
			}
			else if (e->getCurrentTarget()->getName() == ">>")
			{
				if (current_sprite == SPRITE_SIZE - 2)
					right->setVisible(false);
				else if (current_sprite == 0)
					left->setVisible(true);
				if (current_sprite != SPRITE_SIZE - 1)
				{
					help[current_sprite]->setVisible(false);
					help[++current_sprite]->setVisible(true);
				}
			}
			else if (e->getCurrentTarget()->getName() == "X")
			{
				Director::getInstance()->replaceScene(TransitionFlipY::create(0.5f, HelloWorld::createScene()));
			}
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(helpListener->clone(), left);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(helpListener->clone(), right);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(helpListener->clone(), quit);

	return true;
}
