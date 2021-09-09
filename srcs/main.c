#include "woody_woodpacker.h"

int		mmap_file(t_file *file, char *file_name)
{
	int			fd;
	struct stat	stat_buf;

	file->mapped_file = NULL;
	fd = open(file_name, O_RDONLY);
	if (fd < 0 || fstat(fd, &stat_buf) < 0)
	{
		fprintf(stderr, "error: woody_woodpacker: can't open file: %s\n", file_name);
		return (ERROR);
	}
	file->size = stat_buf.st_size;
	file->mapped_file = (char *)mmap(NULL, stat_buf.st_size, PROT_READ | PROT_WRITE,
									 MAP_PRIVATE, fd, 0);
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
	
	file->ehdr->e_shnum += 1;
	fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		fprintf(stderr, "can't open file woody for writing\n");
		exit(EXIT_FAILURE);
	}

	// fprintf(stderr, "hello %d, %p, %zu\n", fd, (char *)file->ehdr, sizeof(elf_ehdr));
	if (write(fd, (char *)file->ehdr, file->size) == -1)
		printf("ah\n"), exit(EXIT_FAILURE);

	
	// care padding !!
	// if (write(fd, (char *)file->phdr, ) == -1)
		// printf("ah\n"), exit(EXIT_FAILURE);
	
	
	// if (write(fd, (char *)file->phdr + file->ehdr->e_phentsize * file->ehdr->e_phnum, file->b_filesz) == -1)
	// 	printf("ah\n"), exit(EXIT_FAILURE);
	

	// if (write(fd, (char *)file->shdr, file->ehdr->e_shentsize * file->ehdr->e_shnum) == -1)
	// 	printf("ah\n"), exit(EXIT_FAILURE);
	
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
	if (write(fd, (char *)(&test), sizeof(Elf64_Shdr)) == -1)
		printf("ah\n"), exit(EXIT_FAILURE);
	
	return 1;
}

int 	main(int argc, char **argv)
{
	t_file	file;

	if (argc != 2)
	{
		fprintf(stderr, USAGE);
		exit(EXIT_FAILURE);
	}
	if (mmap_file(&file, argv[1]) == ERROR)
		exit(EXIT_FAILURE);
	if (parse_elf(&file) == ERROR)
		exit(EXIT_FAILURE);
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
	// blowfish_run("123456789");
	blowfish_run(NULL);
	return (0);
}