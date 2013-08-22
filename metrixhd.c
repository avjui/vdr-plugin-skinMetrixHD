#include <vdr/osd.h>
#include <vdr/menu.h>

#include "metrixhd.h"

#include "displaychannel.h"
#include "displaymenu.h"
#include "displaymessage.h"
#include "displayreplay.h"
#include "displaytracks.h"
#include "displayvolume.h"

cTheme Theme;
static bool menuActive = false;

cMetrixHD::cMetrixHD(void) : cSkin("metrixhd", &::Theme) {
    displayMenu = NULL;
}

const char *cMetrixHD::Description(void) {
    return "MetrixHD";
}

cSkinDisplayChannel *cMetrixHD::DisplayChannel(bool WithInfo) {
    return new cMetrixHDDisplayChannel(WithInfo);
}

cSkinDisplayMenu *cMetrixHD::DisplayMenu(void) {
    cMetrixHDDisplayMenu *menu = new cMetrixHDDisplayMenu;
    displayMenu = menu;
    menuActive = true;
    return menu;
}

cSkinDisplayReplay *cMetrixHD::DisplayReplay(bool ModeOnly) {
    return new cMetrixHDDisplayReplay(ModeOnly);
}

cSkinDisplayVolume *cMetrixHD::DisplayVolume(void) {
    return new cMetrixHDDisplayVolume;
}

cSkinDisplayTracks *cMetrixHD::DisplayTracks(const char *Title, int NumTracks, const char * const *Tracks) {
    return new cMetrixHDDisplayTracks(Title, NumTracks, Tracks);
}

cSkinDisplayMessage *cMetrixHD::DisplayMessage(void) {
    return new cMetrixHDDisplayMessage;
}
