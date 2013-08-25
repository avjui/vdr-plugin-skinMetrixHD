#include "displaychannel.h"

#define NUM_CA_IDS 9

cMetrixHDDisplayChannel::cMetrixHDDisplayChannel(bool WithInfo) {
    present = NULL;
    channelName = "";

    CreateFullOsd();
    if ( !osd )
        return;
    TopBarCreate();
    MessageCreate();

    // von unten noch oben
    // 2 * EPG + 2 * EPGsml
    heightBottom = fontBigHeight + fontHeight + (fontSmlHeight * 3) + ProgressBarHeight(); // Top, Buttom, Between
    int left = heightBottom + marginItem;

    int height = heightBottom;
    chanInfoBottomPixmap = osd->CreatePixmap(1, cRect(0, osdHeight - height, osdWidth, heightBottom));

    //Create LogoPixmap
    chanInfoLogo = osd->CreatePixmap(1, cRect(0, osdHeight - height, heightBottom + marginItem * 2, heightBottom));
    chanInfoLogo->Fill(clrTransparent);

    height += ProgressBarHeight();
    ProgressBarCreate(left, osdHeight - height + fontSmlHeight + fontBigHeight , osdWidth - left -50,
        Theme.Color(clrChannelProgressFg), Theme.Color(clrChannelProgressBarFg), Theme.Color(clrChannelProgressBg));

    //height += heightTop;
    chanInfoTopPixmap = osd->CreatePixmap(1, cRect(marginOsd, marginOsd , osdWidth / 2, fontdoubleHeight));

    //Signal Bar
    SignalPixmap = osd->CreatePixmap(1, cRect((osdWidth / 3) / 2 , osdHeight - (heightBottom + fontHeight + 5), (osdWidth / 3) * 2, fontHeight));
    SignalPixmap->Fill(Theme.Color(clrTopBarBg));
    SignalStrength = 0;
    SignalQuality = 0;
}


cMetrixHDDisplayChannel::~cMetrixHDDisplayChannel() {
    if (osd) {
        osd->DestroyPixmap(chanInfoTopPixmap);
        osd->DestroyPixmap(chanInfoBottomPixmap);
        osd->DestroyPixmap(chanInfoLogo);
        osd->DestroyPixmap(SignalPixmap);
    }
}

void cMetrixHDDisplayChannel::SetChannel(const cChannel *Channel, int Number) {

    cString channelNumber = "";
    cString channelFrequency = "";
    cString Polarization = "";
    cString Ebene = "";
    int threshold = 11700;
    int hor = 100000;
    int ver = 200000;

    if (Channel) {
        channelName = Channel->Name();

        // Pol. und Ebene ermitteln
        int Trans = Channel->Transponder();
        int Frequency = Channel->Frequency();
        if((Trans - Frequency) == hor)
              Polarization = "Horizontal";

        if((Trans - Frequency) == ver)
              Polarization = "Vertikal";

        if(Frequency < threshold) {
               Ebene = "Low";
        } else {
               Ebene = "High";
        }

         //dsyslog("SkinMetrixHD:  Trans %d  --- Frequency %d --- Pol %s --- Ebene %s ", Trans, Frequency, *Polarization, *Ebene);


        const cSource *source = Sources.Get(Channel->Source());
        if (source)
             channelFrequency = cString::sprintf("%d Mhz %s / %s - %s", Frequency, *Polarization, *Ebene , *cSource::ToString(source->Code()));        
        else
             channelFrequency = cString::sprintf("%d Mhz %s / %s", Frequency, *Polarization, *Ebene);

              
        if (!Channel->GroupSep())
            channelNumber = cString::sprintf("%d%s", Channel->Number(), Number ? "-" : "");
        else if (Number)
            channelNumber = cString::sprintf("%d-", Number);
    } else
        channelName = ChannelString(NULL, 0);

    int channelStringwidth = fontBig->Width(*channelNumber) + 15;

    chanInfoTopPixmap->Fill(Theme.Color(clrChannelBg));
    chanInfoTopPixmap->DrawText(cPoint(0, 0), channelNumber, Theme.Color(clrChannelFontTitle), Theme.Color(clrChannelBg), fontBig);
    chanInfoTopPixmap->DrawText(cPoint(channelStringwidth, 0), channelName, Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), font);
    chanInfoTopPixmap->DrawText(cPoint(channelStringwidth, fontHeight), channelFrequency, Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);
   
    
    //CryptSymbols from skinPearlHD
    bool caOn[NUM_CA_IDS];
    for (int i=0;i<NUM_CA_IDS;i++)
         caOn[i] = false;
    const char *caShort[NUM_CA_IDS] = {"B", "I", "NDS", "N", "S", "V", "C", "CW", "BC" };
    int xCa = 300;
    int widthCa = 0;

    for (int i=0; Channel->Ca(i) != 0 ; i++)
	  {
            switch (Channel->Ca(i))
		{
		  case 0x2600 : //Biss
		    caOn[0] = true;
		  break;
	      
		  case 0x0600 ... 0x06FF : //Irdeto
		    caOn[1] = true;
		  break;
		  
		  case 0x0900 ... 0x09FF : //NDS
		    caOn[2] = true;
		  break;
		  
		  case 0x1200 ... 0x12FF : //Nagra
		  case 0x1800 ... 0x18FF : //Nagra
		    caOn[3] = true;
		  break;
		  
		  case 0x0100 ... 0x01FF : //Seca
		    caOn[4] = true;
		  break;
		  
		  case 0x0500 ... 0x05FF : //Viaccess
		    caOn[5] = true;
		  break;
		  
		  case 0x0B00 ... 0x0BFF : //Conax
		    caOn[6] = true;
		  break;
		  
		  case 0x0D00 ... 0x0DFF : //Cryptoworks
		    caOn[7] = true;
		  break;
		  
		  case 0x1700 ... 0x17FF : //Betacrypt
		    caOn[8] = true;
		  break;
		}
	  }
	  
	  for (int i=0;i<NUM_CA_IDS;i++)
	  {
	    DrawCryptSymbol(xCa, osdHeight - fontHeight, caShort[i], Theme.Color(caOn[i] ? clrChannelFontTitle : clrChannelFontEpg), Theme.Color(clrChannelBg));
		widthCa = (font->Width(caShort[i]) + 20);
              xCa += widthCa;
	  }

}

