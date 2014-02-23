#include "PillarLayer.h"

void Pillar::onEnter()
{
	CCNode::onEnter();
	pillarList=CCArray::create();
	pillarList->retain();

	for(int i=0;i<30;i++)
	{
		this->addPillar(100*i);
	}
}

// 添加水管
void  Pillar::addPillar(int offsetX)
{
	CCSize winSize =CCDirector::sharedDirector()->getWinSize();

	CCSprite *hoseupSprite =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("holdback2.png"));
	hoseupSprite->setScale(0.5);
	CCSize hoseSize =hoseupSprite->getContentSize();
	pillarList->addObject(hoseupSprite);
	this->addChild(hoseupSprite);

	CCSprite *hosedownSprite =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("holdback1.png"));
	hosedownSprite->setScale(0.5);
	pillarList->addObject(hosedownSprite);
	this->addChild(hosedownSprite);

	// 设置位置
	int maxUpY =winSize.height+hoseSize.height/4;
	int minUpY =winSize.height-hoseSize.height/4;
	int upRandY =CCRANDOM_0_1()*(maxUpY-minUpY)+minUpY;

	int maxDownY=hoseSize.height/4;
	int minDownY=-hoseSize.height/4;
	int downRandY=CCRANDOM_0_1()*(maxDownY-maxUpY)+minDownY;

	if (upRandY-downRandY-hoseSize.height<160)
	{
		downRandY=upRandY-hoseSize.height-160;
	}

	hoseupSprite->setPosition(ccp(winSize.width+hoseSize.width/2+offsetX,upRandY));
	hosedownSprite->setPosition(ccp(winSize.width+hoseSize.width/2+offsetX,downRandY));
}
