//
//  LegacyUserScene.cpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 14/02/25.
//

#include "LegacyUserScene.h"
#include "CocosUIHelpers.h"
#include "cocos/base/CCDirector.h"
#import "HelpshiftCocos2dx.h"

using namespace cocos2d;
using namespace cocos2d::ui;

void addLegacyUserSection(ui::Layout *container) {
    auto firstRow = ui::HBox::create();
    auto userIdTextField = CocosUIHelpers::createTextField("User ID");
    auto userEmailTextField = CocosUIHelpers::createTextField("Email");
    firstRow->addChild(userIdTextField.first);
    firstRow->addChild(userEmailTextField.first);
    firstRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(userIdTextField.first->getContentSize()));
    container->addChild(firstRow);

    auto secondRow = ui::HBox::create();
    auto userNameTextField = CocosUIHelpers::createTextField("Username");
    auto userAuthTextField = CocosUIHelpers::createTextField("Auth Token");
    secondRow->addChild(userNameTextField.first);
    secondRow->addChild(userAuthTextField.first);
    secondRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(userNameTextField.first->getContentSize()));
    container->addChild(secondRow);

    auto thirdRow = ui::HBox::create();
    auto loginButton = CocosUIHelpers::createButton("Login", [userIdTextField, userEmailTextField, userNameTextField, userAuthTextField]() {
        const std::string& userId = userIdTextField.second->getText();
        const std::string& userEmail = userEmailTextField.second->getText();
        const std::string& userName = userNameTextField.second->getText();
        const std::string& userAuth = userAuthTextField.second->getText();
        cocos2d::ValueMap user;
        user["userName"] = userName;
        user["userEmail"] = userEmail;
        user["userId"] = userId;

        if(!userAuth.empty()) {
            user["userAuthToken"] = userAuth;
        }
        LegacyUserScene::login(user);
    });
    auto logoutButton = CocosUIHelpers::createButton("Logout", []() {
        LegacyUserScene::logout();
    });
    thirdRow->addChild(loginButton);
    thirdRow->addChild(logoutButton);
    thirdRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(loginButton->getContentSize()));
    container->addChild(thirdRow);
}

void addTopSectionForLegacyScene(Layout *layout) {

    auto backButton = CocosUIHelpers::createButton("< Back", [] {
        Director::getInstance()->popScene();
    });
    auto eventsButton = CocosUIHelpers::createButton("Events", [] {
        CocosUIHelpers::showDialog(CocosUIHelpers::getFormattedEventsText());
    });

    layout->addChild(backButton);
    layout->addChild(eventsButton);
}

bool LegacyUserScene::init() {
    if (!Scene::init()) {
        return false;
    }
    auto layout = CocosUIHelpers::createRootView(this);

    addTopSectionForLegacyScene(layout);
    addLegacyUserSection(layout);
    return true;
}

void LegacyUserScene::login(cocos2d::ValueMap user) {
    bool loginResult = HelpshiftCocos2dx::login(user);
    printf("Login Result: %s\n", loginResult ? "Success" : "Failure");
}

void LegacyUserScene::logout() {
    HelpshiftCocos2dx::logout();
}
