#include "displaymessage.h"

cMetrixHDDisplayMessage::cMetrixHDDisplayMessage(void) {
    CreateFullOsd();
    TopBarCreate();
    MessageCreate();
}

cMetrixHDDisplayMessage::~cMetrixHDDisplayMessage() {
}

void cMetrixHDDisplayMessage::SetMessage(eMessageType Type, const char *Text) {
    if (Text)
        MessageSet(Type, Text);
    else
        MessageClear();
}

void cMetrixHDDisplayMessage::Flush(void) {
    TopBarUpdate();
    osd->Flush();
}
