#include "DrawingScene.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* DrawingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    DrawingScene *layer = DrawingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void DrawingScene::LoadData() 
{ 
	const char *pszFileName = "maskdata/maskdata.json";
	unsigned long size = 0;
    const char* pData = 0;
	int i, j;
    do {
		// Load json file content
		CC_BREAK_IF(pszFileName == NULL);
		std::string strFileName(pszFileName);
        pData = (char*)(cocos2d::CCFileUtils::sharedFileUtils()->getFileData(pszFileName, "r", &size));
        CC_BREAK_IF(pData == NULL || strcmp(pData, "") == 0);

		// Make json object by file content
        cs::CSJsonDictionary *jsonDict = new cs::CSJsonDictionary();
        jsonDict->initWithDescription(pData);

		int data_count = jsonDict->getArrayItemCount( "data" );
		
		/*for( i = 0; i < data_count; i ++ ) {
			jsonDict->getSubItemFromArray( "data", i );
		}*/
		cs::CSJsonDictionary *maskData = jsonDict->getSubItemFromArray( "data", 0 );
		{
			// Initialize mask data from mask image
			const char *mask_image_path = maskData->getItemStringValue( "mask_image" );
			CCImage *maskImage = new CCImage();
			maskImage->initWithImageFile(mask_image_path);
			int byte_num = 3;
			if( maskImage->hasAlpha() )
				byte_num++;
			int w = maskImage->getWidth();
			int h = maskImage->getHeight();
			unsigned char *data = maskImage->getData();
			memset( _maskData, 0, sizeof(_maskData) );
			for( i=0; i<h; i++ )
			{
				for( j=0; j<w; j++ )
				{
					unsigned char *pixel = data + (i * w + j) * byte_num;
					unsigned char r = *pixel;
			
					if( r == 0 ) // if this pixel is black
						_maskData[i][j] = 0;
					else // if this pixel is white
						_maskData[i][j] = 1;
				}
			}

			int numOfPoints = maskData->getArrayItemCount( "points" );
			for( i=0; i<numOfPoints; i++ ) {
				cs::CSJsonDictionary *point = maskData->getSubItemFromArray( "points", i );
			
				int x = point->getItemIntValue( "x", _visibleSize.width/2 );
				int y = point->getItemIntValue( "y", _visibleSize.height/2 );
				_vertexInRoute.push_back(CCPoint(x, y));

				CC_SAFE_DELETE( point );
			}
		}
		CC_SAFE_DELETE( maskData );

        CC_SAFE_DELETE(jsonDict);
    } while (0);
}

