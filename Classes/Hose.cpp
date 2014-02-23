#include "Hose.h"

Hose::Hose(void)
{
	hoseList =CCArray::create();
	hoseList->retain();
}
void Hose::onEnter()
{
	CCNode::onEnter();
	for (int i=0;i<30;i++)
	{
		addHose(i);
	}
}

void Hose::update()
{
	CCObject *hs;
	CCARRAY_FOREACH(hoseList,hs)
	{
		CCSprite *hoseSprite =(CCSprite*)hs;
		hoseSprite->setPositionX(hoseSprite->getPositionX()-1);
		if (hoseSprite->getPositionX()<-hoseSprite->getContentSize().width*hoseSprite->getScaleX())
		{
			hoseList->removeObject(hoseSprite);
			this->removeChild(hoseSprite);
		}
	}
}

// 添加障碍物
void Hose::addHose(int num)
{
	CCSize winSize =CCDirector::sharedDirector()->getWinSize();

	float hoseHeight =830;
	float accrossHeight =winSize.height/8+150;
	float maxDownY=winSize.height/2;
	float minDownY =300;
	float downHeight =CCRANDOM_0_1()*(maxDownY-minDownY)+minDownY;
	float upHeight =winSize.height-downHeight-accrossHeight;

	int hoseX =winSize.width+200*num;

	CCSprite *spriteDown =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("holdback3.png"));
	// Z轴大的会覆盖Z轴小的
	spriteDown->setZOrder(1);
	spriteDown->setAnchorPoint(ccp(0,0));
	spriteDown->setPosition(ccp(hoseX,0));
	spriteDown->setScaleX(0.5);
	spriteDown->setScaleY(downHeight/hoseHeight);
	hoseList->addObject(spriteDown);
	this->addChild(spriteDown);

	CCSprite *spriteUp =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("holdback2.png"));
	spriteUp->setZOrder(1);
	spriteUp->setAnchorPoint(ccp(0,0));
	spriteUp->setPosition(ccp(hoseX,downHeight+accrossHeight));
	spriteUp->setScaleX(0.5);
	spriteUp->setScaleY(0.4);
	hoseList->addObject(spriteUp);
	this->addChild(spriteUp);
}