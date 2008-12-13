/*
 * mousemate plugin for VDR
 *
 * MouseMate.c - MouseMate context monitor class
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#include "MouseMateMonitor.h"
#include "MouseMate.h"

cMouseMateMonitor:: cMouseMateMonitor(void)
:menu(false),
replay(false),
DVD(false),
MP3(false),
CDDA(false),
IMAGE(false)
{
}


cMouseMateMonitor:: ~cMouseMateMonitor()
{
}


int cMouseMateMonitor::GetContext(const bool toggle)
{
    if (!toggle)
    {
        if (menu)
        {
            #ifdef DEBUG
            dsyslog("mousemate: Context=Menu DVD=%i MP3=%i CDDA=%i IMAGE=%i\n",DVD,MP3,CDDA,IMAGE);
            #endif
            return kContextMenu;
        }
        if (replay)
        {
            #ifdef DEBUG
            dsyslog("mousemate: Context=Replay DVD=%i MP3=%i CDDA=%i IMAGE=%i\n",DVD,MP3,CDDA,IMAGE);
            #endif
            if (DVD && MouseMateSetup.dvdsensitive)
                return kContextReplayDVD;
            if (MP3&& MouseMateSetup.mp3sensitive)
                return kContextReplayMP3;
            if (CDDA&& MouseMateSetup.cddasensitive)
                return kContextReplayCDDA;
            if (IMAGE&& MouseMateSetup.imagesensitive)
                return kContextReplayIMAGE;

            return kContextReplay;
        }
        #ifdef DEBUG
        dsyslog("mousemate: Context=Normal DVD=%i MP3=%i CDDA=%i IMAGE=%i\n",DVD, MP3,CDDA, IMAGE);
        #endif
        return kContextNormal;
    }
    else
    {
        if (menu)
        {
            #ifdef DEBUG
            dsyslog("mousemate: Context=MenuAlt DVD=%i MP3=%i CDDA=%i IMAGE=%i\n",DVD,MP3,CDDA,IMAGE);
            #endif
            return kContextMenuAlt;
        }
        if (replay)
        {
            #ifdef DEBUG
            dsyslog("mousemate: Context=ReplayAlt DVD=%i MP3=%i CDDA=%i IMAGE=%i\n", DVD,MP3,CDDA,IMAGE);
            #endif
            if (DVD && MouseMateSetup.dvdsensitive)
                return kContextReplayDVDAlt;
            if (MP3&& MouseMateSetup.mp3sensitive)
                return kContextReplayMP3Alt;
            if (CDDA&& MouseMateSetup.cddasensitive)
                return kContextReplayCDDAAlt;
            if (IMAGE&& MouseMateSetup.imagesensitive)
                return kContextReplayIMAGEAlt;
            return kContextReplayAlt;
        }
        #ifdef DEBUG
        dsyslog("mousemate: Context=NormalAlt DVD=%i MP3=%i CDDA=%i IMAGE=%i\n",DVD, MP3,CDDA, IMAGE);
        #endif
        return kContextNormalAlt;
    }

}


void cMouseMateMonitor::Replaying(const cControl *Control, const char *Name, const char *FileName, bool On)
{
    if (On)
    {
        replay=true;
        if (!strcmp(Name,"DVD"))
            DVD=true;
        if (!strcmp(Name,"MP3"))
            MP3=true;
        if (!strcmp(Name,"cdda"))
            CDDA=true;
        if (!strcmp(Name,"image"))
            IMAGE=true;
    }
    else
    {
        DVD=false;
        MP3=false;
        CDDA=false;
        IMAGE=false;
        replay=false;
    }
    #ifdef DEBUG
    dsyslog("mousemate: %s is %i\n", Name, On);
    dsyslog("mousemate: Replaying DVD %i MP3: %i CDDA: %i IMAGE: %i\n",DVD, MP3,CDDA, IMAGE);
    #endif
}


void cMouseMateMonitor::OsdClear(void)
{
    menu =false;
}


void cMouseMateMonitor::OsdTitle(const char *Title)
{
    menu = true;
}


void cMouseMateMonitor::OsdHelpKeys(const char *Red, const char *Green, const char *Yellow, const char *Blue)
{
    menu = true;
}


void cMouseMateMonitor::OsdItem(const char *Text, int Index)
{
    menu = true;
}


void cMouseMateMonitor::OsdCurrentItem(const char *Text)
{
    menu = true;
}


void cMouseMateMonitor::OsdTextItem(const char *Text, bool Scroll)
{
    menu = true;
}
