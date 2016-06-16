#ifndef MAIN_H
#define MAIN_H

extern "C"
#define __STDC_CONSTANT_MACROS
#define __STDC_FORMAT_MACROS
{
}
#define FILE_APPDIR			"/alarm"
#define EVENTLOG_ENABLE

#ifndef EVENTLOG_ENABLE
#define EventLog //
#else
#include "debug.h"
#endif

#endif
