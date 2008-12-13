/*
 * mousemate plugin for VDR
 *
 * MouseMate.c - MouseMate control class
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#include <linux/input.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include "MouseMate.h"
#include "ButtonObserver.h"

// buffer size for reading events
const int BUFFER_SIZE=32;

#ifdef DEBUG
const char * strContextsDebug[kContexts] =
{
    "Normal",
    "Menu",
    "Replay",
    "Replay DVD",
    "Replay MP3",
    "Replay CDDA",
    "Replay IMAGE",
    "Normal alternative",
    "Menu alternative",
    "Replay alternative",
    "Replay DVD alternative",
    "Replay MP3 alternative",
    "Replay CDDA alternative",
    "Replay IMAGE alternative"
};

const char * strEventsDebug[kEvents] =
{
    "TurnWheelLeft",
    "TurnWheelRight",
    "TurnXLeft",
    "TurnXRight",
    "TurnYLeft",
    "TurnYRight",
    "LeftClick",
    "LeftDoubleClick",
    "LeftPushedTurnLeft",
    "LeftPushedTurnRight",
    "MiddleClick",
    "MiddleDoubleClick",
    "MiddlePushedTurnLeft",
    "MiddlePushedTurnRight",
    "RightClick",
    "RightDoubleClick",
    "RightPushedTurnLeft",
    "RightPushedTurnRight",
    "SidePushedTurnLeft",
    "SidePushedTurnRight",
    "SideClick",
    "SideDoubleClick",
    "ExtraClick",
    "ExtraDoubleClick",
    "ExtraPushedTurnLeft",
    "ExtraPushedTurnRight",
    "ForwardClick",
    "ForwardDoubleClick",
    "ForwardPushedTurnLeft",
    "ForwardPushedTurnRight",
    "BackClick",
    "BackDoubleClick",
    "BackPushedTurnLeft",
    "BackPushedTurnRight",
    "TaskClick",
    "TaskDoubleClick"
    "TaskPushedTurnLeft",
    "TaskPushedTurnRight"
};
#endif

cMouseMate * MouseMate;

cMouseMate::cMouseMate()
:pushed(false),
lastPush(0),
offset(0),
offsetX(0),
offsetY(0),
click(false),
turn(false),
toggle(false),
toggletime(0),
btnCount(0),
active(false)
{
    fd = open(MouseMateSetup.device, O_RDONLY);
    if (fd < 0)
    {
        esyslog("mousemate: unable to open %s: %s\n", MouseMateSetup.device, strerror(errno));
        fd = -1;
        return;
    }

    // Check if same EV_VERSION is used, because of hardcoded values below
    if (EV_VERSION!=0x010000)
    {
        esyslog("mousemate: Warning this Plugin is written for another EV_VERSION!"
            "mousemate: This may cause incompatibility!");
    }

    uint8_t evtype_bitmask[KEY_MAX/8+1];
    int yalv;
    memset(evtype_bitmask, 0, sizeof(evtype_bitmask));

    if (ioctl(fd, EVIOCGBIT(EV_KEY, KEY_MAX), evtype_bitmask) < 0)
    {
        esyslog("mousemate: %s: EVIOCGBIT failed: %s\n", MouseMateSetup.device, strerror(errno));
        return;
    }

    for (yalv = BTN_MOUSE; yalv < BTN_MOUSE+9; yalv++)
    {
        if (test_bit(yalv, evtype_bitmask))
        {
            /* this means that the bit is set in the event types list */
            switch (yalv)
            {
                case BTN_LEFT :
                #ifdef DEBUG
                    dsyslog("mousemate: button found BTN_LEFT\n");
                #endif
                    buttonObservers[0].Init(BTN_LEFT, kEventLeftClick, kEventLeftDoubleClick, kEventLeftPushedTurnLeft, kEventLeftPushedTurnRight);
                    btnCount++;
                    break;
                case BTN_RIGHT :
                #ifdef DEBUG
                    dsyslog("mousemate: button found BTN_RIGHT\n");
                #endif
                    buttonObservers[1].Init(BTN_RIGHT, kEventRightClick, kEventRightDoubleClick,kEventRightPushedTurnLeft, kEventRightPushedTurnRight);
                    btnCount++;
                    break;
                case BTN_MIDDLE :
                #ifdef DEBUG
                    dsyslog("mousemate: button found BTN_MIDDLE\n");
                #endif
                    buttonObservers[2].Init(BTN_MIDDLE, kEventMiddleClick, kEventMiddleDoubleClick, kEventMiddlePushedTurnLeft, kEventMiddlePushedTurnRight);
                    btnCount++;
                    break;
                case BTN_SIDE :
                #ifdef DEBUG
                    dsyslog("mousemate: button found TN_SIDE\n");
                #endif
                    buttonObservers[3].Init(BTN_SIDE, kEventSideClick, kEventSideDoubleClick, kEventSidePushedTurnLeft, kEventSidePushedTurnRight);
                    btnCount++;
                    break;
                case BTN_EXTRA :
                #ifdef DEBUG
                    dsyslog("mousemate: button found BTN_EXTRA\n");
                #endif
                    buttonObservers[4].Init(BTN_EXTRA, kEventExtraClick, kEventExtraDoubleClick, kEventExtraPushedTurnLeft, kEventExtraPushedTurnRight);
                    btnCount++;
                    break;
                case BTN_FORWARD :
                #ifdef DEBUG
                    dsyslog("mousemate: button found BTN_FORWARD\n");
                #endif
                    buttonObservers[5].Init(BTN_FORWARD, kEventForwardClick, kEventForwardDoubleClick, kEventForwardPushedTurnLeft, kEventForwardPushedTurnRight);
                    btnCount++;
                    break;
                case BTN_BACK :
                #ifdef DEBUG
                    dsyslog("mousemate: button found BTN_BACK\n");
                #endif
                    buttonObservers[6].Init(BTN_BACK, kEventBackClick, kEventBackDoubleClick,kEventBackPushedTurnLeft, kEventBackPushedTurnRight);
                    btnCount++;
                    break;
                case BTN_TASK :
                #ifdef DEBUG
                    dsyslog("mousemate: button found BTN_TASK\n");
                #endif
                    buttonObservers[7].Init(BTN_TASK, kEventTaskClick, kEventTaskDoubleClick, kEventTaskPushedTurnLeft, kEventTaskPushedTurnRight);
                    btnCount++;
                    break;
            }
        }
    }

    #ifdef DEBUG
        dsyslog("mousemate: number of found buttons btnCount = %i\n",btnCount);
    #endif

    if (btnCount<3)
    {
        esyslog("mousemate: number of buttons < 3 \n"
            "mousemate: ******** plugin not started ******** \n" );
        return;
    }

    Monitor=new cMouseMateMonitor;
    Start();
}


