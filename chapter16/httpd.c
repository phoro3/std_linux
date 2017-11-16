#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <signal.h>

static void log_exit(char *fmt, ...);
static void* xmalloc(size_t sz);


int main(int argc, char *argv[])
{
}

static void log_exit(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fputc('\n', stderr);
	va_end(ap);
	exit(1);
}

static void* xmalloc(size_t sz)
{
	void *p;

	p = malloc(sz);
	if (!p) {
		log_exit("failed to allocate memory");
	}
	return p;
}
