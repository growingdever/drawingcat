#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    void ccTouchesMoved(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void afterShowingMessagebox(CCNode *pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    
private:
	CCSize _visibleSize;
	CCPoint _origin;

    cocos2d::CCRenderTexture *board;
    cocos2d::CCSprite *brush;
	vector<cocos2d::CCPoint> _touches;

	
    void CheckBoard();
	void CheckBoard2();
    
};

#endif // __HELLOWORLD_SCENE_H__
