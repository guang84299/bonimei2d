//
//  MainScene2D.cpp
//  wallmm
//
//  Created by YanChunGuang on 16/6/17.
//
//

#include "MainScene2D.h"

#include "audio/include/SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#endif


using namespace CocosDenshion;

#define DRAG_BODYS_TAG 1


bool MainScene2D::init()
{
    if (!Scene::init() )
    {
        return false;
    }
    
    _debugDraw = false;
    
    if(initWithPhysics())
    {
        _physicsWorld->setGravity(Vec2(0, -980));
        
        Size size = Director::getInstance()->getWinSize();
        Vec2 oriv = Director::getInstance()->getVisibleOrigin();
        
        int boder_left = 2;
        int boder_right = size.width-2;
        int boder_top = size.height;
        int boder_bottom = 0;
        
        auto bg = Sprite::create("bg.jpg");
        bg->setPosition(size.width/2, size.height/2);
        this->addChild(bg);
        
        auto bottom = Sprite::create("bottom.png");
        bottom->setAnchorPoint(Vec2(0.5,0));
        bottom->setPosition(size.width/2, 0);
        this->addChild(bottom);
        
        auto qq = Sprite::create("qq.png");
        qq->setPosition(mx(240), 315);
        qq->setScale(0.6);
        this->addChild(qq);
        
        auto qq2 = Sprite::create("qq.png");
        qq2->setPosition(mx(940), 315);
        qq2->setScale(0.6);
        this->addChild(qq2);
        
        auto tanzi = Sprite::create("tanzi.png");
        tanzi->setPosition(size.width/2, 160);
        this->addChild(tanzi);
        
        auto uilayer = Layer::create();
        uilayer->setName("uilayer");
        this->addChild(uilayer,100);
        
        auto diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(0,size.height);
        uilayer->addChild(diban);
        Button* btn = Button::create("home.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("home");
        
        diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(100,size.height);
        uilayer->addChild(diban);
        btn = Button::create("music.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("music");
        if(!UserDefault::getInstance()->getBoolForKey("music"))
        {
            btn->loadTextureNormal("music2.png");
        }
        
        diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(200,size.height);
        uilayer->addChild(diban);
        btn = Button::create("sound.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("sound");
        if(!UserDefault::getInstance()->getBoolForKey("sound"))
        {
            btn->loadTextureNormal("sound2.png");
        }
        
        diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(size.width*0.8f,size.height);
        uilayer->addChild(diban);
        btn = Button::create("help.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("help");

        if(Director::getInstance()->isDisplayStats())
        {
            // menu for debug layer
            MenuItemFont::setFontSize(18);
            auto item = MenuItemFont::create("Toggle debug", CC_CALLBACK_1(MainScene2D::toggleDebug, this));
            item->setAnchorPoint(Vec2(0,0.5));
            auto menu = Menu::create(item, nullptr);
            menu->setAnchorPoint(Vec2(0,0.5));
            menu->setPosition(Vec2(10, 500));
            uilayer->addChild(menu);
        }
        
        Label * label = Label::createWithSystemFont("", "", 38);
        label->setPosition(size.width/2, size.height*0.6f);
        uilayer->addChild(label,100);
        label->setOpacity(0);
        label->setName("label");
        
        l_score = Label::createWithSystemFont("", "", 28);
        l_score->setAnchorPoint(Vec2(1,1));
        l_score->setPosition(size.width-10, size.height-10);
        uilayer->addChild(l_score,100);

        
        _gameLayer = Layer::create();
        this->addChild(_gameLayer);
        
        auto node = Node::create();
        node->addComponent(PhysicsBody::createEdgeSegment(Vec2(boder_left, boder_bottom), Vec2(boder_left, boder_top),PhysicsMaterial(0.1f,1,1.1f)));
        _gameLayer->addChild(node);
        
        node = Node::create();
        node->addComponent(PhysicsBody::createEdgeSegment(Vec2(boder_right, boder_bottom), Vec2(boder_right, boder_top),PhysicsMaterial(0.1f,1,1.1f)));
        _gameLayer->addChild(node);
        
        node = Node::create();
        node->addComponent(PhysicsBody::createEdgeSegment(Vec2(boder_left, boder_bottom), Vec2(boder_right, boder_bottom),PhysicsMaterial(0.1f,0.1f,0.1f)));
        _gameLayer->addChild(node);
        
        node = Node::create();
        top = PhysicsBody::createEdgeSegment(Vec2(boder_left, boder_top), Vec2(boder_right, boder_top),PhysicsMaterial(0.1f,0.1f,0.1f));
        node->addComponent(top);
        _gameLayer->addChild(node);
        
        
        node = Node::create();
        Vec2 edges[] = {Vec2(mx(0), 78),Vec2(mx(54), 102),Vec2(mx(155), 112),Vec2(mx(285), 110),Vec2(mx(340), 91),Vec2(mx(374), 56),Vec2(mx(410), 93),Vec2(mx(455), 110),Vec2(mx(500), 112),Vec2(mx(715), 112),Vec2(mx(770), 100),Vec2(mx(822), 57),Vec2(mx(860), 91),Vec2(mx(905), 108),Vec2(mx(970), 112),Vec2(mx(1050), 110),Vec2(mx(1145), 100),Vec2(mx(1200), 78)};
        node->addComponent(PhysicsBody::createEdgeChain(edges, 18));
        _gameLayer->addChild(node);
        
        node = Node::create();
        auto circle_body = PhysicsBody::createCircle(30,PhysicsMaterial(0.f,0.1f,0.1f));
        circle_body->setDynamic(false);
        node->addComponent(circle_body);
        node->setPosition(qq->getPosition());
        _gameLayer->addChild(node);
        
        node = Node::create();
        auto circle_body2 = PhysicsBody::createCircle(30,PhysicsMaterial(0.f,0.1f,0.1f));
        circle_body2->setDynamic(false);
        node->addComponent(circle_body2);
        node->setPosition(qq2->getPosition());
        _gameLayer->addChild(node);
        
        node = Node::create();
        Vec2 tv = tanzi->getPosition();
        tv.x = tv.x - 105;
        tv.y = tv.y - 45;
        
        Vec2 edges2[] = {Vec2(tv.x, tv.y+45),Vec2(tv.x+20, tv.y+82),Vec2(tv.x+110, tv.y+89),Vec2(tv.x+200, tv.y+82),Vec2(tv.x+217, tv.y+45),Vec2(tv.x+185, tv.y+3),Vec2(tv.x+110, tv.y+0),Vec2(tv.x+30, tv.y+3),Vec2(tv.x, tv.y+45)};
        node->addComponent(PhysicsBody::createEdgeChain(edges2, 9));
        _gameLayer->addChild(node);

        
        
//        hand = Sprite::create("hand.png");
//        hand->setPosition(size.width*0.9f, size.height*0.9f);
//        _gameLayer->addChild(hand,1000);

        _physicsWorld->setAutoStep(false);
//        auto contactListener = EventListenerPhysicsContact::create();
//        contactListener->onContactBegin = CC_CALLBACK_1(MainScene2D::onContactBegin, this);
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
        
        schedule(CC_SCHEDULE_SELECTOR(MainScene2D::update));
        
        playMusic();
        
        v_font =  FileUtils::getInstance()->getValueVectorFromFile("fonts/font.plist");
        
        auto rain = ParticleRain::create();
        rain->setPosition(Vec2(size.width/2,size.height));
        this->addChild(rain);
    }
    
    return true;
}

bool MainScene2D::onContactBegin(PhysicsContact& contact)
{
//    PhysicsBody* a = contact.getShapeA()->getBody();
//    PhysicsBody* b = contact.getShapeB()->getBody();
   
    log("----");
    return true;
}

void MainScene2D::update(float dt)
{
    for (int i = 0; i < 2; ++i)
    {
        _physicsWorld->step(dt/3);
    }
    
    Size size = Director::getInstance()->getWinSize();
    for(int i=0;i<sprites.size();i++)
    {
        if(sprites[i]->getPositionX() > size.width || sprites[i]->getPositionX() < 0
           || sprites[i]->getPositionY() > size.height)
        {
            if(sel == sprites[i])
            {
                sel->runAction(Sequence::create(TintTo::create(0.2f, 255, 255, 255),RemoveSelf::create(), NULL));
            
                score += 1;
                char c[8];
                sprintf(c, "%i", score);
                std::string s = v_font.at(7).asString()+c;
                l_score->setString(s);
                
                int sco =  UserDefault::getInstance()->getIntegerForKey("score");
                if(score > sco)
                {
                    UserDefault::getInstance()->setIntegerForKey("score", score);
                }
            }
            else{
                sprites[i]->runAction(Sequence::create(TintTo::create(0.2f, 255, 255, 255),RemoveSelf::create(), NULL));
                if(sprites[i] == parSp)
                {
                    particle->removeFromParent();
                    particle = nullptr;
                }
            }
            
            removes.push_back(sprites[i]);
            if(vecs.size() && vecs_min.size() && vecs_max.size())
            {
                vecs.erase(vecs.begin()+i);
                vecs_min.erase(vecs_min.begin()+i);
                vecs_max.erase(vecs_max.begin()+i);
            }
            sprites.erase(sprites.begin()+i);

            if(sprites.size() == 0)
            {
                gameWin();
            }
            break;
        }
        
    }
    
    if(sel && isStart)
    {
        for(int i=0;i<sprites.size();i++)
        {
            if(sprites[i] == sel)
                continue;
            if(sprites[i]->getPosition().getDistance(vecs[i]) > 2)
            {
                gameOver(sprites[i]);
                break;
            }
            Vec2 v1,v2;
            Rect r = sprites[i]->getBoundingBox();
            v1.x = r.getMinX();
            v1.y = r.getMinY();
            v2.x = r.getMaxX();
            v2.y = r.getMaxY();
            if(v1.getDistance(vecs_min[i]) > 2)
            {
                gameOver(sprites[i]);
                break;
            }
            if(v2.getDistance(vecs_max[i]) > 2)
            {
                gameOver(sprites[i]);
                break;
            }
        }
    }
    
    
    if(judgeStart)
    {
        dt_stop += dt;
        if(dt_stop > 0.5f)
        {
            dt_stop = 0;
            bool isb = false;
            for(int i=0;i<sprites.size();i++)
            {
                if(sprites[i]->getPosition().getDistance(vecs[i]) > 2)
                {
                    isb = true;
                }
                Vec2 v1,v2;
                Rect r = sprites[i]->getBoundingBox();
                v1.x = r.getMinX();
                v1.y = r.getMinY();
                v2.x = r.getMaxX();
                v2.y = r.getMaxY();
                if(v1.getDistance(vecs_min[i]) > 2)
                {
                    isb = true;
                }
                if(v2.getDistance(vecs_max[i]) > 2)
                {
                    isb = true;
                }
                
                vecs[i] = sprites[i]->getPosition();
                vecs_min[i] = v1;
                vecs_max[i] = v2;
            }
            if(!isb)
            {
                judgeStart = false;
                
                Label * node = (Label *)(this->getChildByName("uilayer")->getChildByName("label"));
                node->setString(v_font.at(5).asString());
                auto action = Sequence::create(CallFunc::create( std::bind(&MainScene2D::startEnd, this)),FadeOut::create(1), nullptr);
                node->runAction(action);
            }
        }
    }

}

void MainScene2D::gameWin()
{
    isStart = false;
  
    int model = UserDefault::getInstance()->getIntegerForKey("game_model");
    if(model == 1)
    {
        int level =  UserDefault::getInstance()->getIntegerForKey("level");
        if(level < 20)
            UserDefault::getInstance()->setIntegerForKey("level", level+1);
    }
    
    this->runAction(Sequence::create(DelayTime::create(3),CallFunc::create(std::bind(&MainScene2D::addGameTouchLayer, this,true)), NULL));
    
    Size size = Director::getInstance()->getWinSize();
    
    Vec2 v(size.width/2,size.height/2);
    
    particle = ParticleSystemQuad::create("huochai.plist");
    particle->setPosition(v+Vec2(0,50));
    _gameLayer->addChild(particle);

    auto seq = Sequence::create(MoveBy::create(0.2, Vec2(100,0)), MoveBy::create(0.2, Vec2(0,-100)),
                                MoveBy::create(0.2, Vec2(-100,0)),MoveBy::create(0.2, Vec2(0,100)),NULL);
    particle->runAction(Sequence::create(Repeat::create(seq, 3),RemoveSelf::create(true) , NULL));
}

void MainScene2D::gameOver(Sprite* sp)
{
    isStart = false;
    sp->runAction(Sequence::create(TintTo::create(0.2f, 0, 0, 0), NULL));
    
    this->runAction(Sequence::create(DelayTime::create(3),CallFunc::create(std::bind(&MainScene2D::addGameTouchLayer, this,false)), NULL));
    
    particle = ParticleSystemQuad::create("huochai.plist");
    particle->setPosition(sp->getPosition());
    _gameLayer->addChild(particle);
    parSp = sp;
    schedule(CC_SCHEDULE_SELECTOR(MainScene2D::updateParticle));
}

void MainScene2D::updateParticle(float dt)
{
    if(particle && parSp)
    {
        particle->setPosition(parSp->getPosition());
    }
    else{
         unschedule(CC_SCHEDULE_SELECTOR(MainScene2D::updateParticle));
    }
}

void MainScene2D::addGameTouchLayer(bool isWin)
{
    auto layer = TouchLayer::create();
    layer->setName("touchLayer");
    this->addChild(layer, 101);
    
    Size size = Director::getInstance()->getWinSize();
    
    if(isWin)
    {
        std::string s = v_font.at(3).asString();
        Text* label = Text::create(s, "", 50);
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(size.width / 2.0f, size.height * 0.75f));
        layer->addChild(label);
        
        auto diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.3f, size.height * 0.5f));
        layer->addChild(diban);
        Button* btn = Button::create("home.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("home");
        
        diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.7f, size.height * 0.5f));
        layer->addChild(diban);
        btn = Button::create("jixu.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("replay");
        
        
        btn = Button::create("fenxiang2.png");
        btn->setPosition(Vec2(size.width*0.5f, size.height * 0.3f));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        layer->addChild(btn);
        btn->setName("fenxiang");
    }
    else
    {
        std::string s = v_font.at(2).asString();
        Text* label = Text::create(s, "", 50);
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(size.width / 2.0f, size.height * 0.75f));
        layer->addChild(label);
        
        auto diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.3f, size.height * 0.5f));
        layer->addChild(diban);
        Button* btn = Button::create("home.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("home");
        
        diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.7f, size.height * 0.5f));
        layer->addChild(diban);
        btn = Button::create("replay.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("replay");
        
        btn = Button::create("fenxiang2.png");
        btn->setPosition(Vec2(size.width*0.5f, size.height * 0.3f));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        layer->addChild(btn);
        btn->setName("fenxiang");
        
        if(particle)
        {
            particle->removeFromParent();
            particle = nullptr;
        }
       
    }
    
}

