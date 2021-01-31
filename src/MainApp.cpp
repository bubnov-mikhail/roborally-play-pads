#include "MainApp.h"

void MainApp::execute(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    Headline* headline = AbstractApp::sc->getHeadline();

    lcd->clear(false);
    headline->update(true);
    lcd->setCursor(27, 2);
    lcd->setInverted(true);
    lcd->print(StringAssets::roborally);

    while(!keypad->read()) {
        headline->update();
        continue;
    }

    MenuSystem menuSystem(*(AbstractApp::sc->getMenuRenderer()));
    Menu menuGames(StringAssets::games);
    MenuItem menuItemRoborally(StringAssets::roborally, MainApp::handleGamesRoborally);
    menuGames.add_item(&menuItemRoborally);

    Menu menuConfigs(StringAssets::configs);
    MenuItem menuItemBacklight(MainApp::getBacklightMenuName(config), MainApp::handleConfigBacklight);
    MenuItem menuItemSounds(MainApp::getSoundsMenuName(config), MainApp::handleConfigSound);
    MenuItem menuItemContrast(StringAssets::contrast, MainApp::handleConfigContrast);
    MenuItem menuItemClockCtrl(StringAssets::clockCtrl, MainApp::handleConfigClockCtrl);

    menuConfigs.add_item(&menuItemBacklight);
    menuConfigs.add_item(&menuItemSounds);
    menuConfigs.add_item(&menuItemContrast);
    menuConfigs.add_item(&menuItemClockCtrl);

    if (config->isRadioConnected()) {
        MenuItem menuItemRadioCtrlChannel(StringAssets::radioChannel, MainApp::handleConfigRadioChannel);
        MenuItem menuItemRadioCtrlChannelScan(StringAssets::radioChannelScan, MainApp::handleConfigRadioChannelScan);
        MenuItem menuItemRadioCtrlLevel(MainApp::getRadioLevelMenuName(config), MainApp::handleConfigRadioLevel);
        menuConfigs.add_item(&menuItemRadioCtrlChannel);
        menuConfigs.add_item(&menuItemRadioCtrlChannelScan);
        menuConfigs.add_item(&menuItemRadioCtrlLevel);
    }
   
    menuSystem.get_root_menu().set_name(StringAssets::mainMenu);
    menuSystem.get_root_menu().add_menu(&menuGames);
    menuSystem.get_root_menu().add_menu(&menuConfigs);

    menuSystem.display();
    
    headline->update(true);

    while(true) {
        if (!keypad->read()) {
            headline->update();
            continue;
        }

        uint8_t keypadSymbol = keypad->getKeypadSymbol();
        if(handleKeypadSymbol(keypadSymbol, &menuSystem)) {
            headline->update(true);
        }
    }
}

const char* MainApp::getBacklightMenuName(ConfigStorage* config)
{
    strcpy(MainApp::backlightMenuName, StringAssets::backlight);
    strcat(MainApp::backlightMenuName, StringAssets::colon);
    config->isWithBacklight() ? strcat(MainApp::backlightMenuName, StringAssets::stateOn) : strcat(MainApp::backlightMenuName, StringAssets::stateOff);

    return MainApp::backlightMenuName;
}

const char* MainApp::getSoundsMenuName(ConfigStorage* config)
{
    strcpy(MainApp::soundsMenuName, StringAssets::sounds);
    strcat(MainApp::soundsMenuName, StringAssets::colon);
    config->isWithSounds() ? strcat(MainApp::soundsMenuName, StringAssets::stateOn) : strcat(MainApp::soundsMenuName, StringAssets::stateOff);

    return MainApp::soundsMenuName;
}

const char* MainApp::getRadioLevelMenuName(ConfigStorage* config)
{
    strcpy(MainApp::radioLevelMenuName, StringAssets::radioLevel);
    strcat(MainApp::radioLevelMenuName, StringAssets::colon);
    strcat(MainApp::radioLevelMenuName, StringAssets::space);
    switch (config->getRadioLevel()) {
        case 0:
            strcat(MainApp::radioLevelMenuName, StringAssets::radioLevelMin);
            break;
        case 1:
            strcat(MainApp::radioLevelMenuName, StringAssets::radioLevelLow);
            break;
        case 3:
            strcat(MainApp::radioLevelMenuName, StringAssets::radioLevelMax);
            break;
    }

    return MainApp::radioLevelMenuName;
}

void MainApp::handleConfigBacklight(MenuComponent* p_menu_component)
{
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    config->setWithBacklight(!config->isWithBacklight());
    lcd->setBacklight(config->isWithBacklight());
    p_menu_component->set_name(MainApp::getBacklightMenuName(config));
}

void MainApp::handleConfigSound(MenuComponent* p_menu_component)
{
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    config->setWithSounds(!config->isWithSounds());
    AbstractApp::sc->getKeypad()->setBeepOnClick(config->isWithSounds());
    p_menu_component->set_name(MainApp::getSoundsMenuName(config));
}

void MainApp::handleGamesRoborally(MenuComponent* p_menu_component)
{
    RoborallyApp app;
    app.execute();
}

void MainApp::handleConfigContrast(MenuComponent* p_menu_component)
{
    ContrastCtlApp app;
    app.execute();
}

void MainApp::handleConfigClockCtrl(MenuComponent* p_menu_component)
{
    ClockSetupApp app;
    app.execute();
}

void MainApp::handleConfigRadioChannel(MenuComponent* p_menu_component)
{
    RadioChannelApp app;
    app.execute();
}

void MainApp::handleConfigRadioChannelScan(MenuComponent* p_menu_component)
{
    RadioChannelScanApp app;
    app.execute();
}

void MainApp::handleConfigRadioLevel(MenuComponent* p_menu_component)
{
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    uint8_t newLevel;
    switch (config->getRadioLevel()) {
        case 0:
            newLevel = 1;
            break;
        case 1:
            newLevel = 3;
            break;
        case 3:
            newLevel = 0;
            break;
    }
    config->setRadioLevel(newLevel);
    AbstractApp::sc->getRadio()->setPALevel(newLevel);
    p_menu_component->set_name(MainApp::getRadioLevelMenuName(config));
}

bool MainApp::handleKeypadSymbol(uint8_t keypadSymbol, MenuSystem* menuSystem)
{
    switch (keypadSymbol) {
      case Keypad::keyB:
        menuSystem->prev();
        menuSystem->display();
        return true;
      case Keypad::keyC:
        menuSystem->next();
        menuSystem->display();
        return true;
      case Keypad::keyStar:
        menuSystem->select();
        menuSystem->display();
        return true;
      case Keypad::keyD:
        menuSystem->back();
        menuSystem->display();
        return true;
    }

    return false;
}