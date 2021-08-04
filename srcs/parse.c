#include "woody_woodpacker.h"

int     parse_elf(t_file *file)
{
    unsigned char	*e_ident;

    e_ident = (unsigned char *)file->mapped_file;

    // Magic numbers
    if (strncmp((char *)e_ident, ELFMAG, SELFMAG))
    {
        fprintf(stderr, "Wrong magic numbers, not an ELF file\n");
        return ERROR;
    }

    // File class byte
    if (e_ident[EI_CLASS] == ELFCLASS64)
    {
        file->ehdr = (Elf64_Ehdr *)file->mapped_file;
        file->phdr = (Elf64_Phdr *)(file->mapped_file + file->ehdr->e_phoff);
        file->shdr = (Elf64_Shdr *)(file->mapped_file + file->ehdr->e_shoff);
    }
    else if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        fprintf(stderr, "32-bit objects are not supported\n");
        return ERROR;
    }
    else
    {
        fprintf(stderr, "Invalid file class byt index: %hhu\n", e_ident[EI_CLASS]);
        return ERROR;
    }
    parse_bytecode(file);
  
    fprintf(stdout, "section header: %lx\n", file->ehdr->e_shoff + file->ehdr->e_shnum * sizeof(Elf64_Shdr));

    print_64ehdr(file);
    print_64phdr(file);
    print_64shdr(file);
    return SUCCESS;
}
