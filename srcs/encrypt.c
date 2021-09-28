#include "woody_woodpacker.h"

int blowfish_encryption(t_file *file)
{
    uint64_t *datas;
    uint32_t p[18];
    uint32_t s[4][256];

    datas = (uint64_t *)(file->mapped_file + file->to_encrypt_shdr->sh_offset);
    blowfish_init(file->encryption_key, p, s);
    for (Elf64_Xword  i = 0; i < file->to_encrypt_shdr->sh_size; i += 8)
    {
        *datas = blowfish_encrypt(*datas, p, s);
        datas++;
    }
    if (file->encryption_key)
        file->checksum = blowfish_encrypt(*((uint64_t *)file->encryption_key), p, s);

    file->payload = (char *)blowfish_payload;
    file->pld_len = g_blowfish_pld_len;
    file->pld_entry_off = g_blowfish_pld_entry_off;
    file->pld_jmp_off = g_blowfish_pld_jmp_off;
    file->pld_sec_vaddr_off = g_blowfish_pld_sec_vaddr_off;
    file->pld_sec_size_off = g_blowfish_pld_sec_size_off;
    file->pld_vaddr_load_off = g_blowfish_pld_vaddr_load_off;
    file->pld_checksum_off = g_blowfish_pld_checksum_off;
    file->payload = (char *)malloc(file->pld_len);
    memcpy(file->payload, blowfish_payload, file->pld_len);
    return SUCCESS;
}


int xor_encryption(t_file *file)
{
    unsigned char *datas;
    unsigned char encryption_key = 0x9b;

    file->encryption_key = NULL;
    datas = (unsigned char *)(file->mapped_file + file->to_encrypt_shdr->sh_offset);
    for (Elf64_Xword  i = 0; i < file->to_encrypt_shdr->sh_size; i += 1)
    {
        *datas = *datas ^ encryption_key;
        datas++;
    }
    file->checksum = 0;
    file->pld_len = g_xor_pld_len;
    file->pld_entry_off = g_xor_pld_entry_off;
    file->pld_jmp_off = g_xor_pld_jmp_off;
    file->pld_sec_vaddr_off = g_xor_pld_sec_vaddr_off;
    file->pld_sec_size_off = g_xor_pld_sec_size_off;
    file->pld_vaddr_load_off = g_xor_pld_vaddr_load_off;
    file->payload = (char *)malloc(file->pld_len);
    memcpy(file->payload, xor_payload, file->pld_len);
    return 0;
}