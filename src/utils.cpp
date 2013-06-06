#include "utils.h"

QTime msecToQTime(uint msecs)
{
    const int hours = msecs/(1000*60*60);
    const int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    const int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    return QTime(hours,minutes,seconds);
}

