/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

#include <utility>
#include <iostream>
#import "HelpshiftCocos2dx.h"
#import "HelpshiftLog.h"
#include "HelpshiftEvent.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos/base/CCDirector.h"
#include "CocosUIHelpers.h"
#include "LegacyUserScene.h"
#include "IdentityUserScene.h"
#include "DropdownMenu.h"
#define kDefaultProactiveLink "cocos://helpshift.com/proactive/?payload=eyJhY3Rpb24iOiJjaGF0IiwibWV0YSI6eyJpZCI6Im91dGJvdW5kX2M1Nzg4NTliLTY5NDUtNGUyOC04NGQzLTRkMzBmNDMyNTcyOSIsIm9yaWdpbiI6InByb2FjdGl2ZSJ9LCJjaGF0Q29uZmlnIjp7ImluaXRpYWxVc2VyTWVzc2FnZSI6InRlc3QgaW9zIGFuZCBhbmRyb2lkIiwiY3VzdG9tSXNzdWVGaWVsZHMiOnsiY2l0eSI6eyJ0eXBlIjoic2luZ2xlbGluZSIsInZhbHVlIjoiZGVsaGkifSwiYWJjIjp7InR5cGUiOiJzaW5nbGVsaW5lIiwidmFsdWUiOiJhYmMtdGVzdCJ9fX19"
#ifdef IS_IOS
#include "iOSHelper.h"
#endif

using namespace cocos2d;
using namespace cocos2d::ui;

// UI elements and value maps used to construct API config when Helpcenter or Conversation is to be shown.
ValueMap genericConfigMap;
ValueMap cifsMap;
EditBox *_tagsTextField;
CheckBox *_fullPrivacyToggle;
DropdownMenu *_enableContactUsDropdown;
CheckBox *_ignoreConfigToggle;


USING_NS_CC;


void handleAuthFailureEvent(HSAuthenticationFailureReason reason) {
    cocos2d::log("Authentication Failed: %d", reason);
}

// Forward declaration
void processMap(const std::string& key, const Value& value);

void processValue(const std::string& key, const Value& value) {
    if (value.getType() == Value::Type::MAP) {
        processMap(key, value);
    } else if (value.getType() == Value::Type::VECTOR) {
        const auto& vectorValue = value.asValueVector();
        for (size_t i = 0; i < vectorValue.size(); ++i) {
            processValue(key + "[" + std::to_string(i) + "]", vectorValue[i]);
        }
    } else {
        // Handle other types as needed
        cocos2d::log("%s: %s", key.c_str(), value.getDescription().c_str());
    }
}

void processMap(const std::string& key, const Value& value) {
    const auto& mapValue = value.asValueMap();
    for (const auto& entry : mapValue) {
        processValue(key + "." + entry.first, entry.second);
    }
}

void handleHelpshiftEvent(const char *eventName, ValueMap &eventData) {
    cocos2d::log("Event Received: %s", eventName);
    CocosUIHelpers::addMessage(eventName, eventData);

    if (strcmp(eventName, SDK_SESSION_STARTED) == 0) {
        cocos2d::log("Helpshift session started on cocos application\n");
    }

    if (strcmp(eventName, RECEIVED_UNREAD_MESSAGE_COUNT) == 0) {
        if (eventData.find(DATA_MESSAGE_COUNT) != eventData.end()) {
            int count = eventData[DATA_MESSAGE_COUNT].asInt();
            std::string publishId = eventData[DATA_LATEST_ISSUE_PUBLISH_ID].asString();
            cocos2d::log("publish id: %s", publishId.c_str());
            bool countFromCache = eventData[DATA_MESSAGE_COUNT_FROM_CACHE].asBool();
            cocos2d::log("Count: %d, is count from cache: %d", count, countFromCache);
        }
    } else {
        for (const auto& entry : eventData) {
            processValue(entry.first, entry.second);
        }
    }
}

