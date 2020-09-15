#pragma once

#include "AbstractApp.h"
#include <ProgressBar.h>

class ClockSetupApp : public AbstractApp {
    public:
        void execute(void);
    private:
        enum ClockComponent {HOUR, MINUTE, DAY, MONTH, YEAR};
        ClockComponent currentComponent;
        short int componentValue = 0;
        unsigned long blinkLastUpdated;
        bool selectComponent = true; //Highligh a selected component with inverted color
        const static unsigned short int blinkRefreshMilis = 500;
        const static unsigned short int yearOffset = 1970;
        void drawComponents(void);
        void printValue(short int value);
        void setComponentValue(void);
        void nextComponent(void);
        void prevComponent(void);
        void increaseValue(void);
        void decreaseValue(void);
        void changeComponent(bool increase);
        void changeValue(bool increase);
};