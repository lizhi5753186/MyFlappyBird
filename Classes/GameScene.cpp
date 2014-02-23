#include "GameScene.h"
#include <cmath>

GameScene::GameScene()
{
	hightestScore=0;
	score=0;
}

CCScene* GameScene::createScene()
{
	CCScene *gameSceen =CCScene::create();
	GameScene *gameLayer =GameScene::create();

	gameSceen->addChild(gameLayer);
	return gameSceen;
}

void GameScene::onEnter()
{
	CCNode::onEnter();

	// 把图片添加进全局的精灵框帧缓存中
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/flappy_frame.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/flappy_packer.plist");
	
	// 获得窗体大小
	CCSize winSize =CCDirector::sharedDirector()->getWinSize();

	// 添加背景图片1被设置位置
	CCSprite *bg= CCSprite::create("ui/bg.png");
	bg->setAnchorPoint(ccp(0,0));
	bg->setPosition(ccp(0,0));
	bg->setScale(0.7);
	this->addChild(bg);

	// 添加背景图片2被设置位置
	ground1 =CCSprite::create("ui/ground.png");
	ground1->setAnchorPoint(ccp(0,0));
	ground1->setPosition(ccp(0,0));
	ground1->setScaleY(0.7);
	ground1->setZOrder(10);
	this->addChild(ground1);

	// 添加ground
	ground2 =CCSprite::create("ui/ground.png");
	ground2->setAnchorPoint(ccp(0,0));
	ground2->setPosition(ccp(winSize.width,0));
	ground2->setZOrder(10);
	ground2->setScaleY(0.7);
	this->addChild(ground2);

	// 添加FlappyBirdLogo
	CCSprite *flappyBird =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("flappybird.png"));
	flappyBird->setPosition(ccp(winSize.width/2,winSize.height/4*3));
	flappyBird->setScale(0.6);
	flappyBird->setTag(TAG_LOGO);
	this->addChild(flappyBird);

	// 添加GameOver
	CCSprite *gameOver=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gameover.png"));
	gameOver->setPosition(ccp(winSize.width/2,winSize.height/4*3));
	gameOver->setScale(0.6);
	gameOver->setTag(TAG_OVER);
	gameOver->setZOrder(10);
	gameOver->setVisible(false);
	this->addChild(gameOver);

	// 添加GetReady精灵
	CCSprite *getready =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("getready.png"));
	getready->setPosition(ccp(winSize.width/2,winSize.height/4*3-50));
	getready->setScale(0.6);
	getready->setTag(TAG_READY);
	getready->setVisible(false);
	this->addChild(getready);

	// 添加开始按钮
	CCSprite *normalStart =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("start.png"));
	CCSprite *pressStart =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("start1.png"));

	CCMenuItemImage* startBtn=CCMenuItemImage::create();
	startBtn->initWithNormalSprite(normalStart,pressStart,NULL,this,menu_selector(GameScene::gameStart));
	CCMenu *menuStart =CCMenu::create(startBtn,NULL);
	menuStart->setPosition(ccp(winSize.width/16,ground1->boundingBox().size.height/2-30));
	menuStart->setTag(TAG_START_BTN);
	menuStart->setScale(0.6);
	menuStart->setZOrder(10);
	this->addChild(menuStart);

	// 添加退出按钮
	CCSprite *normalEnd =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("grade.png"));

	CCMenuItemImage *endBtn =CCMenuItemImage::create();
	endBtn->initWithNormalSprite(normalEnd,normalEnd,NULL,this,menu_selector(GameScene::gameEnd));
	CCMenu *menuEnd =CCMenu::create(endBtn,NULL);
	menuEnd->setPosition(ccp(winSize.width/2,ground1->boundingBox().size.height/2-30));
	menuEnd->setTag(Tage_End_BTN);
	menuEnd->setScale(0.6);
	menuEnd->setZOrder(10);
	this->addChild(menuEnd);

	// 添加click图片
	CCSprite *click =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("click.png"));
	click->setPosition(ccp(winSize.width/2,winSize.height/2));
	click->setScale(0.6);
	click->setTag(TAG_CLICK);
	click->setVisible(false);
	this->addChild(click);
	
	this->birdReadyAction();

	// 添加Score
	CCLabelBMFont *score=CCLabelBMFont::create("0","fonts/futura-48.fnt");
	score->setPosition(ccp(winSize.width/2,winSize.height/4*3+60));
	score->setTag(TAG_SCORE);
	score->setVisible(false);
	score->setZOrder(10);
	this->addChild(score);

	// 添加记录
	CCSprite *record =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("base.png"));
	record->setPosition(ccp(winSize.width/2,winSize.height/2+30));
	record->setScaleX(0.6);
	record->setScaleY(0.6);
	record->setTag(TAG_RECORD);
	record->setVisible(false);
	record->setZOrder(10);
	this->addChild(record);

	hose=Hose::create();
	this->addChild(hose);

	// 预加载音效
	preloadMusic();

	// 设置可触摸
	this->setTouchEnabled(true);

	// 实现ground滚动和碰撞检测
	this->scheduleUpdate();

	// 初始化游戏状态
	gameState=Welcome;
}

