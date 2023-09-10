#include <tar.h>

uint32_t _tar_oct_to_dec(const char* oct_str, uint32_t len) {
    uint32_t ret = 0;
    while (len--) {
        ret *= 8;
        ret += *oct_str - '0';
        oct_str++;
    }
    return ret;
}

tar_error tar_open(tar* to, const char* filename, const char* mode) {
    to->fp = fopen(filename, mode);
    if (to->fp == NULL) {
        return ERRNO;
    }
    return SUCCESS;
}
tar_error tar_next(tar* to, ustar_header* out) {
    size_t to_read = sizeof(*out);
    size_t ret = fread(out, 1, to_read, to->fp);
    if (ret != to_read) {
        if (ferror(to->fp)) {
            return ERRNO;
        } else {
            return END;
        }
    }

    // Check if the header is valid
    if (memcmp(out->ustar_ind, "ustar", 5) != 0) {
        return INVALID_HEADER;
    }
    // Save the offset to the actual data, so when the user calls tar_read() they can get the file after tar_next()
    to->last = ftell(to->fp);

    // Jump to the end of the tar entry
    long offset = ((_tar_oct_to_dec(out->file_size, 11) + 511) / 512) * 512;
    fseek(to->fp, offset, SEEK_CUR);

    return SUCCESS;
}
tar_error tar_read(tar* to, ustar_header* header, void* out, size_t size) {
    // Assign the size to read
    size_t to_read = size;
    // If the size is 0, read how ever long the file is
    if (to_read == 0) {
        to_read = ((_tar_oct_to_dec(header->file_size, 11) + 511) / 512) * 512;
    }
    // Save the current offset of the file, since it's pointing to AFTER the file data, aka to the next header
    long save = ftell(to->fp);
    fseek(to->fp, to->last, SEEK_SET);

    // Read the data
    size_t ret = fread(out, 1, to_read, to->fp);
    if (ret != to_read) {
        if (ferror(to->fp)) {
            return ERRNO;
        } else {
            return END;
        }
    }

    fseek(to->fp, save, SEEK_SET);
    return SUCCESS;
}

uint32_t tar_size(tar* to, ustar_header* header) {
    (void)to;
    return _tar_oct_to_dec(header->file_size, 11);
}
