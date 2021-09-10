#include "woody_woodpacker.h"

int encrypt_program(t_file *file)
{
    (void)file;
    // uint64_t *datas;
    uint32_t p[18];
    uint32_t s[4][256];

    // datas = (uint64_t *)(file->mapped_file + file->to_encrypt_shdr->sh_offset);
    fprintf(stderr, "%s\n", file->encryption_key);
    blowfish_init(file->encryption_key, p, s);
    
    fprintf(stderr, "bkey=%lx tst %.8s\n", blowfish_encrypt(*((uint64_t *)file->encryption_key), p, s), file->encryption_key);
    // for (Elf64_Xword  i = 0; i < file->to_encrypt_shdr->sh_size; i += 8)
    // {
    //     *datas = blowfish_encrypt(*datas, p, s);
    //     datas++;
    // }
    if (file->encryption_key)
        file->checksum = blowfish_encrypt(*((uint64_t *)file->encryption_key), p, s);
    
    // fprintf(stderr, "%lx %lx %lx %lx\n", datas[0], datas[1], datas[2], datas[3]);
    return 0;
}