void MainScene2D::startGame(Ref *sender)
{
    if(sender)
        playSound("btn.wav");
    
    isStart = false;
    judgeStart = false;
    sel = nullptr;
    score = 0;
    dt_stop = 0;
    
    l_score->setString(v_font.at(7).asString()+"0");
    
    preInit();
}

void MainScene2D::preInit()
{
    top->setEnabled(true);
    for(int i=0;i<sprites.size();i++)
    {
        sprites[i]->removeFromParent();
    }
    sprites.clear();
    removes.clear();
    
    vecs.clear();
    vecs_min.clear();
    vecs_max.clear();

    
     Size size = Director::getInstance()->getWinSize();
    int model = UserDefault::getInstance()->getIntegerForKey("game_model");
    int r = 3;
    if(model == 1)
    {
        int level =  UserDefault::getInstance()->getIntegerForKey("level");
        if(level == 0)
        {
            UserDefault::getInstance()->setIntegerForKey("level", r);
        }
        else
        {
            r = level;
        }
    }
    else{
        r = random(4, 20);
    }
    for(int i=0;i<r;i++)
    {
        addSprite(Vec2(size.width/2,size.height*0.6f));
    }
//
//    auto action = Sequence::create(DelayTime::create(0.8f), CallFunc::create( std::bind(&MainScene2D::start, this)),nullptr);
//    this->runAction(action);
    start();
   
}

