#include <predef.h>
#include <ctype.h>
#include <startnet.h>
#include <dhcpclient.h>
#include <smarttrap.h>
#include <taskmon.h>
#include <NetworkDebug.h>
#include <constants.h>
#include <init.h>

#include "introspec.h"

const char *AppName = "Real-time Logging Example";

#define FTP_PRIO (MAIN_PRIO - 2)

LOGFILEINFO   // Logs the version of this file

// Object logged once per log cycle
START_INTRO_OBJ(MainLogObject, "Log")
public:
    int_element m_time{"time"};
    float_element m_floatElem{"Float Element"};
    uint16_element m_intElem{"Int Element"};

    char GetCharElem(){ return m_charElem; }
    void SetCharElem(char c) { m_charElem = c; }

private:
    char_element m_charElem{ "Char Element" };
END_INTRO_OBJ;

MainLogObject mainLog;

extern "C" void UserMain(void *pd)
{
    // Basic network initialization
    init();
    int secs = 0;

    // FTP Server for Log Transmission
    InitLogFtp(FTP_PRIO);

    // Start Logging
    bLog = true;
    LogFileVersions();

    printf("Starting our loop in UserMain()\r\n");
    while (1)
    {
        // Update data values that will get logged for 20 seconds
        if (++secs > 20)
        {
            bLog = false;
        }
        else
        {
            printf("Logging at %d secs\r\n", secs);
        }
        mainLog.m_time = secs;
        mainLog.m_floatElem = mainLog.m_floatElem / 2;
        mainLog.m_intElem = mainLog.m_intElem * 2;
        mainLog.SetCharElem(mainLog.GetCharElem() + 1);
        mainLog.Log();

        OSTimeDly(TICKS_PER_SECOND);   // Update main every second
    }
}
