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

    MenuItem menuAbout(StringAssets::about, MainApp::handleAbout);
   
    menuSystem.get_root_menu().set_name(StringAssets::mainMenu);
    menuSystem.get_root_menu().add_menu(&menuGames);
    menuSystem.get_root_menu().add_menu(&menuConfigs);
    menuSystem.get_root_menu().add_item(&menuAbout);

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
    return config->isWithBacklight() ? StringAssets::backlightOn : StringAssets::backlightOff;
}

const char* MainApp::getSoundsMenuName(ConfigStorage* config)
{
    return config->isWithSounds() ? StringAssets::soundsOn : StringAssets::soundsOff;
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
    Keypad* keypad = AbstractApp::sc->getKeypad();
    keypad->setBeepOnClick(config->isWithSounds());
    p_menu_component->set_name(MainApp::getSoundsMenuName(config));
}

void MainApp::handleGamesRoborally(MenuComponent* p_menu_component)
{
    RoborallyApp app;
    app.execute();
}

void MainApp::handleAbout(MenuComponent* p_menu_component)
{
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();
    Headline* headline = AbstractApp::sc->getHeadline();

    lcd->clear(false);
    menuRenderer->render_header(StringAssets::playpad);
    lcd->setCursor(0, 2);
    lcd->println(StringAssets::version);
    lcd->println(StringAssets::createdBy);
    lcd->println(StringAssets::mihailBubnov);
    headline->update(true);
    while(true) {
        headline->update();
        if (!keypad->read()) {
            continue;
        }
        if (keypad->getKeypadSymbol() == Keypad::keyD) {
            break;
        }
    }
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