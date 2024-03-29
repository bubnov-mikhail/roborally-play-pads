#pragma once

#include "AbstractApp.h"
#include "ContrastCtlApp.h"
#include "ClockSetupApp.h"
#include "RadioChannelApp.h"
#include "RadioChannelScanApp.h"
#include "RoborallyApp.h"
#include "RF24.h"
#include <AudioAssets.h>

class MainApp : public AbstractApp
{
public:
    MainApp();
    AbstractApp::APPS execute(void);
    static void handleConfigBacklight(MenuComponent *p_menu_component);
    static void handleConfigSound(MenuComponent *p_menu_component);
    static void handleConfigContrast(MenuComponent *p_menu_component);
    static void handleConfigClockCtrl(MenuComponent *p_menu_component);
    static void handleConfigRadioChannel(MenuComponent *p_menu_component);
    static void handleConfigRadioLevel(MenuComponent *p_menu_component);
    static void handleConfigRadioChannelScan(MenuComponent *p_menu_component);
    static void handleGamesRoborally(MenuComponent *p_menu_component);
    static char backlightMenuName[14];
    static char soundsMenuName[11];
    static char radioChannelMenuName[13];
    static char radioLevelMenuName[11];
    static AbstractApp::APPS nextApp;

private:
    bool handleKeypadSymbol(uint8_t keypadSymbol, MenuSystem *menuSystem);
    static const char *getBacklightMenuName(ConfigStorage *config);
    static const char *getSoundsMenuName(ConfigStorage *config);
    static const char *getRadioLevelMenuName(ConfigStorage *config);
};