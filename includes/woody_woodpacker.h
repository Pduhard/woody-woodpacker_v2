#ifndef WOODY_WOODPACKER_H
# define WOODY_WOODPACKER_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <elf.h>
# include <errno.h>

# define C_WARN "\x1b[33;1m"
# define C_ERR "\x1b[31;1m"
# define C_SUCC "\x1b[32;1m"
# define C_RES "\x1b[0m"

# define USAGE "./woody_woodpacker <file_to_pack> [ -e <xor | blowfish> ] ] [ -k <enter key> ]\n"

# define TRUE 0
# define FALSE 1

# define SUCCESS TRUE
# define ERROR FALSE

# define SECTION_TO_ENCRYPT    ".text"

# define DEFAULT_ENCRYPTION_ALGORITHM    "blowfish"

/*
** blowfish payload vars
*/

extern unsigned int g_blowfish_pld_len;
extern unsigned int g_blowfish_pld_entry_off;
extern unsigned int g_blowfish_pld_jmp_off;
extern unsigned int g_blowfish_pld_sec_size_off;
extern unsigned int g_blowfish_pld_vaddr_load_off;
extern unsigned int g_blowfish_pld_checksum_off;

/*
** xor payload vars
*/

extern unsigned int g_xor_pld_len;
extern unsigned int g_xor_pld_entry_off;
extern unsigned int g_xor_pld_jmp_off;
extern unsigned int g_xor_pld_sec_size_off;
extern unsigned int g_xor_pld_vaddr_load_off;

typedef struct      s_file
{
    char            *mapped_file;
    Elf64_Addr      old_entry_point;
    Elf64_Ehdr      *ehdr;
    Elf64_Phdr      *phdr;
    Elf64_Shdr      *shdr;
    Elf64_Off       size;
    char            *strtab;
    char            *encryption_key;
    int             (*encrypt)(struct s_file*);
    Elf64_Shdr      *to_encrypt_shdr;
    char            *payload;
    Elf64_Off       pld_offset;
    Elf64_Addr      pld_vaddr;
    uint32_t        pld_len;
    uint32_t        pld_entry_off;
    uint32_t        pld_jmp_off;
    uint32_t        pld_sec_size_off;
    uint32_t        pld_checksum_off;
    uint32_t        pld_vaddr_load_off;
    uint64_t        checksum;
    uint32_t        bss_zero_fill;
    int             cave_found;
}                   t_file;

int         parse_elf(t_file *file);
char 	    *parse_options(t_file *file, char **argv);

Elf64_Phdr  *find_unused_pt_load_space(t_file *file, Elf64_Off size);
Elf64_Shdr  *find_section_to_encrypt(t_file *file);

void        setup_payload(t_file *file);
int         update_shdr(t_file *file);

int         blowfish_encryption(t_file *file);
int         xor_encryption(t_file *file);

void        blowfish_payload();
void        xor_payload();

void        blowfish_init(char *key, uint32_t p[18], uint32_t s[4][256]);
uint64_t    blowfish_encrypt(uint64_t block, uint32_t p[18], uint32_t s[4][256]);

#endif