ValueVector convertToValueVector(const std::string& commaSeparatedString) {
    ValueVector result;

    std::stringstream ss(commaSeparatedString);
    std::string item;

    while (std::getline(ss, item, ','))
    {
        result.push_back(Value(item));
    }

    return result;
}

ValueMap getConfigMap() {
    ValueMap config;
    if(_ignoreConfigToggle->isSelected()) {
        return config;
    }
    auto tagString = std::string(_tagsTextField->getText());
    auto fullPrivacy = _fullPrivacyToggle->isSelected();
    auto enableContactUs = _enableContactUsDropdown->getSelectedOption();
    if(!tagString.empty()) {
        ValueVector tags = convertToValueVector(tagString);
        config["tags"] = tags;
    }
    if(!enableContactUs.empty()) {
        config["enableContactUs"] = enableContactUs;
    }
    if(!cifsMap.empty()) {
        config["cifs"] = cifsMap;
    }
    config["fullPrivacy"] = fullPrivacy;
    for(const auto entry: genericConfigMap) {
        config[entry.first] = entry.second;
    }
    return config;
}

void resetConfig() {
    _fullPrivacyToggle->setSelected(false);
    _tagsTextField->setText("");
    _enableContactUsDropdown->selectOption("");
    genericConfigMap.clear();
}

ValueMap proactiveConfigCollector() {
    return getConfigMap();
}

ValueMap createValueMap(const std::string& type, Value value) {
    ValueMap valueMap;
    valueMap["type"] = Value(type);
    valueMap["value"] = Value(std::move(value));
    return valueMap;
}

ValueMap defaultCifs() {
    ValueMap cifMap;
    cifMap["joining_date"] = createValueMap("date", Value("1684747527935"));
    cifMap["birthday"] = createValueMap("date", Value("1667886984756"));
    cifMap["date_now"] = createValueMap("date", Value("1684994277231"));
    cifMap["stock_level"] = createValueMap("number", Value(150));
    cifMap["employee_name"] = createValueMap("singleline", Value("Cocos helpshift"));
    cifMap["is_pro"] = createValueMap("boolean", Value(true));
    cifMap["salary_currency"] = createValueMap("dropdown", Value("Dollers"));
    cifMap["essay"] = createValueMap("multiline", Value("Education holds the key to unlocking our potential and shaping a better future. It equips individuals with knowledge, skills, and critical thinking abilities necessary for personal growth and societal progress. Through education, we gain a broader understanding of the world and develop empathy and compassion. It empowers us to challenge existing norms, break barriers, and create meaningful change. Education is not merely confined to classrooms; it is a lifelong journey of exploration and self-discovery. Let us embrace the transformative power of education and strive for a brighter tomorrow."));

    return cifMap;
}

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

void addUserSection(Layout *layout) {
    auto userLabel = CocosUIHelpers::createText("Users", UIConstants::headingFontSize);
    layout->addChild(userLabel);

    auto firstRow = HBox::create();
    auto legacyButton = CocosUIHelpers::createButton("Legacy User Login", []{
        auto legacyUserScene = LegacyUserScene::create();
        Director::getInstance()->pushScene(legacyUserScene);
    });
    firstRow->addChild(legacyButton);
    auto identityButton = CocosUIHelpers::createButton("Identity User Login", []{
        auto legacyUserScene = IdentityUserScene::create();
        Director::getInstance()->pushScene(legacyUserScene);
    });
    firstRow->addChild(identityButton);
    firstRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(legacyButton->getContentSize()));
    layout->addChild(firstRow);

    auto secondRow = HBox::create();
    auto clearAnonUserTrueButton = CocosUIHelpers::createButton("Clear Anon YES", []{
        HelloWorld::clearAnonymousUserOnLogin(true);
    });
    secondRow->addChild(clearAnonUserTrueButton);
    auto clearAnonUserFalseButton = CocosUIHelpers::createButton("Clear Anon NO", []{
        HelloWorld::clearAnonymousUserOnLogin(false);
    });
    secondRow->addChild(clearAnonUserFalseButton);
    secondRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(legacyButton->getContentSize()));
    layout->addChild(secondRow);
}

