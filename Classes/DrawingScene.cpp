#include "DrawingScene.h"
#include "cocos-ext.h"
#include "MainMenuScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int ID_BUTTON_RETRY = 4;
const int ID_BUTTON_GOTOMENU = 5;


CCScene* DrawingScene::scene(int sceneID)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
//    DrawingScene *layer = DrawingScene::create();
//	layer->LoadData(sceneID);
//
//    // add layer as a child to scene
//    scene->addChild(layer);
	
	DrawingScene *pRet = new DrawingScene();
    if (pRet)
    {
		pRet->LoadData(sceneID);
		pRet->init();
        pRet->autorelease();
		scene->addChild(pRet);
    }
    else
    {
        delete pRet;
        pRet = NULL;
		
        return NULL;
    }

    // return the scene
    return scene;
}

void DrawingScene::LoadData( int sceneID )
{
	_nowSceneID = sceneID;
	
	const char *pszFileName = "maskdata.json";
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
		
		int id = 0;
		for( i = 0; i < data_count; i ++ ) {
			cs::CSJsonDictionary *maskData = jsonDict->getSubItemFromArray( "data", i );
			int id = maskData->getItemIntValue("id", 0);
			CCLog("sceneid:%d   i:%d  id:%d", sceneID, i, id);
			if( id == sceneID ) {
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
				break;
			}
			CC_SAFE_DELETE(maskData);
		}

        CC_SAFE_DELETE(jsonDict);
    } while (0);
}

void DrawingScene::Restart()
{
	int i, j;
	for( i=0; i<_checkPointSpriteArray->count(); i++ ) {
		CCSprite *old = dynamic_cast<CCSprite*>(_checkPointSpriteArray->objectAtIndex( i ));
		_checkPointSpriteArray->removeObjectAtIndex( i );
		this->removeChild( old, 11 );
		
		CCSprite *newSpr = CCSprite::create( "checkpoint.png" );
		newSpr->setPosition( CCDirector::sharedDirector()->convertToGL(_vertexInRoute[i] ) );
		_checkPointSpriteArray->insertObject( newSpr, i );
		this->addChild( newSpr, 11 );
	}
	_nextVertexIndex = 0;
	
	_touches.clear();
	
	_board->clear(0, 0, 0, 0);
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
	
	_menuLayer = CCLayer::create();
	_menuLayer->setPosition(CCPointZero);
	this->addChild(_menuLayer, 10);
	
	CCMenuItemImage *pRestartButton = CCMenuItemImage::create("retry.png",
															  "retry-selected.png",
															  this,
															  menu_selector(DrawingScene::menuClickCallback));
	pRestartButton->setPosition(ccp(_visibleSize.width - pRestartButton->getContentSize().width,
									_visibleSize.height - pRestartButton->getContentSize().height));
	pRestartButton->setTag(ID_BUTTON_RETRY);
	
	CCMenuItemImage *pGotoMenuButton = CCMenuItemImage::create("gotomenu.png",
															   "gotomenu-selected.png",
															   this,
															   menu_selector(DrawingScene::menuClickCallback));
	pGotoMenuButton->setPosition(ccp(_visibleSize.width - pGotoMenuButton->getContentSize().width,
									_visibleSize.height - pGotoMenuButton->getContentSize().height * 2));
	pGotoMenuButton->setTag(ID_BUTTON_GOTOMENU);
	
	// create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pRestartButton, pGotoMenuButton, NULL);
    pMenu->setPosition(CCPointZero);
    _menuLayer->addChild(pMenu, 10);
	


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	
    _board = CCRenderTexture::create(_visibleSize.width, _visibleSize.height,
                                     kCCTexture2DPixelFormat_RGBA8888);
    _board->retain();
    _board->setPosition(ccp(_visibleSize.width / 2, _visibleSize.height / 2));
	_board->setZOrder(10);
    this->addChild(_board);
    
    _brush = CCSprite::create("mybrush.png");
    _brush->retain();
	
	CCSprite *back = CCSprite::create("note_background.png");
	back->setPosition(ccp(_visibleSize.width/2, _visibleSize.height/2));
	this->addChild(back, 0);
	
	
	_checkPointSpriteArray = CCArray::create();
	_checkPointSpriteArray->retain();
	int i, j;
	for( i=0; i<_vertexInRoute.size(); i++ ) {
		CCSprite *spr = CCSprite::create( "checkpoint.png" );
		spr->setAnchorPoint( ccp( 0.5, 0.5 ) );
		spr->setPosition( CCDirector::sharedDirector()->convertToGL(_vertexInRoute[i]) );
		this->addChild( spr, 11 );
		_checkPointSpriteArray->addObject( spr );
	}
	_nextVertexIndex = 0;
	
	_character = CCSprite::create("character.png");
	_character->cocos2d::CCNode::setPosition(CCDirector::sharedDirector()->convertToGL(_vertexInRoute[0]));
	this->addChild(_character, 12);

	
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
	
	_dieFlag = false;

    return true;
}


