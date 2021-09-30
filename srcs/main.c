#include "woody_woodpacker.h"

int		mmap_file(t_file *file, char *file_name)
{
	int			fd;
	char		*original_mapped_file;
	struct stat	stat_buf;

	file->mapped_file = NULL;
	fd = open(file_name, O_RDONLY);
	if (fd < 0 || fstat(fd, &stat_buf) < 0)
	{
		fprintf(stderr, "%sError: can't open file: \"%s\"%s\n", C_ERR, file_name, C_RES);
		return ERROR;
	}
	if ((stat_buf.st_mode & __S_IFMT) != __S_IFREG)
	{
		fprintf(stderr, "%sError: Invalid file%s\n", C_ERR, C_RES);
		return ERROR;
	}
	file->size = stat_buf.st_size;
	if (!file->size)
	{
		fprintf(stderr, "%sError: empty file: \"%s\"%s\n", C_ERR, file_name, C_RES);
		return ERROR;
	}
	original_mapped_file = (char *)mmap(NULL, stat_buf.st_size, PROT_READ | PROT_WRITE,
									 MAP_PRIVATE, fd, 0);
	file->mapped_file = (char *)mmap(NULL, stat_buf.st_size, PROT_READ | PROT_WRITE,
									 MAP_PRIVATE | MAP_ANON, -1, 0);
	memcpy(file->mapped_file, original_mapped_file, stat_buf.st_size);
	if (munmap(original_mapped_file, stat_buf.st_size) == -1)
		fprintf(stderr, "%sWarning: munmap error... program still running with leaks%s\n", C_WARN, C_RES);
	if (file->mapped_file == MAP_FAILED)
	{
		fprintf(stderr, "%sError: can't map file: \"%s\"%s\n", C_ERR, file_name, C_RES);
		return ERROR;
	}
	close(fd);
	return SUCCESS;
}

void	print_woody(t_file *file)
{
	int			fd;
	char		*zeros;
	char		*end_file;
	size_t		end_file_size;

	fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		fprintf(stderr, "can't open file woody for writing\n");
		exit(EXIT_FAILURE);
	}
	if (write(fd, file->ehdr, file->pld_offset) == -1)
      	fprintf(stderr, "Error 12: %s\n", strerror(errno));
	if (file->bss_zero_fill > 0)
	{
		zeros = malloc(file->bss_zero_fill);
		bzero(zeros, file->bss_zero_fill);
		if (write(fd, zeros, file->bss_zero_fill) == -1)
      		fprintf(stderr, "Error 3: %s\n", strerror(errno));
		free(zeros);
	}
	if (write(fd, file->payload, file->pld_len) == -1)
      	fprintf(stderr, "Error 5: %s\n", strerror(errno));
	end_file = (char *)file->ehdr + file->pld_offset;
	end_file_size = file->size - file->pld_offset;
	if (file->cave_found == TRUE)
	{
		end_file += file->pld_len;
		end_file_size -= file->pld_len;
	}
	if (write(fd, end_file, end_file_size) == -1)
		fprintf(stderr, "Error 102: %s\n", strerror(errno));
}

int 	main(int argc, char **argv)
{
	t_file	file;
	
	char	*file_name;

	if (!(file_name = parse_options(&file, argv)))
	{
		fprintf(stderr, USAGE);
		exit(EXIT_FAILURE);
	}
	if (mmap_file(&file, argv[1]) == ERROR)
		exit(EXIT_FAILURE);
	if (parse_elf(&file) == ERROR)
		exit(EXIT_FAILURE);
	if (file.encrypt(&file) == ERROR)
		exit(EXIT_FAILURE);
	setup_payload(&file);
	print_woody(&file);
	free(file.payload);
	munmap(file.mapped_file, file.size);
	return (argc ^ argc);
}