#include <time.h>

#include "baserender.h"
#include "metrixhd.h"

cMetrixHDBaseRender::cMetrixHDBaseRender(void) {
    font = cFont::CreateFont(Setup.FontOsd, Setup.FontOsdSize );
    fontSml = cFont::CreateFont(Setup.FontSml, Setup.FontSmlSize);
    fontFixed = cFont::CreateFont(Setup.FontFix, Setup.FontFixSize);
    fontBig = cFont::CreateFont(Setup.FontOsd, Setup.FontOsdSize *2 );

    fontHeight = font->Height();
    fontSmlHeight = fontSml->Height();
    fontBigHeight = fontBig->Height();

    fontdoubleHeight = fontHeight * 2;
    
    topBarTitle = "";
    topBarLastDate = "";
    topBarUpdateTitle = false;
    topBarHeight = 0;

    marginOsd = osdHeight * 0.1;
    marginItem = 10;

    progressBarHeight = 20;
    
    buttonsHeight = 0;
    
    BarPixmap = NULL;
    lefttopBarPixmap = NULL;
    buttonsPixmap = NULL;
    messagePixmap = NULL;
    contentPixmap = NULL;
    progressBarPixmap = NULL;

    time_t ts = time(NULL);
    curDate = ShortDateString(ts);
    curDay = WeekDayNameFull(ts);
    curTime = TimeString(ts);

    //channelname = Channel->Name();

    datewidth = font->Width(*curDate);
    timewidth = (font->Width(*curTime) * 2) + 10;
}

cMetrixHDBaseRender::~cMetrixHDBaseRender(void) {
    delete font;
    delete fontSml;
    delete fontFixed;

    if( osd )
    {
        if( BarPixmap )
            osd->DestroyPixmap(BarPixmap);
        if( lefttopBarPixmap )
            osd->DestroyPixmap(lefttopBarPixmap);
        if( buttonsPixmap )
            osd->DestroyPixmap(buttonsPixmap);
        if( messagePixmap )
            osd->DestroyPixmap(messagePixmap);
        if( contentPixmap )
            osd->DestroyPixmap(contentPixmap);
        if( progressBarPixmap )
            osd->DestroyPixmap(progressBarPixmap);

        delete osd;
    }
}

void cMetrixHDBaseRender::CreateFullOsd(void) {
    CreateOsd(cOsd::OsdLeft(), cOsd::OsdTop(), cOsd::OsdWidth(), cOsd::OsdHeight());
}

void cMetrixHDBaseRender::CreateOsd(int left, int top, int width, int height) {
    osdWidth = width;
    osdHeight = height;

    osd = cOsdProvider::NewOsd(left, top);
    if (osd) {
        tArea Area = { 0, 0, width, height,  32 };
        if (osd->SetAreas(&Area, 1) == oeOk) {  
            return;
        }
    }
    return;
}

void cMetrixHDBaseRender::TopBarCreate(int topbarheight, int topbarweight) {
    int marginleft = (osdWidth * 0.1) / 2;
    int margintop = (osdHeight * 0.1) / 2;

    if (topbarheight == 0) {
        topbarheight = datewidth + timewidth + 75;
    }
    if (topbarweight == 0) {
        topbarweight = fontdoubleHeight;
    }

    topbarheightx1 = osdWidth - (datewidth + timewidth + 75) - marginleft;
    topbarweighty1 = margintop;

    BarPixmap = osd->CreatePixmap(1, cRect(topbarheightx1, topbarweighty1, topbarheight, topbarweight));
    BarPixmap->Fill(clrTransparent);

    lefttopBarPixmap = osd->CreatePixmap(1, cRect(0, 0, topbarheight, topbarweight));
    lefttopBarPixmap->Fill(clrTransparent);
}

void cMetrixHDBaseRender::TimeBareLeftCreate(bool small) {
    int marginleft = (osdWidth * 0.1) / 2;
    int margintop = (osdHeight * 0.1);

    if( BarPixmap )
          osd->DestroyPixmap(BarPixmap);
    if (small){
         BarPixmap = osd->CreatePixmap(1, cRect(marginleft, osdWidth / 100 * 21 + 20 + margintop * 2 , osdWidth / 100 * 21, fontdoubleHeight));
    } else {
         BarPixmap = osd->CreatePixmap(1, cRect(marginleft, osdHeight - fontdoubleHeight - margintop / 2, (osdWidth / 3) - marginleft * 2 - marginItem, fontdoubleHeight));
    }
    //BarPixmap->Fill(clrTransparent);
}


