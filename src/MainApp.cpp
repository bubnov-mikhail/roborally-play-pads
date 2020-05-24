#include "MainApp.h"

void MainApp::execute(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();

    lcd->clear(false);
    lcd->setCursor(76, 0);
    lcd->draw(LcdAssets::batteryFull, sizeof(LcdAssets::batteryFull) / sizeof(uint8_t), true);
    lcd->setCursor(27, 1);
    lcd->setInverted(true);
    lcd->print(StringAssets::intro);
    lcd->setInverted(false);
    lcd->setCursor(5, 3);
    lcd->print(StringAssets::pressAnyKey);

    while(!keypad->isKeypadUpdated()) {
        continue;
    }

    MenuSystem menuSystem(*(AbstractApp::sc->getMenuRenderer()));
    Menu menuGames(StringAssets::games);
    MenuItem menuItemRoborally(StringAssets::roborally, MainApp::handleGamesRoborally);
    menuGames.add_item(&menuItemRoborally);

    Menu menuConfigs(StringAssets::configs);
    MenuItem menuItemBacklight(MainApp::getBacklightMenuName(config), MainApp::handleConfigBacklight);
    MenuItem menuItemSounds(MainApp::getSoundsMenuName(config), MainApp::handleConfigSound);
    MenuItem menuItemContrast(StringAssets::contrast, MainApp::handleGamesRoborally);
    menuConfigs.add_item(&menuItemBacklight);
    menuConfigs.add_item(&menuItemSounds);
    menuConfigs.add_item(&menuItemContrast);

    MenuItem menuAbout(StringAssets::about, MainApp::handleAbout);

    menuSystem.get_root_menu().set_name(StringAssets::mainMenu);
    menuSystem.get_root_menu().add_menu(&menuGames);
    menuSystem.get_root_menu().add_menu(&menuConfigs);
    menuSystem.get_root_menu().add_item(&menuAbout);

    menuSystem.display();

    while(true) {
        if (!keypad->isKeypadUpdated()) {
            continue;
        }
        handleKeypadSymbol(keypad->getKeypadSymbol(), &menuSystem);
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
    TimerFreeTone(6, 350, 200); 
    TimerFreeTone(6, 250, 300); 
}

void MainApp::handleAbout(MenuComponent* p_menu_component)
{
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    lcd->clear(false);
    lcd->setCursor(0, 1);
    lcd->setInverted(true);
    lcd->println(StringAssets::playpad);
    lcd->setInverted(false);
    lcd->println(StringAssets::version);
    lcd->println(StringAssets::createdBy);
    lcd->println(StringAssets::mihailBubnov);
    while(true) {
        if (!keypad->isKeypadUpdated()) {
            continue;
        }

        if (keypad->getKeypadSymbol() == Keypad::keyD) {
            break;
        }
    }
}

void MainApp::handleConfigContrast(MenuComponent* p_menu_component)
{
    ContrastCtlApp contrastApp;
    contrastApp.execute();
}

void MainApp::handleKeypadSymbol(uint8_t keypadSymbol, MenuSystem* menuSystem)
{
    if (keypadSymbol == Keypad::keyMultiSymbol || keypadSymbol == Keypad::keyReleasedSymbol) {
        return;
    }

    switch (keypadSymbol) {
      case Keypad::keyB:
        menuSystem->prev();
        menuSystem->display();
        break;
      case Keypad::keyC:
        menuSystem->next();
        menuSystem->display();
        break;
      case Keypad::keyStar:
        menuSystem->select();
        menuSystem->display();
        break;
      case Keypad::keyD:
        menuSystem->back();
        menuSystem->display();
        break;
    }
}