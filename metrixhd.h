#pragma once

#include <vdr/skins.h>
#include <vdr/videodir.h>

class cMetrixHDDisplayMenu;
extern cTheme Theme;

// BUTTONS
#define CLR_BUTTONRED                       0x99BB0000
#define CLR_BUTTONGREEN                     0x9900BB00
#define CLR_BUTTONYELLOW                    0x99BBBB00
#define CLR_BUTTONBLUE                      0x990000BB

// MESSAGES
#define CLR_MESSAGESTATUS                   0xBB0000FF
#define CLR_MESSAGEINFO                     0xBB009900
#define CLR_MESSAGEWARNING                  0xBBBBBB00
#define CLR_MESSAGEERROR                    0xBBBB0000

// TopBar
THEME_CLR(Theme, clrTopBarBg,               0xBB101010);
THEME_CLR(Theme, clrTopBarFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrTopBarDateTimeFont,     0xFFEEEEEE);
THEME_CLR(Theme, clrTopGreen,               0xFF63EA42);

// Buttons
THEME_CLR(Theme, clrButtonBg,               0xBB101010);
THEME_CLR(Theme, clrButtonFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrButtonRed,              CLR_BUTTONRED);
THEME_CLR(Theme, clrButtonGreen,            CLR_BUTTONGREEN);
THEME_CLR(Theme, clrButtonYellow,           CLR_BUTTONYELLOW);
THEME_CLR(Theme, clrButtonBlue,             CLR_BUTTONBLUE);

// Messages
THEME_CLR(Theme, clrMessageBg,              0xBB101010);
THEME_CLR(Theme, clrMessageFont,            0xFFEEEEEE);

THEME_CLR(Theme, clrMessageStatus,          CLR_MESSAGESTATUS);
THEME_CLR(Theme, clrMessageInfo,            CLR_MESSAGEINFO);
THEME_CLR(Theme, clrMessageWarning,         CLR_MESSAGEWARNING);
THEME_CLR(Theme, clrMessageError,           CLR_MESSAGEERROR);

// Channel
THEME_CLR(Theme, clrChannelBg,              0xBB101010);
THEME_CLR(Theme, clrChannelFontTitle,       0xFF63EA42);
THEME_CLR(Theme, clrChannelFontEpg,         0xFFEEEEEE);
THEME_CLR(Theme, clrChannelProgressFg,      0xFFEEEEEE);
THEME_CLR(Theme, clrChannelProgressBarFg,   0xFF63EA42);
THEME_CLR(Theme, clrChannelProgressBg,      0xBB101010);

// Menu
THEME_CLR(Theme, clrMenuBg,                 0xBB101010);
THEME_CLR(Theme, clrItemBg,                 0xBB101010);
THEME_CLR(Theme, clrItemFont,               0xFFEEEEEE);
THEME_CLR(Theme, clrItemCurrentBg,          0xBB3090B0);
THEME_CLR(Theme, clrItemCurrentFont,        0xFFEEEEEE);
THEME_CLR(Theme, clrItemSelableBg,          0xBB101010);
THEME_CLR(Theme, clrItemSelableFont,        0xFFEEEEEE);
THEME_CLR(Theme, clrScrollbarFg,            0xFFEEEEEE);
THEME_CLR(Theme, clrScrollbarBarFg,         0xFF63EA42);
THEME_CLR(Theme, clrScrollbarBg,            0xBB101010);
// Menu Channel
THEME_CLR(Theme, clrMenuChannelContentBg,   0xFF63EA42);
// Menu Event
THEME_CLR(Theme, clrMenuEventBg,            0xBB101010);
THEME_CLR(Theme, clrMenuEventFontTitle,     0xFFEEEEEE);
THEME_CLR(Theme, clrMenuEventFontInfo,      0xFFEEEEEE);
THEME_CLR(Theme, clrMenuEventContentBg,     0xFF63EA42);
// Menu Recording
THEME_CLR(Theme, clrMenuRecBg,              0xBB101010);
THEME_CLR(Theme, clrMenuRecFontTitle,       0xFFEEEEEE);
THEME_CLR(Theme, clrMenuRecFontInfo,        0xFFEEEEEE);
THEME_CLR(Theme, clrMenuRecContentBg,       0xFF63EA42);
THEME_CLR(Theme, clrIconTransparent,        0x00FFFFFF);
// Menu Text (Multiline)
THEME_CLR(Theme, clrMenuTextBg,             0xBB101010);
THEME_CLR(Theme, clrMenuTextFont,           0xFFEEEEEE);

// Replay
THEME_CLR(Theme, clrReplayBg,               0xBB101010);
THEME_CLR(Theme, clrReplayFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrReplayProgressFg,       0xFFEEEEEE);
THEME_CLR(Theme, clrReplayProgressBarFg,    0xFFEEEEEE);
THEME_CLR(Theme, clrReplayProgressBg,       0xBB101010);
THEME_CLR(Theme, clrReplayMarkFg,           0xFFEEEEEE);
THEME_CLR(Theme, clrReplayMarkCurrentFg,    0xFF3090B0);

// Tracks
THEME_CLR(Theme, clrTrackItemBg,            0xBB909090);
THEME_CLR(Theme, clrTrackItemFont,          0xFFEEEEEE);
THEME_CLR(Theme, clrTrackItemCurrentBg,     0xBB3090B0);
THEME_CLR(Theme, clrTrackItemCurrentFont,   0xFFEEEEEE);

// Volume
THEME_CLR(Theme, clrVolumeBg,               0xBB101010);
THEME_CLR(Theme, clrVolumeFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrVolumeProgressFg,       0xFFEEEEEE);
THEME_CLR(Theme, clrVolumeProgressBarFg,    0xFF63EA42);
THEME_CLR(Theme, clrVolumeProgressBg,       0xBB101010);

class cMetrixHD : public cSkin {
    private:
        cMetrixHDDisplayMenu *displayMenu;
    public:
        cMetrixHD(void);
        virtual const char *Description(void);
        virtual cSkinDisplayChannel *DisplayChannel(bool WithInfo);
        virtual cSkinDisplayMenu *DisplayMenu(void);
        virtual cSkinDisplayReplay *DisplayReplay(bool ModeOnly);
        virtual cSkinDisplayVolume *DisplayVolume(void);
        virtual cSkinDisplayTracks *DisplayTracks(const char *Title, int NumTracks, const char * const *Tracks);
        virtual cSkinDisplayMessage *DisplayMessage(void);
};