void addGenericConfigSubsection(Layout *layout) {
    auto genericConfigLabel = CocosUIHelpers::createText("Generic Config");
    layout->addChild(genericConfigLabel);

    auto genericConfigInputRow = HBox::create();
    auto configKey = CocosUIHelpers::createTextField("Config Key");
    auto configValue = CocosUIHelpers::createTextField("Config Value");
    genericConfigInputRow->addChild(configKey.first);
    genericConfigInputRow->addChild(configValue.first);
    genericConfigInputRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(configKey.first->getContentSize()));
    layout->addChild(genericConfigInputRow);

    auto genericConfigButtonRow = HBox::create();
    auto addGenericConfigButton = CocosUIHelpers::createButton("Add Entry", [configKey, configValue]{
        genericConfigMap[configKey.second->getText()] = configValue.second->getText();
        configKey.second->setText("");
        configValue.second->setText("");
    });
    auto resetGenericConfiButton = CocosUIHelpers::createButton("Reset generic", [configKey, configValue]{
        genericConfigMap.clear();
        configKey.second->setText("");
        configValue.second->setText("");
    });
    genericConfigButtonRow->addChild(addGenericConfigButton);
    genericConfigButtonRow->addChild(resetGenericConfiButton);
    genericConfigButtonRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(addGenericConfigButton->getContentSize()));
    layout->addChild(genericConfigButtonRow);
}

void addCifsSubsection(Layout *layout) {
    auto cifsLabel = CocosUIHelpers::createText("CIFs");
    layout->addChild(cifsLabel);

    auto cifsInputRow = HBox::create();
    auto cifKey = CocosUIHelpers::createTextField("CIF Key");
    auto cifValue = CocosUIHelpers::createTextField("CIF Value");
    auto cifType = CocosUIHelpers::createTextField("CIF Type");
    cifsInputRow->addChild(cifKey.first);
    cifsInputRow->addChild(cifValue.first);
    cifsInputRow->addChild(cifType.first);
    cifsInputRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(cifKey.first->getContentSize()));
    layout->addChild(cifsInputRow);

    auto cifButtonRow = HBox::create();
    auto addCifButton = CocosUIHelpers::createButton("Add CIF", [cifKey, cifValue, cifType]{
        auto cifMap = createValueMap(cifType.second->getText(), Value(cifValue.second->getText()));
        cifsMap[cifKey.second->getText()] = cifMap;
        cifKey.second->setText("");
        cifValue.second->setText("");
        cifType.second->setText("");
    });
    auto resetCifsButton = CocosUIHelpers::createButton("Reset CIFs", [cifKey, cifValue, cifType]{
        cifsMap.clear();
        cifKey.second->setText("");
        cifValue.second->setText("");
        cifType.second->setText("");
    });
    auto defaultCifsButton = CocosUIHelpers::createButton("Default CIFs", []{
        cifsMap = defaultCifs();
    });
    cifButtonRow->addChild(addCifButton);
    cifButtonRow->addChild(resetCifsButton);
    cifButtonRow->addChild(defaultCifsButton);
    cifButtonRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(addCifButton->getContentSize()));
    layout->addChild(cifButtonRow);
}

