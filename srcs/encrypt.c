#include "woody_woodpacker.h"

int blowfish_encryption(t_file *file)
{
    (void)file;
    uint64_t *datas;
    uint32_t p[18];
    uint32_t s[4][256];

    datas = (uint64_t *)(file->mapped_file + file->to_encrypt_shdr->sh_offset);
    fprintf(stderr, "%s\n", file->encryption_key);
    blowfish_init(file->encryption_key, p, s);
    
    
    fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
        file->to_encrypt_shdr->sh_offset,
        file->to_encrypt_shdr->sh_size);
    // fprintf(stderr, "bkey=%lx tst %.8s\n", blowfish_encrypt(*((uint64_t *)file->encryption_key), p, s), file->encryption_key);
    
    for (Elf64_Xword  i = 0; i < file->to_encrypt_shdr->sh_size; i += 8)
    {
        *datas = blowfish_encrypt(*datas, p, s);
        datas++;
    }
    if (file->encryption_key)
        file->checksum = blowfish_encrypt(*((uint64_t *)file->encryption_key), p, s);
    
    fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
        file->to_encrypt_shdr->sh_offset,
        file->to_encrypt_shdr->sh_size);
    // fprintf(stderr, "%lx %lx %lx %lx\n", datas[0], datas[1], datas[2], datas[3]);
    return 0;
}


int xor_encryption(t_file *file)
{
    (void)file;
    unsigned char *datas;
    unsigned char encryption_key = 0x9b;

    fprintf(stderr, "XOOOR\n");
    datas = (unsigned char *)(file->mapped_file + file->to_encrypt_shdr->sh_offset);
    // fprintf(stderr, "%s\n", file->encryption_key);
    // blowfish_init(file->encryption_key, p, s);
    
    
    fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
        file->to_encrypt_shdr->sh_offset,
        file->to_encrypt_shdr->sh_size);
    // fprintf(stderr, "bkey=%lx tst %.8s\n", blowfish_encrypt(*((uint64_t *)file->encryption_key), p, s), file->encryption_key);
    
    for (Elf64_Xword  i = 0; i < file->to_encrypt_shdr->sh_size; i += 1)
    {
        // *datas = *datas ^ encryption_key;
        *datas = *datas ^ encryption_key;
        datas++;
    }
    file->payload = (char *)xor_payload;
    file->pld_len = g_xor_pld_len;
    file->pld_entry_off = g_xor_pld_entry_off;
    file->pld_jmp_off = g_xor_pld_jmp_off;
    file->pld_sec_vaddr_off = g_xor_pld_sec_vaddr_off;
    file->pld_sec_size_off = g_xor_pld_sec_size_off;
    file->pld_vaddr_load_off = g_xor_pld_vaddr_load_off;
    // if (file->encryption_key)
    //     file->checksum = blowfish_encrypt(*((uint64_t *)file->encryption_key), p, s);
    
    // fprintf(stderr, "section to encrypt:\toffset: %lx, size: %lx\n",
    //     file->to_encrypt_shdr->sh_offset,
    //     file->to_encrypt_shdr->sh_size);
    // fprintf(stderr, "%lx %lx %lx %lx\n", datas[0], datas[1], datas[2], datas[3]);
    return 0;
}