//
//  CocosUIHelpers.cpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 14/02/25.
//

#include "CocosUIHelpers.h"

std::vector<std::string> events;

Margin CocosUIHelpers::linearLayoutMargin() {
    // Left, Top, Right, Bottom
    return Margin(UIConstants::hPadding, UIConstants::vPadding, UIConstants::hPadding, UIConstants::vPadding);
}

Size CocosUIHelpers::linearLayoutMarginAdjustedSize(Size size) {
    return size.operator+(Size(UIConstants::hPadding * 2, UIConstants::vPadding * 2));
}

std::pair<Widget *, EditBox *> CocosUIHelpers::createTextField(const std::string& placeholder, float fontSize) {
    // Create a Node as a container for the text field and background
    auto container = RelativeBox::create();
    container->setBackGroundImage("whitebg.png");

    auto params = RelativeLayoutParameter::create();
    params->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);

    auto containerParams = LinearLayoutParameter::create();
    containerParams->setMargin(CocosUIHelpers::linearLayoutMargin());

    // Create a textfield
    auto textField = EditBox::create(container->getBackGroundImageTextureSize(), "whitebg.png");
    textField->setFontSize(fontSize);
    textField->setFontColor(Color3B::BLACK);
    textField->setPlaceHolder(placeholder.c_str());
    textField->setPlaceholderFontSize(fontSize);
    textField->setLayoutParameter(params);
    container->setLayoutParameter(containerParams);
    container->addChild(textField);
    container->setContentSize(container->getBackGroundImageTextureSize());

    return std::make_pair(container, textField);
}

std::pair<Widget *, CheckBox *> CocosUIHelpers::createCheckBox(const std::string& label, const Color4B textColor) {
    auto container = HBox::create();
    auto checkbox = CheckBox::create("unchecked-50.png",
                                     "unchecked-50.png",
                                     "checked.png",
                                     "unchecked-50.png",
                                     "unchecked-50.png");
    auto checkboxLabel = Text::create(label, "", UIConstants::bodyFontSize);
    checkboxLabel->setTextColor(textColor);
    auto params = LinearLayoutParameter::create();
    params->setMargin(Margin(2, 8, 2, 8));
    params->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    checkbox->setLayoutParameter(params);
    checkboxLabel->setLayoutParameter(params);
    container->addChild(checkbox);
    container->addChild(checkboxLabel);
    container->setContentSize(checkbox->getContentSize());
    return std::make_pair(container, checkbox);
}

Button *CocosUIHelpers::createButton(const std::string& title, const std::function<void()>& callback) {
    auto button = Button::create("bg.png");
    button->setTitleText(title);
    button->setTitleFontSize(UIConstants::bodyFontSize);
    button->setTitleColor(Color3B::WHITE);
    button->addTouchEventListener([callback](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            callback();
        }
    });
    auto params = LinearLayoutParameter::create();
    params->setMargin(CocosUIHelpers::linearLayoutMargin());
    button->setLayoutParameter(params);
    return button;
}

DropdownMenu *CocosUIHelpers::createDropdown(const std::vector<std::string> &options) {
    auto dropdown = DropdownMenu::create();
    dropdown->initWithOptions(options);
    return dropdown;
}

Text *CocosUIHelpers::createText(const std::string& title, float fontSize) {
    auto text = Text::create(title, "", fontSize);
    auto params = LinearLayoutParameter::create();
    params->setMargin(CocosUIHelpers::linearLayoutMargin());
    text->setLayoutParameter(params);
    return text;
}

VBox *CocosUIHelpers::createRootView(Scene *scene) {
    // Number of "pages" or full screen sizes available to scroll view for scrolling.
    // Can be a fraction.
    auto pageCount = 2.5;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto scrollView = ScrollView::create();
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(visibleSize);
    scrollView->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * pageCount));
    scrollView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    scrollView->setBackGroundColor(Color3B(24, 24, 24));
    scene->addChild(scrollView);

    auto layout = VBox::create();
    layout->setPosition(Vec2(0, visibleSize.height * pageCount));
    scrollView->addChild(layout);

    return layout;
}

void CocosUIHelpers::showDialog(const std::string& message){
    Size visibleViewSize = Director::getInstance()->getVisibleSize();
    // Semi-transparent background layer
    auto background = LayerColor::create(Color4B(0, 0, 0, 200));
    background->setContentSize(visibleViewSize);

    // Swallow touches so clicks don't pass through
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch *touch, Event *event) {
        return true;
    };
    Director::getInstance()->getEventDispatcher()
            ->addEventListenerWithSceneGraphPriority(listener, background);

    // Create a dialog box (parent container)
    auto dialogBox = Layout::create();
    dialogBox->setContentSize(Size(visibleViewSize.width, visibleViewSize.height));
    // Position the dialog box in the center of the screen
    dialogBox->setPosition(Vec2(
            visibleViewSize.width / 2 - dialogBox->getContentSize().width / 2,
            0
    ));

    float buttonHeight = 50; // Space reserved for the button
    float scrollHeight = dialogBox->getContentSize().height - buttonHeight;

    auto scrollView = ScrollView::create();
    scrollView->setContentSize(Size(visibleViewSize.width, scrollHeight));
    scrollView->setPosition(Vec2(0, buttonHeight));
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->setScrollBarEnabled(true);

    auto eventsLabel = CocosUIHelpers::createText(message, 18);
    eventsLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
    eventsLabel->setTextVerticalAlignment(TextVAlignment::TOP);
    eventsLabel->setAnchorPoint(Vec2(0, 1)); // Align top-left
    eventsLabel->setTextAreaSize(Size(visibleViewSize.width - 10, 0));

    // Calculate needed height for the label
    float contentHeight = eventsLabel->getContentSize().height;
    float scrollViewHeight = scrollView->getContentSize().height;
    // The scrollView should at least be as tall as the label or remain at its own height
    float innerHeight = std::max(contentHeight, scrollViewHeight);

    scrollView->setInnerContainerSize(Size(visibleViewSize.width, innerHeight));
    eventsLabel->setPosition(Vec2(10, innerHeight)); // 10px left padding

    auto closeButton = CocosUIHelpers::createButton("Close", [background] {
        background->removeFromParent(); // Remove entire dialog
    });
    closeButton->setPosition(Vec2(dialogBox->getContentSize().width / 2, 12));

    scrollView->addChild(eventsLabel);

    dialogBox->addChild(scrollView);
    dialogBox->addChild(closeButton);

    background->addChild(dialogBox);

    Director::getInstance()->getRunningScene()->addChild(background, 100);
}

void CocosUIHelpers::addMessage(const std::string& eventName, const ValueMap &data) {
    std::string dataString = "{ ";

    if (!data.empty()) {
        for (const auto& pair : data) {
            dataString += pair.first + " : " + pair.second.getDescription() + " , ";
        }
        dataString.pop_back(); // Remove the last extra space
        dataString.pop_back(); // Remove the last extra comma
    }

    dataString += " }";

    std::string message = "event = " + eventName + ", data = " + dataString;
    events.push_back(message);
}

std::string CocosUIHelpers::getFormattedEventsText() {
    std::string eventsText ;
    for (int i = 0; i < events.size(); ++i) {
        eventsText += "\n" + std::to_string(i + 1) + ". " + events[i];
    }

    return eventsText;
}
