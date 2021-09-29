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

int     update_shdr(t_file *file)
{
    Elf64_Shdr  *shdr;
    Elf64_Half  i;

    for (i = 0, shdr=file->shdr; i < file->ehdr->e_shnum; i++, shdr++)
    {
        if (shdr->sh_offset > file->pld_offset)
            shdr->sh_offset += file->pld_len + file->bss_zero_fill;
    }
    return 1;
}
