#include <TonePlayer.h>

TonePlayer::TonePlayer(ConfigStorage *_configStorage, uint8_t _buzzerPin)
{
    configStorage = _configStorage;
    buzzerPin = _buzzerPin;
}

void TonePlayer::loadTones(const Tone *_tones, const uint8_t _tonesLength, bool _playLoop, bool _tonePriority)
{
    if (!configStorage->isWithSounds())
    {
        return;
    }
    playLoop = _playLoop;
    tones = _tones;
    tonesLength = _tonesLength;
    tonePriority = _tonePriority;
    startAt = millis();
}

void TonePlayer::playTones(const Tone *_tones, const uint8_t _tonesLength, bool _playLoop)
{
    loadTones(_tones, _tonesLength, _playLoop, true);
    while (play())
    {
        // waiting until the end of the tunes
    }
}

void TonePlayer::stop()
{
    tones = NULL;
}

bool TonePlayer::play()
{
    if (tones == NULL)
    {
        return false;
    }

    unsigned long playerHead = millis() - startAt;
    unsigned long cumulativeEndAt = 0;
    unsigned short int i;
    for (i = 0; i < tonesLength; i++)
    {
        cumulativeEndAt += tones[i].endAt;
        if (cumulativeEndAt < playerHead)
        {
            continue;
        }
        if (tones[i].freq == 0)
        {
            return true;
        }

        TimerFreeTone(buzzerPin, tones[i].freq, tonePriority ? cumulativeEndAt - playerHead : nonPriorityToneDelay);

        return true;
    }

    if (playLoop)
    {
        startAt = millis();
    }
    else
    {
        stop();
    }

    return true;
}