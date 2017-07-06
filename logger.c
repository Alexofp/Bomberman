#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

struct Logger* Logger_get()
{
    static struct Logger logger;
    return &logger;
}

void Logger_init()
{
    struct Logger* self = Logger_get();
    self->fp = fopen("log.txt", "w");
    self->sem = Semaphore_init(1);
}

void Logger_print(char* filename, int line, char *fmt,...)
{
    struct Logger* self = Logger_get();
    Semaphore_grab(self->sem);
    fprintf(self->fp,"[%s][line: %d] ",filename,line);

    va_list         list;
    char            *p, *r;
    int             e;
    va_start( list, fmt );
 
    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
            fputc( *p,self->fp );
        }
        else
        {
            switch ( *++p )
            {
                /* string */
            case 's':
            {
                r = va_arg( list, char * );
 
                fprintf(self->fp,"%s", r);
                continue;
            }
 
            /* integer */
            case 'd':
            {
                e = va_arg( list, int );
 
                fprintf(self->fp,"%d", e);
                continue;
            }
 
            default:
                fputc( *p, self->fp );
            }
        }
    }
    va_end( list );
    fputc( '\n', self->fp );
    fflush(self->fp);
    Semaphore_release(self->sem);
}
