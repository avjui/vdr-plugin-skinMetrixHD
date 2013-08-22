#include "displayvolume.h"

cMetrixHDDisplayVolume::cMetrixHDDisplayVolume(void) {
    muted = false;

    labelHeight = fontHeight * 2;

    CreateFullOsd();
    
    labelPixmap = osd->CreatePixmap(1, cRect(0, 0, osdWidth, fontBigHeight));
    volumePixmap = osd->CreatePixmap(2, cRect((osdWidth / 10) * 2, 0, (osdWidth / 10) * 6, fontHeight));
    volumeIconPixmap = osd->CreatePixmap(2, cRect((osdWidth / 10), 0, fontBigHeight, fontBigHeight));
}

cMetrixHDDisplayVolume::~cMetrixHDDisplayVolume() {
    osd->DestroyPixmap(labelPixmap);
    osd->DestroyPixmap(volumePixmap);
    osd->DestroyPixmap(volumeIconPixmap);
}

void cMetrixHDDisplayVolume::SetVolume(int Current, int Total, bool Mute) {
    labelPixmap->Fill(Theme.Color(clrVolumeBg));
    volumePixmap->Fill(clrTransparent);
    cString label = cString::sprintf("%d", Current);
    cString maxLabel = cString::sprintf("%d", 255);
    cString volumeLabel = cString::sprintf("%s", tr("Volume"));
    int maxlabelWidth = fontBig->Width(maxLabel);
    int lefttext = (osdWidth / 20) * 17 ;

    labelPixmap->DrawText(cPoint(lefttext, 0), *label, Theme.Color(clrVolumeProgressBarFg), Theme.Color(clrVolumeBg),
            fontBig, maxlabelWidth, fontHeight, taCenter);

    if (Mute) {
        if( imgLoader.LoadIcon("mute", labelHeight, labelHeight) ) {
            volumeIconPixmap->DrawImage( cPoint(0, 0), imgLoader.GetImage() );
        }
    } else {
        int maxrec = 18;
        int volumeleft = (osdWidth / 10) * 2;
        int recwidth = (osdWidth / 10) * 6 / 34;
        for (int i=1;i<=maxrec;i++)
        {
            if ((Current) >= i * 15) {
                ColorFg = Theme.Color(clrVolumeFont);
            } else {
                ColorFg = Theme.Color(clrVolumeBg);
            }
            volumePixmap->DrawRectangle(cRect(recwidth * i * 2 - recwidth - 2, fontBigHeight * 0.25 - 2, recwidth + 2, fontBigHeight * 0.5), Theme.Color(clrVolumeProgressFg));            
            volumePixmap->DrawRectangle(cRect(recwidth * i * 2 - recwidth, fontBigHeight * 0.25, recwidth, fontBigHeight * 0.5), ColorFg);            
        }
        if(imgLoader.LoadIcon("volume", labelHeight, labelHeight)) {
            volumeIconPixmap->DrawImage( cPoint(0, 0), imgLoader.GetImage() );
     }
    }
}

void cMetrixHDDisplayVolume::Flush(void) {
    //TopBarUpdate();
    osd->Flush();
}
