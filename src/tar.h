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
    charlinked_file_name[100];
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
    const char* start;
    const char* current;
    const char* end;
} tar;

tar_error tar_open(tar* to, const char* buffer, size_t size);
tar_error tar_next(tar* to, ustar_header* out, void** out_data);
uint32_t tar_size(tar* to, ustar_header* header);
tar_error tar_close(tar* to);

#endif
