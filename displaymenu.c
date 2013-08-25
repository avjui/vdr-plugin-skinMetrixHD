#include "displaymenu.h"
#include "services/tvscraper.h"

cMetrixHDDisplayMenu::cMetrixHDDisplayMenu() {
    cString IconName = "";

    CreateFullOsd();
    ButtonsCreate();
    MessageCreate();
    itemHeight = fontHeight + 2;

    marginleft = (osdWidth * 0.1) / 2;
    margintop = (osdHeight * 0.1) / 2;
    imageheight = osdWidth / 100 * 21;
    sitebarwidth = osdWidth / 3 - 10 - marginleft * 2;
 
    siteBarPixmap = NULL;
    scrollBarWidth = 20;
    scrollBarHeight = osdHeight - (topBarHeight + buttonsHeight + marginItem*2 + margintop);
    scrollBarTop = topBarHeight + marginItem;

    menuWidth = (osdWidth / 3) * 2 - scrollBarWidth - 20;
    menuWidthFg = (osdWidth / 3) * 2 - scrollBarWidth - 20;

    menuPixmap = osd->CreatePixmap(1, cRect(osdWidth / 3 + 20 - marginleft, margintop , menuWidth, scrollBarHeight ));
    clear = false;
    iconmenuPixmap = osd->CreatePixmap(2, cRect(marginleft, margintop, imageheight , imageheight));
    logomenuPixmap = osd->CreatePixmap(2, cRect(marginleft + sitebarwidth / 2 - 100, margintop, 200, 200));

    contentTop = topBarHeight + marginItem + fontHeight + (fontSmlHeight*2) + marginItem;

    ProgressBarCreate(marginleft + (sitebarwidth * 0.2) , 200 , sitebarwidth * 0.6,
                      Theme.Color(clrChannelProgressFg), Theme.Color(clrTopGreen), Theme.Color(clrChannelProgressBg));

    scrollbarPixmap = osd->CreatePixmap(2, cRect(osdWidth - scrollBarWidth -marginleft, scrollBarTop, scrollBarWidth, scrollBarHeight));

    //siteBarPixmap->Fill(Theme.Color(clrTransparent));
    //smallsiteBarPixmap->Fill(Theme.Color(clrTransparent));
    iconmenuPixmap->Fill(clrTransparent);
    logomenuPixmap->Fill(clrTransparent);
    menuPixmap->Fill(Theme.Color(clrTopBarBg));
    scrollbarPixmap->Fill(clrTransparent);
  
}

cMetrixHDDisplayMenu::~cMetrixHDDisplayMenu() {
    if (siteBarPixmap)
        osd->DestroyPixmap(siteBarPixmap);
    osd->DestroyPixmap(iconmenuPixmap);
    osd->DestroyPixmap(logomenuPixmap);
    osd->DestroyPixmap(menuPixmap);
    osd->DestroyPixmap(scrollbarPixmap);
}

void cMetrixHDDisplayMenu::DrawScrollbar(int Total, int Offset, int Shown, int Top, int Height, bool CanScrollUp, bool CanScrollDown) {
    if (Total > 0 && Total > Shown) {
        int scrollHeight = max(int((Height) * double(Shown) / Total + 0.5), 5);
        int scrollTop = min(int(Top + (Height) * double(Offset) / Total + 0.5), Top + Height - scrollHeight);

        scrollbarPixmap->Fill(clrTransparent);
        scrollbarPixmap->DrawRectangle(cRect(5, Top, scrollBarWidth-5, Height), Theme.Color(clrScrollbarBg));

        scrollbarPixmap->DrawRectangle(cRect(15, Top, 2, Height), Theme.Color(clrScrollbarFg));
        scrollbarPixmap->DrawRectangle(cRect(15, scrollTop, 5, scrollHeight), Theme.Color(clrScrollbarBarFg));
    }
}

void cMetrixHDDisplayMenu::SetScrollbar(int Total, int Offset) {
    DrawScrollbar(Total, Offset, MaxItems(), margintop, ItemsHeight(), Offset > 0, Offset + MaxItems() < Total);
}

