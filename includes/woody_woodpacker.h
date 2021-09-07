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

# define USAGE "./woody_woodpacker <file_to_pack>"

# define SUCCESS 0
# define ERROR 1
# define SECTION_TO_ENCRYPT    ".text"


# define GET_ALIGN16(x) ((x) % 16 ? 16 - (x) % 16 : 0)
# define ALIGN16(x) ((x) + GET_ALIGN16((x)))

typedef Elf64_Ehdr elf_ehdr;
typedef Elf64_Phdr elf_phdr;
typedef Elf64_Shdr elf_shdr;

extern unsigned int g_payload_len;
// extern unsigned int g_payload_offset;
extern unsigned int g_payload_jmp_offset;
extern unsigned int g_payload_start_offset;

typedef struct      s_file
{
    char            *mapped_file;
    Elf64_Addr      old_entry_point;
    elf_ehdr        *ehdr;
    elf_phdr        *phdr;
    elf_shdr        *shdr;
    // char            *bytecode;
    Elf64_Off       b_offset;
    Elf64_Off       b_filesz;
    char            *payload;
    Elf64_Addr      payload_vaddr;
    Elf64_Off       payload_offset;
    Elf64_Off       payload_filesz;

    // Elf64_Addr      payload_entry;
    Elf64_Off       size;
    Elf64_Shdr      *to_encrypt_shdr;
    char            *strtab;
    // Elf64_Shdr      *strtab_shdr;
}                   t_file;



int         parse_elf(t_file *file);
elf_ehdr    *parse_64ehdr(t_file *file);
elf_phdr    *parse_64phdr(t_file *file);
elf_shdr    *parse_64shdr(t_file *file);
int         parse_bytecode(t_file *file);

elf_phdr    *get_last_pt_load(t_file *file);
char    *empty_bytecode_space(t_file *file, Elf64_Off payload_size);

elf_phdr    *find_unused_pt_load_space(t_file *file, Elf64_Off size);
Elf64_Off   get_phdr_end_offset_aligned(elf_phdr *phdr);

int     setup_payload(t_file *file);

int     update_ehdr(t_file *file);
int     update_phdr(t_file *file);

void    print_64ehdr(t_file *file);
void    print_64phdr(t_file *file);
void    print_64shdr(t_file *file);

void    payload();

int     power(long long x, unsigned int y, int p);
double  get_floating_part(double n);
double    sigma(int n, int a);
uint32_t     bbp_getnth_term(int n);


void    blowfish_init(char *key);

void    uint32_swap(uint32_t *a, uint32_t *b);
uint32_t    feisel(uint32_t in, uint32_t s[4][256]);


#endif