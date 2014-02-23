#pragma once
#include "cocos2d.h"
#include "Hose.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;


class GameScene:public CCLayer
{
public:
	GameScene();
	virtual void onEnter();
	static CCScene* createScene();
	CREATE_FUNC(GameScene);

	virtual void keyBackClicked();

	// ÒÆ¶¯
	virtual void update(float delta);

	virtual void registerWithTouchDispatcher();

	void gameStart(CCObject *pSender);
	void gameEnd(CCObject *pSender);
	void gameOver(float dt);
	
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent);

	void birdRiseAction();
	void birdFallAction();
	void birdReadyAction();
	void checkCollision();

	void preloadMusic();
	void updatehightestRecord();
private:
	Hose* hose;
	CCSprite *ground1;
	CCSprite *ground2;
	GameState gameState;
	CCAnimate *birdanimate;
	int score;
	int hightestScore;
};