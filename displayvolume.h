#pragma once

#include "baserender.h"

class cMetrixHDDisplayVolume : public cMetrixHDBaseRender, public cSkinDisplayVolume {
    private:
        bool muted;

        cPixmap *labelPixmap;
        cPixmap *volumePixmap;
        cPixmap *volumeIconPixmap;
        int labelHeight;
        tColor ColorFg;
    public:
        cMetrixHDDisplayVolume(void);
        virtual ~cMetrixHDDisplayVolume();
        virtual void SetVolume(int Current, int Total, bool Mute);
        virtual void Flush(void);
};
