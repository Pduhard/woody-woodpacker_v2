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
		fprintf(stderr, "error: woody_woodpacker: can't open file: %s\n", file_name);
		return (ERROR);
	}
	file->size = stat_buf.st_size;
	original_mapped_file = (char *)mmap(NULL, stat_buf.st_size, PROT_READ | PROT_WRITE,
									 MAP_PRIVATE, fd, 0);
	file->mapped_file = (char *)mmap(NULL, stat_buf.st_size, PROT_READ | PROT_WRITE,
									 MAP_PRIVATE | MAP_ANON, -1, 0);
	memcpy(file->mapped_file, original_mapped_file, stat_buf.st_size);
	if (!file->size)
	{
		fprintf(stderr, "error: woody_woodpacker: empty file: %s\n", file_name);
		return (ERROR);
	}
	if (file->mapped_file == MAP_FAILED)
	{
		fprintf(stderr, "error: woody_woodpcker: can't map file: %s\n", file_name);
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}


// int		print_woody(t_file *file)
// // old version no sections and not working
// {
// 	int			fd;
// 	int			note_count;
// 	elf_phdr	*phdr;
// 	elf_phdr	*last_pt_load;
// 	char		*padding = NULL;
	
// 	last_pt_load = get_last_pt_load(file);
// 	fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, 0777);

// 	if (write(fd, (char *)file->ehdr, sizeof(elf_ehdr)) == -1)
// 		printf("ah\n"), exit(EXIT_FAILURE);
// 	phdr = file->phdr;
// 	note_count = 0;
// 	for (int i = 0; i < file->ehdr->e_phnum; i++)
// 	{
// 		if (phdr->p_type == PT_NOTE)
// 			note_count++;
// 		else
// 		{
// 			if (phdr->p_type == PT_LOAD)
// 				phdr->p_flags = 7;
// 			if (write(fd, (char *)phdr, file->ehdr->e_phentsize) == -1)
// 				printf("ah\n"), exit(EXIT_FAILURE);
// 			if (phdr == last_pt_load)
// 				if (write(fd, (char *)(&file->payload_phdr), file->ehdr->e_phentsize) == -1)
// 					printf("ah\n"), exit(EXIT_FAILURE);
// 		}
// 		phdr++;
// 	}
// 	if (note_count < 1)
// 		fprintf(stderr, "flute pas de notes \n"), exit(EXIT_FAILURE);
// 	else if (note_count > 1)
// 	{
// 		padding = calloc((note_count - 1), file->ehdr->e_phentsize);
// 		if (write(fd, padding, (note_count - 1) * file->ehdr->e_phentsize) == -1)
// 			printf("ah\n"), exit(EXIT_FAILURE);
// 	}
// 	if (write(fd, file->bytecode, last_pt_load->p_offset + last_pt_load->p_filesz
// 					  - file->ehdr->e_phnum * file->ehdr->e_phentsize
// 					  - sizeof(elf_ehdr)) == -1)
// 		printf("ah\n"), exit(EXIT_FAILURE);
// 	if (padding)
// 		free(padding);
// 	padding = calloc(file->payload_phdr.p_offset - (last_pt_load->p_offset + last_pt_load->p_filesz), 1);
// 	if (write(fd, padding, file->payload_phdr.p_offset - (last_pt_load->p_offset + last_pt_load->p_filesz)) == -1
// 		|| write(fd, file->payload, file->payload_filesz) == -1)
// 		printf("ah\n"), exit(EXIT_FAILURE);
// 	free(padding);
// 	padding = calloc(4096, 1);
// 	if (write(fd, padding, 4096) == -1)
// 		printf("ah\n"), exit(EXIT_FAILURE);
// 	free(padding);
// 	return 1;
// }

