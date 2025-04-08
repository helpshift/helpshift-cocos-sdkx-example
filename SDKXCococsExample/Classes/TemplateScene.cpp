//
//  TemplateScene.cpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 14/02/25.
//

#include "TemplateScene.h"
#include "CocosUIHelpers.h"
#include "cocos/base/CCDirector.h"

using namespace cocos2d;
using namespace cocos2d::ui;

bool TemplateScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto layout = CocosUIHelpers::createRootView(this);

    // Add UI elements to layout

    return true;
}