void addConfigSection(Layout *layout) {
    auto configLabel = CocosUIHelpers::createText("Config", UIConstants::headingFontSize);
    layout->addChild(configLabel);

    auto ignoreConfigToggle = CocosUIHelpers::createCheckBox("Ignore Config", Color4B::ORANGE);
    _ignoreConfigToggle = ignoreConfigToggle.second;
    layout->addChild(ignoreConfigToggle.first);
    auto ignoreConfigInfo = CocosUIHelpers::createText("If enabled, below config will be ignored. Empty config will be passed to Helpshift.", UIConstants::smallFontSize);
    layout->addChild(ignoreConfigInfo);

    auto tagsFpRow = HBox::create();
    auto tagsTextField = CocosUIHelpers::createTextField("Comma separated tags", UIConstants::smallFontSize);
    _tagsTextField = tagsTextField.second;
    auto fullPrivacyToggle = CocosUIHelpers::createCheckBox("Full Privacy");
    _fullPrivacyToggle = fullPrivacyToggle.second;
    tagsFpRow->addChild(tagsTextField.first);
    tagsFpRow->addChild(fullPrivacyToggle.first);
    tagsFpRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(tagsTextField.first->getContentSize()));
    layout->addChild(tagsFpRow);

    auto enableContactUsRow = HBox::create();
    auto enableContactUsLabel = CocosUIHelpers::createText("Enable contact us");
    auto enableContactUsDropdown = CocosUIHelpers::createDropdown({
        "ALWAYS",
        "AFTER_VIEWING_FAQS",
        "AFTER_MARKING_ANSWER_UNHELPFUL",
        "NEVER"
    });
    _enableContactUsDropdown = enableContactUsDropdown;
    enableContactUsRow->addChild(enableContactUsLabel);
    enableContactUsRow->addChild(enableContactUsDropdown);
    enableContactUsRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(enableContactUsDropdown->getContentSize()));
    layout->addChild(enableContactUsRow);

    addGenericConfigSubsection(layout);
    addCifsSubsection(layout);

    auto resetConfigButton = CocosUIHelpers::createButton("Reset Config", []{
        resetConfig();
    });
    layout->addChild(resetConfigButton);
}

void addConversationSection(Layout *layout) {
    auto conversationLabel = CocosUIHelpers::createText("Conversation", UIConstants::headingFontSize);
    layout->addChild(conversationLabel);

    auto showConversationButton = CocosUIHelpers::createButton("Show Conversation", []{
        HelloWorld::showConversation();
    });
    layout->addChild(showConversationButton);

    auto unreadMessageRow = HBox::create();
    auto fromRemoteToggle = CocosUIHelpers::createCheckBox("From Remote");
    auto unreadMessageButton = CocosUIHelpers::createButton("Unread msg count", [fromRemoteToggle]{
        bool fromRemote = fromRemoteToggle.second->isSelected();
        HelloWorld::getUnreadMessageCount(fromRemote);
    });
    unreadMessageRow->addChild(unreadMessageButton);
    unreadMessageRow->addChild(fromRemoteToggle.first);
    unreadMessageRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(unreadMessageButton->getContentSize()));
    layout->addChild(unreadMessageRow);
}

void addHelpcenterSection(Layout *layout) {
    auto helpcenterLabel = CocosUIHelpers::createText("Helpcenter", UIConstants::headingFontSize);
    layout->addChild(helpcenterLabel);

    auto showHelpcenterButton = CocosUIHelpers::createButton("Show Helpcenter", []{
        HelloWorld::showFAQs();
    });
    layout->addChild(showHelpcenterButton);

    auto faqSectionRow = HBox::create();
    auto sectionIdTextField = CocosUIHelpers::createTextField("Section ID");
    auto showSectionButton = CocosUIHelpers::createButton("Show Section", [sectionIdTextField]{
        auto sectionId = sectionIdTextField.second->getText();
        HelloWorld::showFAQSection(sectionId);
    });
    faqSectionRow->addChild(sectionIdTextField.first);
    faqSectionRow->addChild(showSectionButton);
    faqSectionRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(showSectionButton->getContentSize()));
    layout->addChild(faqSectionRow);

    auto singleFaqRow = HBox::create();
    auto faqIdTextField = CocosUIHelpers::createTextField("FAQ ID");
    auto showSingleFaqButton = CocosUIHelpers::createButton("Show Single FAQ", [faqIdTextField]{
        auto faqId = faqIdTextField.second->getText();
        HelloWorld::showSingleFAQ(faqId);
    });
    singleFaqRow->addChild(faqIdTextField.first);
    singleFaqRow->addChild(showSingleFaqButton);
    singleFaqRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(showSingleFaqButton->getContentSize()));
    layout->addChild(singleFaqRow);
}

