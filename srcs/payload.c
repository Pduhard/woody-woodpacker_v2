#include "woody_woodpacker.h"

int     setup_payload(t_file *file)
{
    Elf64_Phdr    *phdr_infect;
    // elf_shdr    *shdr_encrypt;
    Elf64_Off   jmp_offset;
    // char        *file->payload;
    // char        passwd_buf[64];
    Elf64_Off   old_entry_point_offset;
    int         load_off;

    file->payload_filesz = file->pld_len;
    phdr_infect = find_unused_pt_load_space(file, file->payload_filesz);
    file->payload_offset = phdr_infect->p_offset + phdr_infect->p_filesz;
    file->payload_vaddr = phdr_infect->p_vaddr + phdr_infect->p_memsz;
    if (file->cave_found == FALSE && phdr_infect->p_memsz != phdr_infect->p_filesz)
    {
        file->bss_zero_fill = phdr_infect->p_memsz - phdr_infect->p_filesz;
        phdr_infect->p_filesz = phdr_infect->p_memsz + file->payload_filesz;
    }
    else
    {
        file->bss_zero_fill = 0;
        phdr_infect->p_filesz = phdr_infect->p_filesz + file->payload_filesz;
    }
    phdr_infect->p_memsz = phdr_infect->p_memsz + file->payload_filesz;
    file->old_entry_point = file->ehdr->e_entry;
    file->ehdr->e_entry = file->payload_vaddr + file->pld_entry_off;

    jmp_offset = file->payload_vaddr + file->pld_jmp_off;
    old_entry_point_offset = file->old_entry_point - jmp_offset;

    memcpy(file->payload + file->pld_jmp_off - 4, (char *)&old_entry_point_offset, 4); // negative rip value for x86-64 jmp is 32bit 
    if (file->encryption_key)
        memcpy(file->payload + file->pld_checksum_off, (char *)(&file->checksum), 8); // negative rip value for x86-64 jmp is 32bit 

    Elf64_Addr  encryption_start;

    encryption_start = file->to_encrypt_shdr->sh_addr;
    memcpy(file->payload + file->pld_sec_vaddr_off, (char *)(&encryption_start), 8); // negative rip value for x86-64 jmp is 32bit 
    memcpy(file->payload + file->pld_sec_size_off, (char *)(&file->to_encrypt_shdr->sh_size), 8); // negative rip value for x86-64 jmp is 32bit 
    
    load_off = (int)(encryption_start - (file->payload_vaddr + file->pld_vaddr_load_off));

    memcpy(file->payload + file->pld_vaddr_load_off - 4, (char *)&load_off, 4); // negative rip value for x86-64 jmp is 32bit 
    if (file->cave_found == FALSE)
        update_shdr(file);
    return 1;
}

// int     setup_payload(t_file *file)
// // old version inject after all pt_load (not working)
// {
//     elf_phdr    *last_pt_load;
//     Elf64_Off   p_offset;
//     Elf64_Addr  p_vaddr;
//     Elf64_Off   page_align;

//     file->payload = (char *)payload;
//     file->payload_filesz = (unsigned long)setup_payload - (unsigned long)payload;

//     last_pt_load = get_last_pt_load(file);
//     p_offset = last_pt_load->p_offset + last_pt_load->p_filesz;

//     if (p_offset % getpagesize())
//         page_align = getpagesize() - p_offset % getpagesize();
//     else
//         page_align = 0;
//     p_offset = p_offset + page_align;
//     // battage de couille
//     p_vaddr = last_pt_load->p_vaddr + last_pt_load->p_filesz + page_align;

//     file->payload_phdr = (elf_phdr) {
//         PT_LOAD,
//         7,
//         p_offset,
//         p_vaddr,
//         p_vaddr,
//         file->payload_filesz,
//         file->payload_filesz,
//         getpagesize()
//     };
//     return 0;
// }