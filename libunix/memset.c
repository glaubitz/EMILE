/*
 *
 * (c) 2004,2005 Laurent Vivier <Laurent@Vivier.EU>
 *
 */

#include <sys/types.h>

void* memset(void* s, int c, size_t n)
{
        int i;
        char *ss = (char*)s;

        for (i=0;i<n;i++) ss[i] = c;
        return s;
}
