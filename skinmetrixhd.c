/*
 * skinmetrixhd.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include <getopt.h>
#include <vdr/plugin.h>

#if defined(APIVERSNUM) && APIVERSNUM < 10734
#error "VDR-1.7.34 API version or greater is required!"
#endif

#include "metrixhd.h"

static const char *VERSION        = "0.0.2";
static const char *DESCRIPTION    = "skin metrixhd";

class cPluginFlat : public cPlugin {
    private:
        cMetrixHD *metrixhd;
    public:
        cPluginFlat(void);
        virtual ~cPluginFlat();
        virtual const char *Version(void) { return VERSION; }
        virtual const char *Description(void) { return DESCRIPTION; }
        virtual const char *CommandLineHelp(void);
        virtual bool ProcessArgs(int argc, char *argv[]);
        virtual bool Initialize(void);
        virtual bool Start(void);
        virtual void Stop(void);
        virtual void Housekeeping(void);
        virtual void MainThreadHook(void);
        virtual cString Active(void);
        virtual time_t WakeupTime(void);
        virtual const char *MainMenuEntry(void) {return NULL;}
        virtual cOsdObject *MainMenuAction(void);
        virtual cMenuSetupPage *SetupMenu(void);
        virtual bool SetupParse(const char *Name, const char *Value);
        virtual bool Service(const char *Id, void *Data = NULL);
        virtual const char **SVDRPHelpPages(void);
        virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
};

cPluginFlat::cPluginFlat(void) {
    metrixhd = NULL;
}

cPluginFlat::~cPluginFlat() {
}

const char *cPluginFlat::CommandLineHelp(void) {
    return ""; 
}

bool cPluginFlat::ProcessArgs(int argc, char *argv[]) {
    // Implement command line argument processing here if applicable.
    return true;
}

bool cPluginFlat::Initialize(void) {
    return true;
}

bool cPluginFlat::Start(void) {
    if (!cOsdProvider::SupportsTrueColor()) {
        esyslog("skinmetrixhd: No TrueColor OSD found! Aborting!");
        return false;
    } else
        dsyslog("skinmetrixhd: TrueColor OSD found");
    metrixhd = new cMetrixHD;
    return metrixhd;
}

void cPluginFlat::Stop(void) {
}

void cPluginFlat::Housekeeping(void) {
}

void cPluginFlat::MainThreadHook(void) {
}

cString cPluginFlat::Active(void) {
    return NULL;
}

time_t cPluginFlat::WakeupTime(void) {
    return 0;
}

cOsdObject *cPluginFlat::MainMenuAction(void) {
    return NULL;
}

cMenuSetupPage *cPluginFlat::SetupMenu(void) {
    return NULL;
}

bool cPluginFlat::SetupParse(const char *Name, const char *Value) {
    return true;
}

bool cPluginFlat::Service(const char *Id, void *Data) {
    return false;
}

const char **cPluginFlat::SVDRPHelpPages(void) {
    return NULL;
}

cString cPluginFlat::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode) {
    return NULL;
}

VDRPLUGINCREATOR(cPluginFlat); // Don't touch this!