// on "init" you need to initialize your instance
bool DrawingScene::init()
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
                                        menu_selector(DrawingScene::menuCloseCallback));
	pCloseItem->setPosition(ccp(_origin.x + _visibleSize.width - pCloseItem->getContentSize().width/2, _origin.y + pCloseItem->getContentSize().height/2));
    pCloseItem->setTag(1);
    
    CCMenuItemFont *pClearButton = CCMenuItemFont::create("CLEAR BOARD",
                                                          this,
                                                          menu_selector(DrawingScene::menuCloseCallback));
	pClearButton->setFontNameObj("fonts/Moebius.ttf");
	pClearButton->setFontSizeObj(32);
	pClearButton->setColor(ccc3( 0, 0, 0) );
    pClearButton->setPosition( ccp( pClearButton->getContentSize().width/2, _visibleSize.height - 50) );
    pClearButton->setTag(2);
    
    CCMenuItemFont *pCheckButton = CCMenuItemFont::create("CHECK BOARD",
                                                          this,
                                                          menu_selector(DrawingScene::menuCloseCallback));
	pCheckButton->setFontNameObj("fonts/Moebius.ttf");
	pCheckButton->setFontSizeObj(32);
	pCheckButton->setColor(ccc3( 0, 0, 0) );
    pCheckButton->setPosition( ccp( pCheckButton->getContentSize().width/2, _visibleSize.height - 100) );
    pCheckButton->setTag(3);
	
	// create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pClearButton, pCheckButton, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 10);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

	LoadData();
	int i, j;
	for( i=0; i<_vertexInRoute.size(); i++ ) {
		CCSprite *spr = CCSprite::create( "checkpoint.png" );
		spr->setAnchorPoint( ccp( 0.5, 0.5 ) );
		spr->setPosition( CCDirector::sharedDirector()->convertToGL(_vertexInRoute[i]) );
		this->addChild( spr, 11 );
	}

    board = CCRenderTexture::create(_visibleSize.width, _visibleSize.height,
                                     kCCTexture2DPixelFormat_RGBA8888);
    board->retain();
    board->setPosition(ccp(_visibleSize.width / 2, _visibleSize.height / 2));
	board->setZOrder(10);
    this->addChild(board);
    
    brush = CCSprite::create("mybrush.png");
    brush->retain();
	
	
	CCSprite *back = CCSprite::create("note_background.png");
	back->setPosition(ccp(_visibleSize.width/2, _visibleSize.height/2));
	this->addChild(back, 0);

	
	CCImage *visualImage = new CCImage;
	visualImage->initWithImageFile("visualMask.png");
	int w = visualImage->getWidth();
	int h = visualImage->getHeight();
	int byte_num = 3;
	if( visualImage->hasAlpha() )
		byte_num++;
	unsigned char* data = visualImage->getData();
	for( i=0; i<h; i++ ) {
		for ( j=0; j<w; j++ ) {
			if( _maskData[i][j] == 0 ) {
				unsigned char *pixel = data + (i * w + j) * byte_num;
				unsigned char &r = *(pixel);
				unsigned char &g = *(pixel + 1);
				unsigned char &b = *(pixel + 2);
				unsigned char &a = *(pixel + 3);
				r = g = b = a = 0;
			}
		}
	}
	
	CCTexture2D *newtex = new CCTexture2D;
	newtex->initWithImage(visualImage);
	CCSprite *newspr = CCSprite::createWithTexture( newtex );
	newspr->setPosition(ccp(_visibleSize.width/2, _visibleSize.height/2));
	this->addChild(newspr, 5);
	
    
    this->setTouchEnabled(true);

	_touches.clear();

    return true;
}


void DrawingScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
//	CCLog("TOUCH (%.2f %.2f)", location.x, location.y);
    
    board->begin();
    
    brush->setPosition(location);
    brush->visit();
    
    board->end();
	
	location.y = _visibleSize.height - location.y;
	_touches.push_back(location);
}


void DrawingScene::menuCloseCallback(CCObject* pSender)
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
            board->clear(0, 0, 0, 0);
            break;
            
        case 3: // check button
            CheckBoard();
			_touches.clear();
			board->clear(0, 0, 0, 0);
            break;

        default:
            break;
    }
}

void DrawingScene::CheckBoard()
{
	CCImage *maskImage = new CCImage();
	maskImage->initWithImageFile("mask.png");
	int byte_num = 3;
	if( maskImage->hasAlpha() )
		byte_num++;
	
	bool maskData[720][1280];
	unsigned char *data = maskImage->getData();
	int i, j;
	int w = maskImage->getWidth();
	int h = maskImage->getHeight();
	for( i=0; i<h; i++ )
    {
        for( j=0; j<w; j++ )
        {
            unsigned char *pixel = data + (i * w + j) * byte_num;
			
			// You can see/change pixels' RGBA value(0-255) here !
            unsigned char r = *pixel;
//            unsigned char g = *(pixel + 1);
//            unsigned char b = *(pixel + 2);
//            unsigned char a = *(pixel + 3);
			
			if( r == 0 ) // if this pixel is black
				maskData[i][j] = 0;
			else // if this pixel is white
				maskData[i][j] = 1;
        }
    }
	
	int size = _touches.size();
	CCPoint start = _touches[0];
	CCPoint end = _touches[size-1];
	
	for( i=0; i<size; i++ )
	{
		CCPoint p = _touches[i];
		int x = (int)(p.x * CCDirector::sharedDirector()->getContentScaleFactor());
		int y = (int)(p.y * CCDirector::sharedDirector()->getContentScaleFactor());
		
		if( maskData[y][x] == 0 ) {
			CCMessageBox( "Fail!", "System" );
			return;
		}
	}
	
	
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
			CCCallFuncN::create( this, callfuncN_selector(DrawingScene::afterShowingMessagebox) ), 
			NULL ) );

	this->addChild(msg);
}

void DrawingScene::afterShowingMessagebox(CCNode *pSender) 
{
	this->removeChild( pSender );
}