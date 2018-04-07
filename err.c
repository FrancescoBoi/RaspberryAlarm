#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "err.h"
/*
* Fatal error unrelated to a system call.
* Error code passed as explicit parameter.
* Print a message and terminate.
*/
void err_exit(int error, const char *fmt, ...)
{
    va_list  ap;
    va_start (ap, fmt);
    err_doit(1,error, fmt, ap);
    va_end(ap);
    exit(1);
}

/*
* Print a message and return to caller.
* Caller specifies "errnoflag".
*/
void err_doit(int errnoflag, int error,
      const char *fmt, va_list ap)
{
    char buf[MAXLINE];
    vsnprintf(buf, MAXLINE-1, fmt, ap);
    if(errnoflag)
        snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1,
                         ": %s", strerror(error));
    strcat(buf, "\n");
    fflush(stdout); //in case stdout and stderror are the same
    fputs(buf, stderr);
    fflush(NULL);

}
