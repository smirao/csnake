#ifndef SYS_INFO_H_   
#define SYS_INFO_H_

#include <fcntl.h> 
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

struct winsize get_terminal_dimensions();

#endif 