void cMetrixHDBaseRender::TopBarSetTitle(cString title) { 
    topBarTitle = title;
    topBarUpdateTitle = true;
}

// sollte bei jedum "Flush" aufgerufen werden!
void cMetrixHDBaseRender::TopBarUpdate(void) {

    if ( strcmp(curDate, topBarLastDate) || topBarUpdateTitle ) {
        topBarUpdateTitle = false;
        topBarLastDate = curDate;

        BarPixmap->Fill(Theme.Color(clrTopBarBg));
        BarPixmap->DrawText(cPoint(25 + timewidth, fontHeight), curDate, Theme.Color(clrTopBarDateTimeFont), clrTransparent, font, 0, 0, taCenter);
        BarPixmap->DrawText(cPoint(25 + timewidth, 0), curDay, Theme.Color(clrTopBarDateTimeFont), clrTransparent, font, 0, 0, taCenter);
        BarPixmap->DrawText(cPoint(25, 0), curTime, Theme.Color(clrTopGreen), clrTransparent, fontBig);

    }
}

void cMetrixHDBaseRender::ButtonsCreate(void) {
    buttonsHeight = fontHeight + 10;
    buttonsWidth = osdWidth;

    buttonsPixmap = osd->CreatePixmap(1, cRect(0, osdHeight - buttonsHeight, buttonsWidth, buttonsHeight));
    buttonsPixmap->Fill(clrTransparent);
}

void cMetrixHDBaseRender::ButtonsSet(const char *Red, const char *Green, const char *Yellow, const char *Blue) {
    int buttonWidth = (osdWidth / 3 * 2 - 20 - (osdWidth * 0.1 / 2)) / 4 - marginItem;

    buttonsPixmap->Fill(clrTransparent);

    int x = osdWidth / 3;
    buttonsPixmap->DrawText(cPoint(x, 0), Red, Theme.Color(clrButtonFont), Theme.Color(clrButtonBg), font, buttonWidth, buttonsHeight, taCenter);
    buttonsPixmap->DrawRectangle(cRect(x, 0, 15, buttonsHeight), Theme.Color(clrButtonRed));

    x += buttonWidth + marginItem;
    buttonsPixmap->DrawText(cPoint(x, 0), Green, Theme.Color(clrButtonFont), Theme.Color(clrButtonBg), font, buttonWidth, buttonsHeight, taCenter);
    buttonsPixmap->DrawRectangle(cRect(x, 0, 15, buttonsHeight), Theme.Color(clrButtonGreen));

    x += buttonWidth + marginItem;
    buttonsPixmap->DrawText(cPoint(x, 0), Yellow, Theme.Color(clrButtonFont), Theme.Color(clrButtonBg), font, buttonWidth, buttonsHeight, taCenter);
    buttonsPixmap->DrawRectangle(cRect(x, 0, 15, buttonsHeight), Theme.Color(clrButtonYellow));

    x += buttonWidth + marginItem;
    buttonsPixmap->DrawText(cPoint(x, 0), Blue, Theme.Color(clrButtonFont), Theme.Color(clrButtonBg), font, buttonWidth, buttonsHeight, taCenter);
    buttonsPixmap->DrawRectangle(cRect(x, 0, 15, buttonsHeight), Theme.Color(clrButtonBlue));
}

void cMetrixHDBaseRender::MessageCreate(void) {
    messageHeight = fontHeight;
    messagePixmap = osd->CreatePixmap(2, cRect(0, osdHeight - messageHeight - 50, osdWidth, messageHeight));
    messagePixmap->Fill(clrTransparent);
}

