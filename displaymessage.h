#pragma once

#include "baserender.h"

class cMetrixHDDisplayMessage : public cMetrixHDBaseRender, public cSkinDisplayMessage {
    private:
    public:
        cMetrixHDDisplayMessage(void);
        virtual ~cMetrixHDDisplayMessage();
        virtual void SetMessage(eMessageType Type, const char *Text);
        virtual void Flush(void);
};
