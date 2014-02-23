#pragma once
#include "cocos2d.h"

USING_NS_CC;
// ÀÆπ‹¿‡
class Pillar :public CCLayer
{
public:
	CCArray *pillarList;
	virtual void onEnter();

	CREATE_FUNC(Pillar);

	void addPillar(int offsetX);
};