void cMetrixHDDisplayMenu::Scroll(bool Up, bool Page) {
    // Wird das Menü gescrollt oder Content?
    if( ContentIsShown() )
    {
        bool scrolled = ContentScroll(Up, Page);
        if( scrolled )
        {
          DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());
        }
    } else {
        cSkinDisplayMenu::Scroll(Up, Page);
    }
}

int cMetrixHDDisplayMenu::MaxItems(void) {
    return scrollBarHeight / itemHeight;
}

int cMetrixHDDisplayMenu::ItemsHeight(void) {
    return MaxItems() * itemHeight;
}
void cMetrixHDDisplayMenu::Clear(void) {
    textScroller.Reset();
    iconmenuPixmap->Fill(clrTransparent);
    //siteBarPixmap->Fill(Theme.Color(clrTopBarBg));
    menuPixmap->Fill(clrTransparent);
    scrollbarPixmap->Fill(clrTransparent);
    ContentClear();
}

void cMetrixHDDisplayMenu::SetTitle(const char *Title) {
    TopBarSetTitle(Title);
}

void cMetrixHDDisplayMenu::SetButtons(const char *Red, const char *Green, const char *Yellow, const char *Blue) {
    ButtonsSet(Red, Green, Yellow, Blue);
}

void cMetrixHDDisplayMenu::SetMessage(eMessageType Type, const char *Text) {
    if (Text)
        MessageSet(Type, Text);
    else
        MessageClear();
}

void cMetrixHDDisplayMenu::SetItem(const char *Text, int Index, bool Current, bool Selectable) {
    int y = Index * itemHeight;
    bool icon = true;
    //to clear the right Sight
    logomenuPixmap->Fill(clrTransparent);
    ProgressBarClear();

    ClearSiteBar();
    CreateSmallSiteBar();
    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrTopGreen);
        GetMainIconName(Text, Selectable, icon);
    }
    else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrTransparent);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrTransparent);
        }
    }

    for (int i = 0; i < MaxTabs; i++)
    {
        const char *s = GetTabbedText(Text, i);
        if (s) {
            int xt = Tab(i);
            menuPixmap->DrawText(cPoint(xt, y), s, ColorFg, ColorBg, font, menuWidth);
        }
        if (!Tab(i + 1))
            break;
    }
}

bool cMetrixHDDisplayMenu::SetItemEvent(const cEvent *Event, int Index, bool Current, bool Selectable, const cChannel *Channel, bool WithDate, eTimerMatch TimerMatch) {
    int current = 0;
    int total = 0;
    int menuEventWidth = menuWidth;

    if(!Event)
       return false;

    // Inital some things    
    ClearSiteBar();
    CreateSiteBar();
    ContentCreate(marginleft / 2, margintop + fontSmlHeight * 2 + 200, marginleft + sitebarwidth, osdHeight - fontHeight * 2 - fontSmlHeight *2 - 210 - margintop * 2);

    cString title = Event->Title();
    cString description = Event->ShortText();

    // Datum kürzen
    cString date = Event->GetDateString();
    std::string dateshort = *date;
    date = dateshort.substr(0,6).c_str();

    cString startTime = Event->GetTimeString();
    cString endTime = Event->GetEndTimeString();

    cString event = cString::sprintf("%s %s - %s", *date, *startTime, *title);
    cString timeinfo = cString::sprintf("%s - %s",*startTime, *endTime);
    int timewidth = font->Width(*timeinfo);

    //to clear the menu
    //menuPixmap->Fill(clrTransparent);
    if(clear)
        menuPixmap->DrawRectangle(cRect(0, 0, marginleft / 2, scrollBarHeight), clrTransparent);
    siteBarPixmap->Fill(Theme.Color(clrTopBarBg));

    if(Channel) {
        channelname = Channel->Name();
        if( imgLoader.LoadLogo(channelname, 200, 200) ) {
           logomenuPixmap->DrawImage( cPoint(0, 0), imgLoader.GetImage() );
        }
      }
    int y = Index * itemHeight;
    int x = marginleft / 2;
    if (Current) {
        // Variables
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrTopGreen);
        //x = 0;
        clear = true;
        //menuEventWidth = menuWidthFg;
        time_t s = time(NULL);
        current = s - Event->StartTime();
        total = Event->Duration();
 
       //Description left
        siteBarPixmap->DrawText(cPoint((sitebarwidth - timewidth) / 2, 225), timeinfo, ColorFg, clrTransparent, font, menuWidth);
        ProgressBarDraw(current, total);
        //if(MenuCategory() == mcScheduleNow || MenuCategory() == mcScheduleNext) {
        cString content = Event->Description();
        if(*content == NULL)
            content = Event->ShortText();
         ContentSet(*content, ColorFg, ColorBg);
    } 
    else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrTransparent);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrTransparent);
        }
    }
    
    for (int i = 0; i < MaxTabs; i++)
    {             
            menuPixmap->DrawText(cPoint(x, y), event, ColorFg, ColorBg, font, menuEventWidth);
            
        if (!Tab(i + 1))
            break;
    }
   return true;
}


