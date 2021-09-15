#include "woody_woodpacker.h"

int     setup_payload(t_file *file)
{
    elf_phdr    *phdr_infect;
    // elf_shdr    *shdr_encrypt;
    Elf64_Off   jmp_offset;
    char        *payload_memaddr;
    // char        passwd_buf[64];
    Elf64_Off   old_entry_point_offset;

    phdr_infect = find_unused_pt_load_space(file, file->pld_len);
    // printf("Choose a password: ");
    // scanf("%64s", passwd_buf);

    

    // fprintf(stderr, "selected_passwsd: |%s|\n", passwd_buf);
    // fprintf(stderr, "encryption start: %lx\n", file->to_encrypt_shdr->sh_offset);
    // fprintf(stderr, "encryption end: %lx\n", file->to_encrypt_shdr->sh_offset + file->to_encrypt_shdr->sh_size);
    // shdr_encrypt = find_section_to_encrypt(file);
    // fprintf(stderr, "sh_type: %x sh_flags: %lx sh_addr: %lx sh_offset: %lx sh_size: %lx sh_addralign: %lx sh_entsize: %lx\n", shdr_encrypt->sh_type, shdr_encrypt->sh_flags, shdr_encrypt->sh_addr, shdr_encrypt->sh_offset, shdr_encrypt->sh_size, shdr_encrypt->sh_addralign, shdr_encrypt->sh_entsize);

    file->payload_filesz = file->pld_len;
    file->payload_offset = ALIGN16(phdr_infect->p_offset + phdr_infect->p_filesz);
    file->payload_vaddr = ALIGN16(phdr_infect->p_vaddr + phdr_infect->p_memsz);
    
    phdr_infect->p_filesz = ALIGN16(phdr_infect->p_filesz) + file->pld_len;
    phdr_infect->p_memsz = ALIGN16(phdr_infect->p_memsz) + file->pld_len;

    file->old_entry_point = file->ehdr->e_entry;
    file->ehdr->e_entry = file->payload_vaddr + file->pld_entry_off;

    fprintf(stderr, "old entry %lx nex entry %lx payload off %lx\n", file->old_entry_point, file->ehdr->e_entry, file->payload_offset);
    payload_memaddr = file->mapped_file + file->payload_offset;
    // payload_memaddr = file->bytecode + (file->payload_offset - sizeof(Elf64_Ehdr) - (file->ehdr->e_phentsize * file->ehdr->e_phnum));
    jmp_offset = file->payload_vaddr + file->pld_jmp_off;
    old_entry_point_offset = file->old_entry_point - jmp_offset;


    fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
        file->to_encrypt_shdr->sh_offset,
        file->to_encrypt_shdr->sh_size);
    // init blowfish
    // encrypt .Text section
    // save .text section start - end at encrypted_sec_start - encrypted_sec_end
    // save key encrypted at asm checksum to verify key and file sanity after decrypt

    fprintf(stderr, "%p %p %lx", payload_memaddr, file->payload, file->payload_filesz);
    memcpy(payload_memaddr, file->payload, file->payload_filesz);
    
    fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
        file->to_encrypt_shdr->sh_offset,
        file->to_encrypt_shdr->sh_size);
    
    memcpy(payload_memaddr + file->pld_jmp_off - 4, (char *)&old_entry_point_offset, 4); // negative rip value for x86-64 jmp is 32bit 
    
    fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
        file->to_encrypt_shdr->sh_offset,
        file->to_encrypt_shdr->sh_size);
    if (file->encryption_key)
        memcpy(payload_memaddr + file->pld_checksum_off, (char *)(&file->checksum), 8); // negative rip value for x86-64 jmp is 32bit 
    
    fprintf(stderr, "lol: %lx %lx\n, ", file->to_encrypt_shdr->sh_addr, file->to_encrypt_shdr->sh_size);
    Elf64_Addr  encryption_start;

    encryption_start = file->to_encrypt_shdr->sh_addr;
    memcpy(payload_memaddr + file->pld_sec_vaddr_off, (char *)(&encryption_start), 8); // negative rip value for x86-64 jmp is 32bit 
    memcpy(payload_memaddr + file->pld_sec_size_off, (char *)(&file->to_encrypt_shdr->sh_size), 8); // negative rip value for x86-64 jmp is 32bit 
    fprintf(stderr, "sec_vaddr %lx sec_size %lx\n", encryption_start, file->to_encrypt_shdr->sh_size);
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