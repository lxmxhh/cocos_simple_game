#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}
    
	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
    
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

HelloWorld::HelloWorld()
:_targets(NULL)
,_projectiles(NULL)
,_projectilesDestroyed(0)
{

}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    //测试代码
//    Vector<Value*>* vecV = new Vector<Value*>();
// 
//    Value* val1 = new Value(1);
//    Value* val2 = new Value(2);
//
//    vecV->pushBack(val1);
//    vecV->pushBack(val2);
    
    Value val("-656A");

    if ( val.isNull() )
    {
        CCLOG("========= val is null ==============");
    }
    else{
        std::string strValDesc = val.getDescription();
        CCLOG("======== val is %s ============ ", strValDesc.c_str());
        
        CCLOG("========= val.asByte() = %c ==========",val.asByte());
        CCLOG("========= val.asInt() = %d ==========",val.asInt());
        CCLOG("========= val.asString() = %s ==========",val.asString().c_str());
    }
    
    //生成玩家
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    Sprite *player = Sprite::create("Player.png",
                                    cocos2d::Rect(0, 0, 27, 40) );
    player->setPosition( Point(player->getContentSize().width/2, winSize.height/2) );
    this->addChild(player);
    
    //设置schedule，每1秒生成1个怪物
    this->schedule( schedule_selector(HelloWorld::gameLogic), 0.5 );
    
    //this->setTouchEnabled(true);
    
    //_targets = CCArray::create();
    //_targets = new Array();
    
    _targets = __Array::create();
    _targets->retain();
    //_targets->init();
    //_projectiles = CCArray::create();
    //_projectiles = new Array();
    _projectiles = __Array::create();
    _projectiles->retain();
    //_projectiles->init();
    
    // use updateGame instead of update, otherwise it will conflit with SelectorProtocol::update
    // see http://www.cocos2d-x.org/boards/6/topics/1478
    //this->schedule( schedule_selector(HelloWorld::updateGame) );
    
    return true;
}
void HelloWorld::addTarget()
{
	Sprite *target = Sprite::create("Target.png",
                                                Rect(0,0,27,40) );
    
	// Determine where to spawn the target along the Y axis
	Size winSize = Director::getInstance()->getVisibleSize();
	float minY = target->getContentSize().height/2;
	float maxY = winSize.height -  target->getContentSize().height/2;
	int rangeY = (int)(maxY - minY);
	// srand( TimGetTicks() );
	int actualY = ( rand() % rangeY ) + (int)minY;
    
	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	target->setPosition(
                        Point(winSize.width + (target->getContentSize().width/2),
                            Director::getInstance()->getVisibleOrigin().y + actualY) );
	this->addChild(target);
    
	// Determine speed of the target
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	// srand( TimGetTicks() );
	int actualDuration = ( rand() % rangeDuration ) + minDuration;

    // Create the actions
	FiniteTimeAction* actionMove = MoveTo::create( (float)actualDuration,
                                                  Point(0 - target->getContentSize().width/2, winSize.height/2));//actualY) );
    
    // 设置结束的action
	FiniteTimeAction* actionMoveDone = CallFuncN::create( CC_CALLBACK_1(HelloWorld::spriteMoveFinished, this));

    // 设置动作序列, 完成actionMove后, 执行actionMoveDone
	target->runAction( Sequence::create(actionMove, actionMoveDone, NULL) );

	// deprecated calls
    //Create the actions
//	CCFiniteTimeAction* actionMove =
//    CCMoveTo::initWithDuration( (float)actualDuration,
//                                 Point(0 - target->getContentSize().width/2, actualY) );
//	CCFiniteTimeAction* actionMoveDone =
//    CCCallFuncN::actionWithTarget( this,
//                                  callfuncN_selector(HelloWorld::spriteMoveFinished));
//	target->runAction( CCSequence::_actions(actionMove,
//                                           actionMoveDone, NULL) );
    
	// Add to targets array
	target->setTag(1);
	_targets->addObject(target);
    //_vTargets.push_back(target);
}

void HelloWorld::spriteMoveFinished(Node* sender)
{
	Sprite *sprite = (Sprite *)sender;
	this->removeChild(sprite, true);
    
	if (sprite->getTag() == 1)  // target
	{
		_targets->removeObject(sprite);
//        std::vector<Sprite*>::iterator it = _vTargets.begin();
//        std::vector<Sprite*>::iterator itEnd = _vTargets.end();
//        for (; it != itEnd; ++it) {
//            if ( *it == sprite )
//            {
//                _vTargets.erase(it);
//                break;
//            }
//        }
//        
//		GameOverScene *gameOverScene = GameOverScene::node();
//		gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
//		CCDirector::sharedDirector()->replaceScene(gameOverScene);
        
	}
	else if (sprite->getTag() == 2) // projectile
	{
		_projectiles->removeObject(sprite);
	}
}

void HelloWorld::gameLogic(float dt)
{
    //生成怪物
	this->addTarget();
}

void HelloWorld::updateGame(float dt)
{
    //CCLOG("========= test =========");
}

void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
