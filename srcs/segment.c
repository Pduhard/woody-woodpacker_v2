#include "woody_woodpacker.h"

Elf64_Phdr    *find_unused_pt_load_space(t_file *file, Elf64_Off size)
{
    Elf64_Phdr  *phdr, *next, *target;
    Elf64_Off   end_offset_aligned;
    Elf64_Half  i;

    target = NULL;
    file->cave_found = TRUE;
    phdr = file->phdr;
    for (i = 0, phdr = file->phdr; i < file->ehdr->e_phnum - 1; i++, phdr++)
    {
        if (phdr->p_type == PT_LOAD)
        {
            next = phdr + 1;
            end_offset_aligned = phdr->p_offset + phdr->p_filesz;
            if (next->p_offset - end_offset_aligned >= size && !target)
            {
                if (next->p_offset < end_offset_aligned)
                    file->cave_found = FALSE;
                target = phdr;
                phdr->p_flags |= PF_X;
            }
            phdr->p_flags |= (PF_R | PF_W);
        }
    }
    if (!target)
    {
        fprintf(stderr, "%sError: invalid file%s\n", C_ERR, C_RES);
        exit(EXIT_FAILURE);
    }
    return target;
}