static __attribute__((used)) char* rcsid = "$CVSHeader$";
/*
 *
 * (c) 2004 Laurent Vivier <LaurentVivier@wanadoo.fr>
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#include "libemile.h"

#define MAJOR_SD	8	/* SCSI disks */

int emile_scsi_get_dev(char* dev_name, int fd)
{
	struct stat st;
	int ret;
	int dev;
	int major;
	int minor;

	ret = fstat(fd, &st);
	if (ret == -1)
		return -1;

	dev = S_ISREG(st.st_mode) ? st.st_dev : st.st_rdev;

	major = (dev >> 8) & 0x0F;	/* major number = driver id */
	minor = dev & 0xFF;		/* minor number = disk id */

	switch(major)
	{
	case MAJOR_SD:	/* SCSI disks */
		sprintf(dev_name, "/dev/sd%c%d", 'a' + (minor >> 4),
				minor & 0x0F);
		break;
	default:
		fprintf(stderr, "Unknown device major number %d\n", major);
		return -1;
	}

	return 0;
}