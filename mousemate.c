/*
 * mousemate plugin for VDR
 *
 * mousemate.c - main plugin class
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#include <getopt.h>
#include <vdr/plugin.h>

#include "MouseMate.h"
#include "setup.h"


static const char *VERSION        = "0.1.7";
static const char *DESCRIPTION    =  trNOOP("Controls VDR via a mouse");
static const char *MAINMENUENTRY  = NULL;

class cPluginMousemate : public cPlugin
{
    private:
        // Add any member variables or functions you may need here.
    public:
        cPluginMousemate(void);
        virtual ~cPluginMousemate();
        virtual const char *Version(void) { return VERSION; }
        virtual const char *Description(void) { return tr(DESCRIPTION); }
        virtual const char *CommandLineHelp(void);
        virtual bool ProcessArgs(int argc, char *argv[]);
        virtual bool Initialize(void);
        virtual bool Start(void);
        virtual void Housekeeping(void);
        virtual const char *MainMenuEntry(void) { return MAINMENUENTRY; }
        virtual cOsdObject *MainMenuAction(void);
        virtual cMenuSetupPage *SetupMenu(void);
        virtual bool SetupParse(const char *Name, const char *Value);
};

cPluginMousemate::cPluginMousemate(void)
{
    // Initialize any member variables here.
    // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
    // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}


cPluginMousemate::~cPluginMousemate()
{
    // Clean up after yourself!
    delete MouseMate;
}


const char *cPluginMousemate::CommandLineHelp(void)
{
    // Return a string that describes all known command line options.
    return "  -d DEV,   --device=DEV  use DEV as mouse device (default /dev/input/event1)\n"
        "  -c DEV,   --cdevice=DEV use DEV as console device (default /dev/input/event0)\n";
}


bool cPluginMousemate::ProcessArgs(int argc, char *argv[])
{
    // Implement command line argument processing here if applicable.
    static struct option long_options[] =
    {
        { "device", required_argument, NULL, 'd' },
        { "cdevice", required_argument, NULL, 'c' },
        { NULL }
    };
    int c;

    strcpy(MouseMateSetup.device, "/dev/input/event1");
    strcpy(MouseMateSetup.cdevice, "/dev/input/event0");
    while ((c = getopt_long(argc, argv, "d:c:", long_options, NULL)) != -1)
    {
        switch (c)
        {
            case 'd':
                strcpy(MouseMateSetup.device, optarg);
                break;
            case 'c':
                strcpy(MouseMateSetup.cdevice, optarg);
                break;
            default:
                return false;
        }
    }
    return true;
}


bool cPluginMousemate::Initialize(void)
{
    // Initialize any background activities the plugin shall perform.
    return true;
}


bool cPluginMousemate::Start(void)
{
    // Start any background activities the plugin shall perform.
    MouseMate = new cMouseMate();
    return true;
}


void cPluginMousemate::Housekeeping(void)
{
    // Perform any cleanup or other regular tasks.
}


cOsdObject *cPluginMousemate::MainMenuAction(void)
{
    // Perform the action when selected from the main VDR menu.
    return NULL;
}


cMenuSetupPage *cPluginMousemate::SetupMenu(void)
{
    // Return a setup menu in case the plugin supports one.
    return new cMouseMateSetupPage();
}


bool cPluginMousemate::SetupParse(const char *Name, const char *Value)
{
    return MouseMateSetup.Parse(Name, Value);
}


VDRPLUGINCREATOR(cPluginMousemate);               // Don't touch this!
