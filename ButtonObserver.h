/*
 * mousemate plugin for VDR
 *
 * ButtonObserver.h - MouseMate button observer class
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#ifndef __BUTTON_OBSERVER_H
#define __BUTTON_OBSERVER_H

#include <vdr/keys.h>
#include <vdr/thread.h>
#include <vdr/tools.h>
#include <vdr/remote.h>

class cButtonObserver
{
    public:
        bool pushed;
        uint64_t lastPush;
        bool click;

        int clickEvent;
        int doubleClickEvent;
        int pushedTurnLeftEvent;
        int pushedTurnRightEvent;

        unsigned int buttonCode;

        cButtonObserver();

        void Init(unsigned int buCo, int clEv, int dbClEv, int puTuRiEv, int puTuLeEv);
};
#endif
