/*
 *
 * (c) 2005 Laurent Vivier <LaurentVivier@wanadoo.fr>
 *
 */

#include <stdlib.h>

#include "libiso9660.h"

int iso9660_closedir(iso9660_DIR *dir)
{
	if (dir == NULL)
		return -1;

	free(dir);

	return 0;
}