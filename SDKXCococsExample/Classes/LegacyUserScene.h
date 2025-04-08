//
//  LegacyUserScene.hpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 14/02/25.
//

#ifndef LegacyUserScene_hpp
#define LegacyUserScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "ui/CocosGUI.h"

class LegacyUserScene : public cocos2d::Scene {
public:
    virtual bool init();

    static void login(cocos2d::ValueMap user);
    static void logout();

    CREATE_FUNC(LegacyUserScene);
};

#endif /* LegacyUserScene_hpp */
