#include "ContrastCtlApp.h"

void ContrastCtlApp::execute(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();
    Headline* headline = AbstractApp::sc->getHeadline();
    ProgressBar* progressBar = new ProgressBar(lcd, 10, 74, 3, false);
    uint8_t tmpContrast = config->getContrast();

    lcd->clear(false);
    menuRenderer->render_header(StringAssets::contrast);
    headline->update(true);
    update(lcd, progressBar, tmpContrast);

    while(true) {
        headline->update();
        if (!keypad->read()) {
            continue;
        }
        switch (keypad->getKeypadSymbol()) {
            case Keypad::keyB:
                tmpContrast = increment(keypad, lcd, progressBar, headline, tmpContrast, 1);
                break;
            case Keypad::keyC:
                tmpContrast = increment(keypad, lcd, progressBar, headline, tmpContrast, -1);
                break;
            case Keypad::keyHash:
                // Reset to config
                tmpContrast = config->getContrast();
                update(lcd, progressBar, tmpContrast);
                break;
            case Keypad::keyStar:
                // Save and exit
                config->setContrast(tmpContrast);
                delete progressBar;
                return;
            case Keypad::keyD:
                // Exit without saving
                tmpContrast = config->getContrast();
                update(lcd, progressBar, tmpContrast);
                delete progressBar;
                return;
        }
    }
}

uint8_t ContrastCtlApp::increment(Keypad* keypad, Nokia_LCD* lcd, ProgressBar* progressBar, Headline* headline, uint8_t tmpContrast, int8_t direction)
{
    tmpContrast = min(contrastMax, max(contrastMin, tmpContrast + direction));
    update(lcd, progressBar, tmpContrast);
    unsigned long longDelay = millis();
    unsigned long shortDelay;

    while(true) {
        headline->update();
        if(keypad->read()) {
            return tmpContrast;
        }

        if ((millis() - longDelay) < 300) {
            continue;
        } else if(longDelay > 0) {
            longDelay = 0;
            shortDelay = millis();
        }

        if ((millis() - shortDelay) < 50) {
            continue;
        }
        shortDelay = millis();
        tmpContrast = min(contrastMax, max(contrastMin, tmpContrast + direction));
        update(lcd, progressBar, tmpContrast);
    }

    return tmpContrast;
}

void ContrastCtlApp::update(Nokia_LCD* lcd, ProgressBar* progressBar, uint8_t value)
{
    lcd->setContrast(value);
    progressBar->render((value - contrastMin) * 100 / (contrastMax - contrastMin));
}