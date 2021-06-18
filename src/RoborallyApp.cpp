#include "RoborallyApp.h"
#include <ByteLoader.h>
#include <SPI.h>
#include <ProgressBar.h>
#include <AudioAssets.h>

RoborallyApp::RoborallyApp(unsigned short int _playPadNumber, unsigned short int _anounceSelfMillis)
{
    playPadNumber = _playPadNumber;
    anounceSelfMillis = _anounceSelfMillis;
}

AbstractApp::APPS RoborallyApp::execute(void)
{
    Headline *headline = AbstractApp::sc->getHeadline();
    TonePlayer *tonePlayer = AbstractApp::sc->getTonePlayer();

    monitorLastUpdated = millis() - noiseRefreshTimeMilis;
    roundLastUpdated = millis() - roundRefreshTimeMilis;
    anounceSelfLastUpdated = millis() - anounceSelfMillis;
    screenState = GameState::REFRESH_REQUIRED;
    monitorState = GameState::REFRESH_REQUIRED;
    messageState = GameState::REFRESH_REQUIRED;
    round = 0;

    drawMainScreen(LcdAssets::roborallyMainScreenLength, LcdAssets::roborallyMainScreenAddress, 1);
    tonePlayer->playTones(AudioAssets::roborallyIntro, AudioAssets::roborallyIntroLength, false);

    init();
    PlayPad *self = getSelf();
    self->state = GameState::CONNECTING;

    while (true)
    {
        headline->update();
        drawRound();
        updateMonitor();
        printMessage();
        printCardMonitor();
        handleKeypad();
        flashlightBlink();
        listen();
        markOffline();
        handleGameState();
        syncStates();
        // tonePlayer->play(); // In case of a back music
        if (self->state == GameState::DO_EXIT)
        {
            flashlightTurnOff();
            // tonePlayer->stop(); // In case of a back music
            AbstractApp::sc->getLcd()->clear(false);
            self->state = OFFLINE;
            syncStates(true);
            AbstractApp::sc->getRadio()->powerDown();
            return AbstractApp::APPS::MAIN_MENU;
        }
    }
}

RoborallyApp::PlayPad *RoborallyApp::getSelf()
{
    return &playPads[playPadNumber];
}

void RoborallyApp::handleKeypad(void)
{
    Keypad *keypad = AbstractApp::sc->getKeypad();

    if (!keypad->read())
    {
        return;
    }

    PlayPad *self = getSelf();
    uint8_t keypadSymbol = keypad->getKeypadSymbol();
    // We can exit the app at any state
    if (keypadSymbol == Keypad::keyA)
    {
        statePriorExitConfirmation = self->state;
        self->state = GameState::EXIT_CONFIRMATION;
        return;
    }

    switch (self->state)
    {
    case GameState::EXIT_CONFIRMATION:
        if (keypadSymbol == Keypad::keyStar)
        {
            self->state = GameState::DO_EXIT;
            return;
        }
        if (keypadSymbol == Keypad::keyHash || keypadSymbol == Keypad::keyD)
        {
            self->state = statePriorExitConfirmation;
            return;
        }
        break;
    case GameState::CONNECTING:
    case GameState::CONNECTED:
        if (getPlayPadsConnected() == 1)
        {
            break;
        }
        //Start the game with at least 2 players, inc self
        if (keypadSymbol == Keypad::keyStar)
        {
            self->state = GameState::ENTERING_CARD;
            validCardNumberEntered = false;
            round = 1;
            activityRequired();
        }
        break;
    case GameState::ENTERING_CARD:
    {
        uint16_t cardNumber = self->cardNumber;
        if (keypadSymbol <= Keypad::key9)
        {
            uint16_t cardNumberNew = cardNumber * 10 + keypadSymbol;
            if (cardNumberNew > maxCardNumber)
            {
                break;
            }
            self->cardNumber = cardNumberNew;
            break;
        }

        if (keypadSymbol == Keypad::keyD)
        {
            self->cardNumber = 0;
            break;
        }

        if (keypadSymbol == Keypad::keyHash)
        {
            self->cardNumber = max(0, cardNumber / 10);
            break;
        }

        if (keypadSymbol == Keypad::keyStar && validCardNumberEntered)
        {
            if (self->cardNumber == powerDownCardNumber)
            {
                self->cardNumber = 0;
                self->state = GameState::POWER_DOWN;
                return;
            }

            self->state = GameState::WAITING_QUORUM;
            syncStates(true);
        }

        break;
    }
    case GameState::YOUR_MOVE:
    {
        if (keypadSymbol == Keypad::keyStar)
        {
            self->state = GameState::NEXT_PHASE_WAITING;
            self->cardNumber = 0;
            syncStates(true);
        }
        break;
    }
    }
}