void MainScene2D::start()
{
    Vec2 v(1500,1);
    Vec2 v1,v2;
    int ra = random(30, 90);
    for(int i=0;i<sprites.size();i++)
    {
        PhysicsBody* body = (PhysicsBody*)sprites[i]->getComponent("box");
        body->setEnabled(true);
        v.rotate(Vec2::ZERO, ra);
        body->setVelocity(v);
        
        vecs.push_back(sprites[i]->getPosition());
        Rect r = sprites[i]->getBoundingBox();
        v1.x = r.getMinX();
        v1.y = r.getMinY();
        v2.x = r.getMaxX();
        v2.y = r.getMaxY();
        vecs_min.push_back(v1);
        vecs_max.push_back(v2);
    }

    judgeStart = true;
    dt_stop = 0;
    Label * node = (Label *)(this->getChildByName("uilayer")->getChildByName("label"));
    node->setOpacity(255);
    node->setString(v_font.at(4).asString());
}

void MainScene2D::startEnd()
{
    top->setEnabled(false);
    isStart = true;
    log("start game");
    //hand->setVisible(true);
}

void MainScene2D::addSprite(Vec2 v)
{
    int z = random(1, 100);
    
    auto ball = Sprite::create("huochai.png");
    //ball->setAnchorPoint(Vec2::ZERO);
    auto body = PhysicsBody::createBox(Size(16,220),PhysicsMaterial(1,0.8,1));
    body->setName("box");
    body->setDynamic(true);
    body->setEnabled(false);
    ball->addComponent(body);
    body->setTag(DRAG_BODYS_TAG);
    ball->setPosition(v);
    
    //body->setCategoryBitmask(z-1);    // 1000
   // body->setContactTestBitmask(z-2); // 0010
   // body->setCollisionBitmask(2);

    _gameLayer->addChild(ball,z);
    sprites.push_back(ball);
}

