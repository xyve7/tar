#ifndef TAR_H
#define TAR_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    SUCCESS,
    ERRNO,
    END,
    INVALID_HEADER,
} tar_error;

typedef enum {
    NORMAL = '0',
    HARD_LINK,
    SYM_LINK,
    CHAR_DEV,
    BLOCK_DEV,
    DIR,
    NAMED_PIPE
} ustar_type;

typedef struct {
    char file_name[100];
    char mode[8];
    char owner_uid[8];
    char group_uid[8];
    char file_size[12];
    char last_mod_time[12];
    char checksum[8];
    char type;
    char linked_file_name[100];
    char ustar_ind[6];
    char ustar_ver[2];
    char owner_uname[32];
    char owner_gname[32];
    char device_maj_num[8];
    char device_min_num[8];
    char file_name_prefix[155];
    char padding[12];
} ustar_header;

typedef struct {
    FILE* fp;
    long last;
} tar;

tar_error tar_open(tar* to, const char* filename, const char* mode);
tar_error tar_next(tar* to, ustar_header* out);
tar_error tar_read(tar* to, ustar_header* header, void* out, size_t size);
uint32_t tar_size(tar* to, ustar_header* header);
#endif
