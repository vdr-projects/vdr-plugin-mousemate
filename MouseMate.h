/*
 * mousemate plugin for VDR
 *
 * MouseMate.h - MouseMate control class
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#ifndef _MOUSEMATE_H_
#define _MOUSEMATE_H_

#include <vdr/keys.h>
#include <vdr/thread.h>
#include <vdr/tools.h>
#include <vdr/remote.h>

#include "MouseMateMonitor.h"
#include "ButtonObserver.h"
#include "setup.h"

/*
 You may try other relative movement events
*/
#define Wheel REL_WHEEL

/*
 Number of observed mouse-buttons
*/
#define MAX_BTN_COUNT 8

/*
 You may define EVENTCODE_SNIFFER in order to get event code, type and value of
 your device.
*/
// #define EVENTCODE_SNIFFER

/*
 You may define DEBUG in order to get information about context, event, etc.
*/
// #define DEBUG

/*
 ******************* no user defines after this line *************************
 */
#define TimeMs cTimeMs::Now

#define test_bit(bit, array)    (array[bit/8] & (1<<(bit%8)))

class cMouseMate : public cThread
{
    private:

        int fd;                                   // file id for mouse device
        bool pushed;
        uint64_t lastPush;
        int offset;
        int offsetX;
        int offsetY;
        bool click;
        bool turn;
        bool toggle;                              // toggle key binding
        uint64_t toggletime;                      // time for auto off toggle

        // controll more than one button with buttonObservers & for loop
        cButtonObserver buttonObservers[MAX_BTN_COUNT];
        short int btnCount;                       // number of used buttons:

        bool active;
        cMouseMateMonitor *Monitor;

        virtual void Action();
        void Toggle();
        void ProcessInputEvent(struct input_event * ev);
        void ProcessEvent(int event);
        void SendKey(eKeys key);
    public:
        cMouseMate();
        virtual ~cMouseMate();
        bool IsButtonUsed(unsigned int buttoncode);
        short int Get_btnCount();
};

extern cMouseMate * MouseMate;
#endif