cMouseMate::~cMouseMate()
{
    delete Monitor;
    active = false;
    close(fd);
    Cancel(3);
}


bool cMouseMate::IsButtonUsed(unsigned int buCo)
{
    return !buttonObservers[buCo-BTN_MOUSE].buttonCode ? false : true;
}


short int cMouseMate::Get_btnCount()
{
    return btnCount;
}


void cMouseMate::Action()
{
    active = true;
    dsyslog("MouseMate thread started (pid=%d)", getpid());

    struct input_event events[BUFFER_SIZE];
    int r;
    int num;
    int i;

    while (fd >= 0 && active)
    {
        LOCK_THREAD;
        if (cFile::FileReady(fd, 100))
        {
            r = safe_read(fd, events, sizeof(struct input_event) * BUFFER_SIZE);
            if (r > 0)
            {
                num = r / sizeof(struct input_event);
                for (i = 0; i < num; i++)
                {
                    ProcessInputEvent(&events[i]);
                }
            }
            else
            {
                LOG_ERROR;
            }
        }

        // controll more than one button with buttonObserver & for loop
        for (i=0; i < btnCount; i++)
        {
            if (buttonObservers[i].click && !buttonObservers[i].pushed)
            {
                uint64_t current = TimeMs();
                if (current - buttonObservers[i].lastPush > (uint64_t) MouseMateSetup.doubleClickTime)
                {
                    #ifdef DEBUG
                    dsyslog("mousemate: click (main loop)\n");
                    #endif
                    ProcessEvent(buttonObservers[i].clickEvent);
                    buttonObservers[i].click = false;
                }
            }

            // toggle?
            if (buttonObservers[i].pushed && !turn && MouseMateSetup.alternative)
            {
                uint64_t current = TimeMs();
                if (current - buttonObservers[i].lastPush > (uint64_t) MouseMateSetup.toggleClickTime)
                {
                    Toggle();
                    // clear events
                    struct input_event dummyevents[BUFFER_SIZE];
                    r = safe_read(fd, dummyevents, sizeof(struct input_event) * BUFFER_SIZE);
                }
            }
        }

        // handle automatic toggle off after "toggleOffTime" since last event
        if(toggle && MouseMateSetup.autotoggleoff && MouseMateSetup.alternative)
        {
            uint64_t current = TimeMs();
            if (current - toggletime > (uint64_t) MouseMateSetup.toggleOffTime  )
            {
                Toggle();
                #ifdef DEBUG
                dsyslog("mousemate: toggle inactive automatically after %i ms \n",  MouseMateSetup.toggleOffTime);
                #endif
                // clear events
                struct input_event dummyevents[BUFFER_SIZE];
                r = safe_read(fd, dummyevents, sizeof(struct input_event) * BUFFER_SIZE);
            }
        }
    }
    dsyslog("MouseMate thread ended (pid=%d)", getpid());
}


