#pragma once

#include "AbstractApp.h"
#include <ServiceContainer.h>
#include "StringAssets.h"
#include <Keypad.h>
#include <LcdAssets.h>
#include <MenuSystem.h>
#include <TimerFreeTone.h>
#include "ContrastCtlApp.h"

class MainApp : public AbstractApp {
    public:
        void execute(void);
        static void handleConfigBacklight(MenuComponent* p_menu_component);
        static void handleConfigSound(MenuComponent* p_menu_component);
        static void handleConfigContrast(MenuComponent* p_menu_component);
        static void handleGamesRoborally(MenuComponent* p_menu_component);
    private:
        void handleKeypadSymbol(uint8_t keypadSymbol, MenuSystem* menuSystem);
        const char* getBacklightMenuName(ConfigStorage* config);
        const char* getSoundsMenuName(ConfigStorage* config);
};