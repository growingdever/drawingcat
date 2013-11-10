#include "HelloWorldScene.h"
#import <opencv2/opencv.hpp>

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
    pCheckButton->setPosition( ccp( pCheckButton->getContentSize().width/2, visibleSize.height - 100) );
    pCheckButton->setTag(3);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pClearButton, pCheckButton, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    board = CCRenderTexture::create(visibleSize.width, visibleSize.height,
                                     kCCTexture2DPixelFormat_RGBA8888);
    board->retain();
    board->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
    
    this->addChild(board);
    
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
            break;
            
        default:
            break;
    }
}

void HelloWorld::CheckBoard()
{
	const char *BOARD_IMAGE_PATH = "board_content.png";
	const char *RESULT_IMAGE_PATH = "check_result.png";
	
    CCImage *texImg = board->newCCImage();
	texImg->saveToFile(BOARD_IMAGE_PATH);
	
	IplImage *src_image = cvLoadImage( BOARD_IMAGE_PATH, CV_LOAD_IMAGE_UNCHANGED );
	IplImage *dst = cvCreateImage( cvGetSize(src_image), 8, 1 );
	IplImage *gray = cvCreateImage( cvGetSize(src_image), 8, 1 );
	IplImage *color_dst = cvCreateImage( cvGetSize(src_image), 8, 3 );
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i;
	cvCvtColor( src_image, gray, CV_BGR2GRAY );//그레이 이미지로 변환
	cvCanny( gray, dst, 50, 200, 3 );//경계선 이미지로 변환(이진화)
	cvCvtColor( dst, color_dst, CV_GRAY2BGR );
	
	//CV_HOUGH_STANDARD모드
	/*
	 lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );
	 for( i = 0; i < MIN(lines->total,100); i++ )
	 {    //추출 된 직선의 특징을 여기서 찾아낼 수 있다.
	 float* line = (float*)cvGetSeqElem(lines,i);
	 float rho = line[0];
	 float theta = line[1];
	 CvPoint pt1, pt2;
	 double a = cos(theta), b = sin(theta);
	 double x0 = a*rho, y0 = b*rho;
	 pt1.x = cvRound(x0 + 1000*(-b));
	 pt1.y = cvRound(y0 + 1000*(a));
	 pt2.x = cvRound(x0 - 1000*(-b));
	 pt2.y = cvRound(y0 - 1000*(a));
	 cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, 8 );
	 }
	 */
	//CV_HOUGH_PROBABILISTIC 모드
	lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 5 );
	for( i = 0; i < lines->total; i++ )
	{     //추출 된 직선의 특징을 여기서 찾아낼 수 있다.
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );
	}
	cvSaveImage(RESULT_IMAGE_PATH, color_dst);
	
	CCSprite *result = CCSprite::create(RESULT_IMAGE_PATH);
	result->setPosition( ccp(100, 100) );
	this->addChild(result);

//    unsigned char *data = texImg->getData();
//    
//	CCTexture2D *tex = board->getSprite()->getTexture();
	
	
	
//	IplImage *src = cvLoadImage(<#const char *filename#>)
}
