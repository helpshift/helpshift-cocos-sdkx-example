//
//  IdentityUserScene.cpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 17/02/25.
//

#include "IdentityUserScene.h"
#include "CocosUIHelpers.h"
#include "cocos/base/CCDirector.h"
#include <iostream>
#import "HelpshiftCocos2dx.h"
#import "HelpshiftLog.h"

using namespace cocos2d;
using namespace cocos2d::ui;

enum AttributeType {
    Master, App
};

ValueMap loginConfigMap;
ValueMap masterCufsMap;
ValueMap masterAttributesMap;
ValueMap appCufsMap;
ValueMap appAttributesMap;

void loginConfigSubsection(Layout *layout) {
    auto loginConfigInputRow = HBox::create();
    auto loginConfigKey = CocosUIHelpers::createTextField("Config Key");
    auto loginConfigValue = CocosUIHelpers::createTextField("Config Value");
    loginConfigInputRow->addChild(loginConfigKey.first);
    loginConfigInputRow->addChild(loginConfigValue.first);
    loginConfigInputRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(loginConfigKey.first->getContentSize()));
    layout->addChild(loginConfigInputRow);

    auto loginConfigButtonRow = HBox::create();
    auto addLoginConfigButton = CocosUIHelpers::createButton("Add Entry", [loginConfigKey, loginConfigValue]{
        loginConfigMap[loginConfigKey.second->getText()] = loginConfigValue.second->getText();
        loginConfigKey.second->setText("");
        loginConfigValue.second->setText("");
    });
    auto resetLoginConfigButton = CocosUIHelpers::createButton("Reset config", [loginConfigKey, loginConfigValue]{
        loginConfigMap.clear();
        loginConfigKey.second->setText("");
        loginConfigValue.second->setText("");
    });
    loginConfigButtonRow->addChild(addLoginConfigButton);
    loginConfigButtonRow->addChild(resetLoginConfigButton);
    loginConfigButtonRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(addLoginConfigButton->getContentSize()));
    layout->addChild(loginConfigButtonRow);
}

void addLoginSection(Layout *layout) {
    auto loginLabel = CocosUIHelpers::createText("Login", UIConstants::headingFontSize);
    layout->addChild(loginLabel);

    loginConfigSubsection(layout);

    auto jwtTextField = CocosUIHelpers::createTextField("Identity JWT");
    layout->addChild(jwtTextField.first);

    auto loginButtonRow = HBox::create();
    auto loginButton = CocosUIHelpers::createButton("Login", [jwtTextField]{
        cocos2d::log("%s", jwtTextField.second->getText());
        IdentityUserScene::loginWithIdentity(jwtTextField.second->getText());
    });
    auto addUserIdentitiesButton = CocosUIHelpers::createButton("Add User Ids", [jwtTextField]{
        IdentityUserScene::addUserIdentities(jwtTextField.second->getText());
    });
    loginButtonRow->addChild(loginButton);
    loginButtonRow->addChild(addUserIdentitiesButton);
    loginButtonRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(loginButton->getContentSize()));
    layout->addChild(loginButtonRow);
}

