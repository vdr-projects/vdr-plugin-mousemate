/*
 * mousemate plugin for VDR
 *
 * setup.c - settings and setup menu
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#include <linux/input.h>
#include <vdr/plugin.h>
#include <vdr/keys.h>
#include <vdr/interface.h>


#include "setup.h"
#include "MouseMate.h"

cMouseMateSetup MouseMateSetup;

const char * strContexts[kContexts] =
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

const char * strEvents[kEvents] =
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

cMouseMateSetup::cMouseMateSetup()
:sensitivity(1),
doubleClickTime(300),
toggleClickTime(1000),
led(0),
autotoggleoff(0),
extrapushedturn(0),
alternative(0),
dvdsensitive(0),
mp3sensitive(0),
cddasensitive(0),
imagesensitive(0),
extraturns(0),
toggleOffTime(3000)
{
    for (int c = 0; c < kContexts; c++)
        for (int e = 0; e < kEvents; e++)
            keys[c][e] = kNone;
}


bool cMouseMateSetup::Parse(const char *Name, const char *Value)
{
    // Parse your own setup parameters and store their values.

    if (!strcasecmp(Name, "Sensitivity")) sensitivity = atoi(Value);
    else if (!strcasecmp(Name, "SensitivityX")) sensitivityX = atoi(Value);
    else if (!strcasecmp(Name, "SensitivityY")) sensitivityY = atoi(Value);
    else if (!strcasecmp(Name, "DoubleClickTime")) doubleClickTime = atoi(Value);
    else if (!strcasecmp(Name, "ToggleClickTime")) toggleClickTime = atoi(Value);
    else if (!strcasecmp(Name, "LED")) led = atoi(Value);
    else if (!strcasecmp(Name, "AutoToggleOff")) autotoggleoff = atoi(Value);
    else if (!strcasecmp(Name, "ExtraPushedTurn")) extrapushedturn = atoi(Value);
    else if (!strcasecmp(Name, "ExtraTurns")) extraturns = atoi(Value);
    else if (!strcasecmp(Name, "DvdSensitive")) dvdsensitive = atoi(Value);
    else if (!strcasecmp(Name, "Mp3Sensitive")) mp3sensitive = atoi(Value);
    else if (!strcasecmp(Name, "CddaSensitive")) cddasensitive = atoi(Value);
    else if (!strcasecmp(Name, "ImageSensitive")) imagesensitive = atoi(Value);
    else if (!strcasecmp(Name, "Alternative"))  alternative= atoi(Value);
    else if (!strcasecmp(Name, "ToggleOffTime")) toggleOffTime = atoi(Value);
    else if (strstr(Name, "Key") == Name)
    {
        for (int c = 0; c < kContexts; c++)
        {
            for (int e = 0; e < kEvents; e++)
            {
                char str[256];
                sprintf(str, "Key%s%s", strContexts[c], strEvents[e]);
                if (!strcasecmp(Name, str))
                {
                    keys[c][e] = eKeys(atoi(Value));
                    return true;
                }
            }
        }
        return false;
    }
    else
        return false;

    return true;
}


static tKey keyTable[] =
{
    { kNone,          "None"       },
    { kUp,            "Up"         },
    { kDown,          "Down"       },
    { kMenu,          "Menu"       },
    { kOk,            "Ok"         },
    { kBack,          "Back"       },
    { kLeft,          "Left"       },
    { kRight,         "Right"      },
    { kRed,           "Red"        },
    { kGreen,         "Green"      },
    { kYellow,        "Yellow"     },
    { kBlue,          "Blue"       },
    { k0,             "0"          },
    { k1,             "1"          },
    { k2,             "2"          },
    { k3,             "3"          },
    { k4,             "4"          },
    { k5,             "5"          },
    { k6,             "6"          },
    { k7,             "7"          },
    { k8,             "8"          },
    { k9,             "9"          },
    { kInfo,          "Info"       },
    { kPlay,          "Play"       },
    { kPause,         "Pause"      },
    { kStop,          "Stop"       },
    { kRecord,        "Record"     },
    { kFastFwd,       "FastFwd"    },
    { kFastRew,       "FastRew"    },
    { kNext,          "Next"       },
    { kPrev,          "Prev"       },
    { kPower,         "Power"      },
    { kChanUp,        "Channel+"   },
    { kChanDn,        "Channel-"   },
    { kChanPrev,      "PrevChannel"},
    { kVolUp,         "Volume+"    },
    { kVolDn,         "Volume-"    },
    { kMute,          "Mute"       },
    { kAudio,         "Audio"      },
    { kSubtitles,     "Subtitles"  },
    { kSchedule,      "Schedule"   },
    { kChannels,      "Channels"   },
    { kTimers,        "Timers"     },
    { kRecordings,    "Recordings" },
    { kSetup,         "Setup"      },
    { kCommands,      "Commands"   },
    { kUser1,         "User1"      },
    { kUser2,         "User2"      },
    { kUser3,         "User3"      },
    { kUser4,         "User4"      },
    { kUser5,         "User5"      },
    { kUser6,         "User6"      },
    { kUser7,         "User7"      },
    { kUser8,         "User8"      },
    { kUser9,         "User9"      },
    { kNone,          NULL         },
};

cMouseMateSetupPage::cMouseMateSetupPage()
{
    context = kContextNormal;
    contextOld = kContextNormal;
    sensitivity = MouseMateSetup.sensitivity;
    sensitivityX = MouseMateSetup.sensitivityX;
    sensitivityY = MouseMateSetup.sensitivityY;
    doubleClickTime = MouseMateSetup.doubleClickTime;
    toggleClickTime = MouseMateSetup.toggleClickTime;
    led=MouseMateSetup.led;
    autotoggleoff=MouseMateSetup.autotoggleoff;
    extrapushedturn=MouseMateSetup.extrapushedturn;
    extraturns=MouseMateSetup.extraturns;
    dvdsensitive=MouseMateSetup.dvdsensitive;
    mp3sensitive=MouseMateSetup.mp3sensitive;
    cddasensitive=MouseMateSetup.cddasensitive;
    imagesensitive=MouseMateSetup.imagesensitive;
    alternative=MouseMateSetup.alternative;
    toggleOffTime = MouseMateSetup.toggleOffTime;

    for (numKeys = 0; keyTable[numKeys].name; numKeys++)
    {
        for (int c = 0; c < kContexts; c++)
            for (int e = 0; e < kEvents; e++)
                if (keyTable[numKeys].type == MouseMateSetup.keys[c][e])
                    keys[c][e] = numKeys;
        strKeys[numKeys] = new char[strlen(keyTable[numKeys].name) + 1];
        strcpy(strKeys[numKeys], keyTable[numKeys].name);
    }
    for (int e = 0; e < kEvents; e++)
        setKeys[e] = keys[context][e];
    Setup();
}


void cMouseMateSetupPage::Setup()
{
    int current = Current();
    Clear();
    SetHelp("One4all",NULL,NULL,NULL);

    Add(new cMenuEditIntItem(tr("Sensitivity wheel"), &sensitivity, 1, 10));
    Add(new cMenuEditIntItem(tr("Double click time (ms)"), &doubleClickTime, 100, 1000));

    Add(new cMenuEditBoolItem(tr("Alternative key binding"), &alternative,tr("no"), tr("yes")));
    int nContexts;
    if (alternative)
    {
        nContexts=kContexts;
        Add(new cMenuEditIntItem(tr("Toggle click time (ms)"), &toggleClickTime, 100, 5000));
        Add(new cMenuEditBoolItem(tr("Show toggle with scroll lock led"), &led,tr("no"), tr("yes")));
        Add(new cMenuEditBoolItem(tr("Toggle automatic off"), &autotoggleoff,tr("no"), tr("yes")));
        if (autotoggleoff)
        {
            Add(new cMenuEditIntItem(tr("Toggle off time (ms)"), &toggleOffTime, 1000, 30000));
        }
    }
    else
    {
        nContexts=kContexts/2;
        if (context>nContexts) context=nContexts;
    }

    Add(new cMenuEditBoolItem(tr("Extra pushed turn events"), &extrapushedturn,tr("no"), tr("yes")));
    Add(new cMenuEditBoolItem(tr("Extra x,y turn events"), &extraturns, tr("no"), tr("yes")));

    if (extraturns)
    {
        Add(new cMenuEditIntItem(tr("Sensitivity x"), &sensitivityX, 1, 1000));
        Add(new cMenuEditIntItem(tr("Sensitivity y"), &sensitivityY, 1, 1000));
    }

    Add(new cMenuEditBoolItem(tr("DVD sensitive replay context"), &dvdsensitive,tr("no"), tr("yes")));
    Add(new cMenuEditBoolItem(tr("MP3 sensitive replay context"), &mp3sensitive,tr("no"), tr("yes")));
    Add(new cMenuEditBoolItem(tr("CDDA sensitive replay context"), &cddasensitive,tr("no"), tr("yes")));
    Add(new cMenuEditBoolItem(tr("IMAGE sensitive replay context"), &imagesensitive,tr("no"), tr("yes")));

    Add(itemContext = new cMenuEditStraItem(tr("Set keys for context"), &context, nContexts, strContexts));
    Add(new cMenuEditStraItem(tr("Turn wheel left"), &setKeys[kEventTurnWheelLeft], numKeys, strKeys));
    Add(new cMenuEditStraItem(tr("Turn wheel right"), &setKeys[kEventTurnWheelRight], numKeys, strKeys));

    if (extraturns)
    {
        Add(new cMenuEditStraItem(tr("Turn x axis left"), &setKeys[kEventTurnXLeft], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Turn x axis right"), &setKeys[kEventTurnXRight], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Turn y axis left"), &setKeys[kEventTurnYLeft], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Turn y axis right"), &setKeys[kEventTurnYRight], numKeys, strKeys));
    }

    Add(new cMenuEditStraItem(tr("Left click"), &setKeys[kEventLeftClick], numKeys, strKeys));
    Add(new cMenuEditStraItem(tr("Left double-click"), &setKeys[kEventLeftDoubleClick], numKeys, strKeys));

    Add(new cMenuEditStraItem(tr("Middle click"), &setKeys[kEventMiddleClick], numKeys, strKeys));
    Add(new cMenuEditStraItem(tr("Middle double-click"), &setKeys[kEventMiddleDoubleClick], numKeys, strKeys));
    Add(new cMenuEditStraItem(tr("Middle pushed turn left"), &setKeys[kEventMiddlePushedTurnLeft], numKeys, strKeys));
    Add(new cMenuEditStraItem(tr("Middle pushed turn right"), &setKeys[kEventMiddlePushedTurnRight], numKeys, strKeys));

    Add(new cMenuEditStraItem(tr("Right click"), &setKeys[kEventRightClick], numKeys, strKeys));
    Add(new cMenuEditStraItem(tr("Right double-click"), &setKeys[kEventRightDoubleClick], numKeys, strKeys));

    if (MouseMate->IsButtonUsed(BTN_SIDE))
    {
        Add(new cMenuEditStraItem(tr("Side click"), &setKeys[kEventSideClick], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Side double-click"), &setKeys[kEventSideDoubleClick], numKeys, strKeys));

    }
    if (MouseMate->IsButtonUsed(BTN_EXTRA))
    {
        Add(new cMenuEditStraItem(tr("Extra click"), &setKeys[kEventExtraClick], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Extra double-click"), &setKeys[kEventExtraDoubleClick], numKeys, strKeys));

    }
    if (MouseMate->IsButtonUsed(BTN_FORWARD))
    {
        Add(new cMenuEditStraItem(tr("Forward click"), &setKeys[kEventForwardClick], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Forward double-click"), &setKeys[kEventForwardDoubleClick], numKeys, strKeys));
    }
    if (MouseMate->IsButtonUsed(BTN_BACK))
    {
        Add(new cMenuEditStraItem(tr("Back click"), &setKeys[kEventBackClick], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Back double-click"), &setKeys[kEventBackDoubleClick], numKeys, strKeys));
    }
    if (MouseMate->IsButtonUsed(BTN_TASK))
    {
        Add(new cMenuEditStraItem(tr("Task click"), &setKeys[kEventTaskClick], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Task double-click"), &setKeys[kEventTaskDoubleClick], numKeys, strKeys));
    }

    if (extrapushedturn)
    {
        Add(new cMenuEditStraItem(tr("Left pushed turn left"), &setKeys[kEventLeftPushedTurnLeft], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Left pushed turn right"), &setKeys[kEventLeftPushedTurnRight], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Right pushed turn left"), &setKeys[kEventRightPushedTurnLeft], numKeys, strKeys));
        Add(new cMenuEditStraItem(tr("Right pushed turn right"), &setKeys[kEventRightPushedTurnRight], numKeys, strKeys));
        if (MouseMate->IsButtonUsed(BTN_SIDE))
        {
            Add(new cMenuEditStraItem(tr("Side pushed turn left"), &setKeys[kEventSidePushedTurnLeft], numKeys, strKeys));
            Add(new cMenuEditStraItem(tr("Side pushed turn right"), &setKeys[kEventSidePushedTurnRight], numKeys, strKeys));
        }
        if (MouseMate->IsButtonUsed(BTN_EXTRA))
        {
            Add(new cMenuEditStraItem(tr("Extra pushed turn left"), &setKeys[kEventExtraPushedTurnLeft], numKeys, strKeys));
            Add(new cMenuEditStraItem(tr("Extra pushed turn right"), &setKeys[kEventExtraPushedTurnRight], numKeys, strKeys));
        }
        if (MouseMate->IsButtonUsed(BTN_FORWARD))
        {
            Add(new cMenuEditStraItem(tr("Forward pushed turn left"), &setKeys[kEventForwardPushedTurnLeft], numKeys, strKeys));
            Add(new cMenuEditStraItem(tr("Forward pushed turn right"), &setKeys[kEventForwardPushedTurnRight], numKeys, strKeys));
        }

        if (MouseMate->IsButtonUsed(BTN_BACK))
        {
            Add(new cMenuEditStraItem(tr("Back pushed turn left"), &setKeys[kEventBackPushedTurnLeft], numKeys, strKeys));
            Add(new cMenuEditStraItem(tr("Back pushed turn right"), &setKeys[kEventBackPushedTurnRight], numKeys, strKeys));
        }
        if (MouseMate->IsButtonUsed(BTN_TASK))
        {
            Add(new cMenuEditStraItem(tr("Task pushed turn left"), &setKeys[kEventTaskPushedTurnLeft], numKeys, strKeys));
            Add(new cMenuEditStraItem(tr("Task pushed turn right"), &setKeys[kEventTaskPushedTurnRight], numKeys, strKeys));
        }
    }

    SetCurrent(Get(current));
    Display();
}


cMouseMateSetupPage::~cMouseMateSetupPage()
{
    for (int i = 0; i < numKeys; i++)
    {
        delete[] strKeys[i];
    }
}


void cMouseMateSetupPage::Store()
{
    SetupStore("Sensitivity", MouseMateSetup.sensitivity = sensitivity);
    SetupStore("SensitivityX", MouseMateSetup.sensitivityX = sensitivityX);
    SetupStore("SensitivityY", MouseMateSetup.sensitivityY = sensitivityY);
    SetupStore("DoubleClickTime", MouseMateSetup.doubleClickTime = doubleClickTime);
    SetupStore("ToggleClickTime", MouseMateSetup.toggleClickTime = toggleClickTime);
    SetupStore("LED", MouseMateSetup.led = led);
    SetupStore("AutoToggleOff", MouseMateSetup.autotoggleoff = autotoggleoff);
    SetupStore("ExtraPushedTurn", MouseMateSetup.extrapushedturn = extrapushedturn);
    SetupStore("ExtraTurns", MouseMateSetup.extraturns = extraturns);
    SetupStore("DvdSensitive", MouseMateSetup.dvdsensitive = dvdsensitive);
    SetupStore("Mp3Sensitive", MouseMateSetup.mp3sensitive = mp3sensitive);
    SetupStore("CddaSensitive", MouseMateSetup.cddasensitive = cddasensitive);
    SetupStore("ImageSensitive", MouseMateSetup.imagesensitive = imagesensitive);
    SetupStore("Alternative", MouseMateSetup.alternative = alternative);
    SetupStore("ToggleOffTime", MouseMateSetup.toggleOffTime = toggleOffTime);
    for (int e = 0; e < kEvents; e++)
        keys[contextOld][e] = setKeys[e];
    for (int c = 0; c < kContexts; c++)
    {
        for (int e = 0; e < kEvents; e++)
        {
            char str[256];
            MouseMateSetup.keys[c][e] = keyTable[keys[c][e]].type;
            sprintf(str, "Key%s%s", strContexts[c], strEvents[e]);
            SetupStore(str, MouseMateSetup.keys[c][e]);
        }
    }

}


eOSState cMouseMateSetupPage::ProcessKey(eKeys key)
{
    int oldautotoggleoff = autotoggleoff;
    int oldextrapushedturn = extrapushedturn;
    int oldalternative = alternative;
    int oldextraturns = extraturns;

    eOSState state = cMenuSetupPage::ProcessKey(key);

    //calculating number of used events
    // 4 = turn left, turn right, middle pushed turv left, middle pushed turn right
    // extraturns*4 = turn x left, turn x right, turn y left, turn y right
    // Get_btnCount() * 2 = click & doubleclick for each used button
    // extrapushedturn *2* (MouseMate->Get_btnCount()-1) = extra pushed turns (0,1) left & right (2)
    //                                                     for each used button except middle (-1)
    short int eventCount=4 + extraturns * 4
        + MouseMate->Get_btnCount() * 2
        + extrapushedturn *2* (MouseMate->Get_btnCount()-1);

    if (state == osContinue)
    {
        if (key == kLeft || key == kRight)
        {
            cOsdItem * item = Get(Current());
            if (item == itemContext)
            {
                for (int e = 0; e < kEvents; e++)
                {
		    		keys[contextOld][e] = setKeys[e];
                    setKeys[e] = keys[context][e];
                }
                for (int e = 0; e < eventCount; e++)
                {
                   item = Get(Current() + e + 1);
                   item->Set();
				}	

                contextOld = context;
                Display();
            }
        }
    }
    
	// One4All
    if (key == kRed)
		if (!Interface->Confirm(tr("Set current key binding for all contexts?"), 5, true))
	    {
			for (int i=0 ;i < kContexts;i++)
				for (int e = 0; e < kEvents; e++)
				keys[i][e]=setKeys[e];
		}
    
    if (key != kNone && ( (autotoggleoff != oldautotoggleoff)
        | (oldextrapushedturn != extrapushedturn)
        | (oldalternative != alternative)
        | (oldextraturns != extraturns) ))
    {
        Setup();
    }

    return state;
}
