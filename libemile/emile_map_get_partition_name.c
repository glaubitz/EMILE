/*
 *
 * (c) 2004 Laurent Vivier <LaurentVivier@wanadoo.fr>
 *
 */

#include <stdio.h>

#include "partition.h"
#include "libemile.h"

char* emile_map_get_partition_name(emile_map_t *map)
{
	if (!emile_map_partition_is_valid(map))
		return NULL;

	return map->partition.PartName;
}