void GameScene::preloadMusic()
{
	// 预加载Resources\sound下所有音乐文件
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/lh_die.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/lh_hit.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/lh_point.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/lh_swooshing.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/lh_wing.ogg");
}

// 按Start按钮开始游戏
void GameScene::gameStart( CCObject *pSender )
{	
	if (gameState==GameOver)
	{
		// 重置游戏
		hose->removeAllChildren();
		hose->hoseList->removeAllObjects();
		hose->onEnter();
		CCDirector::sharedDirector()->resume();
		CCLabelBMFont* scoreSprite =(CCLabelBMFont*)this->getChildByTag(TAG_SCORE);
		scoreSprite->setString("0");
		this->birdReadyAction();
	}

	gameState=Ready;
	// 播放音效
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_swooshing.ogg");
	this->getChildByTag(TAG_START_BTN)->setVisible(false);
	this->getChildByTag(Tage_End_BTN)->setVisible(false);
	this->getChildByTag(TAG_OVER)->setVisible(false);

	// 移除分数记录
	this->removeChildByTag(TAG_CURRENSCORE);
	this->removeChildByTag(TAG_HIGESTSCORE);

	this->getChildByTag(TAG_RECORD)->setVisible(false);
	this->getChildByTag(TAG_LOGO)->setVisible(false);
	this->getChildByTag(TAG_READY)->setVisible(true);
	this->getChildByTag(TAG_SCORE)->setVisible(true);
	this->getChildByTag(TAG_BIRD)->setVisible(true);
	this->getChildByTag(TAG_CLICK)->setVisible(true);	
}

void  GameScene::gameEnd(CCObject *pSender)
{
	// 退出游戏
	CCDirector::sharedDirector()->end();
}

void GameScene::gameOver(float dt)
{
	CCDirector::sharedDirector()->pause();
	this->removeChildByTag(TAG_BIRD);

	// 设置分数在记录卡上
	CCLabelBMFont *currentscore=CCLabelBMFont::create("0","fonts/futura-48.fnt");
	CCPoint recordPoint = this->getChildByTag(TAG_RECORD)->getPosition();
	currentscore->setString(((CCLabelBMFont*)this->getChildByTag(TAG_SCORE))->getString());
	currentscore->setPosition(ccp(recordPoint.x+125,recordPoint.y+20));
	currentscore->setScale(0.7);
	currentscore->setZOrder(15);
	currentscore->setTag(TAG_CURRENSCORE);
	this->addChild(currentscore);

	// 历史最高分
	CCLabelBMFont *hightestscore=CCLabelBMFont::create("0","fonts/futura-48.fnt");
	hightestscore->setPosition(ccp(recordPoint.x+125,recordPoint.y-35));
	hightestscore->setZOrder(15);
	hightestscore->setScale(0.7);
	hightestscore->setTag(TAG_HIGESTSCORE);

	this->addChild(hightestscore);
	CCString *hestString =CCString::createWithFormat("%d",this->hightestScore);
	hightestscore->setString(hestString->m_sString.c_str());
	this->getChildByTag(TAG_OVER)->setVisible(true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_swooshing.ogg");

	this->getChildByTag(TAG_RECORD)->setVisible(true);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_swooshing.ogg");

	this->getChildByTag(TAG_START_BTN)->setVisible(true);
	this->getChildByTag(Tage_End_BTN)->setVisible(true);
}


void GameScene::keyBackClicked()
{
	// 退出游戏
	CCDirector::sharedDirector()->end();
}

// 地面滚动实现
void GameScene::update(float dt)
{
	CCObject *hoseSprite;
	float dif =4;
	float sourceX =CCDirector::sharedDirector()->getWinSize().width/2-80;
	ground1->setPositionX(ground1->getPositionX()-dif);
	ground2->setPositionX(ground1->getPositionX()+ground1->getContentSize().width-4);
	if(ground2->getPositionX()==0)
	{
		ground1->setPositionX(0);
	}
	
	if (gameState==Start)
	{
		this->hose->update();
		this->checkCollision();
	}
}

// 碰撞检测
void GameScene::checkCollision()
{
	CCSprite *bird =(CCSprite*)this->getChildByTag(TAG_BIRD);
	CCRect birdRect =bird->boundingBox();

	if (bird->getPositionY()<ground1->boundingBox().size.height)
	{
		gameState=GameOver;
		score=0;
		this->birdFallAction();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_hit.ogg");
		return;
	}

	for(int i=0;i<hose->hoseList->count();i++)
	{
		CCSprite *hoseSprite =(CCSprite*)hose->hoseList->objectAtIndex(i);
		CCRect hoseRect =hoseSprite->boundingBox();
		if (birdRect.intersectsRect(hoseRect))
		{
			gameState=GameOver;
			score=0;

			this->birdFallAction();
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_hit.ogg");
			break;
		}

		// 更新分数
		int pPosX =hoseSprite->getPositionX()+hoseSprite->getContentSize().width*hoseSprite->getScaleX();
		int birdX =bird->getPositionX()-bird->getContentSize().width*bird->getScaleX();
		if (pPosX==birdX)
		{
			score++;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_point.ogg");
			CCLabelBMFont* scoreSprite =(CCLabelBMFont*)this->getChildByTag(TAG_SCORE);
			CCString *strScore =CCString::createWithFormat("%d",score/2);
			scoreSprite->setString(strScore->m_sString.c_str());
		}
	}

	updatehightestRecord();
}

bool GameScene::ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent )
{
	return true;
}