void MainScene2D::getSel(Touch* touch)
{
    auto location = touch->getLocation();
    Vec2 v = location;
    auto arr = _physicsWorld->getShapes(v);
    
    PhysicsBody* body = nullptr;
    for (auto& obj : arr)
    {
        if ((obj->getBody()->getTag() & DRAG_BODYS_TAG) != 0)
        {
            body = obj->getBody();
            break;
        }
    }
    
    if (body != nullptr)
    {
        Node* mouse = Node::create();
        auto physicsBody = PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY);
        physicsBody->setDynamic(false);
        mouse->addComponent(physicsBody);
        mouse->setPosition(v);
        this->addChild(mouse);
        
        
        for(int i=0;i<sprites.size();i++)
        {
            if(sprites[i]->getComponent("box") == body)
            {
                sel = sprites[i];
                sprites[i]->runAction(TintTo::create(0.2f, 255, 0, 0));
                playSound("sel.wav");
                break;
            }
        }
        
        PhysicsJointPin* joint = PhysicsJointPin::construct(physicsBody, body, v);
        joint->setMaxForce(8000.0f * body->getMass());
        _physicsWorld->addJoint(joint);
        _mouses.insert(std::make_pair(touch->getID(), mouse));
        
        if(sel)
        {
            vecs.clear();
            vecs_min.clear();
            vecs_max.clear();
            Vec2 v1,v2;
            for(int i=0;i<sprites.size();i++)
            {
                vecs.push_back(sprites[i]->getPosition());
                Rect r = sprites[i]->getBoundingBox();
                v1.x = r.getMinX();
                v1.y = r.getMinY();
                v2.x = r.getMaxX();
                v2.y = r.getMaxY();
                vecs_min.push_back(v1);
                vecs_max.push_back(v2);
            }
            
        }
    }

}

