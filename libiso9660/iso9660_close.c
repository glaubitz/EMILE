/*
 *
 * (c) 2005 Laurent Vivier <LaurentVivier@wanadoo.fr>
 *
 */

#include <stdlib.h>

#include "libiso9660.h"

void iso9660_close(iso9660_FILE *file)
{
	free(file);
}