#include "RadioChannelScanApp.h"
#include "RF24.h"


AbstractApp::APPS RadioChannelScanApp::execute(void) {
    Keypad* keypad = AbstractApp::sc->getKeypad();
    Headline* headline = AbstractApp::sc->getHeadline();
    
    uint8_t bestChannel = getBestChannel();

    while(true) {
        headline->update();
        if (!keypad->read()) {
            continue;
        }
        switch (keypad->getKeypadSymbol()) {
            case Keypad::keyStar:
                // Save and exit
                AbstractApp::sc->getConfigStorage()->setRadioChannel(bestChannel);
                AbstractApp::sc->getRadio()->setChannel(bestChannel);
                return AbstractApp::APPS::MAIN_MENU;
            case Keypad::keyHash:
                // Scan again
                bestChannel = getBestChannel();
                break;
            case Keypad::keyD:
                // Exit without saving
                return AbstractApp::APPS::MAIN_MENU;
        }
    }
}

uint8_t RadioChannelScanApp::getBestChannel(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();
    Headline* headline = AbstractApp::sc->getHeadline();
    RF24* radio = AbstractApp::sc->getRadio();
    uint8_t bestChannel = config->getRadioChannel();
    uint8_t bestRangeLength = 0;
    uint8_t bestRangeStartChannel = 0;
    uint8_t currentRangeLength = 0;
    uint8_t currentRangeStartChannel = 0;
    uint8_t scanDelay = 255;

    lcd->clear(false);
    menuRenderer->render_header(StringAssets::radioChannelScan);
    headline->update(true);
    radio->powerUp();
    radio->setAutoAck(false);

    for (uint8_t ch = channelMin; ch <= channelMax; ch++) {
        headline->update();
        switch (ch) {
            case 1:
                lcd->setCursor(0, 3);
                lcd->print(ch);
                break;
            case 32:
            case 64:
                lcd->setCursor(ch - 5, 3);
                lcd->print(ch);
                break;
            case 65:
                lcd->setCursor(0, 5);
                lcd->print(ch);
                break;
            case 96:
            case 128:
                lcd->setCursor(ch - 70, 5);
                lcd->print(ch);
                break;
        }

        radio->setChannel(ch);
        radio->startListening();
        delayMicroseconds(scanDelay);
        radio->stopListening();

        if (ch <= 64) {
            lcd->setCursor(ch - 1, 2);
        } else {
            lcd->setCursor(ch - 65, 4);
        }

        if (radio->testRPD() || radio->testCarrier()){
            currentRangeLength = 0;
            lcd->draw(LcdAssets::vLine, 1, true);
        } else {
            if (currentRangeLength == 0) {
                currentRangeStartChannel = ch;
            }

            currentRangeLength++;
            
            if (currentRangeLength > bestRangeLength) {
                bestRangeStartChannel = currentRangeStartChannel;
                bestRangeLength = currentRangeLength;
            }

            lcd->draw(LcdAssets::dot, 1, true);
        }
        delay(20);
        headline->update();
    }
    radio->setAutoAck(true);
    radio->powerDown();

    if (bestRangeLength <= 2) {
        bestChannel = config->getRadioChannel();
    } else {
        bestChannel = bestRangeStartChannel + bestRangeLength / 2;
    }

    delay(500);
    lcd->clear(false);
    menuRenderer->render_header(StringAssets::radioChannelScan);
    headline->update(true);

    lcd->setCursor(0, 3);
    lcd->print(StringAssets::radioChannel);
    lcd->print(StringAssets::colon);
    lcd->print(bestChannel);

    return bestChannel;
}
