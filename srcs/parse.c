#include "woody_woodpacker.h"

Elf64_Shdr    *find_section_to_encrypt(t_file *file)
{
    Elf64_Shdr    *curr;

    curr = file->shdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_shnum; i++)
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

    if (strncmp((char *)e_ident, ELFMAG, SELFMAG))
    {
        fprintf(stderr, "%sError: Wrong magic numbers, not an ELF file%s\n", C_ERR, C_RES);
        return ERROR;
    }
    if (e_ident[EI_CLASS] == ELFCLASS64)
    {
        file->ehdr = (Elf64_Ehdr *)file->mapped_file;
        file->phdr = (Elf64_Phdr *)(file->mapped_file + file->ehdr->e_phoff);
        file->shdr = (Elf64_Shdr *)(file->mapped_file + file->ehdr->e_shoff);
        file->strtab = file->mapped_file + (file->shdr + file->ehdr->e_shstrndx)->sh_offset;
    }
    else if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        fprintf(stderr, "%sError: 32-bit objects are not supported%s\n", C_ERR, C_RES);
        return ERROR;
    }
    else
    {
        fprintf(stderr, "%sError: invalid file class byte index: %hhu%s\n", C_ERR, e_ident[EI_CLASS], C_RES);
        return ERROR;
    }
    file->to_encrypt_shdr = find_section_to_encrypt(file);
    if (!file->to_encrypt_shdr)
    {
        fprintf(stderr, "%sError: can't find \"%s\" section for encryption\n%s", C_ERR, SECTION_TO_ENCRYPT, C_RES);
        return ERROR;
    }
    return SUCCESS;
}
