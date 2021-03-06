#include "displayreplay.h"

cMetrixHDDisplayReplay::cMetrixHDDisplayReplay(bool ModeOnly) {
    labelHeight = fontHeight;
    current = "";
    total = "";

    CreateFullOsd();
    TopBarCreate();
    MessageCreate();

    labelPixmap = osd->CreatePixmap(1, cRect(0, osdHeight - labelHeight, osdWidth, labelHeight));

    ProgressBarCreate(0, osdHeight - labelHeight - ProgressBarHeight() - marginItem, osdWidth,
        Theme.Color(clrReplayProgressFg), Theme.Color(clrReplayProgressBarFg), Theme.Color(clrReplayProgressBg));

    labelPixmap->Fill(Theme.Color(clrReplayBg));
}

cMetrixHDDisplayReplay::~cMetrixHDDisplayReplay() {
    osd->DestroyPixmap(labelPixmap);
}

void cMetrixHDDisplayReplay::SetRecording(const cRecording *Recording) {
    const cRecordingInfo *recInfo = Recording->Info();
    SetTitle( recInfo->Title() );
    cString info = "";
    if (recInfo->ShortText())
        info = cString::sprintf("%s - %s %s", recInfo->ShortText(), *ShortDateString(Recording->Start()), *TimeString(Recording->Start()));
    else
        info = cString::sprintf("%s %s", *ShortDateString(Recording->Start()), *TimeString(Recording->Start()));

    labelPixmap->DrawText(cPoint(0, 0), info, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font, osdWidth);
}

void cMetrixHDDisplayReplay::SetTitle(const char *Title) {
    TopBarSetTitle(Title);
}

void cMetrixHDDisplayReplay::SetMode(bool Play, bool Forward, int Speed) {
    if( Setup.ShowReplayMode ) {
        int left = osdWidth - (labelHeight * 4 + marginItem * 3);
        left /= 2;

        labelPixmap->DrawRectangle(cRect( left - font->Width("33") - marginItem, 0, labelHeight*4 + marginItem*6 + font->Width("33")*2, labelHeight), Theme.Color(clrReplayBg) );
        labelPixmap->DrawRectangle(cRect( left - marginItem, 0, labelHeight*4 + marginItem*4, labelHeight), clrTransparent );

        cString rewind, pause, play, forward;
        cString speed;
        
        if( Speed == -1 ) {
            if( Play ) {
                rewind = "rewind";
                pause = "pause";
                play = "play_sel";
                forward = "forward";
            } else {
                rewind = "rewind";
                pause = "pause_sel";
                play = "play";
                forward = "forward";
            }                
        } else {
            speed = cString::sprintf("%d", Speed);
            if( Forward ) {
                rewind = "rewind";
                pause = "pause";
                play = "play";
                forward = "forward_sel";
                labelPixmap->DrawText(cPoint(left + labelHeight*4 + marginItem*4, 0), speed, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font);
            } else {
                rewind = "rewind_sel";
                pause = "pause";
                play = "play";
                forward = "forward";
                labelPixmap->DrawText(cPoint(left - font->Width(speed) - marginItem, 0), speed, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font);
            }                
        }

        if( imgLoader.LoadIcon(*rewind, labelHeight, labelHeight) )
            labelPixmap->DrawImage( cPoint(left, 0), imgLoader.GetImage() );
        if( imgLoader.LoadIcon(*pause, labelHeight, labelHeight) )
            labelPixmap->DrawImage( cPoint(left + labelHeight + marginItem, 0), imgLoader.GetImage() );
        if( imgLoader.LoadIcon(*play, labelHeight, labelHeight) )
            labelPixmap->DrawImage( cPoint(left + labelHeight*2 + marginItem*2, 0), imgLoader.GetImage() );
        if( imgLoader.LoadIcon(*forward, labelHeight, labelHeight) )
            labelPixmap->DrawImage( cPoint(left + labelHeight*3 + marginItem*3, 0), imgLoader.GetImage() );

    }
}

void cMetrixHDDisplayReplay::SetProgress(int Current, int Total) {
    ProgressBarDraw(Current, Total);
    if( Total > 0 )
        ProgressBarDrawMarks(marks, Theme.Color(clrReplayMarkFg), Theme.Color(clrReplayMarkCurrentFg));
}

void cMetrixHDDisplayReplay::SetCurrent(const char *Current) {
    current = Current;
    UpdateInfo();
}

void cMetrixHDDisplayReplay::SetTotal(const char *Total) {
    total = Total;
    UpdateInfo();
}

void cMetrixHDDisplayReplay::UpdateInfo(void) {
    cString Info = cString::sprintf("%s - %s", *current, *total);
    int right = osdWidth - font->Width(Info);
    labelPixmap->DrawText(cPoint(right, 0), Info, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font, font->Width(Info), fontHeight);
}

void cMetrixHDDisplayReplay::SetJump(const char *Jump) {
    int left = osdWidth - font->Width(Jump);
    left /= 2;
    
    labelPixmap->DrawText(cPoint(left, 0), Jump, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font, font->Width(Jump), fontHeight, taCenter);
}

void cMetrixHDDisplayReplay::SetMessage(eMessageType Type, const char *Text) {
    if (Text)
        MessageSet(Type, Text);
    else
        MessageClear();
}

void cMetrixHDDisplayReplay::Flush(void) {
    TopBarUpdate();
    osd->Flush();
}