void GameScene::ccTouchEnded( CCTouch* pTouch,CCEvent* pEvent )
{
	// 触摸完成时，进入游戏场景
	this->getChildByTag(TAG_READY)->setVisible(false);
	this->getChildByTag(TAG_CLICK)->setVisible(false);
	if (gameState==Ready)
	{
		gameState=Start;
	}
	if (gameState==GameOver)
	{
		return; 
	}

	this->birdRiseAction();
}

void GameScene::registerWithTouchDispatcher()
{
	// 把当前层注册到触摸分发器中
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
}

// 更新最高分数
void GameScene::updatehightestRecord()
{
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveHistoryScore"))
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveHistoryScore",true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",score/2);
		this->hightestScore=score/2;
		CCUserDefault::sharedUserDefault()->flush();
	}
	else
	{
		int beforesocre=CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore");
		if (beforesocre<(score/2))
		{	
			CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",score/2);
			this->hightestScore=score/2;
		}
		else
		{
			this->hightestScore=beforesocre;
		}
	}
}

// 实现小鸟上飞的过程
void GameScene::birdRiseAction()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_wing.ogg");
	CCSize winSize =CCDirector::sharedDirector()->getWinSize();
	float riseHeight =45;
	float birdX =this->getChildByTag(TAG_BIRD)->getPositionX();
	float birdY= this->getChildByTag(TAG_BIRD)->getPositionY();
	float time =birdY/600;

	CCRepeat *flyAction =CCRepeat::create(birdanimate,90000);
	if(birdY+riseHeight>winSize.height)
	{
		birdY=winSize.height-riseHeight;
	}

	CCMoveTo *riseAction1 =CCMoveTo::create(0.2,ccp(birdX,birdY+riseHeight));
	CCRotateTo *riseAction2 =CCRotateTo::create(0,-30);
	CCSpawn *riseAction =CCSpawn::create(riseAction1,riseAction2,NULL);

	CCMoveTo *fallAction1 =CCMoveTo::create(time,ccp(birdX,60));
	CCSequence *fallAction2=CCSequence::create(CCDelayTime::create(time/6),CCRotateTo::create(0,30),NULL);
	CCSpawn *fallAction =CCSpawn::create(fallAction1,fallAction2,NULL);

	this->getChildByTag(TAG_BIRD)->stopAllActions();
	this->getChildByTag(TAG_BIRD)->runAction(CCSpawn::create(flyAction,CCSequence::create(riseAction,CCDelayTime::create(0.05),fallAction,NULL),NULL));
}

void GameScene::birdFallAction()
{
	gameState=GameOver;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lh_die.ogg");
	CCSize winSize =CCDirector::sharedDirector()->getWinSize();

	this->getChildByTag(TAG_BIRD)->stopAllActions();

	float birdX=this->getChildByTag(TAG_BIRD)->getPositionX();
	float birdY =this->getChildByTag(TAG_BIRD)->getPositionY();

	float time =birdY/2000;
	this->getChildByTag(TAG_BIRD)->runAction(
		CCSpawn::create(
		CCRotateTo::create(time,90),
		CCMoveTo::create(time,ccp(birdX,ground1->boundingBox().size.height)),NULL));

	// 使分数不可见
	this->getChildByTag(TAG_SCORE)->setVisible(false);

	//延迟一秒回调游戏结束方法
	this->scheduleOnce(schedule_selector(GameScene::gameOver),1.0);
}

void GameScene::birdReadyAction()
{
	CCSize winSize =CCDirector::sharedDirector()->getWinSize();

	// 添加bird精灵
	CCSprite *bird =CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bird1.png"));
	bird->setPosition(ccp(winSize.width/2-80,winSize.height/2+20));
	bird->setZOrder(20);
	bird->setScale(0.6);
	bird->setTag(TAG_BIRD);
	bird->setVisible(false);
	this->addChild(bird);

	// 创建动画对象
	CCAnimation *birdAnimation =CCAnimation::create();
	birdAnimation->setDelayPerUnit(0.2);
	birdAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bird1.png"));
	birdAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bird2.png"));
	birdAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bird3.png"));

	// 创建动画动作
	birdanimate =CCAnimate::create(birdAnimation);

	CCActionInterval *action =CCMoveBy::create(0.2,ccp(0,10));
	CCActionInterval *action1 =action->reverse();
	CCActionInterval *sequence =CCSequence::create(action,action1,NULL);

	CCSpawn *spawn =CCSpawn::create(sequence,birdanimate,NULL);
	CCRepeatForever *birdrepeatForever =CCRepeatForever::create(spawn);

	// 让bird精灵一直运行动画
	bird->runAction(birdrepeatForever);
}