void cMetrixHDDisplayChannel::SetEvents(const cEvent *Present, const cEvent *Following) {
    present = Present;
    cString epgShort("");
    cString epg("");

    int leftepg = fontBigHeight + fontHeight + (fontSmlHeight * 3) + ProgressBarHeight() + marginItem;
    chanInfoBottomPixmap->Fill(Theme.Color(clrChannelBg));

    int imageHeight = heightBottom - marginItem*2;
   
    if( imgLoader.LoadLogo(*channelName, imageHeight, imageHeight) ) {
        int imageLeft = marginItem;
        int imageTop = (heightBottom - imgLoader.Height() ) / 2;
        
        chanInfoLogo->DrawImage( cPoint(imageLeft, imageTop), imgLoader.GetImage() );
    }

    if( Present ) {
        cString startTime = Present->GetTimeString();
        cString endTime = Present->GetEndTimeString();

        cString timeString = cString::sprintf("%s - %s", *startTime, *endTime);
        int timeStringWidth = fontSml->Width(*timeString);

        int epgWidth = font->Width(Present->Title());
        int epgShortWidth = fontSml->Width(Present->ShortText());
        
        int s = (int)(time(NULL) - Present->StartTime()) / 60;
        int sleft = (Present->Duration() / 60) - s;

        cString seen = cString::sprintf("+%d min", sleft);
        int seenWidth = fontSml->Width(*seen);

        if( epgWidth > osdWidth - left - timeStringWidth ) {
            int dotsWidth = font->Width("... ");
            cTextWrapper epgInfoWrapper(Present->Title(), font, osdWidth - left - timeStringWidth - dotsWidth);
            epg = epgInfoWrapper.GetLine(0);
            epg = cString::sprintf("%s...", *epg);
        } else {
            epg = Present->Title();
        }

        if( epgShortWidth > osdWidth - left - timeStringWidth ) {
            int dotsWidth = fontSml->Width("... ");
            cTextWrapper epgInfoWrapper(Present->ShortText(), fontSml, osdWidth - left - timeStringWidth - dotsWidth);
            epgShort = epgInfoWrapper.GetLine(0);
            epgShort = cString::sprintf("%s...", *epgShort);
        } else {
            epgShort = Present->ShortText();
        }
        
        chanInfoBottomPixmap->DrawText(cPoint(osdWidth - timeStringWidth, fontSmlHeight), *timeString,
                Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);
        chanInfoBottomPixmap->DrawText(cPoint(osdWidth - seenWidth, fontSmlHeight*2), *seen,
                Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);
        
        chanInfoBottomPixmap->DrawText(cPoint(leftepg, 0), *epg, Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontBig);
    }
    if( Following ) {
        cString startTime = Following->GetTimeString();
        cString endTime = Following->GetEndTimeString();

        cString timeString = cString::sprintf("%s - %s", *startTime, *endTime);
        int timeStringWidth = fontSml->Width(*timeString);

        int epgWidth = fontSml->Width(Following->Title());
        int epgShortWidth = fontSml->Width(Following->ShortText());
        
        cString dur = cString::sprintf("%d min", Following->Duration() / 60);
        int durWidth = fontSml->Width(*dur);

        if( epgWidth > osdWidth - left - timeStringWidth ) {
            int dotsWidth = font->Width("... ");
            cTextWrapper epgInfoWrapper(Following->Title(), font, osdWidth - left - timeStringWidth - dotsWidth);
            epg = epgInfoWrapper.GetLine(0);
            epg = cString::sprintf("%s...", *epg);
        } else {
            epg = Following->Title();
        }
        if (epgShortWidth > osdWidth - left - timeStringWidth ) {
            int dotsWidth = fontSml->Width("... ");
            cTextWrapper epgInfoWrapper(Following->ShortText(), fontSml, osdWidth - left - timeStringWidth - dotsWidth);
            epgShort = epgInfoWrapper.GetLine(0);
            epgShort = cString::sprintf("%s...", *epgShort);
        } else {
            epgShort = Following->ShortText();
        }

        chanInfoBottomPixmap->DrawText(cPoint(osdWidth - timeStringWidth, fontBigHeight + fontSmlHeight), *timeString,
                Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);
        chanInfoBottomPixmap->DrawText(cPoint(osdWidth - durWidth, fontBigHeight + fontSmlHeight*2), *dur,
                Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);

        chanInfoBottomPixmap->DrawText(cPoint(leftepg, fontBigHeight + fontSmlHeight), *epg,
                Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), font);
      }
}


