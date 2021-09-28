#include "woody_woodpacker.h"

int     update_shdr(t_file *file)
{
    Elf64_Shdr *shdr;

    shdr = file->shdr;

    for (Elf64_Half i = 0; i < file->ehdr->e_shnum; i++)
    {
        if (shdr->sh_offset > file->payload_offset)
            shdr->sh_offset += file->payload_filesz + file->bss_zero_fill;
        shdr++;
    }
    return 1;
}
