#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include <vector>

#define DEVICE_WIDTH 1280
#define DEVICE_HEIGHT 720

USING_NS_CC;
using namespace std;

class MainMenuScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
	void update(float dt);

    // a selector callback
    void menuClickCallback(CCObject* pSender);
	
	void SetMessageBoxNeedMoney();

    // implement the "static node()" method manually
    CREATE_FUNC(MainMenuScene);
    
    
private:
	CCSize _visibleSize;
	CCPoint _origin;
	
	cocos2d::CCLayer *_messageBoxLayer;
};

#endif // __MAINMENU_SCENE_H__
