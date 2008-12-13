/*
 * mousemate plugin for VDR
 *
 * setup.h - settings and setup menu
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#ifndef _SETUP_H_
#define _SETUP_H_

#include <vdr/plugin.h>

const int kEvents = 38;
const int kEventTurnWheelLeft          = 0;
const int kEventTurnWheelRight         = 1;
const int kEventTurnXLeft              = 2;
const int kEventTurnXRight             = 3;
const int kEventTurnYLeft              = 4;
const int kEventTurnYRight             = 5;
const int kEventLeftClick              = 6;
const int kEventLeftDoubleClick        = 7;
const int kEventLeftPushedTurnLeft     = 8;
const int kEventLeftPushedTurnRight    = 9;
const int kEventMiddleClick            = 10;
const int kEventMiddleDoubleClick      = 11;
const int kEventMiddlePushedTurnLeft   = 12;
const int kEventMiddlePushedTurnRight  = 13;
const int kEventRightClick             = 14;
const int kEventRightDoubleClick       = 15;
const int kEventRightPushedTurnLeft    = 16;
const int kEventRightPushedTurnRight   = 17;
const int kEventSideClick              = 18;
const int kEventSideDoubleClick        = 19;
const int kEventSidePushedTurnLeft     = 20;
const int kEventSidePushedTurnRight    = 21;
const int kEventExtraClick             = 22;
const int kEventExtraDoubleClick       = 23;
const int kEventExtraPushedTurnLeft    = 24;
const int kEventExtraPushedTurnRight   = 25;
const int kEventForwardClick           = 26;
const int kEventForwardDoubleClick     = 27;
const int kEventForwardPushedTurnLeft  = 28;
const int kEventForwardPushedTurnRight = 29;
const int kEventBackClick              = 30;
const int kEventBackDoubleClick        = 31;
const int kEventBackPushedTurnLeft     = 32;
const int kEventBackPushedTurnRight    = 33;
const int kEventTaskClick              = 34;
const int kEventTaskDoubleClick        = 35;
const int kEventTaskPushedTurnLeft     = 36;
const int kEventTaskPushedTurnRight    = 37;

const int kContexts = 14;
const int kContextNormal = 0;
const int kContextMenu = 1;
const int kContextReplay = 2;
const int kContextReplayDVD = 3;
const int kContextReplayMP3 = 4;
const int kContextReplayCDDA = 5;
const int kContextReplayIMAGE = 6;
const int kContextNormalAlt = 7;
const int kContextMenuAlt = 8;
const int kContextReplayAlt = 9;
const int kContextReplayDVDAlt = 10;
const int kContextReplayMP3Alt = 11;
const int kContextReplayCDDAAlt = 12;
const int kContextReplayIMAGEAlt = 13;

class cMouseMateSetup
{
    private:
        // nothing yet
    public:
        char device[256];                         // device for mouse
        char cdevice[256];                        // device for console
        int sensitivity;
        int sensitivityX;
        int sensitivityY;
        int doubleClickTime;
        int toggleClickTime;
        int led;                                  // for scroll-lock led
        int autotoggleoff;                        // auto toggle off after toggletime yes no
        int extrapushedturn;
        int alternative;
        int dvdsensitive;
        int mp3sensitive;
        int cddasensitive;
        int imagesensitive;
        int extraturns;
        int toggleOffTime;
        eKeys keys[kContexts][kEvents];

        cMouseMateSetup();
        bool Parse(const char *Name, const char *Value);

};

class cMouseMateSetupPage : public cMenuSetupPage
{
    private:
        char * strKeys[100];
        int numKeys;
        int sensitivity;
        int sensitivityX;
        int sensitivityY;
        int doubleClickTime;
        int toggleClickTime;
        int led;
        int autotoggleoff;                        // auto toggle off after toggletime yes no
        int extrapushedturn;
        int alternative;
        int dvdsensitive;
        int mp3sensitive;
        int cddasensitive;
        int imagesensitive;
        int extraturns;
        int toggleOffTime;                        // timer after toggle is switched off automatically
        int keys[kContexts][kEvents];
        int setKeys[kEvents];
        int context;
        int contextOld;

        cOsdItem * itemContext;
        cOsdItem * itemToggle;
    protected:
        virtual void Store();
        virtual eOSState ProcessKey(eKeys key);
        void Setup();
    public:
        cMouseMateSetupPage();
        virtual ~cMouseMateSetupPage();
};

extern cMouseMateSetup MouseMateSetup;
#endif
