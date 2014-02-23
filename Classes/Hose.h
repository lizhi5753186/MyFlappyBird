#pragma once
#include "cocos2d.h"
#include "Resource.h"
USING_NS_CC;

class Hose:public CCNode
{
public:
	CCArray *hoseList;
	Hose(void);
	CREATE_FUNC(Hose);
	virtual void onEnter();
	void update();
	void addHose(int num);	
};