void RoborallyApp::drawRound()
{
    GameState gameState = playPads[playPadNumber].state;
    if (gameState == GameState::CONNECTING || gameState == GameState::CONNECTED)
    {
        // DO not draw round while the game is not started yet
        return;
    }

    if (!isReachedTimer(roundLastUpdated, roundRefreshTimeMilis))
    {
        return;
    }
    roundLastUpdated = millis();

    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    roundDisplayCurrent = !roundDisplayCurrent;
    for (uint8_t x = 0; x < 5; x++)
    {
        lcd->setCursor(x * 12 + 5, 4);
        if (round != 0 && x == round - 1)
        {
            lcd->draw(roundDisplayCurrent ? LcdAssets::roborallyRoundFilled : LcdAssets::roborallyRoundEmpty, 2, true);
            continue;
        }
        lcd->draw(round > 0 && (x <= round - 1) ? LcdAssets::roborallyRoundFilled : LcdAssets::roborallyRoundEmpty, 2, true);
    }
}

void RoborallyApp::printCardMonitor()
{
    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(4, 2);
    GameState gameState = playPads[playPadNumber].state;
    switch (gameState)
    {
    case GameState::CONNECTING:
    case GameState::CONNECTED:
        lcd->print(StringAssets::players);
        lcd->print(StringAssets::colon);
        lcd->print(getPlayPadsConnected());
        lcd->print(StringAssets::space);
        break;
    case GameState::ENTERING_CARD:
    case GameState::YOUR_MOVE:
    {
        uint16_t cardNumber = playPads[playPadNumber].cardNumber;
        for (uint8_t i = 4; i > 0; i--)
        {
            if (pow(10, i - 1) <= cardNumber)
            {
                lcd->print(cardNumber);
                break;
            }
            lcd->print(StringAssets::space);
        }
        break;
    }
    default:
        for (uint8_t i = 0; i < 4; i++)
        {
            lcd->print(StringAssets::space);
        }
    }
}

void RoborallyApp::printMessage()
{
    GameState gameState = playPads[playPadNumber].state;
    if (gameState == messageState && (gameState != GameState::ENTERING_CARD || round > 1 || !isReachedTimer(pwdHintLastUpdated, pwdHintBlinkMillis)))
    {
        return;
    }

    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(3, 5);
    for (uint8_t i = 0; i < 13; i++)
    {
        lcd->print(StringAssets::space);
    }
    lcd->setCursor(3, 5);

    switch (gameState)
    {
    case GameState::CONNECTING:
        lcd->print(StringAssets::connecting);
        break;
    case GameState::CONNECTED:
        lcd->print(StringAssets::connected);
        break;
    case GameState::ENTERING_CARD:
        lcd->print(pwdHintDisplay && round == 1 ? StringAssets::pwdHint : StringAssets::enterCard);

        pwdHintDisplay = !pwdHintDisplay;
        pwdHintLastUpdated = millis();
        break;
    case GameState::WAITING_QUORUM:
    case GameState::WAITING_HIGHER_CARD_NUMBER:
    case GameState::NEXT_PHASE_WAITING:
        lcd->print(StringAssets::waiting);
        break;
    case GameState::YOUR_MOVE:
        lcd->print(StringAssets::yourMove);
        break;
    case GameState::POWER_DOWN:
        lcd->print(StringAssets::powerDown);
        break;
    case GameState::EXIT_CONFIRMATION:
        lcd->print(StringAssets::exitConfirmation);
        break;
    default:
        lcd->print(gameState);
    }

    messageState = gameState;
}

