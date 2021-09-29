#include "woody_woodpacker.h"

void    update_headers(t_file *file, Elf64_Phdr *phdr_infect)
{
    file->bss_zero_fill = 0;
    if (file->cave_found == TRUE)
    {
        phdr_infect->p_filesz = phdr_infect->p_filesz + file->pld_len;
        phdr_infect->p_memsz = phdr_infect->p_memsz + file->pld_len;
        return ;
    }
    if (phdr_infect->p_memsz != phdr_infect->p_filesz)
        file->bss_zero_fill = phdr_infect->p_memsz - phdr_infect->p_filesz;
    phdr_infect->p_filesz = phdr_infect->p_memsz + file->pld_len;
    phdr_infect->p_memsz = phdr_infect->p_memsz + file->pld_len;
    update_shdr(file);
    file->ehdr->e_shoff += file->pld_len + file->bss_zero_fill;
}

void     setup_payload(t_file *file)
{
    Elf64_Phdr  *phdr_infect;
    Elf64_Off   jmp_offset, old_entry_rip_offset;
    int         load_off;

    phdr_infect = find_unused_pt_load_space(file, file->pld_len);
    file->pld_offset = phdr_infect->p_offset + phdr_infect->p_filesz;
    file->pld_vaddr = phdr_infect->p_vaddr + phdr_infect->p_memsz;
    file->old_entry_point = file->ehdr->e_entry;
    file->ehdr->e_entry = file->pld_vaddr + file->pld_entry_off;
    jmp_offset = file->pld_vaddr + file->pld_jmp_off;
    old_entry_rip_offset = file->old_entry_point - jmp_offset;
    load_off = (int)(file->to_encrypt_shdr->sh_addr - (file->pld_vaddr + file->pld_vaddr_load_off));

    // relative jump to old entry point
    memcpy(file->payload + file->pld_jmp_off - 4, (char *)&old_entry_rip_offset, 4);
    // section to encrypt/decrypt relative rip offset
    memcpy(file->payload + file->pld_vaddr_load_off - 4, (char *)&load_off, 4);
    // section to encrypt/decrypt size
    memcpy(file->payload + file->pld_sec_size_off, (char *)(&file->to_encrypt_shdr->sh_size), 8);
    // checksum in case of password
    if (file->encryption_key)
        memcpy(file->payload + file->pld_checksum_off, (char *)(&file->checksum), 8);
    update_headers(file, phdr_infect);
}