void cMetrixHDDisplayMenu::SetEvent(const cEvent *Event) {
    int current = 0;
    int Total = 0;

    if( !Event )
        return;

    ClearSiteBar();
    CreateSiteBar();
    menuPixmap->Fill(clrTransparent);
    cString date = Event->GetDateString();
    cString startTime = Event->GetTimeString();
    cString endTime = Event->GetEndTimeString();

    cString timeString = cString::sprintf("%s %s - %s", *date, *startTime, *endTime);

    cString title = Event->Title();
    cString shortText = Event->ShortText();

    menuPixmap->DrawText(cPoint(0, 0), timeString, Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg), fontSml, menuWidth);
    menuPixmap->DrawText(cPoint(0, fontSmlHeight), title, Theme.Color(clrMenuEventFontTitle), Theme.Color(clrMenuEventBg), font, menuWidth);
    menuPixmap->DrawText(cPoint(0, fontSmlHeight + fontHeight), shortText, Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg), fontSml, menuWidth);
    
    ContentCreate((osdWidth / 3) + 20, contentTop, menuWidth, scrollBarHeight - fontHeight*2 - 20);
    ContentSet( Event->Description(), Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg) );

    time_t s = time(NULL);
    if(Event->StartTime() < s) {
        current = s - Event->StartTime();
        Total = Event->Duration();
        ProgressBarDraw(current, Total);
       }
    

    if( ContentScrollable() )
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());
}

bool cMetrixHDDisplayMenu::SetItemChannel(const cChannel *Channel, int Index, bool Current, bool Selectable, bool WithProvider) {
    int y = Index * itemHeight;
    

    ClearSiteBar();
    CreateSiteBar();
    cString channel = cString::sprintf(" %d   %s",Channel->Number(), Channel->Name());
    if(Channel->GroupSep())
        channel = cString::sprintf("--------- %s ---------", Channel->Name());

    if(Channel && Current) {
        if( imgLoader.LoadLogo(Channel->Name(), 400, 400) ) {
           iconmenuPixmap->DrawImage( cPoint(0, 0), imgLoader.GetImage() );
        }
      }

    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrTopGreen); 
    } else {
        if( Selectable ) {
            cString channel = cString::sprintf("%d   %s",Channel->Number(), Channel->Name());
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrTransparent);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrTransparent);
        }
    }

    for (int i = 0; i < MaxTabs; i++)
    {
        menuPixmap->DrawText(cPoint(0, y), channel, ColorFg, ColorBg, font, menuWidth);
        if (!Tab(i + 1))
            break;
    }
   return true;
}


