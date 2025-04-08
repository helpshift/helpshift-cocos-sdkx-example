//
//  IdentityUserScene.hpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 17/02/25.
//

#ifndef IdentityUserScene_hpp
#define IdentityUserScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "ui/CocosGUI.h"

class IdentityUserScene : public cocos2d::Scene {
public:
    virtual bool init();
    static void loginWithIdentity(const std::string jwt);
    static void addUserIdentities(const std::string jwt);
    static void updateMasterAttributes();
    static void updateAppAttributes();

    CREATE_FUNC(IdentityUserScene);
};

#endif /* IdentityUserScene_hpp */