bool MainScene2D::onTouchBegan(Touch* touch, Event* event)
{
    if(!isStart)
        return false;
    sel = nullptr;
    getSel(touch);
    lastDir = touch->getLocation();
    return true;
}

void MainScene2D::onTouchMoved(Touch* touch, Event* event)
{
    if(!isStart)
        return ;
   
    if(sel)
    {
        auto it = _mouses.find(touch->getID());
        
        if (it != _mouses.end())
        {
            it->second->setPosition(touch->getLocation());
        }
        lastDir = touch->getPreviousLocation();
    }
    else
    {
        getSel(touch);
    }
}

void MainScene2D::onTouchEnded(Touch* touch, Event* event)
{
    if(sel)
    {
        bool isRemove = false;
        for(int i=0;i<removes.size();i++)
        {
            if(sel == removes[i])
            {
                isRemove = true;
                removes.erase(removes.begin()+i);
                break;
            }
        }
        if(!isRemove)
            sel->runAction(TintTo::create(0.2f, 255, 255, 255));
        
        auto it = _mouses.find(touch->getID());
        
        if (it != _mouses.end())
        {
            this->removeChild(it->second);
            _mouses.erase(it);
        }
        if(touch->getLocation().getDistance(lastDir) > 5)
        {
            PhysicsBody* body = (PhysicsBody*)sel->getComponent("box");
            Vec2 dir =  touch->getLocation() - lastDir;
            dir.normalize();
            body->setVelocity(dir*2000);
        }
        
    }
}

