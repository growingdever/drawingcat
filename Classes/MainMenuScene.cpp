#include "MainMenuScene.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int ID_BUTTON_CLOSE = 10;

CCScene* MainMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	
	CCSprite *background = CCSprite::create("mainmenu_background.png");
	background->setPosition(_visibleSize/2);
	this->addChild(background);
	
	SetMessageBoxNeedMoney();
	_messageBoxLayer->setVisible(true);


    return true;
}


void MainMenuScene::update(float dt)
{
	
}


void MainMenuScene::menuClickCallback(CCObject* pSender)
{
    int tag = ((CCNode*)pSender)->getTag();
    switch (tag) {
        case 1: // close button
            break;
            
        case 2: // clear button
            break;
            
        case 3: // check button
            break;
			
		case ID_BUTTON_CLOSE:
			_messageBoxLayer->setVisible(false);
			break;

        default:
            break;
    }
}

void MainMenuScene::SetMessageBoxNeedMoney()
{
	_messageBoxLayer = CCLayer::create();
	_messageBoxLayer->setPosition(_visibleSize/2);
	this->addChild(_messageBoxLayer, 10);
	
	CCSprite *back = CCSprite::create("messagebox_needmoney.png");
	back->setPosition(CCPointZero);
	_messageBoxLayer->addChild(back);
	
	CCMenuItemImage *closebutton = CCMenuItemImage::create("button_close.png",
														   "button_close-selected.png",
														   this,
														   menu_selector(MainMenuScene::menuClickCallback) );
	closebutton->setPosition(ccp(back->getContentSize().width/2 - closebutton->getContentSize().width,
								 back->getContentSize().height/2 - closebutton->getContentSize().height));
	closebutton->setTag(ID_BUTTON_CLOSE);
	
	CCMenu *pMenu = CCMenu::create( closebutton, NULL );
	pMenu->setPosition(CCPointZero);
	_messageBoxLayer->addChild(pMenu);
}