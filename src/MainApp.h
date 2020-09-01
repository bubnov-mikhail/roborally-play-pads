#pragma once

#include "AbstractApp.h"
#include "ContrastCtlApp.h"
#include "ClockSetupApp.h"
#include "RoborallyApp.h"

class MainApp : public AbstractApp {
    public:
        void execute(void);
        static void handleConfigBacklight(MenuComponent* p_menu_component);
        static void handleConfigSound(MenuComponent* p_menu_component);
        static void handleConfigContrast(MenuComponent* p_menu_component);
        static void handleConfigClockCtrl(MenuComponent* p_menu_component);
        static void handleAbout(MenuComponent* p_menu_component);
        static void handleGamesRoborally(MenuComponent* p_menu_component);
    private:
        bool handleKeypadSymbol(uint8_t keypadSymbol, MenuSystem* menuSystem);
        static const char* getBacklightMenuName(ConfigStorage* config);
        static const char* getSoundsMenuName(ConfigStorage* config);
};