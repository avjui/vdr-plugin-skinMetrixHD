#pragma once

#include "baserender.h"

class cMetrixHDDisplayTracks : public cMetrixHDBaseRender, public cSkinDisplayTracks {
    private:
        cPixmap *tracksPixmap;

        int itemHeight;
        int maxItemWidth;
        int currentIndex;

        void SetItem(const char *Text, int Index, bool Current);
    public:
        cMetrixHDDisplayTracks(const char *Title, int NumTracks, const char * const *Tracks);
        virtual ~cMetrixHDDisplayTracks();
        virtual void SetTrack(int Index, const char * const *Tracks);
        virtual void SetAudioChannel(int AudioChannel);
        virtual void Flush(void);
};