void cMetrixHDBaseRender::MessageSet(eMessageType Type, const char *Text) {
    tColor col = Theme.Color(clrMessageStatus);
    switch (Type) {
        case mtStatus: 
            col = Theme.Color(clrMessageStatus);
            break;
        case mtInfo:
            col = Theme.Color(clrMessageInfo);
            break;
        case mtWarning:
            col = Theme.Color(clrMessageWarning);
            break;
        case mtError:
            col = Theme.Color(clrMessageError);
            break;
    }
    messagePixmap->Fill(Theme.Color(clrMessageBg));

    messagePixmap->DrawRectangle(cRect( 0, 0, 50, messageHeight), col);
    messagePixmap->DrawRectangle(cRect( osdWidth - 50, 0, 50, messageHeight), col);

    int textWidth = font->Width(Text);
    messagePixmap->DrawText(cPoint((osdWidth - textWidth) / 2, 0), Text, Theme.Color(clrMessageFont), Theme.Color(clrMessageBg), font);
}

void cMetrixHDBaseRender::MessageClear(void) {
    messagePixmap->Fill(clrTransparent);
}

void cMetrixHDBaseRender::ContentCreate(int Left, int Top, int Width, int Height) {
    contentHasScrollbar = false;
    contentShown = false;

    contentLeft = Left;
    contentTop = Top;
    contentWidth = Width;
    contentHeight = Height;
    int lines = ContentVisibleLines();
    contentHeight = lines * fontHeight;
}

void cMetrixHDBaseRender::ContentSet(const char *Text, tColor ColorFg, tColor ColorBg) {
    contentWrapper.Set(Text, font, contentWidth);
    contentColorFg = ColorFg;
    contentColorBg = ColorBg;

    int contentWrapperHeight = (contentWrapper.Lines()+1) * fontHeight;

    if( contentWrapperHeight > contentHeight ) {
        contentDrawPortHeight = contentWrapperHeight;
        contentHasScrollbar = true;
    } else {
        contentDrawPortHeight = contentHeight;
        contentHasScrollbar = false;
    }

    if (contentPixmap)
        osd->DestroyPixmap(contentPixmap);
    contentPixmap = osd->CreatePixmap(2, cRect(contentLeft, contentTop, contentWidth, contentHeight),
            cRect(0, 0, contentWidth, contentDrawPortHeight));

    contentPixmap->Fill(contentColorBg);

    contentDraw();
    contentShown = true;
}

bool cMetrixHDBaseRender::ContentScrollable(void) {
    return contentHasScrollbar;
}

double cMetrixHDBaseRender::ScrollbarSize(void) {
    return (double)contentHeight / (double)contentDrawPortHeight;
}

int cMetrixHDBaseRender::ContentScrollTotal(void) {
    return contentWrapper.Lines();
}

int cMetrixHDBaseRender::ContentGetHeight(void) {
    return contentHeight;
}

int cMetrixHDBaseRender::ContentScrollOffset(void) {
    double offset;
    if ( ((-1)*contentPixmap->DrawPort().Point().Y() + contentHeight + fontHeight) > contentDrawPortHeight)
        offset = (double)1 - ScrollbarSize();
    else
        offset = (double)((-1)*contentPixmap->DrawPort().Point().Y())/(double)((-1)*contentPixmap->DrawPort().Point().Y() + contentHeight);
    return ContentScrollTotal() * offset;
}

int cMetrixHDBaseRender::ContentVisibleLines(void) {
    return (contentHeight / fontHeight);
}

bool cMetrixHDBaseRender::ContentScroll(bool Up, bool Page) {
    int aktHeight = contentPixmap->DrawPort().Point().Y();
    int totalHeight = contentPixmap->DrawPort().Height();
    int screenHeight = contentPixmap->ViewPort().Height();
    int lineHeight = fontHeight;
    bool scrolled = false;
    if (Up) {
        if (Page) {
            int newY = aktHeight + screenHeight;
            if (newY > 0)
                newY = 0;
            contentPixmap->SetDrawPortPoint(cPoint(0, newY));
            scrolled = true;
        } else {
            if (aktHeight < 0) {
                contentPixmap->SetDrawPortPoint(cPoint(0, aktHeight + lineHeight));
                scrolled = true;
            }
        }
    } else {
        if (Page) {
            int newY = aktHeight - screenHeight;
            if ((-1)*newY > totalHeight - screenHeight)
                newY = (-1)*(totalHeight - screenHeight);
            contentPixmap->SetDrawPortPoint(cPoint(0, newY));
            scrolled = true;
        } else {
            if (totalHeight - ((-1)*aktHeight + lineHeight) > screenHeight) {
                contentPixmap->SetDrawPortPoint(cPoint(0, aktHeight - lineHeight));
                scrolled = true;
            }
        }
    }
    return scrolled;
}

