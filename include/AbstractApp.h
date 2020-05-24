#pragma once
#include <ServiceContainer.h>
#include "StringAssets.h"
#include <Keypad.h>
#include <LcdAssets.h>
#include <MenuSystem.h>
#include <TimerFreeTone.h>

class AbstractApp {
    public:
        static ServiceContainer* sc; 
        virtual void execute(void) = 0;
};