#include "woody_woodpacker.h"

elf_phdr    *parse_64phdr(t_file *file)
{
    elf_phdr    *phdr;

    phdr = calloc(file->ehdr->e_phnum + 1, sizeof(elf_phdr));
    memcpy(phdr, file->mapped_file + file->ehdr->e_phoff, file->ehdr->e_phnum * sizeof(elf_phdr));
    return phdr;
}

elf_phdr    *get_first_pt_load(t_file *file)
{
    elf_phdr *phdr;
    
    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
    {
        if (phdr->p_type == PT_LOAD)
            return phdr;
        phdr++;
    }
    return NULL;
}


elf_phdr    *get_last_pt_load(t_file *file)
{
    elf_phdr *phdr;
    elf_phdr *last_pt_load;
    
    last_pt_load = NULL;
    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
    {
        if (phdr->p_type == PT_LOAD)
            last_pt_load = phdr;
        phdr++;
    }
    return last_pt_load;
}

Elf64_Off   get_phdr_end_offset_aligned(elf_phdr *phdr)
{
    return ALIGN16(phdr->p_offset + phdr->p_filesz);
}

elf_phdr    *find_unused_pt_load_space(t_file *file, Elf64_Off size)
{
    elf_phdr    *phdr;
    elf_phdr    *next;
    Elf64_Off   end_offset_aligned;
    elf_phdr    *target;
    int         n = 0;

    target = NULL;
    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum - 1; i++)
    {
        if (phdr->p_type == PT_LOAD)
        {
            next = phdr + 1;

            end_offset_aligned = get_phdr_end_offset_aligned(phdr);
            fprintf(stderr, "get_align %lx filesz %lx\n", GET_ALIGN16(phdr->p_offset + phdr->p_filesz), phdr->p_filesz);
            fprintf(stderr, "startoff %lx end off %lx next_off %lx size %lx\n",phdr->p_offset, end_offset_aligned, next->p_offset, size);
            
            fprintf(stderr, "available size: %lx, payload size: %lx\n", next->p_offset - end_offset_aligned, size);

            if (next->p_offset - end_offset_aligned >= size && !target)
            {
                fprintf(stderr, "THIS ONE!\n");
                target = phdr;
                n++;

            }
            
            phdr->p_flags = 7;

        }
        phdr++;
    }
    if (!target)
    {
        fprintf(stderr, "no suffisant padding size in pt loads => exit\n");
        exit(EXIT_FAILURE);
    }
    return target;
}

int     update_phdr(t_file *file)
{
    // elf_phdr    *phdr;
    elf_phdr    *first_pt_load;
    Elf64_Off   first_pt_load_offend;

    // PHDR segment update
    // phdr = file->phdr;

    // if (phdr->p_type != PT_PHDR)
    //     printf("First phdr is not a PT_PHDR\n"), exit(EXIT_FAILURE);

    // phdr->p_memsz += file->ehdr->e_phentsize;
    // phdr->p_filesz += file->ehdr->e_phentsize;

    // First PT_LOAD segment update
    first_pt_load = get_first_pt_load(file);
    first_pt_load_offend = first_pt_load->p_offset + first_pt_load->p_filesz;

    if (first_pt_load_offend + file->ehdr->e_phentsize > (first_pt_load + 1)->p_offset)
        printf("No space left in first PT_LOAD segment\n"), exit(EXIT_FAILURE);

    first_pt_load->p_memsz += file->ehdr->e_phentsize;
    first_pt_load->p_filesz += file->ehdr->e_phentsize;
    return 1;
}



void    print_64phdr(t_file *file)
{
    Elf64_Phdr *phdr;

    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
    {
        fprintf(stderr, "type: %x flags %u off %lx v_addr %lx paddr %lx p_filesz %lx p_memsz %lx p_align %lu\n", phdr->p_type, phdr->p_flags, phdr->p_offset, phdr->p_vaddr, phdr->p_paddr, phdr->p_filesz, phdr->p_memsz, phdr->p_align);
        phdr++;
    }
}