bool cMetrixHDBaseRender::ContentIsShown(void) {
    return contentShown;
}

void cMetrixHDBaseRender::ContentClear(void) {
    if( contentPixmap )
        contentPixmap->Fill(clrTransparent);
    contentShown = false;
}

void cMetrixHDBaseRender::contentDraw(void) {
    int linesText = contentWrapper.Lines();
    int currentHeight = 0;
    for (int i=0; i < linesText; i++) {
        //printf("drawLine: %d %s\n", i, contentWrapper.GetLine(i));
        currentHeight = (i)*fontHeight;
        contentPixmap->DrawText(cPoint(0, currentHeight), contentWrapper.GetLine(i), contentColorFg, contentColorBg, font, contentWidth);
    }
}

void cMetrixHDBaseRender::ProgressBarCreate(int Left, int Top, int Width, tColor ColorFg, tColor ColorBarFg, tColor ColorBg) {
    progressBarTop = Top;
    progressBarWidth = Width;

    progressBarColorFg = ColorFg;
    progressBarColorBarFg = ColorBarFg;
    progressBarColorBg = ColorBg;
    
    progressBarPixmap = osd->CreatePixmap(2, cRect(Left, Top, Width, progressBarHeight));
    progressBarPixmap->Fill(clrTransparent);
}

void cMetrixHDBaseRender::ProgressBarDraw(int Current, int Total) {
    progressBarCurrent = Current;
    progressBarTotal = Total;
    int top = progressBarHeight / 2 - 3;
    double percentLeft = ((double)Current) / (double)Total;

    progressBarPixmap->Fill( progressBarColorBg );

    progressBarPixmap->DrawRectangle(cRect( 0, top + 2, progressBarWidth, 2), progressBarColorFg);
    if (Current > 0)
        progressBarPixmap->DrawRectangle(cRect( 0, top, (progressBarWidth * percentLeft), 6), progressBarColorBarFg);
}

int cMetrixHDBaseRender::ProgressBarHeight(void) {
    return progressBarHeight;
}

void cMetrixHDBaseRender::ProgressBarClear(void) {
    progressBarPixmap->Fill(clrTransparent);
}

void cMetrixHDBaseRender::ProgressBarDrawMarks(const cMarks *Marks, tColor Color, tColor ColorCurrent) {
    progressBarColorMark = Color;
    progressBarColorMarkCurrent = ColorCurrent;

    if( Marks ) {
        bool Start = true;
        for( const cMark *m = Marks->First(); m; m = Marks->Next(m) ) {
            int p1 = ProgressBarMarkPos( m->Position() );
            ProgressBarDrawMark(p1, Start, m->Position() == progressBarCurrent);
            Start = !Start;
        }
    }
}

int cMetrixHDBaseRender::ProgressBarMarkPos(int P) {
    return P * progressBarWidth / progressBarTotal;
}

void cMetrixHDBaseRender::ProgressBarDrawMark(int X, bool Start, bool Current)
{
    if( Start )
        if( Current )
            progressBarPixmap->DrawRectangle(cRect( X-5, 0, 10, 3), progressBarColorMarkCurrent);
        else
            progressBarPixmap->DrawRectangle(cRect( X-3, 0, 6, 3), progressBarColorMark);            
    else
        if( Current )
            progressBarPixmap->DrawRectangle(cRect( X-5, progressBarHeight - 3, 10, 3), progressBarColorMarkCurrent);
        else
            progressBarPixmap->DrawRectangle(cRect( X-3, progressBarHeight - 3, 6, 3), progressBarColorMark);
    
    if( Current )
        progressBarPixmap->DrawRectangle(cRect( X-1, 0, 2, progressBarHeight), progressBarColorMarkCurrent);
    else
        progressBarPixmap->DrawRectangle(cRect( X-1, 0, 2, progressBarHeight), progressBarColorMark);
}