void cMouseMate::Toggle()
{
    if (toggle)
    {
        toggle= false;
        #ifdef DEBUG
           dsyslog("mousemate: toggle inactive \n");
        #endif
    }
    else
    {
        toggle=true;
        toggletime=TimeMs();
        #ifdef DEBUG
           dsyslog("mousemate: toggle active after %i ms pushed button\n", MouseMateSetup.toggleClickTime);
        #endif
    }

    // controll more than one button with buttonObservers & for loop
    int i;
    for (i=0; i<btnCount; i++)
    {
        buttonObservers[i].pushed=false;
        buttonObservers[i].click=false;
    }

    // set scroll lock led if led = true
    if (MouseMateSetup.led)
    {
        int fc;
        fc = open(MouseMateSetup.cdevice, O_RDWR);
        // can console device be opened?
        if (fc)
        {
            struct input_event ev;
            ev.type = EV_LED;
            ev.code = LED_SCROLLL;
            ev.value = toggle;
            write(fc, &ev, sizeof(struct input_event));
            close(fc);
        }
        else
        {
            esyslog("mousemate: unable to open console device %s: %s\n", MouseMateSetup.cdevice, strerror(errno));
            fc = -1;
            return;
        }
    }
}


void cMouseMate::ProcessInputEvent(struct input_event * ev)
{
    bool pushed=false;
    int button=2;

    switch (ev->type)
    {
        case EV_REL:
            if (ev->code == Wheel)
            {
                turn = true;
                offset += (int) ev->value;
                for (int i=0; i < btnCount; i++)
                {
                    if (buttonObservers[i].pushed)
                    {
                        pushed=true;
                        button=i;
                    }
                }
                if (pushed)
                {
                    if ((int) ev->value < 0)
                    {
                        while (offset <= -MouseMateSetup.sensitivity)
                        {
                            ProcessEvent(buttonObservers[button].pushedTurnLeftEvent);
                            offset += MouseMateSetup.sensitivity;
                        }
                    }
                    if ((int) ev->value > 0)
                    {
                        while (offset >= MouseMateSetup.sensitivity)
                        {
                            ProcessEvent(buttonObservers[button].pushedTurnRightEvent);
                            offset -= MouseMateSetup.sensitivity;
                        }
                    }
                }
                else
                {
                    for (int i=0; i < btnCount; i++)
                    {
                        if (buttonObservers[i].click)
                        {
                            #ifdef DEBUG
                                dsyslog("mousemate: click (ProcessInputEvent)\n");
                            #endif
                            ProcessEvent(buttonObservers[i].clickEvent);
                            buttonObservers[i].click = false;
                        }
                    }
                    if ((int) ev->value < 0)
                    {
                        while (offset <= -MouseMateSetup.sensitivity)
                        {
                            ProcessEvent(kEventTurnWheelLeft);
                            offset += MouseMateSetup.sensitivity;
                        }
                    }
                    if ((int) ev->value > 0)
                    {
                        while (offset >= MouseMateSetup.sensitivity)
                        {
                            ProcessEvent(kEventTurnWheelRight);
                            offset -= MouseMateSetup.sensitivity;
                        }
                    }
                }
            }

            if(MouseMateSetup.extraturns)
            {
                if (ev->code == REL_X)
                {
                    offsetX += (int) ev->value;
                    if ((int) ev->value < 0)
                    {
                        while (offsetX <= -MouseMateSetup.sensitivityX)
                        {
                            ProcessEvent(kEventTurnXLeft);
                            offsetX += MouseMateSetup.sensitivityX;
                        }
                    }
                    if ((int) ev->value > 0)
                    {
                        while (offsetX >= MouseMateSetup.sensitivityX)
                        {
                            ProcessEvent(kEventTurnXRight);
                            offsetX -= MouseMateSetup.sensitivityX;
                        }
                    }
                }
                if (ev->code == REL_Y)
                {
                    offsetY += (int) ev->value;
                    if ((int) ev->value < 0)
                    {
                        while (offsetY <= -MouseMateSetup.sensitivityY)
                        {
                            ProcessEvent(kEventTurnYLeft);
                            offsetY += MouseMateSetup.sensitivityY;
                        }
                    }
                    if ((int) ev->value > 0)
                    {
                        while (offsetY >= MouseMateSetup.sensitivityY)
                        {
                            ProcessEvent(kEventTurnYRight);
                            offsetY -= MouseMateSetup.sensitivityY;
                        }
                    }
                }
            }
            break;
        case EV_KEY:

            for (int i=0; i<btnCount; i++)
            {
                if (ev->code == buttonObservers[i].buttonCode)
                {
                    if (ev->value)
                    {
                        buttonObservers[i].pushed = true;
                        turn = false;
                        offset = 0;
                        buttonObservers[i].lastPush = TimeMs();
                    }
                    else
                    {
                        if (buttonObservers[i].click)
                        {
                            #ifdef DEBUG
                                dsyslog("mousemate: double click\n");
                            #endif
                            ProcessEvent(buttonObservers[i].doubleClickEvent);
                            buttonObservers[i].click = false;
                        }
                        else
                        {
                            if (!turn)
                                buttonObservers[i].click = true;
                        }
                        buttonObservers[i].pushed = false;
                        turn = false;
                        offset = 0;
                    }
                }
            }
            break;
    }
    #ifdef EVENTCODE_SNIFFER
    printf("mousemate: ev->type  = 0x%04x\n", ev->type);
    printf("mousemate: ev->code  = 0x%04x\n", ev->code);
    printf("mousemate: ev->value = %i\n", int(ev->value));
    printf("-----------------------------\n");
    #endif
}


void cMouseMate::ProcessEvent(int event)
{
    int context= Monitor->GetContext(toggle);

    if (!toggle)
    {
        SendKey(MouseMateSetup.keys[context][event]);
    }
    else
    {
        toggletime=TimeMs();                      // updating toggletime for key is pressed in toggle mode
        SendKey(MouseMateSetup.keys[context][event]);
    }

    #ifdef DEBUG
        dsyslog("mousemate: toggle=%i, event=%s, context=%s\n",toggle,
                 strEventsDebug[event],strContextsDebug[context]);
        eKeys keyNr=MouseMateSetup.keys[context][event];
        dsyslog("mousemate: sent key= nr %i, %s\n",keyNr, cKey::ToString(keyNr));
    #endif
}


void cMouseMate::SendKey(eKeys key)
{
    if (key != kNone)
        cRemote::Put(key);
}