void cMetrixHDDisplayMenu::SetRecording(const cRecording *Recording) {
    if( !Recording )
        return;
    menuPixmap->Fill(clrTransparent);

    ClearSiteBar();
    CreateSiteBar();
    const cRecordingInfo *recInfo = Recording->Info();
    cString timeString = cString::sprintf("%s  %s  %s", *DateString(Recording->Start()), *TimeString(Recording->Start()), recInfo->ChannelName() ? recInfo->ChannelName() : "");

    cString title = recInfo->Title();
    if( isempty(title) )
        title = Recording->Name();
    cString shortText = recInfo->ShortText();

    menuPixmap->DrawText(cPoint(0, 0), timeString, Theme.Color(clrMenuRecFontInfo), Theme.Color(clrMenuRecBg), fontSml, menuWidth);
    menuPixmap->DrawText(cPoint(0, fontSmlHeight), title, Theme.Color(clrMenuRecFontTitle), Theme.Color(clrMenuRecBg), font, menuWidth);
    menuPixmap->DrawText(cPoint(0, fontSmlHeight + fontHeight), shortText, Theme.Color(clrMenuRecFontTitle), Theme.Color(clrMenuRecBg), fontSml, menuWidth);

    ContentSet( recInfo->Description(), Theme.Color(clrMenuRecFontInfo), Theme.Color(clrMenuRecBg) );
    if( ContentScrollable() )
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());
}

bool cMetrixHDDisplayMenu::SetItemRecording(const cRecording *Recording, int Index, bool Current, bool Selectable, int Level, int Total, int New) {

    int y = Index * itemHeight;

    if( !Recording )
        return false;

    ClearSiteBar();
    CreateSiteBar();
    siteBarPixmap->Fill(Theme.Color(clrTransparent));

    const cRecordingInfo *recInfo = Recording->Info();
    cString timeString = cString::sprintf("%s  %s - %s", *DateString(Recording->Start()), *TimeString(Recording->Start()), recInfo->ChannelName() ? recInfo->ChannelName() : "");

    //cString title = recInfo->Title();
    //if( isempty(title) )
    cString title = Recording->Name();
    cString shortText = recInfo->ShortText();
    cString path = Recording->FileName();
    dsyslog("SkinMetrixHD: Path of recording %s", *path);
    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrTopGreen); 
    } else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrTransparent);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrTransparent);
        }
    }

    for (int i = 0; i < MaxTabs; i++)
    {
      if( Total > 0) {
        menuPixmap->DrawText(cPoint(0, y), title, ColorFg, ColorBg, font, menuWidth);
        if( imgLoader.LoadIcon("folder", 400, 400) ) {
           iconmenuPixmap->DrawImage( cPoint(0, 0), imgLoader.GetImage() );
        }
      } else {
           title = cString::sprintf("%s  %s  %s", *DateString(Recording->Start()), *TimeString(Recording->Start()), recInfo->Title()) ;
           menuPixmap->DrawText(cPoint(0, y), title, ColorFg, ColorBg, font, menuWidth);      

      }
      if (!Tab(i + 1))
          break;
    }

      return true;
}

void cMetrixHDDisplayMenu::SetText(const char *Text, bool FixedFont) {
    if( !Text )
        return;
    menuPixmap->Fill(clrTransparent);

    ContentSet( Text, Theme.Color(clrMenuTextFont), Theme.Color(clrMenuTextBg) );
    if( ContentScrollable() )
        SetScrollbar( ContentScrollTotal(), 0 );
}

int cMetrixHDDisplayMenu::GetTextAreaWidth(void) const {
    return menuWidth - (marginItem*2);
}

const cFont *cMetrixHDDisplayMenu::GetTextAreaFont(bool FixedFont) const {
    const cFont *rfont = FixedFont ? fontFixed : font;
    return rfont;
}

std::string cMetrixHDDisplayMenu::items[16] = { "Schedule", "Channels", "Timers", "Recordings", "Setup", "Commands",
                                                "OSD", "EPG", "DVB", "LNB", "CAM", "Recording", "Replay", "Miscellaneous", "Plugins", "Restart"};