int		print_woody(t_file *file)
{
	int			fd;
	// char *test_wr;	
	file->ehdr->e_shnum += 1;
	
	fprintf(stderr, "file->ehdr %p %lx\n", file->ehdr, file->size);
	
	// fprintf(stderr, "hello %d, %p, %zu\n", fd, (char *)file->ehdr, sizeof(elf_shdr));

	// test_wr = malloc(file->size + 64);

	fprintf(stderr, "hallo\n");
	// memcpy(test_wr, file->ehdr, file->size);

	fprintf(stderr, "hallo 2 \n");
	Elf64_Shdr test = (Elf64_Shdr){
		0,
		SHT_PROGBITS,
		SHF_WRITE | SHF_EXECINSTR | SHF_ALLOC,
		file->payload_vaddr,
		file->payload_offset,
		file->payload_filesz,
		0,
		0,
		16,
		0		
	};

	// memcpy(test_wr + file->size, &test, 64);

	// fprintf(stderr, "hallo 3 \n");
	// munmap(file->ehdr, file->size);

	if (file->cave_found == FALSE)
		file->ehdr->e_shoff += file->payload_filesz;
	fprintf(stderr, "new sh off %lx \n", file->ehdr->e_shoff);
	fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		fprintf(stderr, "can't open file woody for writing\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "hallo 4 \n");


	fprintf(stderr, "hello %d, %p, %zu\n", fd, (char *)file->ehdr, sizeof(elf_shdr));
	if (file->payload_offset > file->size)
		printf("heyy\n");
	else
		printf("ogggg %lx %lx\n", file->payload_offset, file->size);
	if (write(fd, file->ehdr, file->payload_offset > file->size ? file->size : file->payload_offset) == -1)
	// if (write(fd, test_wr, file->size + 64) == -1)
      	printf("Error: %s\n", strerror(errno));
		// printf("ah 3\n"), exit(EXIT_FAILURE);

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
	
	if (write(fd, (char *)(&test), sizeof(Elf64_Shdr)) == -1)
		printf("ah 4\n"), exit(EXIT_FAILURE);
	
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
	int i;
	char	*file_name;

	file->encryption_key = NULL;
	file->encrypt = NULL;
	i = 1;
	while (argv[i])
	{
		if (!strcmp("-k", argv[i]) || !strcmp("--key", argv[i]))
		{
			if (!(file->encryption_key = parse_key(argv[i + 1])))
				return NULL;
			i++;
		}
		else if (!strcmp("-e", argv[i]) || !strcmp("--encryption", argv[i]))
		{
			if (!(file->encrypt = parse_encryption_algorithm(argv[i + 1])))
				return NULL;
			i++;
		}
		else
			file_name = argv[i];
		i++;
	}
	if (!file->encrypt)
		file->encrypt = parse_encryption_algorithm(DEFAULT_ENCRYPTION_ALGORITHM);
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
		fprintf(stderr, "exit(1)\n"),exit(EXIT_FAILURE);
	}
	if (mmap_file(&file, argv[1]) == ERROR)
		fprintf(stderr, "exit(2)\n"),exit(EXIT_FAILURE);
	if (parse_elf(&file) == ERROR)
		fprintf(stderr, "exit(3)\n"),exit(EXIT_FAILURE);
	fprintf(stderr, "encrypt ok\n");
	if (file.encrypt(&file) == ERROR)
		fprintf(stderr, "exit(4)\n"),exit(EXIT_FAILURE);
	if (!setup_payload(&file))
	{
		fprintf(stderr, "Doesn't work need to insert end update offsets\n");
		exit(0);
	}
	else
	{
		// update_ehdr(&file);
		print_woody(&file);
		fprintf(stderr, "%x\n", file.ehdr->e_flags);

	}
	// blowfish_run(NULL);
	// fprintf(stderr, "bbp call rip %d %x\n ", bbp_call_rip, bbp_call_rip);
	// blowfish_run("123456789");
	// fprintf(stderr, "%lf %lf\n ", 1.2, test_get_floating_part(1.2));
	return (0);
}