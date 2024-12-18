#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

long int get_epoch_millis()
{
    struct timespec tv;

    if(clock_gettime(CLOCK_REALTIME, &tv))
    {
        perror("error clock_gettime\n");
    }

    char time_str[32];

    sprintf(time_str, "%ld.%.9ld", tv.tv_sec, tv.tv_nsec);

    double epoch = round(atof(time_str)*1e3);

    return (long int) epoch;
}

void delay(int milli_seconds)
{
    long int start_time = get_epoch_millis();
    while (get_epoch_millis() < start_time + milli_seconds){};
}
