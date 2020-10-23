#pragma once

#include <TimerFreeTone.h>
#include <ConfigStorage.h>

class TonePlayer
{
    public:
        struct Tone
        {
            unsigned long freq;
            unsigned long endAt;

        };
        TonePlayer(ConfigStorage* _configStorage, uint8_t _buzzerPin);
        void loadTones(const Tone* _tones, const unsigned short int _tonesLength, bool _playLoop = false, bool _tonePriority = false);
        void playTones(const Tone* _tones, const unsigned short int _tonesLength, bool _playLoop = false, bool _tonePriority = false);
        bool play();
        void stop();
    private:
        ConfigStorage* configStorage;
        uint8_t buzzerPin;
        bool playLoop; 
        bool tonePriority; 
        unsigned long startAt;
        unsigned short int tonesLength = 0; // how many tones in a melody
        const static unsigned short int nonPriorityToneDelay = 40;
        const Tone* tones;
};