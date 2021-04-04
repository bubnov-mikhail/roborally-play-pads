#pragma once
#include <ServiceContainer.h>
#include "StringAssets.h"
#include <Keypad.h>
#include <LcdAssets.h>
#include <MenuSystem.h>

class AbstractApp {
    public:
        static ServiceContainer* sc; 
        enum APPS {
            MAIN_MENU,
            CONTRAST,
            CLOCK,
            RADIO_CHANNEL,
            RADIO_CHANNEL_SCAN,
            ROBORALLY
        };
        virtual APPS execute(void) = 0;
        virtual ~AbstractApp(){};
};