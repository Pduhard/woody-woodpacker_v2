#include "woody_woodpacker.h"

int     check_elf(t_file *file)
{
    unsigned char	*e_ident;
    int             encoding_test = 1;
    char            encoding;

    encoding = *(char *)&encoding_test == 1 ? ELFDATA2LSB : ELFDATA2MSB;
    e_ident = (unsigned char *)file->mapped_file;
    if (strncmp((char *)e_ident, ELFMAG, SELFMAG))
    {
        fprintf(stderr, "%sError: Wrong magic numbers, not an ELF file%s\n", C_ERR, C_RES);
        return ERROR;
    }
    if (e_ident[EI_CLASS] != ELFCLASS64)
    {
        fprintf(stderr, "%sError: only 64-bit objects are supported%s\n", C_ERR, C_RES);
        return ERROR;
    }
    if (e_ident[EI_DATA] != encoding)
    {
        fprintf(stderr, "%sError: endianness differ from system one%s\n", C_ERR, C_RES);
        return ERROR;    
    }
    return SUCCESS; 
}

int     parse_elf(t_file *file)
{
    if (check_elf(file) == ERROR)
        return (ERROR);
    file->ehdr = (Elf64_Ehdr *)file->mapped_file;
    file->phdr = (Elf64_Phdr *)(file->mapped_file + file->ehdr->e_phoff);
    file->shdr = (Elf64_Shdr *)(file->mapped_file + file->ehdr->e_shoff);
    file->strtab = file->mapped_file + (file->shdr + file->ehdr->e_shstrndx)->sh_offset;
    file->to_encrypt_shdr = find_section_to_encrypt(file);
    if (!file->to_encrypt_shdr)
    {
        fprintf(stderr, "%sError: can't find \"%s\" section for encryption\n%s", C_ERR, SECTION_TO_ENCRYPT, C_RES);
        return ERROR;
    }
    return SUCCESS;
}