void cMetrixHDDisplayMenu::GetMainIconName(const char *Text, bool sel, bool icon) {
    std::string text = skipspace(Text);
    bool found = false;
    bool doBreak = false;
    cString menuIcon = "";
    size_t i = 0;
    for (; i < text.length(); i++) {
        char s = text.at(i);
        if (i==0) {
            //if text directly starts with nonnumeric, break
            if (!(s >= '0' && s <= '9')) {
                break;
            }
        }
        if (found) {
            if (!(s >= '0' && s <= '9')) {
                doBreak = true;
            }           
        }
        if (s >= '0' && s <= '9') {
            found = true;
        }
        if (doBreak)
            break;
        if (i>4)
            break;
    }
    if (found) {
        menuNumber = skipspace(text.substr(0,i).c_str());
        menuEntry = skipspace(text.substr(i).c_str());
    } else {
        menuNumber = "";
        menuEntry = text.c_str();       
    }
    std::string element = *menuEntry;
    //check for standard menu entries
    for (int i=0; i<16; i++) {
        std::string s = trVDR(items[i].c_str());
        if (s == element) {
            cString menuIcon = cString::sprintf("menuIcons/%s", items[i].c_str());
        }
    }
    //check for special main menu entries "stop recording", "stop replay"
    std::string stopRecording = skipspace(trVDR(" Stop recording "));
    std::string stopReplay = skipspace(trVDR(" Stop replaying"));
    try {
        if (element.substr(0, stopRecording.size()) == stopRecording)
            cString menuIcon = cString::sprintf("menuIcons/StopRecording");
        if (element.substr(0, stopReplay.size()) == stopReplay)
            cString menuIcon = cString::sprintf("menuIcons/StopReplay");
    } catch (...) {}
    //check for Plugins
    for (int i = 0; ; i++) {
        cPlugin *p = cPluginManager::GetPlugin(i);
        if (p) {
            const char *mainMenuEntry = p->MainMenuEntry();
            if (mainMenuEntry) {
                std::string plugMainEntry = mainMenuEntry;
                try {
                    if (element.substr(0, plugMainEntry.size()) == plugMainEntry) {
                        cString menuIcon = cString::sprintf("pluginIcons/%s", p->Name());
                    }
                } catch (...) {}
            } 
        } else
            break;
    }
    dsyslog("SkinMetrixHD: Name Found - %s", *menuEntry);
    if(icon) {
        if( imgLoader.LoadIcon(*menuEntry, 400) ) {
            iconmenuPixmap->DrawImage( cPoint(0, 0), imgLoader.GetImage() );
	 }
    }else {
        if( imgLoader.LoadLogo(*menuEntry, 400, 400) ) {
            iconmenuPixmap->DrawImage( cPoint(0, 0), imgLoader.GetImage() );    
         }
    }
}
void cMetrixHDDisplayMenu::CreateSmallSiteBar(void) {  
    bool small = true;
    if (siteBarPixmap)
        osd->DestroyPixmap(siteBarPixmap);
    siteBarPixmap = osd->CreatePixmap(1, cRect(marginleft, margintop , imageheight, imageheight + marginleft));
    siteBarPixmap->Fill(Theme.Color(clrMenuBg));
    TimeBareLeftCreate(small);
}

void cMetrixHDDisplayMenu::CreateSiteBar(void) {   
    bool small = false;
    if (siteBarPixmap)
        osd->DestroyPixmap(siteBarPixmap);
    siteBarPixmap = osd->CreatePixmap(1, cRect(marginleft, margintop , sitebarwidth, osdHeight - fontHeight * 2 - 20 - margintop * 2));
    siteBarPixmap->Fill(Theme.Color(clrMenuBg));
    TimeBareLeftCreate(small);
}

void cMetrixHDDisplayMenu::ClearSiteBar(void) {
    if(siteBarPixmap)
       siteBarPixmap->Fill(clrTransparent);
}

void cMetrixHDDisplayMenu::Flush(void) {
    TopBarUpdate();
    osd->Flush();    
}