void MainScene2D::onEnter()
{
    Scene::onEnter();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MainScene2D::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(MainScene2D::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MainScene2D::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create(std::bind(&MainScene2D::startGame, this,nullptr)), NULL));
     //startGame(nullptr);
}


void MainScene2D::toggleDebug(Ref* sender)
{
    playSound("btn.wav");
#if CC_USE_PHYSICS
    _debugDraw = !_debugDraw;
    _physicsWorld->setDebugDrawMask(_debugDraw ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
#endif
}

void MainScene2D::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            playSound("btn.wav");
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
            
            if(name == "home")
            {
                auto scene = TransitionFade::create(1, HomeScene::create(), Color3B::WHITE);
                Director::getInstance()->replaceScene(scene);
                stopMusic();
            }
            else if(name == "music")
            {
                bool music = UserDefault::getInstance()->getBoolForKey("music");
                UserDefault::getInstance()->setBoolForKey("music", !music);
               if(music)
               {
                    pauseMusic();
                   btn->loadTextureNormal("music2.png");
               }
               else
               {
                   resumeMusic();
                   btn->loadTextureNormal("music.png");
               }
            }
            else if(name == "sound")
            {
                bool sound = UserDefault::getInstance()->getBoolForKey("sound");
                UserDefault::getInstance()->setBoolForKey("sound", !sound);
                if(sound)
                {
                    btn->loadTextureNormal("sound2.png");
                }
                else
                {
                    btn->loadTextureNormal("sound.png");
                }
            }
            else if(name == "help")
            {
                createPages();
            }
            else if(name == "close")
            {
                Node* node = this->getChildByName("pageView");
                if(node)
                    node->removeFromParent();
            }
            else if(name == "replay")
            {
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
                startGame(nullptr);
            }
            else if(name == "fenxiang")
            {
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
                fenxiang(2);
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void MainScene2D::playMusic()
{
    if(UserDefault::getInstance()->getBoolForKey("music"))
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bg.mp3",true);
}

void MainScene2D::pauseMusic()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void MainScene2D::stopMusic()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
void MainScene2D::resumeMusic()
{
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        playMusic();

}

void MainScene2D::playSound(std::string sound)
{
    if(UserDefault::getInstance()->getBoolForKey("sound"))
        SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
}

void MainScene2D::createPages()
{
    Node* node = this->getChildByName("pageView");
    if(node)
        node->removeFromParent();
    Size winSize = Director::getInstance()->getWinSize();
    // Create the page view
    PageView* pageView = PageView::create();
    pageView->setName("pageView");
    pageView->setDirection(PageView::Direction::VERTICAL);
    pageView->setContentSize(winSize);
    pageView->setPosition(Vec2(0,0));
    pageView->setCustomScrollThreshold(40);
    
    pageView->removeAllPages();
    
    
    int pageCount = 2;
    for (int i = 0; i < pageCount; ++i)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(winSize);
    
        
        ImageView* bg = ImageView::create("bg.jpg");
        // bg->setContentSize(winSize);
        //bg->setAnchorPoint(Vec2(0.5,0.5));
        bg->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
        layout->addChild(bg);
        
        
        ImageView* huocai = ImageView::create("huochai.png");
        huocai->setAnchorPoint(Vec2(0.5,0.5));
        huocai->setPosition(Vec2(layout->getContentSize().width / 2.0f, 150.0f));
        layout->addChild(huocai);
        
        ImageView* shou = ImageView::create("hand.png");
        shou->setAnchorPoint(Vec2(0.5,0.5));
        shou->setPosition(huocai->getPosition() + Vec2(100, -80));
        layout->addChild(shou);
        
        if(i == 0)
        {
            auto action = Sequence::create(MoveTo::create(1, huocai->getPosition() + Vec2(23, -25)),MoveTo::create(2, Vec2(huocai->getPositionX(),450) + Vec2(23, -25)),FadeOut::create(1.1), MoveTo::create(0.f,huocai->getPosition() + Vec2(100, -80)),FadeIn::create(1), nullptr);
            shou->runAction(RepeatForever::create(action));
            
            auto action2 = Sequence::create(DelayTime::create(0.9),TintTo::create(0.1, 255, 0, 0),MoveTo::create(2, Vec2(huocai->getPositionX(),450)),TintTo::create(0.1, 255, 255, 255),FadeOut::create(1), MoveTo::create(0.f,huocai->getPosition()),FadeIn::create(1), nullptr);
            huocai->runAction(RepeatForever::create(action2));
            
            ImageView* shou2 = ImageView::create("hand.png");
            shou2->setAnchorPoint(Vec2(0.5,0.5));
            shou2->setRotation(210);
            shou2->setPosition(Vec2(layout->getContentSize().width-100, 80));
            layout->addChild(shou2);
            
            std::string s = v_font.at(6).asString();
            Text* label = Text::create(s, "", 18);
            label->setColor(Color3B::RED);
            label->setPosition(shou2->getPosition() + Vec2(0, -45));
            layout->addChild(label);

            
            auto action3 = Sequence::create(Blink::create(1, 3),DelayTime::create(2), nullptr);
            shou2->runAction(RepeatForever::create(action3));
        }
        else if(i == 1){
            shou->setPosition(Vec2(huocai->getPositionX(),450) + Vec2(23, -25));
            huocai->setPosition(Vec2(huocai->getPositionX(),450));
            
            auto action = Sequence::create(EaseOut::create(MoveBy::create(0.2f, Vec2(100,100)), 0.1f),FadeOut::create(1),MoveTo::create(0.f,shou->getPosition()), FadeIn::create(1),nullptr);
            shou->runAction(RepeatForever::create(action));
            
            auto sp = Spawn::create(RotateTo::create(0.2, 90),EaseOut::create(MoveBy::create(0.2f, Vec2(180,180)), 0.1f) ,nullptr);
            auto action2 = Sequence::create(TintTo::create(0.f, 255, 0, 0),sp,FadeOut::create(1),MoveTo::create(0.f,huocai->getPosition()), TintTo::create(0.f, 255, 255, 255),RotateTo::create(0.f, 0),FadeIn::create(1),nullptr);
            huocai->runAction(RepeatForever::create(action2));
        }
        
        std::string s = v_font.at(i).asString();
        Text* label = Text::create(s, "", 30);
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height * 0.9f));
        layout->addChild(label);
        
        pageView->addPage(layout);
    }
    
    //pageView->removePageAtIndex(0);
   // pageView->scrollToPage(0);
    
    Button* btn = Button::create("002.png","003.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setPosition(Vec2(winSize.width-10,winSize.height-10));
    btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
    pageView->addChild(btn);
    btn->setName("close");
    
    pageView->addEventListener(CC_CALLBACK_2(MainScene2D::pageViewEvent, this));
    
    this->addChild(pageView,100);
}

