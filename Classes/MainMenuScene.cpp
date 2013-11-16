#include "MainMenuScene.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int ID_BUTTON_CLOSE = 10;
const int ID_BUTTON_DRAWING_LINE = 1;
const int ID_BUTTON_DRAWING_FIGURE = 2;
const int ID_BUTTON_DRAWING_HANGUL = 3;
const int ID_BUTTON_DRAWING_ALPHABET = 4;

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
	
	
	CCMenuItemImage *pItem1 = CCMenuItemImage::create("menu_line.png",
													  "menu_line_selected.png",
													  this,
													  menu_selector(MainMenuScene::menuClickCallback));
	pItem1->setPosition(ccp(_visibleSize.width * 0.35, _visibleSize.height * 0.8));
	pItem1->setTag(ID_BUTTON_DRAWING_LINE);
	
	CCMenuItemImage *pItem2 = CCMenuItemImage::create("menu_figure.png",
													  "menu_figure_selected.png",
													  this,
													  menu_selector(MainMenuScene::menuClickCallback));
	pItem2->setPosition(ccp(_visibleSize.width * 0.17, _visibleSize.height * 0.48	));
	pItem2->setTag(ID_BUTTON_DRAWING_FIGURE);
	
	CCMenuItemImage *pItem3 = CCMenuItemImage::create("menu_hangul.png",
													  "menu_hangul_selected.png",
													  this,
													  menu_selector(MainMenuScene::menuClickCallback));
	pItem3->setPosition(ccp(_visibleSize.width * 0.4, _visibleSize.height * 0.25));
	pItem3->setTag(ID_BUTTON_DRAWING_HANGUL);
	
	CCMenuItemImage *pItem4 = CCMenuItemImage::create("menu_alphabet.png",
													  "menu_alphabet_selected.png",
													  this,
													  menu_selector(MainMenuScene::menuClickCallback));
	pItem4->setPosition(ccp(_visibleSize.width * 0.8, _visibleSize.height * 0.35));
	pItem4->setTag(ID_BUTTON_DRAWING_ALPHABET);
	
	CCMenu *pMenu = CCMenu::create(pItem1, pItem2, pItem3, pItem4, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);
	
	SetMessageBoxNeedMoney();


    return true;
}


void MainMenuScene::update(float dt)
{
	
}


void MainMenuScene::menuClickCallback(CCObject* pSender)
{
    int tag = ((CCNode*)pSender)->getTag();
    switch (tag) {
		case ID_BUTTON_DRAWING_LINE:
			break;
			
		case ID_BUTTON_DRAWING_FIGURE:
			break;
			
		case ID_BUTTON_DRAWING_HANGUL:
			_messageBoxLayer->setVisible(true);
			break;
			
		case ID_BUTTON_DRAWING_ALPHABET:
			_messageBoxLayer->setVisible(true);
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