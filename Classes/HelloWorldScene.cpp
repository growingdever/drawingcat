#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));
    pCloseItem->setTag(1);
    
    CCMenuItemFont *pClearButton = CCMenuItemFont::create("CLEAR BOARD",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback));
    pClearButton->setPosition( ccp( pClearButton->getContentSize().width/2, visibleSize.height - 50) );
    pClearButton->setTag(2);
    
    CCMenuItemFont *pCheckButton = CCMenuItemFont::create("CHECK BOARD",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback));
    pCheckButton->setTag(3);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pClearButton, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    target = CCRenderTexture::create(visibleSize.width, visibleSize.height,
                                     kCCTexture2DPixelFormat_RGBA8888);
    target->retain();
    target->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    
    this->addChild(target);
    
    brush = CCSprite::create("mybrush.png");
    brush->retain();
    
    this->setTouchEnabled(true);
    
    return true;
}


void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    target->begin();
    
    brush->setPosition(location);
    brush->visit();
    
    target->end();
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    int tag = ((CCNode*)pSender)->getTag();
    switch (tag) {
        case 1: // close button
            CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
            break;
            
        case 2: // clear button
            target->clear(0, 0, 0, 1);
            break;
            
        case 3: // check button
            
            break;
            
        default:
            break;
    }
}
