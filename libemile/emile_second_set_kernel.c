static __attribute__((used)) char* rcsid = "$CVSHeader$";
/*
 *
 * (c) 2004 Laurent Vivier <LaurentVivier@wanadoo.fr>
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libemile.h"
#include "emile.h"
#include "bootblock.h"

int emile_second_set_kernel(int fd, char *kernel_image, 
			   u_int32_t kernel_offset, char* ramdisk)
{
	emile_l2_header_t header;
	int ret;
	off_t location;

	location = lseek(fd, 0, SEEK_CUR);
	if (ret == -1)
		return EEMILE_CANNOT_READ_SECOND;

	ret = read(fd, &header, sizeof(header));
	if (ret != sizeof(header))
		return EEMILE_CANNOT_READ_SECOND;

	if (kernel_image != NULL)
	{
		write_long(&header.kernel_image_offset, kernel_offset);
		write_long(&header.kernel_image_size, 
					emile_file_get_size(kernel_image));
	}

	if (ramdisk == NULL)
	{
		write_long(&header.ramdisk_offset, 0);
		write_long(&header.ramdisk_size, 0);
	}
	else
	{
		write_long(&header.ramdisk_offset, 
			   read_long(&header.kernel_image_offset) + 
			   read_long(&header.kernel_image_size));
		write_long(&header.ramdisk_size, emile_file_get_size(ramdisk));
	}

	ret = lseek(fd, location, SEEK_SET);
	if (ret == -1)
		return EEMILE_CANNOT_WRITE_SECOND;

	ret = write(fd, &header, sizeof(header));
	if (ret != sizeof(header))
		return EEMILE_CANNOT_WRITE_SECOND;

	return 0;
}