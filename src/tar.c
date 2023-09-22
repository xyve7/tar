#include "tar.h"
#include <stdio.h>
uint32_t _tar_oct_to_dec(const char* oct_str, uint32_t len) {
    uint32_t ret = 0;
    while (len--) {
        ret *= 8;
        ret += *oct_str - '0';
        oct_str++;
    }
    return ret;
}
tar_error tar_open(tar* to, const char* buffer, size_t size) {
    to->start = buffer;
    to->current = buffer;
    to->end = buffer + size;
    return SUCCESS;
}
tar_error tar_next(tar* to, ustar_header* out) {
    size_t to_read = sizeof(*out);
    if (to->current + to_read < to->end) {
        // Copy the header
        memcpy(out, to->current, to_read);
        // Increment the current pointer to point past the header to the actual data
        to->current += to_read;
        // Confirm the USTAR header, if it isn't return END
        if (memcmp(out->ustar_ind, "ustar", 5) != 0) {
            return END;
        }

        to->save = to->current;

        uint32_t tar_size = _tar_oct_to_dec(out->file_size, 11);
        // Get the size of the file including the padding that TAR expects
        to->current += ((tar_size + 511) / 512) * 512;

    } else {
        return END;
    }
    return SUCCESS;
}
uint32_t tar_size(tar* to, ustar_header* header) {
    (void)to;
    return _tar_oct_to_dec(header->file_size, 11);
}
tar_error tar_read(tar* to, ustar_header* header, void* out_data, size_t size) {
    uint32_t tar_size = size == 0 ? _tar_oct_to_dec(header->file_size, 11) : size;

    // Get the size of the file including the padding that TAR expects
    size_t offset = ((tar_size + 511) / 512) * 512;

    to->current = to->save;
    if (to->current + offset < to->end) {
        // Copy the actual data
        memcpy(out_data, to->current, tar_size);
        // Increment to the end
        to->current += offset;
    } else {
        return END;
    }
    return SUCCESS;
}
tar_error tar_close(tar* to) {
    to->start = NULL;
    to->current = NULL;
    to->end = NULL;
    return SUCCESS;
}