void RoborallyApp::updateMonitor(void)
{
    GameState gameState = playPads[playPadNumber].state;

    if (gameState == monitorState && gameState != GameState::ENTERING_CARD && gameState != GameState::WAITING_QUORUM && gameState != GameState::WAITING_HIGHER_CARD_NUMBER && gameState != GameState::NEXT_PHASE_WAITING && gameState != GameState::CONNECTING && gameState != GameState::CONNECTED && gameState != GameState::EXIT_CONFIRMATION)
    {
        return;
    }

    unsigned long monitorRefreshTimeMilis = (gameState == GameState::CONNECTED || gameState == GameState::CONNECTING || gameState == GameState::WAITING_QUORUM || gameState == GameState::WAITING_HIGHER_CARD_NUMBER || gameState == GameState::NEXT_PHASE_WAITING || gameState == GameState::EXIT_CONFIRMATION)
                                                ? waitingRefreshTimeMilis
                                                : noiseRefreshTimeMilis;

    if (!isReachedTimer(monitorLastUpdated, monitorRefreshTimeMilis))
    {
        return;
    }
    monitorLastUpdated = millis();

    unsigned char *bitmap = new unsigned char[LcdAssets::roborallyMovesBitmapLength];

    ByteLoader *bitmapLoader = AbstractApp::sc->getByteLoader32();
    switch (gameState)
    {
    case GameState::CONNECTING:
    case GameState::CONNECTED:
    case GameState::WAITING_QUORUM:
    case GameState::WAITING_HIGHER_CARD_NUMBER:
    case GameState::NEXT_PHASE_WAITING:
    case GameState::EXIT_CONFIRMATION:
        bitmapLoader->loadBitmap(
            bitmap,
            LcdAssets::roborallyMoveWait1Address + waitingPhase * LcdAssets::roborallyMovesBitmapLength,
            LcdAssets::roborallyMovesBitmapLength);
        waitingPhase++;
        if (waitingPhase > waitingPhaseMax)
        {
            waitingPhase = 0;
        }
        break;
    case GameState::POWER_DOWN:
        bitmapLoader->loadBitmap(
            bitmap,
            LcdAssets::roborallyMovePowerDownAddress,
            LcdAssets::roborallyMovesBitmapLength);
        break;
    case GameState::ENTERING_CARD:
    case GameState::YOUR_MOVE:
    {
        validCardNumberEntered = false;
        uint16_t cardNumber = playPads[playPadNumber].cardNumber;
        if (cardNumber > maxCardNumber || (cardNumber == powerDownCardNumber && round > 1))
        {
            generateNoise(bitmap);
            break;
        }

        if (cardNumber == powerDownCardNumber && round == 1)
        {
            bitmapLoader->loadBitmap(
                bitmap,
                LcdAssets::roborallyMovePowerDownAddress,
                LcdAssets::roborallyMovesBitmapLength);
            validCardNumberEntered = true;
            break;
        }

        ByteLoader *cardsLoader = AbstractApp::sc->getByteLoader08();
        unsigned int moveCardAddress = cardNumber / 10;

        uint8_t moveCard = moveCardAddress * 10 == cardNumber
                               ? uint8_t(cardsLoader->loadByte(cardNumber / 10))
                               : 0;

        unsigned int moveAddress = 0;
        switch (moveCard)
        {
        case Moves::U_TURN:
            moveAddress = LcdAssets::roborallyMoveUTurnAddress;
            break;
        case Moves::BACKUP:
            moveAddress = LcdAssets::roborallyMoveBackAddress;
            break;
        case Moves::ROTATE_RIGHT:
            moveAddress = LcdAssets::roborallyMoveRightAddress;
            break;
        case Moves::ROTATE_LEFT:
            moveAddress = LcdAssets::roborallyMoveLeftAddress;
            break;
        case Moves::FORWARD:
            moveAddress = LcdAssets::roborallyMoveForwardAddress;
            break;
        case Moves::FORWARD_2:
            moveAddress = LcdAssets::roborallyMoveForward2Address;
            break;
        case Moves::FORWARD_3:
            moveAddress = LcdAssets::roborallyMoveForward3Address;
            break;
        }

        if (moveAddress == 0)
        {
            // unknown card
            generateNoise(bitmap);
            break;
        }

        validCardNumberEntered = true;
        bitmapLoader->loadBitmap(
            bitmap,
            moveAddress,
            LcdAssets::roborallyMovesBitmapLength);
        break;
    }
    default:
        // fallback to noise
        generateNoise(bitmap);
    }

    drawMonitor(bitmap);
    monitorState = gameState;
    delete bitmap;
}

