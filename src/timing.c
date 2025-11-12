#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

long int get_epoch_millis()
{
    struct timespec tv;

    if(clock_gettime(CLOCK_REALTIME, &tv)) // populate timespec struct & check for errors
    {
        perror("error clock_gettime\n");
    }

    char time_str[32];

    sprintf(time_str, "%ld.%.9ld", tv.tv_sec, tv.tv_nsec); // save formated time data as string

    double epoch = round(atof(time_str)*1e3); // change string to float and multiply by 1000

    return (long int) epoch; // convert to int and return 
}

void delay(int milli_seconds)
{
    long int start_time = get_epoch_millis();
    while (get_epoch_millis() < start_time + milli_seconds){}; // use while loop for call-less delay 
}
