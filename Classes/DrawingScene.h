#ifndef __DRAWING_SCENE_H__
#define __DRAWING_SCENE_H__

#include "cocos2d.h"
#include <vector>

#define DEVICE_WIDTH 1280
#define DEVICE_HEIGHT 720

USING_NS_CC;
using namespace std;

class DrawingScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
	void update(float dt);
    void ccTouchesMoved(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void afterShowingMessagebox(CCNode *pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(DrawingScene);
    
    
private:
	void LoadData();
	void Restart();
    void CheckBoard();

	CCSize _visibleSize;
	CCPoint _origin;

    cocos2d::CCRenderTexture *_board;
    cocos2d::CCSprite *_brush;

	bool _maskData[DEVICE_HEIGHT][DEVICE_WIDTH];


	vector<cocos2d::CCPoint> _touches;

	vector<cocos2d::CCPoint> _vertexInRoute;
	int _nextVertexIndex;
	cocos2d::CCArray *_checkPointSpriteArray;
	
	
	cocos2d::CCLayer *_menuLayer;
    
};

#endif // __DRAWING_SCENE_H__
