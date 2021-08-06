#include "woody_woodpacker.h"

elf_shdr    *find_section_to_encrypt(t_file *file)
{
    elf_shdr    *curr;
    // char    *strtab;


    // strtab = file->mapped_file + file->ehdr->e_shstrndx * file
    curr = file->shdr;
    for (Elf32_Half i = 0; i < file->ehdr->e_shnum; i++)
    {
        if (strcmp(file->strtab + curr->sh_name, SECTION_TO_ENCRYPT) == 0)
            return curr;
        curr += 1;
    }
    return NULL;
}

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
        file->strtab = file->mapped_file + (file->shdr + file->ehdr->e_shstrndx)->sh_offset;
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
    file->to_encrypt_shdr = find_section_to_encrypt(file);

    if (!file->to_encrypt_shdr)
    {
        fprintf(stderr, "Can't find \"%s\" section for encryption\n", SECTION_TO_ENCRYPT);
        return ERROR;
    }
    fprintf(stdout, "section header: %lx\n", file->ehdr->e_shoff + file->ehdr->e_shnum * sizeof(Elf64_Shdr));

    print_64ehdr(file);
    print_64phdr(file);
    print_64shdr(file);

    fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
        file->to_encrypt_shdr->sh_offset,
        file->to_encrypt_shdr->sh_size);
    return SUCCESS;
}