void addAttributesSection(Layout *layout, AttributeType type) {
    auto labelText = type == App ? "App Attributes" : "Master Attributes";
    auto attributesLabel = CocosUIHelpers::createText(labelText, UIConstants::headingFontSize);
    layout->addChild(attributesLabel);

    auto attributeInputRow = HBox::create();
    auto attributeKey = CocosUIHelpers::createTextField("Attr Key");
    auto attributeValue = CocosUIHelpers::createTextField("Attr Value");
    auto addAttributeButton = CocosUIHelpers::createButton("Add Attr", [type, attributeKey, attributeValue]{
        auto& attributesMap = type == App ? appAttributesMap : masterAttributesMap;
        attributesMap[attributeKey.second->getText()] = attributeValue.second->getText();
        attributeKey.second->setText("");
        attributeValue.second->setText("");
    });
    attributeInputRow->addChild(attributeKey.first);
    attributeInputRow->addChild(attributeValue.first);
    attributeInputRow->addChild(addAttributeButton);
    attributeInputRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(addAttributeButton->getContentSize()));
    layout->addChild(attributeInputRow);

    auto cufInputRow = HBox::create();
    auto cufKey = CocosUIHelpers::createTextField("CUF Key");
    auto cufValue = CocosUIHelpers::createTextField("CUF Value");
    auto addCufButton = CocosUIHelpers::createButton("Add CUF", [type, cufKey, cufValue]{
        auto& cufMap = type == App ? appCufsMap : masterCufsMap;
        cufMap[cufKey.second->getText()] = cufValue.second->getText();
        cufKey.second->setText("");
        cufValue.second->setText("");
    });
    cufInputRow->addChild(cufKey.first);
    cufInputRow->addChild(cufValue.first);
    cufInputRow->addChild(addCufButton);
    cufInputRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(addCufButton->getContentSize()));
    layout->addChild(cufInputRow);

    auto attributesButtonRow = HBox::create();
    auto updateAttributesButton = CocosUIHelpers::createButton("Update Attrs", [type]{
        switch (type) {
            case Master: IdentityUserScene::updateMasterAttributes(); break;
            case App: IdentityUserScene::updateAppAttributes(); break;
        }
    });
    auto resetAttributesButton = CocosUIHelpers::createButton("Reset Attrs", [type, attributeKey, attributeValue, cufKey, cufValue]{
        auto& attributesMap = type == App ? appAttributesMap : masterAttributesMap;
        auto& cufMap = type == App ? appCufsMap : masterCufsMap;
        attributesMap.clear();
        cufMap.clear();
        attributeKey.second->setText("");
        attributeValue.second->setText("");
        cufKey.second->setText("");
        cufValue.second->setText("");
    });
    attributesButtonRow->addChild(updateAttributesButton);
    attributesButtonRow->addChild(resetAttributesButton);
    attributesButtonRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(updateAttributesButton->getContentSize()));
    layout->addChild(attributesButtonRow);
}

void addTopSectionForIdentityScene(Layout *layout) {
    auto backButton = CocosUIHelpers::createButton("< Back", [] {
        Director::getInstance()->popScene();
    });
    auto eventsButton = CocosUIHelpers::createButton("Events", [] {
        CocosUIHelpers::showDialog(CocosUIHelpers::getFormattedEventsText());
    });

    layout->addChild(backButton);
    layout->addChild(eventsButton);
}

bool IdentityUserScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto layout = CocosUIHelpers::createRootView(this);

    addTopSectionForIdentityScene(layout);
    addLoginSection(layout);
    addAttributesSection(layout, Master);
    addAttributesSection(layout, App);

    return true;
}

void handleLoginSuccess() {
    CocosUIHelpers::addMessage("Login Successful", {});
    cocos2d::log("Login Successful");
}

void handleLoginFailure(const char *reason, const cocos2d::ValueMap &errorMap) {
    CocosUIHelpers::addMessage(reason, errorMap);
    cocos2d::log("Login failed with reason = %s", reason);
}

void IdentityUserScene::loginWithIdentity(const std::string jwt) {
    HelpshiftCocos2dx::loginWithIdentity(jwt.c_str(), loginConfigMap, handleLoginSuccess,
                                         handleLoginFailure);
}

void IdentityUserScene::addUserIdentities(const std::string jwt) {
    HelpshiftCocos2dx::addUserIdentities(jwt.c_str());
}

void IdentityUserScene::updateMasterAttributes() {
    ValueMap attrs = masterAttributesMap;
    if(masterCufsMap.size() > 0) {
        attrs["custom_user_fields"] = masterCufsMap;
    }
    HelpshiftCocos2dx::updateMasterAttributes(attrs);
}

void IdentityUserScene::updateAppAttributes() {
    ValueMap attrs = appAttributesMap;
    if(appCufsMap.size() > 0) {
        attrs["custom_user_fields"] = appCufsMap;
    }
    HelpshiftCocos2dx::updateAppAttributes(attrs);
}