void RoborallyApp::drawMonitor(unsigned char *bitmap)
{
    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(60, 2);

    unsigned char *bitmapUpper = new unsigned char[LcdAssets::roborallyMovesBitmapHalfLength];
    unsigned char *bitmapLower = new unsigned char[LcdAssets::roborallyMovesBitmapHalfLength];
    for (uint8_t i = 0; i < LcdAssets::roborallyMovesBitmapHalfLength; i++)
    {
        bitmapUpper[i] = bitmap[i];
    }
    for (uint8_t i = LcdAssets::roborallyMovesBitmapHalfLength; i < LcdAssets::roborallyMovesBitmapLength; i++)
    {
        bitmapLower[i - LcdAssets::roborallyMovesBitmapHalfLength] = bitmap[i];
    }

    lcd->draw(bitmapUpper, LcdAssets::roborallyMovesBitmapHalfLength, false);
    lcd->setCursor(60, 3);
    lcd->draw(bitmapLower, LcdAssets::roborallyMovesBitmapHalfLength, false);

    delete bitmapUpper;
    delete bitmapLower;
}

void RoborallyApp::generateNoise(unsigned char *bitmap)
{
    for (uint8_t i = 0; i < LcdAssets::roborallyMovesBitmapLength; i++)
    {
        if (i == 0 || i == LcdAssets::roborallyMovesBitmapHalfLength - 1 || i == LcdAssets::roborallyMovesBitmapHalfLength || i == LcdAssets::roborallyMovesBitmapLength - 1)
        {
            bitmap[i] = 0x0;
            continue;
        }
        bitmap[i] = LcdAssets::noise[random(0, 5)];
    }
}

void RoborallyApp::flashlightBlink()
{
    if (!flashlightOn)
    {
        return;
    }

    if (isReachedTimer(flashlightCreated, flashlightBlinkMillis * flashlightBlinks))
    {
        flashlightTurnOff();
        return;
    }

    uint8_t flashState = ((millis() - flashlightCreated) / flashlightBlinkMillis) % 2 == 0
                             ? flashlightBAddress
                             : flashlightAAddress;

    if (flashlightLastState == flashState)
    {
        return;
    }

    SPI.begin();
    digitalWrite(spiMoSiCs, LOW);
    SPI.transfer(flashState);
    digitalWrite(spiMoSiCs, HIGH);
    SPI.end();
}

void RoborallyApp::flashlightTurnOn()
{
    flashlightOn = true;
    flashlightCreated = millis();
}

void RoborallyApp::flashlightTurnOff()
{
    flashlightOn = false;
    SPI.begin();
    digitalWrite(spiMoSiCs, LOW);
    SPI.transfer(0x00);
    digitalWrite(spiMoSiCs, HIGH);
    SPI.end();
}

void RoborallyApp::listen(void)
{
    RF24 *radio = AbstractApp::sc->getRadio();
    uint8_t pipe;

    // Reinit the radio ???
    // if (playPadNumber != 0)
    // {
    //     radio->openReadingPipe(0, addresses[0]);
    // }
    if (!radio->available(&pipe))
    {
        return;
    }

    if (pipe >= maxPlayers || pipe == playPadNumber)
    {
        // Invalid pipe
        return;
    }

    PlayPad input;
    radio->read(&input, sizeof(input));

    if (!isValidPlayPad(input))
    {
        return;
    }

    bool nextPhaseWaitingBefore = haveQuorum(GameState::NEXT_PHASE_WAITING);

    playPads[pipe].state = input.state;
    playPads[pipe].cardNumber = input.cardNumber;
    // Update heart beat so, the markOffline will not mark this pad as offline... for now.
    playPads[pipe].heartBeatLastUpdated = millis();

    if (getSelf()->state == GameState::POWER_DOWN && !nextPhaseWaitingBefore && haveQuorum(GameState::NEXT_PHASE_WAITING)) {
        round++;
    }
}

