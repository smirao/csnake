#include <fcntl.h> 
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

struct winsize get_terminal_dimensions()
{

  int tty_fd = open("/dev/tty", O_EVTONLY | O_NONBLOCK); // get tty file descriptor

  if (tty_fd == -1)
  {
		fprintf(stderr, "Opening `/dev/tty` failed (%d): %s\n", errno, strerror(errno));
		exit(1);
	}

	struct winsize ws;
	int result = ioctl(tty_fd, TIOCGWINSZ, &ws); // use ioctl sys cal to populate ws struct
	close(tty_fd);

	if (result == -1)
  {
		fprintf(stderr, "Getting the size failed (%d): %s\n", errno, strerror(errno));
		exit(1);
	}

    return ws;
}
