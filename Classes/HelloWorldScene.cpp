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
    
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
	pCloseItem->setPosition(ccp(_origin.x + _visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                _origin.y + pCloseItem->getContentSize().height/2));
    pCloseItem->setTag(1);
    
    CCMenuItemFont *pClearButton = CCMenuItemFont::create("CLEAR BOARD",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback));
    pClearButton->setPosition( ccp( pClearButton->getContentSize().width/2, _visibleSize.height - 50) );
    pClearButton->setTag(2);
    
    CCMenuItemFont *pCheckButton = CCMenuItemFont::create("CHECK BOARD",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback));
    pCheckButton->setPosition( ccp( pCheckButton->getContentSize().width/2, _visibleSize.height - 100) );
    pCheckButton->setTag(3);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pClearButton, pCheckButton, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    board = CCRenderTexture::create(_visibleSize.width, _visibleSize.height,
                                     kCCTexture2DPixelFormat_RGBA8888);
    board->retain();
    board->setPosition(ccp(_visibleSize.width / 2, _visibleSize.height / 2));
    
    this->addChild(board);
    
    brush = CCSprite::create("mybrush.png");
    brush->retain();
    
    this->setTouchEnabled(true);

	_touches.clear();

    return true;
}


void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
	_touches.push_back(location);

    
    board->begin();
    
    brush->setPosition(location);
    brush->visit();
    
    board->end();
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
            board->clear(0, 0, 0, 1);
            break;
            
        case 3: // check button
            CheckBoard();
			_touches.clear();
			board->clear(0, 0, 0, 1);
            break;
            
        default:
            break;
    }
}

void HelloWorld::CheckBoard()
{
	// 일단 가로줄 긋기 검증부터 먼저 만들어본다.

	int size = _touches.size();
	if( size < 1 )
		return;

	CCPoint start = _touches[0];
	CCPoint end = _touches[size-1];
	
	// 선이 기울어져 있으면 가로줄 긋기 실패
	if( abs(start.y - end.y) > 50 )
		return;

	int averageY = (start.y + end.y)/2;

	// 각각의 점들 오차율을 확인
	int tolerate = 300;
	int i;
	for( i=0; i<size; i++ ) {
		CCPoint pos = _touches[i];
		tolerate -= abs(averageY - pos.y);
	}

	CCLog( "Diff : %d", tolerate );
	if( tolerate < 0 )
		return;

	//CCMessageBox( "Success!", "System" );

	CCSprite *msg = CCSprite::create( "praise.png" );
	msg->setPosition( ccp( _visibleSize.width/2, _visibleSize.height/2 ) );
	msg->setZOrder( 10 );
	msg->setScale( 0 );
	msg->runAction( 
		CCSequence::create( 
			CCScaleTo::create( 0.3, 0.5 ), 
			CCDelayTime::create( 0.5 ),
			CCScaleTo::create( 0.2, 0 ), 
			CCCallFuncN::create( this, callfuncN_selector(HelloWorld::afterShowingMessagebox) ), 
			NULL ) );

	this->addChild(msg);
}

void HelloWorld::afterShowingMessagebox(CCNode *pSender) 
{
	this->removeChild( pSender );
}