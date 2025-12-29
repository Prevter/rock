#include <rock/RoundedRect.hpp>

using namespace cocos2d;
using namespace rock;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
    bool init() override {
        if (!MenuLayer::init()) return false;

        auto winSize = CCDirector::get()->getWinSize();

        auto roundedRect = RoundedRect::create(
            {200, 150, 250, 230},
            {40.f, 20.f, 60.f, 10.f},
            {200.f, 100.f}
        );

        auto roundedSprite = RoundedSprite::create(
            "groundSquare_15_001.png",
            Radii(30.f, 10.f, 50.f, 5.f)
        );

        roundedRect->setPosition(winSize*0.5f - ccp(150, 0));
        roundedSprite->setPosition(winSize*0.5f + ccp(150, 0));
        this->addChild(roundedRect);
        this->addChild(roundedSprite);

        return true;
    }
};