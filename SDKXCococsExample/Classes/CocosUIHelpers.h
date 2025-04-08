//
//  CocosUIHelpers.hpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 14/02/25.
//

#ifndef CocosUIHelpers_hpp
#define CocosUIHelpers_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "ui/CocosGUI.h"
#include "DropdownMenu.h"

using namespace cocos2d;
using namespace cocos2d::ui;

struct UIConstants {
    constexpr static const float headingFontSize = 24;
    constexpr static const float bodyFontSize = 12;
    constexpr static const float smallFontSize = 8;
    constexpr static const float hPadding = 8;
    constexpr static const float vPadding = 8;
};

class CocosUIHelpers {
public:
    static std::pair<Widget *, EditBox *> createTextField(const std::string& placeholder, float fontSize = UIConstants::bodyFontSize);
    static std::pair<Widget *, CheckBox *> createCheckBox(const std::string& label, const Color4B textColor = Color4B::WHITE);
    static Button *createButton(const std::string& title, const std::function<void()>& callback);
    static DropdownMenu *createDropdown(const std::vector<std::string>& options);
    static Text *createText(const std::string& title, float fontSize = UIConstants::bodyFontSize);
    static Margin linearLayoutMargin();
    static Size linearLayoutMarginAdjustedSize(Size size);
    static VBox *createRootView(Scene *scene);
    static void showDialog(const std::string &message);
    static void addMessage(const std::string& eventName, const ValueMap &data);
    static std::string getFormattedEventsText() ;
};

#endif /* CocosUIHelpers_hpp */
