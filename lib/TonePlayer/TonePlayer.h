#pragma once

#include <TimerFreeTone.h>
#include <ConfigStorage.h>

class TonePlayer
{
public:
    struct Tone
    {
        unsigned short int freq;
        unsigned short int endAt;
    };
    TonePlayer(ConfigStorage *_configStorage, uint8_t _buzzerPin);
    void loadTones(const Tone *_tones, const uint8_t _tonesLength, bool _playLoop = false, bool _tonePriority = false);
    void playTones(const Tone *_tones, const uint8_t _tonesLength, bool _playLoop = false);
    bool play();
    void stop();

private:
    ConfigStorage *configStorage;
    uint8_t buzzerPin;
    bool playLoop;
    bool tonePriority;
    unsigned long startAt;
    uint8_t tonesLength = 0; // how many tones in a melody
    const static unsigned short int nonPriorityToneDelay = 40;
    const Tone *tones;
};