void DrawingScene::update(float dt)
{
	
}


void DrawingScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
	if( _maskData[(int)location.y][(int)location.x] == 0 && !_dieFlag ) {
		_dieFlag = true;
		ShowFailMessage();
	}
	
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    _board->begin();
    
    _brush->setPosition(location);
    _brush->visit();
    
    _board->end();
	
	location.y = _visibleSize.height - location.y;
	_touches.push_back(location);
	
	_character->setPosition(CCDirector::sharedDirector()->convertToUI(location));

	
	if( _nextVertexIndex >= _vertexInRoute.size() )
		return;

	// It should be use NEW CCLAYER
	CCPoint nextVertex = _vertexInRoute[_nextVertexIndex];
	CCRect vertexCollisionCheckRect = CCRectMake( nextVertex.x - 32, nextVertex.y - 32, 64, 64 );
	if( vertexCollisionCheckRect.containsPoint( location ) ) {
		CCSprite *old = dynamic_cast<CCSprite*>(_checkPointSpriteArray->objectAtIndex( _nextVertexIndex ));
		_checkPointSpriteArray->removeObjectAtIndex( _nextVertexIndex );
		this->removeChild( old, 11 );

		CCSprite *newSpr = CCSprite::create( "checkpoint_passed.png" );
		newSpr->setPosition( CCDirector::sharedDirector()->convertToGL(_vertexInRoute[_nextVertexIndex] ) );
		_checkPointSpriteArray->insertObject( newSpr, _nextVertexIndex );
		this->addChild( newSpr, 11 );

		_nextVertexIndex ++;
		if( _nextVertexIndex >= _vertexInRoute.size() ) {
			CheckBoard();
		}
	}
}


void DrawingScene::menuClickCallback(CCObject* pSender)
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
            _board->clear(0, 0, 0, 0);
            break;
            
        case 3: // check button
            CheckBoard();
			_touches.clear();
			_board->clear(0, 0, 0, 0);
            break;
			
		case ID_BUTTON_RETRY:
			Restart();
			break;
			
		case ID_BUTTON_GOTOMENU:
			CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1, MainMenuScene::scene(), false));
			break;

        default:
            break;
    }
}

void DrawingScene::CheckBoard()
{
	int size = _touches.size();
	CCPoint start = _touches[0];
	CCPoint end = _touches[size-1];
	int i;
	for( i=0; i<size; i++ )
	{
		CCPoint p = _touches[i];
		int x = (int)(p.x * CCDirector::sharedDirector()->getContentScaleFactor());
		int y = (int)(p.y * CCDirector::sharedDirector()->getContentScaleFactor());
		
		if( _maskData[y][x] == 0 ) {
			CCMessageBox( "Fail!", "System" );
			Restart();
			return;
		}
	}
	
	
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
	
	CCScene *pScene = DrawingScene::scene(_nowSceneID);
	CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1, pScene, false));
}

void DrawingScene::ShowFailMessage()
{
	CCSprite *msg = CCSprite::create( "messagebox_replay.png" );
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