void MainScene2D::pageViewEvent(Ref *pSender, PageView::EventType type)
{
    switch (type)
    {
        case PageView::EventType::TURNING:
        {
           // PageView* pageView = dynamic_cast<PageView*>(pSender);
            
//            _displayValueLabel->setString(StringUtils::format("page = %ld", pageView->getCurPageIndex() + 1));
        }
            break;
            
        default:
            break;
    }
}

float MainScene2D::mx(float x)
{
    x -= 32;
    Size s = Director::getInstance()->getWinSize();
    float sca = (1136.0f-s.width)/2.0f;
    return x-sca;
}


//------------------------------------
static bool jump_home = false;
bool HomeScene::init()
{
    if (!Scene::init() )
    {
        return false;
    }
    
    if(jump_home)
    {
        goHome();
        return true;
    }
    
    Size s = Director::getInstance()->getWinSize();
    
    auto layer = LayerColor::create(Color4B::WHITE);
    this->addChild(layer);
    
    auto logo = Sprite::create("logo.png");
    logo->setPosition(s.width/2,s.height/2);
    this->addChild(logo);
    
    bool first = UserDefault::getInstance()->getBoolForKey("first");
    if(!first)
    {
        UserDefault::getInstance()->setBoolForKey("music", true);
        UserDefault::getInstance()->setBoolForKey("sound", true);
        UserDefault::getInstance()->setBoolForKey("first", true);
    }
    
    this->runAction(Sequence::create(DelayTime::create(3),CallFunc::create(std::bind(&HomeScene::goHome, this)), NULL));

    jump_home = true;
    return true;
}

