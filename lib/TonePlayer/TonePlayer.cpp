#include <TonePlayer.h>

TonePlayer::TonePlayer(ConfigStorage* _configStorage, uint8_t _buzzerPin)
{
    configStorage = _configStorage;
    buzzerPin = _buzzerPin;
}

void TonePlayer::loadTones(const Tone* _tones, const uint8_t _tonesLength, bool _playLoop = false, bool _tonePriority = false)
{
    if (!configStorage->isWithSounds()) {
        return;
    }
    playLoop = _playLoop;
    tones = _tones;
    tonesLength = _tonesLength;
    tonePriority = _tonePriority;
    startAt = millis();
}

void TonePlayer::playTones(const Tone* _tones, const uint8_t _tonesLength, bool _playLoop = false, bool _tonePriority = false)
{
    loadTones(_tones, _tonesLength, _playLoop, _tonePriority);
    while(play()) {
        // waiting until the end of the tunes
    }
}

void TonePlayer::stop()
{
    tones = NULL;
}

bool TonePlayer::play()
{
    if (tones == NULL) {
        return false;
    }

    unsigned long playerHead = millis() - startAt;
    unsigned short int i;
    for (i = 0; i < tonesLength; i++) {
        if (tones[i].endAt < playerHead) {
            continue;
        }
        if (tones[i].freq == 0) {
            return true;
        }
        TimerFreeTone(buzzerPin, tones[i].freq, tonePriority ? tones[i].endAt - playerHead: nonPriorityToneDelay);

        return true;
    }

    if (playLoop) {
        startAt = millis();
    } else {
        stop();
    }

    return true;
}