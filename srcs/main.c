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
		return (ERROR);
	}
	file->size = stat_buf.st_size;
	if (!file->size)
	{
		fprintf(stderr, "%sError: empty file: \"%s\"%s\n", C_ERR, file_name, C_RES);
		return (ERROR);
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
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}

int		print_woody(t_file *file)
{
	int			fd;
	
	// Elf64_Shdr test = (Elf64_Shdr){
	// 	0,
	// 	SHT_PROGBITS,
	// 	SHF_WRITE | SHF_EXECINSTR | SHF_ALLOC,
	// 	file->payload_vaddr,
	// 	file->payload_offset,
	// 	file->payload_filesz,
	// 	0,
	// 	0,
	// 	0,
	// 	0		
	// };

	if (file->cave_found == FALSE)
		file->ehdr->e_shoff += file->payload_filesz + file->bss_zero_fill;
	fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		fprintf(stderr, "can't open file woody for writing\n");
		exit(EXIT_FAILURE);
	}

	if (write(fd, file->ehdr, file->payload_offset) == -1)
      	printf("Error: %s\n", strerror(errno));
		// printf("ah 3\n"), exit(EXIT_FAILURE);
	if (file->bss_zero_fill > 0)
	{
		char *zeros = malloc(file->bss_zero_fill);
		bzero(zeros, file->bss_zero_fill);
		if (write(fd, zeros, file->bss_zero_fill) == -1)
			printf("Error: %s\n", strerror(errno));
		free(zeros);
	}


	if (write(fd, file->payload, file->payload_filesz) == -1)
	// if (write(fd, test_wr, file->size + 64) == -1)
      	printf("Error: %s\n", strerror(errno));
		// printf("ah 3\n"), exit(EXIT_FAILURE);
	
	if (file->cave_found == TRUE)
	{
		if (write(fd, (char *)file->ehdr + file->payload_filesz + file->payload_offset, file->size - file->payload_offset - file->payload_filesz) == -1)
      		printf("Error: %s\n", strerror(errno));

	}
	else
	{
		
		if (write(fd, (char *)file->ehdr + file->payload_offset, file->size - file->payload_offset) == -1)
	      	printf("Error: %s\n", strerror(errno));

	}
		// printf("ah 3\n"), exit(EXIT_FAILURE);

	// care padding !!
	// if (write(fd, (char *)file->phdr, ) == -1)
		// printf("ah\n"), exit(EXIT_FAILURE);
	
	
	// if (write(fd, (char *)file->phdr + file->ehdr->e_phentsize * file->ehdr->e_phnum, file->b_filesz) == -1)
	// 	printf("ah\n"), exit(EXIT_FAILURE);
	

	// if (write(fd, (char *)file->shdr, file->ehdr->e_shentsize * file->ehdr->e_shnum) == -1)
	// 	printf("ah\n"), exit(EXIT_FAILURE);
	
	// if (write(fd, (char *)(&test), sizeof(Elf64_Shdr)) == -1)
	// 	printf("ah 4\n"), exit(EXIT_FAILURE);
	
	return 1;
}

char	*parse_key(char *key)
{
	size_t	key_len;

	if (!key)
		return NULL;
	key_len = strlen(key);
	if (key_len < 8 || key_len > 56)
	{
		fprintf(stderr, "Encryption key length should lie between 8 and 56 bytes\n");
		return NULL;
	}
	return key;
}

int (*parse_encryption_algorithm(char *name))(t_file *file)
{
	char	*algo_names[2] = {
		"blowfish",
		"xor"
	};
	int		(*algo_table[2])(t_file *) = {
		&blowfish_encryption,
		&xor_encryption
	};

	for (int i = 0; i < 2; i++)
	{
		if (!strcmp(algo_names[i], name))
			return algo_table[i];
	}
	return NULL;
}

char 	*parse_options(t_file *file, char **argv)
{
	int		i;
	char	*file_name;

	file->encryption_key = NULL;
	file->encrypt = parse_encryption_algorithm(DEFAULT_ENCRYPTION_ALGORITHM);
	file->verbose = 0;
	i = 1;
	file_name = NULL;
	while (argv[i])
	{
		if (!strcmp("-k", argv[i]) || !strcmp("--key", argv[i]))
		{
			if (!(file->encryption_key = parse_key(argv[++i])))
				return NULL;
		}
		else if (!strcmp("-e", argv[i]) || !strcmp("--encryption", argv[i]))
		{
			if (!(file->encrypt = parse_encryption_algorithm(argv[++i])))
				return NULL;
		}
		else
			file_name = argv[i];
		i++;
	}
	return file_name;
}

int 	main(int argc, char **argv)
{
	t_file	file;
	char	*file_name;

	(void)argc;
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
	if (!setup_payload(&file))
	{
		fprintf(stderr, "%sError: exit failure%s\n", C_ERR, C_RES);
		exit(EXIT_FAILURE);
	}
	else
		print_woody(&file);
	free(file.payload);
	munmap(file.mapped_file, file.size);
	return (0);
}