void HomeScene::goHome()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto homebg = Sprite::create("home.jpg");
    homebg->setPosition(s.width/2,s.height/2);
    this->addChild(homebg);
 

    Button* btn_start = Button::create("cuangguan.png");
    btn_start->setPosition(Vec2(s.width/2.0f,s.height*0.7f));
    btn_start->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    this->addChild(btn_start);
    btn_start->setName("guoguan");
    
    btn_start = Button::create("suiji.png");
    btn_start->setPosition(Vec2(s.width/2.0f,s.height*0.5f));
    btn_start->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    this->addChild(btn_start);
    btn_start->setName("suiji");
    
    auto diban = Sprite::create("diban.png");
    diban->setPosition(s.width/3.0f,s.height*0.2f);
    this->addChild(diban);
    
    Button* btn = Button::create("fankui.png");
    btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
    btn->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    diban->addChild(btn);
    btn->setName("fankui");
    
    
    diban = Sprite::create("diban.png");
    diban->setPosition(s.width/3.0f*2,s.height*0.2f);
    this->addChild(diban);
    
    btn = Button::create("fenxiang.png");
    btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
    btn->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    diban->addChild(btn);
    btn->setName("fenxiang");
    //Director::getInstance()->replaceScene(MainScene2D::create());
}

void HomeScene::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
             MainScene2D::playSound("btn.wav");
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
           
            if(name == "guoguan")
            {
                UserDefault::getInstance()->setIntegerForKey("game_model", 1);
                auto scene = TransitionFade::create(1, MainScene2D::create(), Color3B::WHITE);
                Director::getInstance()->replaceScene(scene);
            }
            else if(name == "suiji")
            {
                UserDefault::getInstance()->setIntegerForKey("game_model", 2);
                auto scene = TransitionFade::create(1, MainScene2D::create(), Color3B::WHITE);
                Director::getInstance()->replaceScene(scene);
            }
            else if(name == "fankui")
            {
                
            }
            else if(name == "fenxiang")
            {
                MainScene2D::fenxiang(1);
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}


bool TouchLayer::init()
{
    if (!Layer::init() )
    {
        return false;
    }
    Size s = Director::getInstance()->getWinSize();
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 160),s.width,s.height);
    this->addChild(bg);
    
    return true;
}

bool TouchLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void TouchLayer::onEnter()
{
    Layer::onEnter();
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TouchLayer::onTouchBegan, this);
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
   
}

void TouchLayer::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(touchListener);
}



void MainScene2D::fenxiang(int f)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/xugu/bonimei2d/AppActivity", "fenxiang","(I)V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, f);
    }
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
//    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_LiveWallReceiver_center(JNIEnv *env, jobject obj)
//    {
//        return;
//    }
//    
//    JNIEXPORT void JNICALL Java_com_qinglu_livewall_LiveWallpaperService_clearBitmaps(JNIEnv *env, jobject obj)
//    {
//        return;
//    }
}
#endif