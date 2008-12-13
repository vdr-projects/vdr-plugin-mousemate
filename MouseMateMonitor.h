/*
 * mousemate plugin for VDR
 *
 * MouseMateMonitor.h - MouseMate context monitor class
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#ifndef _MOUSEMATEMONITOR_H_
#define _MOUSEMATEMONITOR_H_

#include "setup.h"
#include <vdr/status.h>
#include <vdr/tools.h>

class cMouseMateMonitor : public cStatus
{
    private:
        bool menu;
        bool replay;
        bool DVD;
        bool MP3;
        bool CDDA;
        bool IMAGE;

    public:
        cMouseMateMonitor(void);
        virtual ~cMouseMateMonitor();
        int GetContext(const bool toggle);

        virtual void Replaying(const cControl *Control, const char *Name, const char *FileName, bool On);
        virtual void OsdClear(void);
        virtual void OsdTitle(const char *Title);
        virtual void OsdHelpKeys(const char *Red, const char *Green, const char *Yellow, const char *Blue);
        virtual void OsdItem(const char *Text, int Index);
        virtual void OsdCurrentItem(const char *Text);
        virtual void OsdTextItem(const char *Text, bool Scroll);
};
#endif
