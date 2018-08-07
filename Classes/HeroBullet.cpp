#include "HeroBullet.h"
#include "Block.h"



bool HeroBullet::init()
{
	if (!Sprite::initWithFile("Run/AWepon[0].png"))
		return false;

	setPhysicsBody(PhysicsBody::createCircle(getContentSize().width / 2));
	setTag(TAG_HERO_BULLET);
	getPhysicsBody()->setGravityEnable(false);
	getPhysicsBody()->setContactTestBitmask(1);

	NOTIFICATION->addObserver(this, callfuncO_selector(HeroBullet::handleHeroOnDead), "dead", NULL);

	auto bullet_Listener = EventListenerPhysicsContact::create();
	bullet_Listener->onContactBegin = [](PhysicsContact& contact)
	{
		Node* nodeBlock = NULL;
		HeroBullet* bullet = NULL;

		if (contact.getShapeA()->getBody()->getNode()->getTag() == TAG_HERO_BULLET)
		{
			if (contact.getShapeB()->getBody()->getNode()->getTag() == TAG_HERO)
			{
				log("---- W T F - hero_b ---");
			}
			log("--- A bullet");
			bullet = (HeroBullet*)contact.getShapeA()->getBody()->getNode();
			nodeBlock = contact.getShapeB()->getBody()->getNode();
		}
		else if (contact.getShapeB()->getBody()->getNode()->getTag() == TAG_HERO_BULLET)
		{
			if (contact.getShapeA()->getBody()->getNode()->getTag() == TAG_HERO)
			{
				log("---- W T F - hero_a ---");
			}
			log("--- B bullet");
			bullet = (HeroBullet*)contact.getShapeB()->getBody()->getNode();
			nodeBlock = contact.getShapeA()->getBody()->getNode();
		}
		else
		{
			/*if (contact.getShapeA()->getBody()->getNode()->getTag() == TAG_HERO)
			{
				log("---- W T F -tag_hero-%d--", TAG_HERO);
				log("---- W T F -tag_hero_bullet-%d--", TAG_HERO_BULLET);
				log("---- W T F -tag_bullet-%d--", TAG_BULLET);
				log("---- W T F -tag_b-%d--", contact.getShapeB()->getBody()->getNode()->getTag());
			}
			else if (contact.getShapeB()->getBody()->getNode()->getTag() == TAG_HERO)
			{
				log("---- W T F ----");
				log("---- W T F -tag_hero-%d--", TAG_HERO);
				log("---- W T F -tag_hero_bullet-%d--", TAG_HERO_BULLET);
				log("---- W T F -tag_bullet-%d--", TAG_BULLET);
				log("---- W T F -tag_a-%d--", contact.getShapeA()->getBody()->getNode()->getTag());
			}*/
			return false;
		}

		if (nodeBlock->getTag() == TAG_BULLET)
		{
			log("---conllition: 2 bullets ----");
		}
		else if (nodeBlock->getTag() == TAG_EMOJI)
		{
			log("---conllition: emoji ----");
			((Block*)nodeBlock)->cleanConllition();
			bullet->cleanConllition();
		}
		else if (nodeBlock->getTag() == TAG_FLOOR || nodeBlock->getTag() == TAG_FLOOR_UP)
		{
			bullet->cleanConllition();
			log("---conllition: floor ----");
		}
		else
		{
			log("---conllition: error ----");
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bullet_Listener, this);

	scheduleUpdate();

	return true;
}

void HeroBullet::update(float dt)
{
	setPosition(getPositionX() + 5, getPositionY());
	if (getPositionX() > getContentSize().width / 2 + Director::getInstance()->getVisibleSize().width)
	{
		NOTIFICATION->removeAllObservers(this);
		unscheduleUpdate();
		removeFromParent();
	}
}

void HeroBullet::handleHeroOnDead(CCObject * obj)
{
	NOTIFICATION->removeAllObservers(this);
	pause();
}

void HeroBullet::cleanConllition()
{
	NOTIFICATION->removeAllObservers(this);
	unscheduleUpdate();
	removeFromParent();
}

