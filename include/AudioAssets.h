
#include <TonePlayer.h>

namespace AudioAssets {
    const TonePlayer::Tone roborallyIntro[] = {
        {0, 200},
        {520, 350},
        {585, 490},
        {625, 650},
        {520, 800},
        {585, 950},
        {625, 1080},
        {0, 1130},
        {1055, 1220},
        {0, 1250},
        {1055, 1380},
        {0, 1410},
        {1055, 1540},
        {0, 1570},
        {1055, 1700},
    };
    const uint8_t roborallyIntroLength = 15;
    const TonePlayer::Tone roborallyAction[] = {
        {800, 70},
        {200, 80},
        {800, 150},
        {200, 160},
        {800, 230},
        {0, 240},
        {800, 310},
    };
    const uint8_t roborallyActionLength = 7;
}