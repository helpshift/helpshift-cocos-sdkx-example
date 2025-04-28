//
//  TemplateScene.hpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 14/02/25.
//

#ifndef TemplateScene_hpp
#define TemplateScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "ui/CocosGUI.h"

// A template scene class for copy pasting when creating a new scene.
class TemplateScene : public cocos2d::Scene {
public:
    virtual bool init();

    CREATE_FUNC(TemplateScene);
};

#endif
