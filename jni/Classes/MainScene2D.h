//
//  MainScene2D.h
//  wallmm
//
//  Created by YanChunGuang on 16/6/17.
//
//

#ifndef MainScene2D_h
#define MainScene2D_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class MainScene2D : public Scene
{
public:
    static MainScene2D* getInstance();
    virtual bool init() override;
    virtual void update(float delta) override;
    
    void preInit();
    void start();
    void startEnd();
    void addSprite(Vec2 v);
    
    static void playSound(std::string sound);
    static float mx(float x);
    
    void getSel(Touch* touch);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onEnter() override;
    
    static void fenxiang(int f);
    static void fankui();
    static void level_guoguan();
    static void level_suiji();
    static void copyStr(std::string str);
    static void showAd(int i);
    
    CREATE_FUNC(MainScene2D);
    
public:
    bool onContactBegin(PhysicsContact& contact);
    void toggleDebug(Ref* sender);
    void startGame(Ref* sender);
    void gameOver(Sprite* sp);
    void gameWin();
    void randZongFen();
    
    void addGameTouchLayer(bool isWin);
    
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
    void playMusic();
    void pauseMusic();
    void stopMusic();
    void resumeMusic();
    
    void updateParticle(float dt);
    
    void createPages();
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    
    void createHongBao();
    
protected:
    Layer* _gameLayer;
    bool _debugDraw;
    bool judgeStart = false;
    bool isStart = false;
    std::unordered_map<int, Node*> _mouses;
    std::vector<Sprite*> sprites;
    std::vector<Sprite*> removes;
    std::vector<Vec2> vecs;
    std::vector<Vec2> vecs_min;
    std::vector<Vec2> vecs_max;
    Sprite* hand;
    Sprite* sel;
    Sprite* parSp;
    PhysicsBody* top;
    ParticleSystemQuad* particle;
    
    int score;
    int zongfen;
    Label* l_score;
    float dt_stop = 0;
    Vec2 lastDir;
    ValueVector v_font;
};

class HomeScene : public Scene
{
public:
     virtual bool init() override;
    void goHome();
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
    CREATE_FUNC(HomeScene);
    
    
    //解压数据
    void decData(float);
    void updateLabel(float);
    
private:
    int _num;
    cocos2d::Label *label_text;
};

class TouchLayer : public Layer
{
public:
    virtual bool init() override;
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(TouchLayer);
    
private:
    
    EventListenerTouchOneByOne* touchListener;
    
};

#endif /* MainScene2D_h */