void RoborallyApp::syncStates(bool force)
{
    if (!force && !isReachedTimer(anounceSelfLastUpdated, anounceSelfMillis))
    {
        return;
    }
    anounceSelfLastUpdated = millis();

    PlayPad *self = getSelf();
    RF24 *radio = AbstractApp::sc->getRadio();

    radio->stopListening();
    for (uint8_t i = 0; i < maxPlayers; i++)
    {
        if (playPadNumber == i)
        {
            continue;
        }
        radio->write(self, sizeof(PlayPad));
    }
    radio->startListening();
}

void RoborallyApp::activityRequired()
{
    TonePlayer *tonePlayer = AbstractApp::sc->getTonePlayer();
    tonePlayer->playTones(AudioAssets::roborallyAction, AudioAssets::roborallyActionLength, false);
    flashlightTurnOn();
}

uint8_t RoborallyApp::getPlayPadsConnected(void)
{
    uint8_t connected = 0;
    for (uint8_t i = 0; i < maxPlayers; i++)
    {
        if (playPads[i].state != GameState::OFFLINE)
        {
            connected++;
        }
    }

    return connected;
}

void RoborallyApp::markOffline(void)
{
    unsigned long m = millis();
    unsigned long heartBeatLastUpdated;

    for (uint8_t i = 0; i < maxPlayers; i++)
    {
        if (playPadNumber == i || playPads[i].state == GameState::OFFLINE)
        {
            continue;
        }

        heartBeatLastUpdated = playPads[i].heartBeatLastUpdated;
        if (m > heartBeatLastUpdated && m - heartBeatLastUpdated > heartBeatMaxMillis)
        {
            playPads[i].state = GameState::OFFLINE;
        }
    }
}

void RoborallyApp::init(void)
{
    RF24 *radio = AbstractApp::sc->getRadio();
    radio->powerUp();
    radio->setPayloadSize(sizeof(PlayPad));
    radio->setAutoAck(false);
    /**
     * According to the datasheet, the auto-retry features's delay value should
     * be "skewed" to allow the RX node to receive 1 transmission at a time.
     * So, use varying delay between retry attempts and 15 (at most) retry attempts
    */
    radio->setRetries(((playPadNumber * 3) % 12) + 3, 15);
    for (uint8_t i = 0; i < maxPlayers; i++)
    {
        playPads[i].heartBeatLastUpdated = millis();

        // set pipes
        if (i == playPadNumber)
        {
            radio->openWritingPipe(addresses[i]);
        }
        else
        {
            radio->openReadingPipe(i, addresses[i]);
        }
    }
}

void RoborallyApp::drawMainScreen(const unsigned short int length, unsigned int address, uint8_t lcdY)
{
    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    ByteLoader *byteLoader32 = AbstractApp::sc->getByteLoader32();
    Headline *headline = AbstractApp::sc->getHeadline();

    lcd->clear(false);
    headline->update(true);
    AbstractApp::sc->getMenuRenderer()->render_header(StringAssets::loading);
    ProgressBar *progressBar = new ProgressBar(lcd, 10, 74, 3, true);
    progressBar->render(0);

    unsigned char *bitmap = new unsigned char[LcdAssets::roborallyMainScreenLength];

    for (unsigned int i = 0; i < LcdAssets::roborallyMainScreenLength; i++)
    {
        byteLoader32->loadByteToPosition(bitmap, LcdAssets::roborallyMainScreenAddress + i, i);
        progressBar->render(i * 100 / LcdAssets::roborallyMainScreenLength);
        headline->update();
    }
    delete progressBar;

    lcd->setCursor(0, 1);
    lcd->draw(bitmap, LcdAssets::roborallyMainScreenLength, false);
    delete bitmap;
}

