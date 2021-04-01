#define STR_(X) #X
#define STR(X) STR_(X)

namespace StringAssets {
    const char mainMenu[] = "Menu";
    const char games[] = "Apps";
    const char configs[] = "Configs";
    const char pad[] = "Pad #" STR(PAD_NUMBER);
    const char clockCtrl[] = "Clock";
    const char clockCtrlTime[] = "Time";
    const char clockCtrlDate[] = "Date";
    const char contrast[] = "Contrast";
    const char loading[] = "Loading";
    const char backlight[] = "Light";
    const char sounds[] = "Sounds";
    const char space[] = " ";
    const char dot[] = ".";
    const char stateOn[] = "on";
    const char stateOff[] = "off";
    const char colon[] = ":";
    const char radio[] = "Network";
    const char radioLevel[] = "Level";
    const char radioChannel[] = "Channel";
    const char radioChannelScan[] = "Scan";
    const char radioSuggestedChannels[] = "Suggested";
    const char radioLevelMin[] = "MIN";
    const char radioLevelLow[] = "LOW";
    const char radioLevelMax[] = "MAX";


    //Roborally game
    const char robopad[] = "Robopad";
    const char players[] = "P";
    const char connecting[] = "Connecting";
    const char connected[] = "OK to start";
    const char enterCard[] = "Enter card";
    const char waiting[] = "Waiting";
    const char yourMove[] = "Your move";
    const char powerDown[] = "Power down";
    const char exitConfirmation[] = "OK to exit";
}