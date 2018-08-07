#include "FSMHero.h"
#include "Block.h"


FSMHero* FSMHero::create(Hero* hero)
{
	auto fsmhero = new FSMHero();
	if (fsmhero && fsmhero->init(hero))
	{
		return fsmhero;
	}
	delete fsmhero;
	return nullptr;
}

bool FSMHero::init(Hero* _hero)
{
	if (!Node::init())
		return false;

	hero = _hero;
	visibleSize = Director::getInstance()->getVisibleSize();
	floorHeight = Sprite::create("Run/ground.png")->getContentSize().height;


	//Åö×²¼ì²âÉèÖÃ
	auto listenerConllition = EventListenerPhysicsContact::create();
	listenerConllition->onContactBegin = [this](PhysicsContact& contact)
	{
		Node* nodeHero = NULL;
		Node* nodeOther = NULL;

		if (!contact.getShapeA()->getBody()->getNode() || !contact.getShapeB()->getBody()->getNode())
		{
			log("-hero error-");
			return false;
		}
			

		if (contact.getShapeA()->getBody()->getNode()->getTag() == TAG_HERO)
		{
			nodeHero = contact.getShapeA()->getBody()->getNode();
			nodeOther = contact.getShapeB()->getBody()->getNode();
		}
		else if (contact.getShapeB()->getBody()->getNode()->getTag() == TAG_HERO)
		{
			nodeHero = contact.getShapeB()->getBody()->getNode();
			nodeOther = contact.getShapeA()->getBody()->getNode();
		}
		else
		{
			return false;
		}

		if (((Hero*)nodeHero)->getState() == STATE_HERO_DIE)
			return false;

		int otherTag = nodeOther->getTag();
		switch (otherTag)
		{
		case TAG_FLOOR:
			NOTIFICATION->postNotification("land");
			break;
		case TAG_FLOOR_UP:
			((Block*)nodeOther)->cleanConllition();
			NOTIFICATION->postNotification("landUp");
			break;
		case TAG_NAIL_TOP:
		case TAG_NAIL_BOTTOM:
		case TAG_EMOJI:
		case TAG_WALL:
			NOTIFICATION->postNotification("dead");
			break;
		case TAG_BULLET:
			((Hero*)nodeHero)->addBullet();
			((Block*)nodeOther)->cleanConllition();
			NOTIFICATION->postNotification("updatebullet");
			break;
		default:
			break;
		}

		return false;
	};
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(listenerConllition, hero);

	scheduleUpdate();

	return true;
}

bool FSMHero::handleMessage(Gesture gesture)
{
	switch (gesture)
	{
	case GESTURE_UP:
		return handleUp();
		break;
	case GESTURE_DOWN:
		return handleDown();
		break;
	case GESTURE_LEFT:
		return handleLeft();
		break;
	case GESTURE_RIGHT:
		return handleRight();
		break;
	case GESTURE_TOUCH:
		return handleTouch();
		break;
	}
	return false;
}

bool FSMHero::handleUp()
{
	if (hero->getPositionY() >= visibleSize.height / 2)
		return false;
	if (hero->getState() != STATE_HERO_RUN && hero->getState() != STATE_HERO_RUN_UP)
		return false;
	if (hero->getState() == STATE_HERO_RUN_UP)
	{
		if (hero->getPositionY() <= Director::getInstance()->getVisibleSize().height / 4)
			return false;
	}
	hero->transferUp(visibleSize, floorHeight);
	return true;
}

bool FSMHero::handleDown()
{
	if (hero->getPositionY() < visibleSize.height / 2)
		return false;
	if (hero->getState() != STATE_HERO_RUN && hero->getState() != STATE_HERO_RUN_UP)
		return false;
	if (hero->getState() == STATE_HERO_RUN_UP)
	{
		if (hero->getPositionY() <= 3 * Director::getInstance()->getVisibleSize().height / 4)
			return false;
	}
	hero->transferDown(visibleSize, floorHeight);
	return true;
}

bool FSMHero::handleLeft()
{
	if (!hero->getState() == STATE_HERO_RUN)
		return false;
	hero->setState(STATE_HERO_FALL);
	hero->fall();
	return true;
}

bool FSMHero::handleRight()
{
	hero->hit();
	return true;
}

bool FSMHero::handleTouch()
{
	if (!hero->getState() == STATE_HERO_RUN)
		return false;
	hero->setState(STATE_HERO_JUMP);
	hero->jump();
	return true;
}