bool RoborallyApp::haveQuorum(GameState state)
{
    for (uint8_t i = 0; i < maxPlayers; i++)
    {
        if (playPads[i].state == GameState::OFFLINE || (state != GameState::POWER_DOWN && playPads[i].state == GameState::POWER_DOWN))
        {
            continue;
        }

        if (state != playPads[i].state)
        {
            return false;
        }
    }

    return true;
}

bool RoborallyApp::isValidPlayPad(PlayPad input)
{
    return input.cardNumber >= 0 && input.cardNumber <= maxCardNumber && input.state >= GameState::OFFLINE && input.state <= GameState::DO_EXIT;
}

bool RoborallyApp::isReachedTimer(unsigned long lastUpdated, unsigned long refreshTimeMilis)
{
    unsigned long m = millis();
    return m < lastUpdated
               ? true
               : m - lastUpdated > refreshTimeMilis;
}

void RoborallyApp::handleGameState(void)
{
    PlayPad *self = getSelf();
    if (getPlayPadsConnected() == 1 && self->state != GameState::CONNECTING && self->state != GameState::EXIT_CONFIRMATION && self->state != GameState::DO_EXIT)
    {
        self->state = GameState::CONNECTING;
        self->cardNumber = 0;
        return;
    }

    switch (self->state)
    {
    case GameState::CONNECTING:
    case GameState::CONNECTED:
        if (getPlayPadsConnected() > 1)
        {
            self->state = GameState::CONNECTED;
        }
        for (uint8_t i = 0; i < maxPlayers; i++)
        {
            if (playPads[i].state == GameState::ENTERING_CARD)
            {
                // Some one started the game
                round = 1;
                validCardNumberEntered = false;
                self->state = GameState::ENTERING_CARD;
                activityRequired();
                return;
            }
        }
        break;
    case GameState::POWER_DOWN:
        if (haveQuorum(GameState::POWER_DOWN))
        {
            // All pads are Power down.
            self->state = GameState::ENTERING_CARD;
            // Delay the anounceSelf to make sure all pads will switch to the ENTERING_CARD state
            anounceSelfLastUpdated += anounceSelfMillis;
            activityRequired();
            break;
        }

        if (haveQuorum(GameState::NEXT_PHASE_WAITING))
        {
            if (round > 5)
            {
                self->state = GameState::NEXT_PHASE_WAITING;
                // Delay the anounceSelf to make sure all pads will switch to the ENTERING_CARD state
                anounceSelfLastUpdated += anounceSelfMillis;
            }
        }
        break;
    case GameState::WAITING_QUORUM:
        if (haveQuorum(GameState::WAITING_QUORUM))
        {
            self->state = GameState::WAITING_HIGHER_CARD_NUMBER;
            // Delay the anounceSelf to make sure all pads will switch to the ENTERING_CARD state
            anounceSelfLastUpdated += anounceSelfMillis;
        }
        break;
    case GameState::WAITING_HIGHER_CARD_NUMBER:
    {
        // Check the highest card number left
        uint16_t highestCardNumberLeft = 0;
        for (uint8_t i = 0; i < maxPlayers; i++)
        {
            highestCardNumberLeft = max(highestCardNumberLeft, playPads[i].cardNumber);
        }

        if (self->cardNumber == highestCardNumberLeft)
        {
            self->state = GameState::YOUR_MOVE;
            activityRequired();
        }
        break;
    }
    case GameState::NEXT_PHASE_WAITING:
        if (haveQuorum(GameState::NEXT_PHASE_WAITING))
        {
            round = round + 1;
            if (round > 5)
            {
                round = 1;
            }
            validCardNumberEntered = false;
            self->state = GameState::ENTERING_CARD;
            // Delay the anounceSelf to make sure all pads will switch to the ENTERING_CARD state
            anounceSelfLastUpdated += anounceSelfMillis;
            activityRequired();
        }
        break;
    }
}
