#pragma once

#include "baserender.h"

class cMetrixHDDisplayChannel : public cMetrixHDBaseRender, public cSkinDisplayChannel {
    private:
        const cEvent *present;

        cString channelName;
        unsigned char caName;

        cPixmap *chanInfoTopPixmap;
        cPixmap *chanInfoBottomPixmap;
        cPixmap *chanInfoLogo;
        cPixmap *SignalPixmap;
        int heightBottom;
        int marginleft;
        int margintop;
        int left;
        int lastSignalStrength;
        int SignalStrength;
        int lastSignalQuality;
        int SignalQuality;
    public:
        cMetrixHDDisplayChannel(bool WithInfo);
        virtual ~cMetrixHDDisplayChannel();
        virtual void SetChannel(const cChannel *Channel, int Number);
        virtual void SetEvents(const cEvent *Present, const cEvent *Following);
        virtual void DrawCryptSymbol(int xSymbol, int ySymbol, const char *Text, tColor color, tColor bgcolor);
        virtual void DrawQuality(void);
        virtual void DrawSignal(void);
        virtual void SetMessage(eMessageType Type, const char *Text);
        virtual void Flush(void);
};
