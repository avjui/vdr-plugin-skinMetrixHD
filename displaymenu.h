#pragma once

#include <string>
#include <vector>

#include "baserender.h"
#include "services/tvscraper.h"


class cMetrixHDDisplayMenu : public cMetrixHDBaseRender,  public cSkinDisplayMenu {
    private:

        cPixmap *menuPixmap;
        cPixmap *iconmenuPixmap;
        cPixmap *logomenuPixmap;
        cPixmap *scrollbarPixmap;
        cPixmap *siteBarPixmap;
        cPixmap *smallsiteBarPixmap;
        tColor ColorFg;
        tColor ColorBg;
        int contentTop;
        int scrollBarTop, scrollBarWidth, scrollBarHeight;
        int menuWidth;
        int menuWidthFg;
        bool clear;
        int itemHeight;
        int marginleft;
        int margintop;
        int imageheight;
        int sitebarwidth;
        void DrawScrollbar(int Total, int Offset, int Shown, int Top, int Height, bool CanScrollUp, bool CanScrollDown);
        int ItemsHeight(void);
        cString menuNumber;
        cString menuEntry;
        static std::string items[16];
        cString IconName;
        void GetMainIconName(const char *Text, bool sel, bool icon);
        void CreateSmallSiteBar(void);
        void CreateSiteBar(void);
        void ClearSiteBar(void);
        virtual cString DrawPoster(const cRecording *Recording);
        TVScraperGetFullInformation mediainfo;
    public:
        cMetrixHDDisplayMenu();
        virtual ~cMetrixHDDisplayMenu();
        virtual void Scroll(bool Up, bool Page);
        virtual int MaxItems(void);
        virtual void Clear(void);
        virtual void SetTitle(const char *Title);
        virtual void SetButtons(const char *Red, const char *Green = NULL, const char *Yellow = NULL, const char *Blue = NULL);
        virtual void SetMessage(eMessageType Type, const char *Text);
        void SetItem(const char *Text, int Index, bool Current, bool Selectable);
        virtual void SetScrollbar(int Total, int Offset);
        virtual bool SetItemEvent(const cEvent *Event, int Index, bool Current, bool Selectable, const cChannel *Channel, bool WithDate, eTimerMatch TimerMatch);
        virtual void SetEvent(const cEvent *Event);
        virtual bool SetItemChannel(const cChannel *Channel, int Index, bool Current, bool Selectable, bool WithProvider);
        virtual void SetRecording(const cRecording *Recording);
        virtual bool SetItemRecording(const cRecording *Recording, int Index, bool Current, bool Selectable, int Level, int Total, int New);
        virtual void SetText(const char *Text, bool FixedFont);
        virtual int GetTextAreaWidth(void) const;
        virtual const cFont *GetTextAreaFont(bool FixedFont) const;
        virtual void Flush(void);
};
