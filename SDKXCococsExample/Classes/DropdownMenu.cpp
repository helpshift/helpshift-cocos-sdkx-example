//
//  DropdownMenu.cpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 19/02/25.
//

#include "DropdownMenu.h"
#include "CocosUIHelpers.h"

void DropdownMenu::initWithOptions(const std::vector<std::string>& options) {
    _options = options;
    _checkboxes = {};

    float width = 0;
    float height = 0;

    for(const auto& option: _options) {
        auto optionCheckbox = CocosUIHelpers::createCheckBox(option);
        optionCheckbox.second->setName(option);
        _checkboxes.push_back(optionCheckbox.second);
        addChild(optionCheckbox.first);
        width = std::max(width, optionCheckbox.first->getContentSize().width);
        height += optionCheckbox.first->getContentSize().height;
    }

    _selectedOption = "";

    for(const auto& checkbox: _checkboxes) {
        checkbox->addEventListener([this, checkbox](Ref *target, CheckBox::EventType eventType){
            if(eventType == CheckBox::EventType::SELECTED) {
                deselectAllExcept(checkbox);
                _selectedOption = checkbox->getName();
            } else if(eventType == CheckBox::EventType::UNSELECTED) {
                _selectedOption = "";
            }
        });
    }

    setContentSize(Size(width, height));
}

void DropdownMenu::selectOption(const std::string &option) {
    for(const auto& checkbox: _checkboxes) {
        if (checkbox->getName() == option) {
            checkbox->setSelected(true);
        } else {
            checkbox->setSelected(false);
        }
    }
    _selectedOption = option;
}

void DropdownMenu::deselectAllExcept(ui::CheckBox *selected) {
    for(const auto& checkbox: _checkboxes) {
        if (checkbox == selected) {
            continue;
        }
        checkbox->setSelected(false);
    }
}
