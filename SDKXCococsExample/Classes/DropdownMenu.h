//
//  DropdownMenu.hpp
//  HelloWorld-mobile
//
//  Created by Jaydeep Joshi on 19/02/25.
//

#ifndef __DROPDOWN_MENU_H__
#define __DROPDOWN_MENU_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class DropdownMenu : public ui::VBox {
public:
    CREATE_FUNC(DropdownMenu);

    // Initialize with options
    void initWithOptions(const std::vector<std::string>& options);

    // Select an option
    void selectOption(const std::string& option);

    // Get currently selected option
    const std::string& getSelectedOption() const { return _selectedOption; }

private:
    std::vector<std::string> _options;
    std::vector<ui::CheckBox *> _checkboxes;
    std::string _selectedOption;
    void deselectAllExcept(ui::CheckBox *selected);
};

#endif // __DROPDOWN_MENU_H__