void addLogsSection(Layout *layout) {
    auto logsLabel = CocosUIHelpers::createText("Logs", UIConstants::headingFontSize);
    layout->addChild(logsLabel);

    auto logLevelRow = HBox::create();
    auto logLevelTextField = CocosUIHelpers::createTextField("Level v,d,i,w,e or empty", UIConstants::smallFontSize);
    auto logTagTextField = CocosUIHelpers::createTextField("Tag");
    logLevelRow->addChild(logLevelTextField.first);
    logLevelRow->addChild(logTagTextField.first);
    logLevelRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(logLevelTextField.first->getContentSize()));
    layout->addChild(logLevelRow);

    auto messageRow = HBox::create();
    auto messageTextField = CocosUIHelpers::createTextField("Message");
    auto addLogButton = CocosUIHelpers::createButton("Add Log", [logLevelTextField, logTagTextField, messageTextField]{
        HelloWorld::addDebugLog(logLevelTextField.second->getText(),
                                logTagTextField.second->getText(),
                                messageTextField.second->getText());
    });
    messageRow->addChild(messageTextField.first);
    messageRow->addChild(addLogButton);
    messageRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(addLogButton->getContentSize()));
    layout->addChild(messageRow);
}

void addMiscSection(Layout *layout) {
    auto miscLabel = CocosUIHelpers::createText("Miscellaneous", UIConstants::headingFontSize);
    layout->addChild(miscLabel);

    auto eventsRow = HBox::create();
    auto eventsButton = CocosUIHelpers::createButton("Events", []{
        CocosUIHelpers::showDialog(CocosUIHelpers::getFormattedEventsText());
    });

    eventsRow->addChild(eventsButton);
    eventsRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(eventsButton->getContentSize()));

    layout->addChild(eventsRow);

    auto setLangRow = HBox::create();
    auto langCodeTextField = CocosUIHelpers::createTextField("Lang code");
    auto setLangButton = CocosUIHelpers::createButton("Set Language", [langCodeTextField]{
        auto langCode = langCodeTextField.second->getText();
        HelloWorld::setLanguage(langCode);
    });
    setLangRow->addChild(langCodeTextField.first);
    setLangRow->addChild(setLangButton);
    setLangRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(setLangButton->getContentSize()));
    layout->addChild(setLangRow);

    auto userTrailRow = HBox::create();
    auto userTrailTextField = CocosUIHelpers::createTextField("User trail");
    auto addUserTrailButton = CocosUIHelpers::createButton("Add User Trail", [userTrailTextField]{
        auto trail = userTrailTextField.second->getText();
        HelpshiftCocos2dx::addUserTrail(trail);
    });
    userTrailRow->addChild(userTrailTextField.first);
    userTrailRow->addChild(addUserTrailButton);
    userTrailRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(addUserTrailButton->getContentSize()));
    layout->addChild(userTrailRow);

    auto breadcrumbRow = HBox::create();
    auto breadcrumbTextField = CocosUIHelpers::createTextField("Breadcrumb");
    auto leaveBreadcrumbButton = CocosUIHelpers::createButton("Leave Breadcrumb", [breadcrumbTextField]{
        auto crumb = breadcrumbTextField.second->getText();
        HelloWorld::leaveBreadcrumb(crumb);
    });
    breadcrumbRow->addChild(breadcrumbTextField.first);
    breadcrumbRow->addChild(leaveBreadcrumbButton);
    breadcrumbRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(leaveBreadcrumbButton->getContentSize()));
    layout->addChild(breadcrumbRow);

    auto clearBreadcrumbRow = HBox::create();
    auto clearBreadcrumbButton = CocosUIHelpers::createButton("Clear Breadcrumb", []{
        HelpshiftCocos2dx::clearBreadCrumbs();
    });
    clearBreadcrumbRow->addChild(clearBreadcrumbButton);
    auto versionButton = CocosUIHelpers::createButton("Print SDK Version", []{
        std::cout << "Helpshift SDK Version : " << HelpshiftCocos2dx::sdkVersion() << std::endl;
    });
    clearBreadcrumbRow->addChild(versionButton);
    clearBreadcrumbRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(clearBreadcrumbButton->getContentSize()));
    layout->addChild(clearBreadcrumbRow);

    auto closeRow = HBox::create();
    auto closeButton = CocosUIHelpers::createButton("Close SDK Session", [closeRow]{
        cocos2d::Director::getInstance()->getScheduler()->schedule([closeRow](float dt) {
            HelpshiftCocos2dx::closeSession();
        }, closeRow, 0, 0, 7.0f, false, "closeSession");
    });
    closeRow->addChild(closeButton);
    auto purgeButton = CocosUIHelpers::createButton("Purge", []{
#ifdef IS_IOS
        purgeData();
#endif
    });
    closeRow->addChild(purgeButton);
    closeRow->setContentSize(CocosUIHelpers::linearLayoutMarginAdjustedSize(closeButton->getContentSize()));
    layout->addChild(closeRow);
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    if (!Scene::init()) {
        return false;
    }

    auto layout = CocosUIHelpers::createRootView(this);
    addUserSection(layout);
    addConfigSection(layout);
    addConversationSection(layout);
    addHelpcenterSection(layout);
    addLogsSection(layout);
    addMiscSection(layout);

    HelpshiftCocos2dx::setHelpshiftEventsListener(handleHelpshiftEvent, handleAuthFailureEvent);
    HelpshiftCocos2dx::setHelpshiftProactiveConfigCollector(proactiveConfigCollector);

    return true;
}

