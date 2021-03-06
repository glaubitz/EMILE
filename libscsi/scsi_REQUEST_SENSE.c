/*
 *
 * (c) 2004-2006 Laurent Vivier <Laurent@Vivier.EU>
 *
 */

#include <sys/types.h>

#include <scsi/scsi.h>

#include <macos/types.h>
#include <macos/errors.h>

#include "libscsi.h"

int scsi_REQUEST_SENSE(int target, void *buffer, size_t count)
{
	unsigned char	cdb[6];
	TIB_t	tib[2];

	cdb[0] = REQUEST_SENSE;
	cdb[1] = 0;
	cdb[2] = 0;
	cdb[3] = 0;
	cdb[4] = count;
	cdb[5] = 0;

	tib[0].opcode = op_no_inc;
	tib[0].param1 = (int)buffer;
	tib[0].param2 = count;
	tib[1].opcode = op_stop;
	tib[1].param1 = 0;
	tib[1].param2 = 0;

	return scsi_command(target, cdb, 6, tib);
}
