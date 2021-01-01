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
    lcd->print(StringAssets::intro);
    lcd->setInverted(false);
    lcd->setCursor(5, 3);
    lcd->print(StringAssets::pressAnyKey);

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
        MenuItem menuItemRadioCtrlChannel(MainApp::getRadioChannelMenuName(config), MainApp::handleConfigRadioChannel);
        MenuItem menuItemRadioCtrlChannelScan(StringAssets::radioChannelScan, MainApp::handleConfigRadioChannel);
        MenuItem menuItemRadioCtrlLevel(MainApp::getRadioLevelMenuName(config), MainApp::handleConfigRadioLevel);
        MenuItem menuItemRadioCtrlAddress(MainApp::getRadioAddressMenuName(config), MainApp::handleConfigRadioAddress);
        menuConfigs.add_item(&menuItemRadioCtrlChannel);
        menuConfigs.add_item(&menuItemRadioCtrlChannelScan);
        menuConfigs.add_item(&menuItemRadioCtrlLevel);
        menuConfigs.add_item(&menuItemRadioCtrlAddress);
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
        if(handleKeypadSymbol(keypad->getKeypadSymbol(), &menuSystem)) {
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

const char* MainApp::getRadioChannelMenuName(ConfigStorage* config)
{
    strcpy(MainApp::radioChannelMenuName, StringAssets::radioChannel);
    strcat(MainApp::radioChannelMenuName, StringAssets::colon);
    strcat(MainApp::radioChannelMenuName, StringAssets::space);

    char ch[3];
    sprintf(ch, "%d", config->getRadioChannel());
    strcat(MainApp::radioChannelMenuName, ch);

    return MainApp::radioChannelMenuName;
}

const char* MainApp::getRadioAddressMenuName(ConfigStorage* config)
{
    strcpy(MainApp::radioAddressMenuName, StringAssets::radioAddress);
    strcat(MainApp::radioAddressMenuName, StringAssets::colon);
    strcat(MainApp::radioAddressMenuName, StringAssets::space);

    char ch[2];
    sprintf(ch, "%d", config->getRadioAddress());
    strcat(MainApp::radioAddressMenuName, ch);

    return MainApp::radioAddressMenuName;
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
    p_menu_component->set_name(MainApp::getRadioChannelMenuName(AbstractApp::sc->getConfigStorage()));
}

void MainApp::handleConfigRadioAddress(MenuComponent* p_menu_component)
{
    RadioAddressApp app;
    app.execute();
    p_menu_component->set_name(MainApp::getRadioAddressMenuName(AbstractApp::sc->getConfigStorage()));
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