void HelloWorld::clearAnonymousUserOnLogin(bool clearUser) {
    HelpshiftCocos2dx::clearAnonymousUserOnLogin(clearUser);
}

void HelloWorld::showFAQs() {
    ValueMap config = getConfigMap();
    HelpshiftCocos2dx::showFAQs(config);
}

void HelloWorld::showSingleFAQ(const char *publishId) {
    ValueMap config = getConfigMap();
    HelpshiftCocos2dx::showSingleFAQ(publishId, config);
}

void HelloWorld::showFAQSection(const char *sectionId) {
    ValueMap config = getConfigMap();
    HelpshiftCocos2dx::showFAQSection(sectionId, config);
}

void HelloWorld::showConversation() {
    ValueMap config = getConfigMap();
    HelpshiftCocos2dx::showConversation(config);
}

void HelloWorld::leaveBreadcrumb(const char *breadcrumb) {
    cocos2d::log("Sending breadcrumb: %s", breadcrumb);
    HelpshiftCocos2dx::leaveBreadCrumb(breadcrumb);
}

void HelloWorld::setLanguage(const char *language) {
    HelpshiftCocos2dx::setLanguage(language);
}

void HelloWorld::getUnreadMessageCount(bool shouldFetchFromServer) {
    HelpshiftCocos2dx::requestUnreadMessageCount(shouldFetchFromServer);
}

void HelloWorld::handleProactiveLink(const char *proactiveLink) {
    HelpshiftCocos2dx::handleProactiveLink(proactiveLink);
}

void HelloWorld::addDebugLog(const std::string& level, const std::string& tag, const std::string& message) {
    auto cTag = level.c_str();
    auto cMessage = message.c_str();
    if(level.empty() || tag.empty()) {
        HelpshiftLog::log(cMessage);
    } else if(level == "v") {
        HelpshiftLog::v(cTag, cMessage);
    } else if(level == "d") {
        HelpshiftLog::d(cTag, cMessage);
    } else if(level == "i") {
        HelpshiftLog::i(cTag, cMessage);
    } else if(level == "w") {
        HelpshiftLog::w(cTag, cMessage);
    } else if(level == "e") {
        HelpshiftLog::e(cTag, cMessage);
    } else {
        HelpshiftLog::log(cMessage);
    }
}

