#include "MainMenuScene.h"
#include "cocos-ext.h"
#include "DrawingScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int ID_BUTTON_CLOSE = 10;
const int ID_BUTTON_DRAWING_LINE = 1;
const int ID_BUTTON_DRAWING_FIGURE = 2;
const int ID_BUTTON_DRAWING_HANGUL = 3;
const int ID_BUTTON_DRAWING_ALPHABET = 4;

const int SELECT_IMAGE_WIDTH = 160;
const int SELECT_IMAGE_HEIGHT = 90;

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
	CCSprite *lock1 = CCSprite::create("lock_icon.png");
	lock1->setPosition(ccp(pItem3->getContentSize().width/2, pItem3->getContentSize().height/2));
	pItem3->addChild(lock1);
	
	CCMenuItemImage *pItem4 = CCMenuItemImage::create("menu_alphabet.png",
													  "menu_alphabet_selected.png",
													  this,
													  menu_selector(MainMenuScene::menuClickCallback));
	pItem4->setPosition(ccp(_visibleSize.width * 0.8, _visibleSize.height * 0.35));
	pItem4->setTag(ID_BUTTON_DRAWING_ALPHABET);
	CCSprite *lock2 = CCSprite::create("lock_icon.png");
	lock2->setPosition(ccp(pItem4->getContentSize().width/2, pItem4->getContentSize().height/2));
	pItem4->addChild(lock2);
	
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
			ShowMessageBoxSelectImage(ID_BUTTON_DRAWING_LINE);
			break;
			
		case ID_BUTTON_DRAWING_FIGURE:
			ShowMessageBoxSelectImage(ID_BUTTON_DRAWING_FIGURE);
			break;
			
		case ID_BUTTON_DRAWING_HANGUL:
			_messageBoxLayer->setVisible(true);
			break;
			
		case ID_BUTTON_DRAWING_ALPHABET:
			_messageBoxLayer->setVisible(true);
			break;
			
		case ID_BUTTON_CLOSE:
			_messageBoxLayerSelectImage->setVisible(false);
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
	
	_messageBoxLayer->setVisible(false);
}


void MainMenuScene::menuClickCallbackForSelectImage(CCObject *pSender)
{
	int tag = ((CCNode*)pSender)->getTag();
	CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1, DrawingScene::scene(tag), false));
}

void MainMenuScene::ShowMessageBoxSelectImage(int id)
{
	CCLayer *msgbox = CCLayer::create();
	msgbox->setPosition(_visibleSize/2);
	this->addChild(msgbox, 10);
	
	CCSprite *back = CCSprite::create("messagebox_selectimage.png");
	back->setPosition(CCPointZero);
	msgbox->addChild(back);
	
	if( id == ID_BUTTON_DRAWING_LINE ) {
		CCMenuItemImage *img1 = CCMenuItemImage::create("mask_1.png",
														"mask_1.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img1->setScale(1.0f / 8);
		img1->setPosition(ccp(200, 250));
		img1->setTag(1);
		
		CCMenuItemImage *img2 = CCMenuItemImage::create("mask_2.png",
														"mask_2.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img2->setScale(1.0f / 8);
		img2->setPosition(ccp(380, 250));
		img2->setTag(2);
		
		CCMenuItemImage *img3 = CCMenuItemImage::create("mask_5.png",
														"mask_5.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img3->setScale(1.0f / 8);
		img3->setPosition(ccp(560, 250));
		img3->setTag(5);
		
		CCMenuItemImage *img4 = CCMenuItemImage::create("mask_6.png",
														"mask_6.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img4->setScale(1.0f / 8);
		img4->setPosition(ccp(740, 250));
		img4->setTag(6);
		
		CCMenu *pMenu = CCMenu::create(img1, img2, img3, img4, NULL);
		pMenu->setPosition(CCPointZero);
		back->addChild(pMenu);
		
	} else if( id == ID_BUTTON_DRAWING_FIGURE ) {
		CCMenuItemImage *img1 = CCMenuItemImage::create("mask_11.png",
														"mask_11.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img1->setScale(1.0f / 8);
		img1->setPosition(ccp(200, 250));
		img1->setTag(11);
		
		CCMenuItemImage *img2 = CCMenuItemImage::create("mask_12.png",
														"mask_12.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img2->setScale(1.0f / 8);
		img2->setPosition(ccp(380, 250));
		img2->setTag(12);
		
		CCMenuItemImage *img3 = CCMenuItemImage::create("mask_13.png",
														"mask_13.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img3->setScale(1.0f / 8);
		img3->setPosition(ccp(560, 250));
		img3->setTag(13);
		
		CCMenuItemImage *img4 = CCMenuItemImage::create("mask_14.png",
														"mask_14.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img4->setScale(1.0f / 8);
		img4->setPosition(ccp(740, 250));
		img4->setTag(14);
		
		CCMenuItemImage *img5 = CCMenuItemImage::create("mask_18.png",
														"mask_18.png",
														this,
														menu_selector(MainMenuScene::menuClickCallbackForSelectImage));
		img5->setScale(1.0f / 8);
		img5->setPosition(ccp(200, 140));
		img5->setTag(18);
		
		CCMenu *pMenu = CCMenu::create(img1, img2, img3, img4, img5, NULL);
		pMenu->setPosition(CCPointZero);
		back->addChild(pMenu);
	}
}