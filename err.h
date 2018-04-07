#include<stdarg.h>

#define MAXLINE 4096

void err_exit(int error, const char *fmt, ...);

void err_doit(int errnoflag, int error,
      const char *fmt, va_list ap);