void cMetrixHDDisplayChannel::SetMessage(eMessageType Type, const char *Text) {
    // Wenn es einen Text gibt, diesen Anzeigen ansonsten Message ausblenden
    if( Text )
        MessageSet(Type, Text);
    else
        MessageClear();
}

void cMetrixHDDisplayChannel::DrawCryptSymbol(int xSymbol, int ySymbol, const char *Text, tColor color, tColor bgcolor)
{
  osd->DrawText (xSymbol+2, ySymbol+5, Text, color, clrTransparent, font, 0, 0, taCenter);
}

void cMetrixHDDisplayChannel::DrawSignal(void) {
        cString signalStrength = "STR";
        int signalBarWidth = (osdWidth / 3) * 2 / 10;
        int strwidth = fontSml->Width(*signalStrength);
        SignalStrength = cDevice::ActualDevice()->SignalStrength();
        SignalPixmap->DrawRectangle(cRect(signalBarWidth * 2 ,fontHeight / 2 - 2 , signalBarWidth * 2, 4), progressBarColorFg);
        SignalPixmap->DrawText(cPoint(signalBarWidth * 2 - 5 - strwidth, (fontHeight - fontSmlHeight) / 2), *signalStrength, Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);
            SignalPixmap->DrawRectangle(cRect(signalBarWidth * 2 ,fontHeight / 2 - 4 , (signalBarWidth * 2) / 100 * SignalStrength, 8), progressBarColorBarFg);
        lastSignalStrength = SignalStrength;

}

void cMetrixHDDisplayChannel::DrawQuality(void) {
        int signalBarWidth = (osdWidth / 3) * 2 / 10;
        cString signalQuality = "SNR";
        int snrwidth = fontSml->Width(*signalQuality);
        SignalQuality = cDevice::ActualDevice()->SignalQuality();
        SignalPixmap->DrawRectangle(cRect(signalBarWidth * 6 ,fontHeight / 2 - 2 , signalBarWidth * 2, 4), progressBarColorFg);
        SignalPixmap->DrawText(cPoint(signalBarWidth * 6 - 5 -snrwidth, (fontHeight - fontSmlHeight) / 2), *signalQuality, Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg),fontSml);
        if (SignalQuality > 0)
           SignalPixmap->DrawRectangle(cRect(signalBarWidth * 6 ,fontHeight / 2 - 4 , (signalBarWidth * 2) / 100 * SignalQuality, 8), progressBarColorBarFg);
        lastSignalQuality= SignalQuality;
}

void cMetrixHDDisplayChannel::Flush(void) {
    int Current = 0;
    int Total = 0;
    if( present ) {
        time_t t = time(NULL);
        if (t > present->StartTime())
            Current = t - present->StartTime();
        Total = present->Duration();
        ProgressBarDraw(Current, Total);
    }
    DrawQuality();
    DrawSignal();
    TopBarUpdate();
    osd->Flush();
}
