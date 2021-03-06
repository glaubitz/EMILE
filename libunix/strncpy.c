/*
 *
 * (c) 2004,2005 Laurent Vivier <Laurent@Vivier.EU>
 *
 */

#include <sys/types.h>

char *strncpy (char *__restrict __dest,
                      __const char *__restrict __src, size_t __n)
{
	while ( (*__dest ++ = *__src++) && __n--);

	return __dest;
}
