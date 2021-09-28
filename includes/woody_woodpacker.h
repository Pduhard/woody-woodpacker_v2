#ifndef WOODY_WOODPACKER_H
# define WOODY_WOODPACKER_H


# include <sys/mman.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <elf.h>
# include <math.h>
# include "blowfish.h"

#include <errno.h>

# define C_WARN "\x1b[33;1m"
# define C_ERR "\x1b[31;1m"
# define C_SUCC "\x1b[32;1m"
# define C_RES "\x1b[0m"

// # define V_WARN 0b1
// # define 

# define USAGE "./woody_woodpacker <file_to_pack> [ -e <xor | blowfish> ] ] [ -k <enter key> ]\n"

# define TRUE 0
# define FALSE 1

# define SUCCESS TRUE
# define ERROR FALSE


# define SECTION_TO_ENCRYPT    ".text"

# define DEFAULT_ENCRYPTION_ALGORITHM    "blowfish"

# define GET_ALIGN16(x) ((x) % 16 ? 16 - (x) % 16 : 0)
# define ALIGN16(x) ((x) + GET_ALIGN16((x)))

// typedef Elf64_Ehdr Elf64_Ehdr;
// typedef Elf64_Phdr Elf64_Phdr;
// typedef Elf64_Shdr Elf64_Shdr;

/*
** blowfish payload vars
*/

// extern unsigned int g_payload_offset;

extern unsigned int g_blowfish_pld_len;
extern unsigned int g_blowfish_pld_entry_off;
extern unsigned int g_blowfish_pld_jmp_off;
extern unsigned int g_blowfish_pld_sec_vaddr_off;
extern unsigned int g_blowfish_pld_sec_size_off;
extern unsigned int g_blowfish_pld_vaddr_load_off;
extern unsigned int g_blowfish_pld_checksum_off;

// extern int bbp_call_rip;

/*
** xor payload vars
*/

extern unsigned int g_xor_pld_len;
extern unsigned int g_xor_pld_entry_off;
extern unsigned int g_xor_pld_jmp_off;
extern unsigned int g_xor_pld_sec_vaddr_off;
extern unsigned int g_xor_pld_sec_size_off;
extern unsigned int g_xor_pld_vaddr_load_off;

typedef struct      s_file
{
    char            *mapped_file;
    Elf64_Addr      old_entry_point;
    Elf64_Ehdr      *ehdr;
    Elf64_Phdr      *phdr;
    Elf64_Shdr      *shdr;
    char            *payload;
    Elf64_Addr      payload_vaddr;
    Elf64_Off       payload_offset;
    Elf64_Off       payload_filesz;

    // Elf64_Addr      payload_entry;
    Elf64_Off       size;
    Elf64_Shdr      *to_encrypt_shdr;
    // Elf64_Shdr      *to_encrypt_shdr;
    char            *strtab;
    // Elf64_Shdr      *strtab_shdr;
    char            *encryption_key;
    uint64_t        checksum;
    int             (*encrypt)(struct s_file*);


    uint32_t        pld_len;
    uint32_t        pld_entry_off;
    uint32_t        pld_jmp_off;
    uint32_t        pld_sec_vaddr_off;
    uint32_t        pld_sec_size_off;

    uint32_t        pld_checksum_off;
    uint32_t        pld_vaddr_load_off;

    int             cave_found;
    uint32_t        bss_zero_fill;
    int             verbose;
}                   t_file;



int             parse_elf(t_file *file);

Elf64_Phdr    *get_last_pt_load(t_file *file);
char    *empty_bytecode_space(t_file *file, Elf64_Off payload_size);

Elf64_Phdr    *find_unused_pt_load_space(t_file *file, Elf64_Off size);
Elf64_Shdr    *find_section_to_encrypt(t_file *file);

Elf64_Off   get_phdr_end_offset_aligned(Elf64_Phdr *phdr);

int     setup_payload(t_file *file);

int     update_ehdr(t_file *file);
int     update_phdr(t_file *file);

int     update_shdr(t_file *file);


// void    print_64ehdr(t_file *file);
// void    print_64phdr(t_file *file);
// void    print_64shdr(t_file *file);

int blowfish_encryption(t_file *file);
int xor_encryption(t_file *file);


void    blowfish_payload();
void    xor_payload();

int     power(long long x, unsigned int y, int p);
double  get_floating_part(double n);
double  test_get_floating_part(double n);
double    sigma(int n, int a);
uint32_t     bbp_getnth_term(int n);


// void    blowfish_run(char *key);
void    blowfish_init(char *key, uint32_t p[18], uint32_t s[4][256]);


void    uint32_swap(uint32_t *a, uint32_t *b);
uint32_t    feisel(uint32_t in, uint32_t s[4][256]);
uint64_t    blowfish_decrypt(uint64_t block, uint32_t p[18], uint32_t s[4][256]);
uint64_t    blowfish_encrypt(uint64_t block, uint32_t p[18], uint32_t s[4][256]);


#endif